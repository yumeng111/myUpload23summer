#include "emu/pc/FEDInterface.h"

#include "emu/utils/System.h"
#include "emu/utils/DOM.h"
#include "emu/utils/Chamber.h"

#include "xdata/Integer64.h"
#include "xdata/UnsignedInteger32.h"

#include <vector>
#include <sstream>

emu::pc::FEDInterface::FEDInterface( xdaq::Application *parent, set<string> chamberLabels )
  : parent_( parent )
  , chamberLabels_( chamberLabels )
  , messenger_( new emu::soap::Messenger( parent_ ) )
{
}

emu::pc::FEDInterface::~FEDInterface(){
  delete messenger_;
}

void emu::pc::FEDInterface::startFED( bool inPassthroughMode ){
  // If we use FED crate(s), we control the FED system here. (If the DDU is in the PCrate, we'll set it up together with it for each individual test.)
  set<const xdaq::ApplicationDescriptor *> apps = parent_->getApplicationContext()->getDefaultZone()->getApplicationDescriptors( "emu::fed::Communicator" );

  if ( apps.size() == 0 ){
    LOG4CPLUS_WARN( parent_->getApplicationLogger(), "No emu::fed::Communicator applications found. Will look for DDU in the peripheral crate instead." );
    return;
  }
  else{
    LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Found " << apps.size() << " emu::fed::Communicator application(s)." );
  }

  // Make three attempts to set up FED. (Sometimes it fails...)
  const int nAttempts = 3;
  bool successful = false;
  int iAttempt = 1;
  while ( iAttempt <= nAttempts && !successful ){
    try{
      // Get the FED settings file names
      set<string> fedSettingsFileNames;
      for ( set<const xdaq::ApplicationDescriptor*>::iterator app = apps.begin(); app != apps.end(); ++app ) {
	xdata::String xmlFileName;
	messenger_->getParameters( *app, emu::soap::Parameters().add( "xmlFileName", &xmlFileName ) );
	fedSettingsFileNames.insert( xmlFileName.toString() );
	LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Found " << (*app)->getClassName() << " of instance " << (*app)->getInstance() << " with settings in " << xmlFileName.toString() );
      }

      // Build XPath expression to unkill tested chambers' fibers in the FED settings files
      for ( set<string>::iterator fn = fedSettingsFileNames.begin(); fn != fedSettingsFileNames.end(); ++fn ) {
	string fedSettingsXML = emu::utils::readFile( emu::utils::performExpansions( *fn ) );
	if ( fedSettingsXML.size() == 0 ){
	  XCEPT_RAISE( xcept::Exception, "FED configuration file '" + *fn + "' could not be read in or is empty." );
	}
	// First kill all chambers' fibers...
	fedSettingsXML = emu::utils::setSelectedNodesValues( fedSettingsXML, "//FEDSystem/FEDCrate/DDU/Fiber/@KILLED" , "1" );
	// ...then unkill the tested chambers'
	for ( set<string>::iterator iChamber = chamberLabels_.begin(); iChamber != chamberLabels_.end(); ++iChamber ){
	  string chamberName = emu::utils::Chamber( *iChamber ).name().substr( 2 ); // Chop off 'ME'
	  // In the FED settings XML file, chamber names are zero-padded, but without "ME", e.g., CHAMBER="-1/2/08"
	  // while in the PCrate settings XML file, it's the other way round...
	  fedSettingsXML = emu::utils::setSelectedNodesValues( fedSettingsXML, "//FEDSystem/FEDCrate/DDU/Fiber[@CHAMBER='" + chamberName + "']/@KILLED", "0" );
	  // Also, collect the RUIs that are supposed to read data:
	  string RUI = emu::utils::getSelectedNodeValue( fedSettingsXML, "//FEDSystem/FEDCrate/DDU[Fiber/@CHAMBER='" + chamberName + "']/@RUI" );
	  LOG4CPLUS_INFO( parent_->getApplicationLogger(), "FED settings XML in " << emu::utils::performExpansions( *fn ) << ":\n" << fedSettingsXML << "\n" );
	  if ( RUI.length() > 0 ){
	    LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Found RUI '" << RUI << "' in FED settings XML to read out chamber " << chamberName );
	  }
	  else{
	    LOG4CPLUS_ERROR( parent_->getApplicationLogger(), "Found no RUI in FED settings XML to read out chamber " << chamberName );
	  }
	}
	// cout << fedSettingsXML << endl;
	utils::writeFile( *fn, fedSettingsXML );
	LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Updated FED Crates' configuration XML in " << *fn );
      }
      
      // Halt all Communicators
      messenger_->sendCommand( "emu::fed::Communicator", "Halt" );

      // Configure DDUs in passthrough mode
      // This will write 0x8787 to the FakeL1A register.
      // It will be cleared if you send any reset stronger than a Resync; it's not affected by Resync.
      xdata::Boolean dduInPassthroughMode = inPassthroughMode;
      // Also, tell the Communicators to configure from the XML file
      xdata::String configMode = "XML";
      messenger_->setParameters( "emu::fed::Communicator",
				 emu::soap::Parameters()
				 .add( "dduInPassthroughMode", &dduInPassthroughMode )
				 .add( "configMode"          , &configMode           )
				 );
      
      messenger_->sendCommand( "emu::fed::Communicator", "Configure" );
      sleep( 1 );

      // Start all Communicators
      messenger_->sendCommand( "emu::fed::Communicator", "Enable" );
      sleep( 1 );
      successful = true;
    }
    catch ( xcept::Exception &e ){
      ostringstream oss;
      oss << "Attempt " << iAttempt << " to set up FED failed. " << xcept::stdformat_exception_history( e );
      LOG4CPLUS_WARN( parent_->getApplicationLogger(), oss.str() );
      if ( iAttempt == nAttempts ){
	oss.str("");
	oss << "All " << nAttempts << " attempts to set up FED failed.";
	XCEPT_RETHROW( xcept::Exception, oss.str(), e );
      }
      iAttempt++;
      sleep( 2 );
    }
  } // while ( iAttempt <= nAttempts && !successful ){
  LOG4CPLUS_INFO( parent_->getApplicationLogger(), "Configured DDUs in " << ( inPassthroughMode ? "passthrough" : "normal" ) << " mode." );
}

void emu::pc::FEDInterface::haltFED(){
  // If we use FED crate(s), we control the FED system here. (If the DDU is in the PCrate, we'll set it up together with it for each individual test.)
  if ( parent_->getApplicationContext()->getDefaultZone()->getApplicationDescriptors( "emu::fed::Communicator" ).size() == 0 ) return;

  // Set flag in Communicators to normal (not passthrough) mode so that on next configure the DDUs are *not* configured for passthrough mode
  // in case a non-STEP run follows.
  xdata::Boolean dduInPassthroughMode = false;
  // Also, return to configuring from database.
  xdata::String configMode = "Database";
  messenger_->setParameters( "emu::fed::Communicator",
			     emu::soap::Parameters()
			     .add( "dduInPassthroughMode", &dduInPassthroughMode )
			     .add( "configMode"          , &configMode           )
			     );
  // Halt all Communicators
  messenger_->sendCommand( "emu::fed::Communicator", "Halt" );
}
