#include "emu/pc/TCDSInterface.h"

#include "emu/pc/EmuPeripheralCrateConfig.h"
#include "emu/supervisor/RegDumpPreprocessor.h"

#include "emu/utils/String.h"
#include "emu/utils/System.h"

#include "xdata/Integer64.h"
#include "xdata/UnsignedInteger32.h"

#include "toolbox/string.h"

#include <vector>
#include <sstream>

emu::pc::TCDSInterface::TCDSInterface( xdaq::Application *parent )
  : parent_( parent )
  , ci_( NULL )
  , pi_( NULL )
{
  // Create controller objects
  createControllers();
}



emu::pc::TCDSInterface::~TCDSInterface(){
  delete pi_;
  delete ci_;
}

void emu::pc::TCDSInterface::createControllers(){
  //
  // Find the TCDS Controller apps
  //
  std::set<const xdaq::ApplicationDescriptor *> PIs = parent_->getApplicationContext()->getDefaultZone()->getApplicationDescriptors( "tcds::pi::PIController" );
  if ( PIs.size() == 0 ){
    XCEPT_RAISE( xcept::Exception, "No tcds::pi::PIController application found." );
  }
  std::set<const xdaq::ApplicationDescriptor *> CIs = parent_->getApplicationContext()->getDefaultZone()->getApplicationDescriptors( "tcds::ici::ICIController" );
  if ( PIs.size() == 0 ){
    XCEPT_RAISE( xcept::Exception, "No tcds::ici::ICIController application found." );
  }
  //
  // Take the first ones found and create controller objects for them
  //
  try{
    // Find out which side we're on
    std::string endcap( toolbox::tolower( emu::utils::performExpansions( "$ENDCAP" ) ) );
    xdata::String partition;
    if      ( endcap == "m" || endcap == "-" ) partition = "ME-";
    else if ( endcap == "p" || endcap == "+" ) partition = "ME+";
    else{
      XCEPT_RAISE( xcept::Exception, "Value of environment parameter ENDCAP='" + endcap + "' is invalid." );
    }
    // Create the controller objects
    if ( pi_ == NULL ) pi_ = new emu::supervisor::PIControl( parent_, *PIs.begin(), partition );
    if ( ci_ == NULL ) ci_ = new emu::supervisor::CIControl( parent_, *CIs.begin(), partition );
    xdata::String runType( "local" );
    pi_->setRunType( runType );
    ci_->setRunType( runType );
  } catch( xcept::Exception &e ){
    XCEPT_RETHROW( xcept::Exception,  "Failed to create controller object for TCDS.", e );
  }
}

emu::pc::TCDSInterface& emu::pc::TCDSInterface::configureCCB(){
  //
  // Get state of TCDS
  //
  string pi_state, ci_state;
  try{
    pi_state = pi_->getSteadyState();
    ci_state = ci_->getSteadyState();
  } catch( xcept::Exception &e ){
    XCEPT_RETHROW( xcept::Exception, "Failed to get state of TCDS app.", e );
  }
  //
  // Try to halt if in Failed state
  //
  try{
    if ( ci_state == "Failed" ){
      LOG4CPLUS_WARN( parent_->getApplicationLogger(), "TCDS iCI was found in 'Failed' state. Trying to halt it.");
      if ( ! ci_->halt().waitForState( "Halted", 10 ) ){
	XCEPT_RAISE( xcept::Exception, "Failed to halt 'failed' TCDS iCI." );
      }
    }
    if ( pi_state == "Failed" ){
      LOG4CPLUS_WARN( parent_->getApplicationLogger(), "TCDS PI was found in 'Failed' state. Trying to halt it.");
      if ( ! pi_->halt().waitForState( "Halted", 10 ) ){
	XCEPT_RAISE( xcept::Exception, "Failed to halt 'failed' TCDS PI." );
      }
    }
  } catch( xcept::Exception &e ){
    XCEPT_RETHROW( xcept::Exception, "Failed to halt 'failed' TCDS.", e );
  }
  //
  // Configure if not yet configured
  //
  bool isPIToHalt = false;
  bool isCIToHalt = false;
  try{
    // First the TCDS PI...
    if ( pi_state == "Halted" ){
      LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Configuring TCDS PI");
      xdata::String  PIConfig( RegDumpPreprocessor().process( dynamic_cast<emu::pc::EmuPeripheralCrateConfig*>( parent_ )->TCDSPIConf_.toString() ) );
      xdata::Boolean usePrimaryTCDS( true );
      if ( ! pi_->configure( PIConfig, usePrimaryTCDS ).waitForState( "Configured", 10 ) ){
	XCEPT_RAISE( xcept::Exception, "Failed to configure TCDS PI." );
      }
      isPIToHalt = true;
    }
    // ...then the TCDS iCI
    if ( ci_state == "Halted" ){
      LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Configuring TCDS iCI");
      xdata::String  CIConfig( RegDumpPreprocessor().process( dynamic_cast<emu::pc::EmuPeripheralCrateConfig*>( parent_ )->TCDSCIConf_.toString() ) );
      if ( ! ci_->configure( CIConfig ).waitForState( "Configured", 10 ) ){
	XCEPT_RAISE( xcept::Exception, "Failed to configure TCDS iCI." );
      }
      isCIToHalt = true;
    }
  } catch( xcept::Exception &e ){
    XCEPT_RETHROW( xcept::Exception, "Failed to halt TCDS.", e );
  }
  //
  // Executing TCDS iCI configure sequence including a command to clear the CCBs' discrete logic decoder
  //
  try{
    if ( ! pi_->waitForState( "Configured|Enabled|Paused", 30 ) ){
      XCEPT_RAISE( xcept::Exception, "TCDS PI failed to reach Configured|Enabled|Paused state." );
    }
    if ( ci_->waitForState( "Configured|Enabled|Paused", 30 ) ){
      LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Executing TCDS iCI configure sequence including a command to clear the CCBs' discrete logic decoder");
      ci_->configureSequence();
    }
    else{
      XCEPT_RAISE( xcept::Exception, "TCDS iCI failed to reach Configured|Enabled|Paused state." );
    }
  } catch( xcept::Exception &e ){
    XCEPT_RETHROW( xcept::Exception, "Failed to execute iCI configure sequence.", e );
  }
  //
  // Halt them if they were originally halted in order not to go on holding the hardware lease.
  //
  bool CIFailedToHalt = isCIToHalt;
  bool PIFailedToHalt = isPIToHalt;
  try{
    if ( isCIToHalt ){
      LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Halting TCDS iCI.");
      if ( ci_->halt().waitForState( "Halted", 10 ) ) CIFailedToHalt = false;
    }
    if ( isPIToHalt ){
      LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Halting TCDS PI.");
      if ( pi_->halt().waitForState( "Halted", 10 ) ) PIFailedToHalt = false;
    }
    if ( CIFailedToHalt || PIFailedToHalt ){
      ostringstream msg;
      msg << "Failed to halt TCDS " 
	  << ( CIFailedToHalt ? "iCI" : "" ) 
	  << ( CIFailedToHalt && PIFailedToHalt ? " and " : "" ) 
	  << ( PIFailedToHalt ? "PI" : "" ) << " due to timeout.";
      XCEPT_RAISE( xcept::Exception, msg.str() );
    }
  } catch( xcept::Exception &e ){
      ostringstream msg;
      msg << "Failed to halt TCDS " 
	  << ( CIFailedToHalt ? "iCI" : "" ) 
	  << ( CIFailedToHalt && PIFailedToHalt ? " and " : "" ) 
	  << ( PIFailedToHalt ? "PI" : "" ) << ".";
    XCEPT_RETHROW( xcept::Exception, msg.str(), e );
  }
  return *this;
}
