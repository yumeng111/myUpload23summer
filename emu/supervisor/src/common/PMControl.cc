#include "emu/supervisor/PMControl.h"

#include "xdata/UnsignedInteger32.h"

emu::supervisor::PMControl::PMControl( xdaq::Application *parent, 
				       const xdaq::ApplicationDescriptor* tcdsApplicationDescriptor, 
				       xdata::String partition )
  : emu::supervisor::TCDSControl::TCDSControl( parent, tcdsApplicationDescriptor, partition, "PM" ){}

emu::supervisor::PMControl& emu::supervisor::PMControl::setRunType( xdata::String& runType ){
  this->emu::supervisor::TCDSControl::setRunType( runType );
  return *this;
}

emu::supervisor::PMControl& emu::supervisor::PMControl::configure( xdata::String& hardwareConfigurationString ){
  //
  // First, set the FED enable mask.
  //
  xdata::UnsignedInteger32 fedId( 1029 ); // This is supposed to be our LPM's FED id.
  // Trusting is good, but verifying is better. Try to retrieve LPM's FED id from it itself:
  try{
    messenger_->getParameters( tcdsApplicationDescriptor_, emu::soap::Parameters().add( "fedId", &fedId ) );
  }
  catch( xcept::Exception& e ){
    std::stringstream ss;
    ss << "Failed to get PM's FED id. Continuing assuming it is " << fedId.toString() 
       << "." << xcept::stdformat_exception_history(e);
    LOG4CPLUS_WARN( parentApplication_->getApplicationLogger(), ss.str() );
  }
  // In order to enable backpressure/DAQ readout, use xxx&1%, with xxx your LPM FED id. Otherwise, use xxx&0%.
  fedEnableMask_ = fedId.toString() + "&0%";

  //
  // Next, set the TTC partition map
  //
  // See https://cmsdoc.cern.ch/cms/TRIDAS/RCMS/Docs/Manuals/manuals/level1FMFSM_1_10_2.pdf
  // Jeroen 2022-02-11: "All partitions known to the CPMController/LPMController need to be specified in the TTC partition vector."
  ttcPartitionMap_ = "CSC+&3%CSC-&3%GEM+&0%GEM-&0%GEMPILOT&0%";

  //
  // Now configure with that FED enable mask and TTC partition map.
  //
  hardwareConfiguration_ = hardwareConfigurationString;
  string initialState( waitForASteadyState( 20 ) );
  if ( initialState == "Halted" ){
    messenger_->sendCommand( tcdsApplicationDescriptor_, "Configure",
			     emu::soap::Parameters()
			     .add( "hardwareConfigurationString", &hardwareConfiguration_ )
			     .add( "fedEnableMask"              , &fedEnableMask_         ) 
			     .add( "ttcPartitionMap"            , &ttcPartitionMap_       ), 
			     emu::soap::Attributes().add( "actionRequestorId", &actionRequestorId_ ) );
  }
  else{
    ostringstream oss;
    oss << "Attempted to 'Configure' " << tcdsApplicationDescriptor_->getClassName() 
	<< " of "                      << tcdsApplicationDescriptor_->getAttribute( "service" ) 
	<< " (instance "               << tcdsApplicationDescriptor_->getInstance()
	<< ") when it is in '"         << initialState << "' state.";
    XCEPT_RAISE( xcept::Exception, oss.str() );
  }
  toolbox::TimeInterval interval;
  interval.fromString( "PT5S" );
  hardwareLease_.reset( new TCDSHardwareLease( parentApplication_,
					       tcdsApplicationDescriptor_,
					       actionRequestorId_,
					       interval                    ) );
  return *this;
}


emu::supervisor::PMControl& emu::supervisor::PMControl::configureSequence(){
  switch ( runType_ ){
  case global:
    // In global, we're not in control of the PM, therefore:
    return *this;
  case local:           // fall through
  case AFEBcalibration: // fall through
  case CFEBcalibration: 
    // First of all, wait for PM to complete 'Configure' transition
    if ( ! waitForState( "Configured", 20 ) ){ XCEPT_RAISE( xcept::Exception, "TCDS LPM failed to reach Configured state." ); }
    break;
  default:
    XCEPT_RAISE( xcept::Exception, "Unknown run type." );
  }
  return *this;
}

emu::supervisor::PMControl& emu::supervisor::PMControl::enableSequence(){
  switch ( runType_ ){
  case global:
    // In global, we're not in control of the PM, therefore:
    return *this;
  case local:           // fall through
  case AFEBcalibration: // fall through
  case CFEBcalibration:
    // First of all, wait for PM to complete 'Enable' transition
    if ( ! waitForState( "Enabled", 20 ) ){ XCEPT_RAISE( xcept::Exception, "TCDS LPM failed to reach Enabled state." ); }
    break;
  default:
    XCEPT_RAISE( xcept::Exception, "Unknown run type." );
  }
  return *this;
}

emu::supervisor::PMControl& emu::supervisor::PMControl::stopSequence(){
  switch ( runType_ ){
  case global:
    // In global, we're not in control of the PM, therefore:
    return *this;
  case local:           // fall through
  case AFEBcalibration: // fall through
  case CFEBcalibration:
    // First of all, wait for PM to complete the 'Stop' transition
    if ( ! waitForState( "Configured", 20 ) ){ XCEPT_RAISE( xcept::Exception, "TCDS LPM failed to reach Configured state." ); }
    break;
  default:
    XCEPT_RAISE( xcept::Exception, "Unknown run type." );
  }
  return *this;
}
