#include "emu/pc/PipelineDepthScan.h"

#include "emu/pc/CCB.h"
#include "emu/pc/TMB.h"
#include "emu/pc/ALCTController.h"
#include "emu/pc/FEDInterface.h"
#include "emu/pc/LocalDAQInterface.h"
#include "emu/soap/Messenger.h"
#include "emu/utils/System.h"

#include "xdata/Integer64.h"
#include "xdata/UnsignedInteger32.h"

#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>

emu::pc::PipelineDepthScan::PipelineDepthScan( xdaq::Application *parent, emu::pc::DAQMB* dmb )
  : parent_( parent )
{
  hwVersions_.insert( dmb->GetHardwareVersion() );
  DMBs_.insert( dmb );
  crates_.insert( dmb->getCrate() );
}

emu::pc::PipelineDepthScan::PipelineDepthScan( xdaq::Application *parent, emu::pc::Crate* crate, set<unsigned int> hwVersions )
  : parent_    ( parent     )
  , hwVersions_( hwVersions )
{
  // Collect the DMBs of the requested hardware version
  std::vector<DAQMB*> DMBs = crate->daqmbs();
  for ( std::vector<DAQMB*>::iterator iDMB = DMBs.begin(); iDMB != DMBs.end(); ++iDMB ){
    if ( hwVersions_.find( (*iDMB)->GetHardwareVersion() ) != hwVersions_.end() ){
      DMBs_.insert( *iDMB );
      crates_.insert( (*iDMB)->getCrate() );
    }
  }
}

emu::pc::PipelineDepthScan::PipelineDepthScan( xdaq::Application *parent, emu::pc::EmuEndcap* endcap, set<unsigned int> hwVersions )
  : parent_    ( parent     )
  , hwVersions_( hwVersions )
{
  // Collect the DMBs of the requested hardware version
  std::vector<DAQMB*> DMBs = endcap->daqmbs();
  for ( std::vector<DAQMB*>::iterator iDMB = DMBs.begin(); iDMB != DMBs.end(); ++iDMB ){
    if ( hwVersions_.find( (*iDMB)->GetHardwareVersion() ) != hwVersions_.end() ){
      DMBs_.insert( *iDMB );
      crates_.insert( (*iDMB)->getCrate() );
    }
  }
}

emu::pc::PipelineDepthScan::~PipelineDepthScan(){
}

void emu::pc::PipelineDepthScan::run( int fromDepth, int toDepth, int incrementDepth, unsigned int durationInSec ){
  if ( fromDepth > toDepth ||
       fromDepth > 0xff    ||
       fromDepth < 0       ||
       toDepth   > 0xff    ||
       toDepth   < 0
       ) return; // TODO throw

  set<string> chamberLabels;
  
  string now( emu::utils::getDateTime( true ) );
  string chamberListName( "/tmp/PipelineDepthScan_" + now + "_chambers.txt" );
  string    dataListName( "/tmp/PipelineDepthScan_" + now + "_data.txt"     );
  string analysisLogName( "/tmp/PipelineDepthScan_" + now + "_analysis.log" );
  ofstream dataList, chamberList;

  chamberList.open( chamberListName.c_str() );
  for ( std::set<DAQMB*>::iterator dmb = DMBs_.begin(); dmb != DMBs_.end(); ++dmb ){
    chamberLabels.insert( (*dmb)->GetLabel() );
    chamberList << (*dmb)->crate() << "\t" << (*dmb)->slot() << "\t" << (*dmb)->GetLabel() << endl;
  }
  chamberList.close();

  // Tell TCDS not to send L1As now
  pauseL1A();
  
  FEDInterface fed( parent_, chamberLabels );
  fed.startFED( true );

  // setSingleLayerTrigger(); // TODO: not in pulsed STEP runs!
  
  LocalDAQInterface ldaq( parent_ );

  // Loop over pipeline depth values and take data at each
  const int nSteps = abs( ( toDepth - fromDepth ) / incrementDepth + 1 );
  for ( int depth = fromDepth; depth <= toDepth; depth+=incrementDepth ){
    
    setPipelineDepth( depth );

    //
    // Start local DAQ and L1A, wait, and then stop them
    //
    ostringstream runType;
    runType << "Pipeline" << setw(3) << setfill('0') << depth;
    LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Starting lDAQ run " << runType.str() );
    ldaq.startRun( runType.str() );
    resumeL1A();
    LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Waiting " << ceil( float(durationInSec) / nSteps ) << " s" );
    sleep( ceil( float(durationInSec) / nSteps ) );
    pauseL1A();
    LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Stopping lDAQ run " << runType.str() );
    ldaq.endRun();
    
    set<string> dataFileNames( ldaq.getDataFileNames() );
    dataList.open( dataListName.c_str(), ios_base::trunc );
    for ( set<string>::iterator dfn = dataFileNames.begin(); dfn != dataFileNames.end(); ++dfn ){
      dataList << *dfn << endl;
    }
    dataList.close();
  }

  fed.haltFED();

  ostringstream analyze;
  analyze << "analyzePipelineDepthScanWithDAQ.exe "
	  << chamberListName                        << " "
	  << dataListName                           << " > "
	  << analysisLogName                        << " 2>&1 &";
  LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Executing shell command\n" + analyze.str() );
  try{
    emu::utils::execShellCommand( analyze.str() );
  }
  catch( xcept::Exception& e ){
    LOG4CPLUS_ERROR( parent_->getApplicationLogger(), "Error in pipeline depth scan analysis: " << stdformat_exception_history( e ) );
  }
}

void emu::pc::PipelineDepthScan::setPipelineDepth( const int pipelineDepth ){
  
  for ( std::set<DAQMB*>::iterator dmb = DMBs_.begin(); dmb != DMBs_.end(); ++dmb ){

    vector <emu::pc::CFEB> cfebs = (*dmb)->cfebs();
    for( vector<emu::pc::CFEB>::reverse_iterator cfeb = cfebs.rbegin(); cfeb != cfebs.rend(); ++cfeb){

      (*dmb)->dcfeb_set_PipelineDepth( *cfeb, pipelineDepth ); // set the pipeline depth
      usleep( 100000 );
      (*dmb)->Pipeline_Restart( *cfeb ); // and then restart the pipeline
      usleep( 100000 );
      
      if( (*dmb)->DMBversion() <= 1  &&  (*dmb)->CFEBversion() > 1 ) {
	// set DCFEBs to behave like CFEBs and send data on any L1A, required when not using ODMB
	(*dmb)->dcfeb_Set_ReadAnyL1a( *cfeb );
      }
      
      (*dmb)->shift_all( NORM_RUN );
      (*dmb)->buck_shift();
      usleep( 100000 );

      LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Set pipeline depth to " << pipelineDepth << " on " << (*dmb)->GetLabel() << " in DCFEB #" << cfeb->number() );
    }
    
  }

  // for ( std::set<Crate*>::iterator iCrate = crates_.begin(); iCrate != crates_.end(); ++iCrate ){
  //   (*iCrate)->ccb()->l1aReset(); // need to do this after restarting DCFEB pipelines
  // }
  
}

void emu::pc::PipelineDepthScan::setSingleLayerTrigger(){
  // Basically a copy of EmuPeripheralCrateConfig::SetRadioactivityTrigger,
  // but only acting on the scanned chambers.

  for ( std::set<DAQMB*>::iterator dmb = DMBs_.begin(); dmb != DMBs_.end(); ++dmb ){

    emu::pc::TMB *tmb = (*dmb)->getCrate()->GetChamber( *dmb )->GetTMB();
    emu::pc::ALCTController *alct = tmb->alctController();

    int initial_alct_nplanes_hit_pretrig = alct->GetPretrigNumberOfLayers();
    int initial_alct_nplanes_hit_pattern = alct->GetPretrigNumberOfPattern();
    alct->SetPretrigNumberOfLayers(1);
    alct->SetPretrigNumberOfPattern(1);
    alct->WriteConfigurationReg();

    int initial_clct_nplanes_hit_pretrig = tmb->GetHsPretrigThresh();
    int initial_clct_nplanes_hit_pattern = tmb->GetMinHitsPattern();
    tmb->SetHsPretrigThresh(1);
    tmb->SetMinHitsPattern(1);
    tmb->WriteRegister(0x70);

    // set the number of BX's that a CFEB channel must be ON in order for TMB to be labeled as "bad"
    int initial_cfeb_badbits_nbx = tmb->GetCFEBBadBitsNbx();
    tmb->SetCFEBBadBitsNbx(20);
    tmb->WriteRegister(0x124);

    // Reset the software back to the initial values.  Leave the hardware in radioactivity mode...
    alct->SetPretrigNumberOfLayers(initial_alct_nplanes_hit_pretrig);
    alct->SetPretrigNumberOfPattern(initial_alct_nplanes_hit_pattern);

    tmb->SetHsPretrigThresh(initial_clct_nplanes_hit_pretrig);
    tmb->SetMinHitsPattern(initial_clct_nplanes_hit_pattern);

    tmb->SetCFEBBadBitsNbx(initial_cfeb_badbits_nbx);
  }
}
  
void emu::pc::PipelineDepthScan::pauseL1A(){
  xdata::String actionRequestorId_= "PM-ME";
  try{
    emu::soap::Messenger( parent_ ).sendCommand( "tcds::lpm::LPMController", "Pause",
						 emu::soap::Parameters::none, 
						 emu::soap::Attributes().add( "actionRequestorId", &actionRequestorId_ ) );
  }
  catch( xcept::Exception &e ){
    LOG4CPLUS_WARN( parent_->getApplicationLogger(), "Failed to pause L1A by command to LPM"  << xcept::stdformat_exception_history(e) );
  }
}
  
void emu::pc::PipelineDepthScan::resumeL1A(){
  xdata::String actionRequestorId_= "PM-ME";
  try{
    emu::soap::Messenger( parent_ ).sendCommand( "tcds::lpm::LPMController", "Resume",
						 emu::soap::Parameters::none, 
						 emu::soap::Attributes().add( "actionRequestorId", &actionRequestorId_ ) );
  }
  catch( xcept::Exception &e ){
    LOG4CPLUS_WARN( parent_->getApplicationLogger(), "Failed to resume L1A by command to LPM"  << xcept::stdformat_exception_history(e) );
  }
}
