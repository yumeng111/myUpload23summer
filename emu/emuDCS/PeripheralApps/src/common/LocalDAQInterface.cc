#include "emu/pc/LocalDAQInterface.h"

#include "xdata/Integer64.h"
#include "xdata/UnsignedInteger32.h"

#include <vector>
#include <sstream>

emu::pc::LocalDAQInterface::LocalDAQInterface( xdaq::Application *parent )
  : parent_( parent )
  , messenger_( new emu::soap::Messenger( parent_ ) )
{
}

emu::pc::LocalDAQInterface::~LocalDAQInterface(){
  delete messenger_;
}

void emu::pc::LocalDAQInterface::startRun( string type ){
  const uint64_t daqTimeOutInSeconds = 15;
  //
  // Halt in case it's not halted
  //
  messenger_->sendCommand( "emu::ldaq::manager::Application", "Halt" );      
  if ( ! successfullyExecuted( "Halt", daqTimeOutInSeconds ) ){
    XCEPT_RAISE( xcept::Exception, string( "DAQ failed to execute 'Halt' in ") + utils::stringFrom<uint64_t>( daqTimeOutInSeconds ) + " seconds." );
  }
  //
  // Configure
  //
  xdata::String             runType   = type;
  xdata::Integer64  maxNumberOfEvents = -1; // unlimited if negative
  xdata::Boolean writeBadEventsOnly   = false;
  messenger_->setParameters( "emu::ldaq::manager::Application", 
			     emu::soap::Parameters()
			     .add( "runType"           , &runType            )
			     .add( "maxNumberOfEvents" , &maxNumberOfEvents  )
			     .add( "writeBadEventsOnly", &writeBadEventsOnly ) );
  messenger_->sendCommand( "emu::ldaq::manager::Application", "Configure" );      
  if ( ! successfullyExecuted( "Configure", daqTimeOutInSeconds ) ){
    XCEPT_RAISE( xcept::Exception, string( "DAQ failed to execute 'Configure' in ") + utils::stringFrom<uint64_t>( daqTimeOutInSeconds ) + " seconds." );
  }
  //
  // Enable
  //
  messenger_->sendCommand( "emu::ldaq::manager::Application", "Enable" );
  if ( ! successfullyExecuted( "Enable", daqTimeOutInSeconds ) ){
    XCEPT_RAISE( xcept::Exception, string( "DAQ failed to execute 'Enable' in ") + utils::stringFrom<uint64_t>( daqTimeOutInSeconds ) + " seconds." );
  }
  // Get the number and start time of run and the data directories from the local DAQ and pass them on to the Testers
  xdata::String runStartTime;
  xdata::UnsignedInteger32 runNumber;
  xdata::Vector<xdata::String> dataDirNames; // all RUIs' data directory names
  messenger_->getParameters( "emu::ldaq::manager::Application", 0, 
			     emu::soap::Parameters()
			     .add( "runNumber"   , &runNumber    )
			     .add( "runStartTime", &runStartTime )
			     .add( "dataDirNames", &dataDirNames ) );
}

void emu::pc::LocalDAQInterface::endRun(){
  const uint64_t daqTimeOutInSeconds = 15;
  messenger_->sendCommand( "emu::ldaq::manager::Application", "Halt" );
  if ( ! successfullyExecuted( "Halt", daqTimeOutInSeconds ) ){
    XCEPT_RAISE( xcept::Exception, string( "DAQ failed to execute 'Halt' in ") + utils::stringFrom<uint64_t>( daqTimeOutInSeconds ) + " seconds." );
  }
  updateDataFileNames();
}

bool emu::pc::LocalDAQInterface::successfullyExecuted( const string command, const uint64_t timeoutInSeconds ){
  string expectedState;
  if      ( command == "Configure" ){ expectedState = "Configured"; }
  else if ( command == "Enable"    ){ expectedState = "Enabled";    }
  else if ( command == "Halt"      ){ expectedState = "Halted";     }
  else                              { return true; }

  // Poll, and return TRUE if and only if DAQ gets into the expected state before timeout.
  xdata::String  daqState;
  for ( uint64_t i=0; i<=timeoutInSeconds; ++i ){
    messenger_->getParameters( "emu::ldaq::manager::Application", 0, emu::soap::Parameters().add( "daqState", &daqState ) );
    if ( daqState.toString() == "Failed" ){
      LOG4CPLUS_ERROR( parent_->getApplicationLogger(), "Local DAQ is in 'Failed' state. Please destroy and recreate local DAQ." );
      return false;
    }
    if ( daqState.toString() == expectedState ){ return true; }
    LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Waited " << i << " sec so far for local DAQ to get " 
		    << expectedState << ". It is still in " << daqState.toString() << " state." );
    ::sleep( 1 );
  }

  LOG4CPLUS_ERROR( parent_->getApplicationLogger(), "Timeout after waiting " << timeoutInSeconds << " sec for local DAQ to get " << expectedState 
		   << ". It is in " << daqState.toString() << " state." );
  return false;
}

void emu::pc::LocalDAQInterface::updateDataFileNames(){
  // Make three attempts get data file names. (Sometimes it fails...)
  const int nAttempts = 3;
  bool successful = false;
  int iAttempt = 1;
  while ( iAttempt <= nAttempts && !successful ){
    try{
      // Get data file names written in this test
      xdata::Vector<xdata::String> dataFileNames;
      messenger_->getParameters( "emu::ldaq::manager::Application", 0, emu::soap::Parameters().add( "dataFileNames", &dataFileNames ) );
      // cout << "dataFileNames" << dataFileNames.toString() << endl;
      // Add them to the list of all data file names written in this run
      for ( size_t iFile = 0; iFile < dataFileNames.elements(); iFile++ ){
	dataFileNames_.insert( ( dynamic_cast<xdata::String*>( dataFileNames.elementAt( iFile ) ) )->toString() );
      }
      // if ( dataFileNames_.size() > 0 ) XCEPT_RAISE( xcept::Exception, "Forced exception for test purposes." );
      successful = true;
    } catch( xcept::Exception& e ){
      ostringstream oss;
      oss << "Attempt " << iAttempt << " to get data file names failed. " << xcept::stdformat_exception_history( e );
      LOG4CPLUS_WARN( parent_->getApplicationLogger(), oss.str() );
      if ( iAttempt == nAttempts ){
	XCEPT_RETHROW( xcept::Exception, "Failed to get data file names.", e );
      }
      iAttempt++;
      sleep( 2 );
    } catch( std::exception& e ){
      ostringstream oss;
      oss << "Attempt " << iAttempt << " to get data file names failed. " << e.what();
      LOG4CPLUS_WARN( parent_->getApplicationLogger(), oss.str() );
      if ( iAttempt == nAttempts ){
	XCEPT_RAISE( xcept::Exception, string( "Failed to get data file names: ") + e.what() );
      }
      iAttempt++;
      sleep( 2 );
    } catch( ... ){
      ostringstream oss;
      oss << "Attempt " << iAttempt << " to get data file names failed. Unknown exception.";
      LOG4CPLUS_WARN( parent_->getApplicationLogger(), oss.str() );
      if ( iAttempt == nAttempts ){
	XCEPT_RAISE( xcept::Exception, "Failed to get data file names. Unknown exception." );
      }
      iAttempt++;
      sleep( 2 );
    }
  }// while ( iAttempt <= nAttempts && !successful )

  cout << (dynamic_cast<ostringstream*>( &(ostringstream() << iAttempt << "attempts made.") ))->str() << endl;
}
