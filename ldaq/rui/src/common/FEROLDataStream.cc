#include "emu/ldaq/rui/FEROLDataStream.h"

#include "emu/ldaq/rui/CDF_FED_Header.h"
#include "emu/ldaq/rui/CDF_FED_Trailer.h"

#include "toolbox/mem/HeapAllocator.h"
// #include "toolbox/mem/CommittedHeapAllocator.h"
#include "toolbox/mem/MemoryPoolFactory.h"
#include "evb/Constants.h"                 // for FEROL_BLOCK_SIZE
#include "interface/shared/ferol_header.h" // for ferolh_t and FEROL_SIGNATURE
#include "interface/shared/fed_header.h"    // for fedh_t and FED_SOID_EXTRACT
#include "interface/shared/fed_trailer.h"    // for fedt_t

#include <deque>
#include <sstream>
#include <limits>

emu::ldaq::rui::FEROLDataStream::FEROLDataStream( uint16_t fedId )
  : fedId_         ( fedId                                      )
  , fedIdFromData_ ( 0                                          )
  , maxPayloadSize_( evb::FEROL_BLOCK_SIZE - sizeof( ferolh_t ) )
  , isFirstBlock_  ( true                                       )
  , blockNumber_   ( 0                                          )
  , l1a_           ( 0                                          )
  , pool_          ( NULL                                       )
  , poolURN_       ( NULL                                       )
{
  createMemoryPool();
}

emu::ldaq::rui::FEROLDataStream::~FEROLDataStream(){
  toolbox::mem::getMemoryPoolFactory()->destroyPool( *poolURN_ );
  delete poolURN_;
}

void emu::ldaq::rui::FEROLDataStream::createMemoryPool(){
  ostringstream poolName;
  poolName << "rui-to-ru-" << fedId_;
  try{
    poolURN_ = new toolbox::net::URN( "toolbox-mem-pool", poolName.str() );
    toolbox::mem::HeapAllocator* a = new toolbox::mem::HeapAllocator();
    // toolbox::mem::CommittedHeapAllocator* a = new toolbox::mem::CommittedHeapAllocator( 0x10000 );
    pool_ = toolbox::mem::getMemoryPoolFactory()->createPool( *poolURN_, a );
  }
  catch (xcept::Exception &e){
    string s = "Failed to create pool: " + poolName.str();
    XCEPT_RETHROW(xcept::Exception, s, e);
  }
  catch(...){
    string s = "Failed to create pool: " + poolName.str() + " : Unknown exception";
    XCEPT_RAISE(xcept::Exception, s);
  }
}

size_t emu::ldaq::rui::FEROLDataStream::insertData( char *const data,
						    const size_t dataLength,
						    const emu::ldaq::server::PositionInEvent_t position,
						    const uint32_t l1a ){

  cout << "Inserting " << dataLength << " bytes of event " << l1a
       << ( ( emu::ldaq::server::startsEvent & position ) == emu::ldaq::server::startsEvent ? " header"  : "" )
       << ( ( emu::ldaq::server::endsEvent   & position ) == emu::ldaq::server::endsEvent   ? " trailer" : "" )
       << endl;

  // Get FED id from the first header
  if ( fedIdFromData_ == 0 && ( ( emu::ldaq::server::startsEvent & position ) == emu::ldaq::server::startsEvent ) ){
    fedIdFromData_ = FED_SOID_EXTRACT( ((fedh_t *)data)->sourceid );
    cout << "Got FED id " << fedIdFromData_ << " from event " << l1a << endl;
    // TODO: what fedId if no header?
  }
  
  // Load the data into FEROL blocks
  const char *readLocation = data;
  toolbox::mem::Reference *bufRef = NULL;
  size_t added = 0;
  size_t remaining = dataLength;
  while ( remaining ){
    // At most FEROL_BLOCK_SIZE
    size_t payloadSize = ( remaining <= maxPayloadSize_ ? remaining : maxPayloadSize_ );
    size_t blockSize   = sizeof( ferolh_t ) + payloadSize;
    // Get memory buffer
    bufRef = NULL;
    try{
      bufRef = toolbox::mem::getMemoryPoolFactory()->getFrame( pool_, blockSize  );
      // The pool factory doesn't return a buffer of blockSize but a buffer the size of power of 2 which can contain blockSize.
      // We must therefore explicitly set the exact data size we actually want to use:
      bufRef->setDataSize( blockSize );
    }
    catch(xcept::Exception &e){
      ostringstream oss;
      oss << "Failed to allocate a data block of " << blockSize << " bytes from the " << poolURN_->getNSS() << " pool.";
      XCEPT_RETHROW( xcept::Exception, oss.str(), e );
    }
    // Zero buffer contents
    unsigned char* dataLocationInBlock = (unsigned char*) bufRef->getDataLocation();
    memset( dataLocationInBlock, 0, blockSize );

    // Start with the FEROL header (see https://twiki.cern.ch/twiki/bin/viewauth/CMS/CMD_FEROL_DOC#2_Description)
    ferolh_t* ferolHeader = (ferolh_t*) dataLocationInBlock;
    ferolHeader->set_signature();
    ferolHeader->set_fed_id( fedIdFromData_ );
    ferolHeader->set_data_length( payloadSize );

    // Start counting blocks from 0 at the start of the event
    // if ( ( position & emu::ldaq::server::startsEvent ) == emu::ldaq::server::startsEvent && blockNumber_ == std::numeric_limits<uint16_t>::max() ){
    if ( ( position & emu::ldaq::server::startsEvent ) == emu::ldaq::server::startsEvent && isFirstBlock_ ){
      blockNumber_ = 0;
      l1a_ = l1a;
      ferolHeader->set_first_packet();
      // The previous fragment may not have been complete. If that's the case, then flag its last block as final.
      forcePreviousBlockToEndEvent();
      // Update first block flag for the next block
      isFirstBlock_ = false;
    }
    else{
      ++blockNumber_;
    }
    ferolHeader->set_packet_number( blockNumber_ );
    ferolHeader->set_event_number( l1a_ );
    
    if ( ( ( position & emu::ldaq::server::endsEvent ) == emu::ldaq::server::endsEvent ) && ( remaining <= maxPayloadSize_ ) ){
      // This data chunk ends the events, and this is the last of the blocks this data chunk has been filled into
      // (because this remaining data fits into one block).
      ferolHeader->set_last_packet();
      // blockNumber_ = std::numeric_limits<uint16_t>::max();
      // Update first block flag and block number for the next block
      isFirstBlock_ = true;
      blockNumber_ = 0;
    }

    // Copy the data after the FEROL header
    dataLocationInBlock += sizeof( ferolh_t );
    memcpy( dataLocationInBlock, readLocation, payloadSize );
    // Append this block to the queue
    blocks_.push_back( bufRef );
    // Update the read pointer
    readLocation += payloadSize;
    // Update the number of bytes remaining
    added += payloadSize;
    remaining = ( remaining <= payloadSize ? 0 : remaining - payloadSize );

    ostringstream oss;
    oss << "\nBlock " << ferolHeader->packet_number() << " ("
	<< ( ferolHeader->is_first_packet() ? " first " : "" )
	<< ( ferolHeader->is_last_packet()  ? " last "  : "" )
	<< ") in event " << ferolHeader->event_number()
	<< " has " << blockSize << " bytes ("
	<< ferolHeader->data_length() << " bytes of payload)\n";
    printData( oss, (char *const) bufRef->getDataLocation(), blockSize );
    if ( ferolHeader->is_first_packet() ){
      //uint64_t testdata( 0xfedcba9876543210 );
      //oss << emu::ldaq::rui::CDF_FED_Header( &testdata );
      oss << emu::ldaq::rui::CDF_FED_Header( (uint64_t *const)( ( (char *const) bufRef->getDataLocation() ) + sizeof( ferolh_t ) ) );
    }
    if ( ferolHeader->is_last_packet() ){
      oss << emu::ldaq::rui::CDF_FED_Trailer( (uint64_t *const)( ( (char *const) bufRef->getDataLocation() ) + blockSize - sizeof( fedt_t ) ) );
    }
    cout << oss.str() << endl;
  } // while ( remaining )
  cout << "Inserted " << added << " bytes of event " << l1a_ << ".  " << blocks_.size() << " blocks pending transmission" << endl;
  return added;
}

toolbox::mem::Reference* emu::ldaq::rui::FEROLDataStream::extractBlock(){
  toolbox::mem::Reference* bufRef = blocks_.front();
  blocks_.pop_front();
  cout << "Extracted a block. " << blocks_.size() << " blocks pending transmission" << endl;
  return bufRef;
}

void emu::ldaq::rui::FEROLDataStream::forcePreviousBlockToEndEvent(){
  if ( blocks_.empty() ) return;
  ferolh_t *FEROLHeader = (ferolh_t *) blocks_.back()->getDataLocation();
  if ( ! FEROLHeader->is_last_packet() ){
    FEROLHeader->set_last_packet();
  }
}

bool emu::ldaq::rui::FEROLDataStream::hasBlocksToExtract() const {
  // Return true iff there's at least one block flagged as last
  for ( deque<toolbox::mem::Reference*>::const_iterator b = blocks_.begin(); b != blocks_.end(); ++b ){
    if ( ( (ferolh_t *) (*b)->getDataLocation() )->is_last_packet() ) return true;
  }
  return false;
}

void emu::ldaq::rui::FEROLDataStream::printData(std::ostream& os, char *const data, const size_t dataLength){
  uint16_t *shortData = reinterpret_cast<uint16_t *>(data);
  os << "_________________________________" << endl;
  os << "                +3   +2   +1   +0" << endl;
  for(size_t i = 0; i < dataLength/2; i+=4)
    {
      os << std::dec;
      os.width(8); os.fill(' ');
      os << i;
      os << "      ";
      os << std::hex;
      os.width(4); os.fill('0');    
      os << shortData[i+3] << " ";
      os.width(4); os.fill('0');    
      os << shortData[i+2] << " ";
      os.width(4); os.fill('0');    
      os << shortData[i+1] << " ";
      os.width(4); os.fill('0');    
      os << shortData[i  ] << std::endl;
    }
  os<<std::dec;
  os.width(0);
}
