package rcms.fm.ajaxFM;

import java.util.List;
import java.util.ArrayList;
import java.util.Date;

import java.io.StringReader;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
//import java.util.logging.Level;
//import java.util.logging.Logger;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import rcms.util.logger.RCMSLogger;

import rcms.resourceservice.db.resource.config.ConfigProperty;
import rcms.resourceservice.db.resource.Resource;

import rcms.stateFormat.StateNotification;

import rcms.errorFormat.CMS.CMSError;

import rcms.xdaqctl.XDAQParameter;
import rcms.xdaqctl.XDAQDocument;

import rcms.fm.fw.StateEnteredEvent;
import rcms.fm.fw.user.UserActionException;
import rcms.fm.fw.parameter.CommandParameter;
import rcms.fm.fw.parameter.FunctionManagerParameter;
import rcms.fm.fw.parameter.ParameterSet;
import rcms.fm.fw.parameter.ParameterException;
import rcms.fm.fw.parameter.type.StringT;
import rcms.fm.fw.parameter.type.BooleanT;
import rcms.fm.fw.parameter.type.IntegerT;
import rcms.fm.fw.parameter.type.UnsignedIntegerT;
import rcms.fm.fw.parameter.type.VectorT;

import rcms.fm.resource.QualifiedResource;
//import rcms.fm.resource.qualifiedresource.XdaqApplicationContainer;
import rcms.fm.resource.qualifiedresource.XdaqApplication;
import rcms.fm.resource.qualifiedresource.XdaqExecutive;
import rcms.fm.resource.qualifiedresource.JobControl;
import rcms.fm.resource.qualifiedresource.XdaqExecutiveConfiguration;

//import rcms.fm.app.cscL1FM.MyFunctionManager;
//import rcms.fm.app.cscL1FM.MyParameters;
//import rcms.fm.app.cscL1FM.MyStates;

import rcms.fm.ajaxFM.myParameters.MyParameterSet;
import rcms.fm.ajaxFM.MyStateMachineDefinition;

import rcms.utilities.runinfo.RunInfo;
import rcms.utilities.runinfo.RunInfoConnectorIF;
//import rcms.utilities.runinfo.RunInfoException;
import rcms.utilities.runinfo.RunNumberData;
import rcms.utilities.runinfo.RunSequenceNumber;

import net.hep.cms.xdaqctl.DefaultNamespaceContext;
import net.hep.cms.xdaqctl.XDAQException;
import net.hep.cms.xdaqctl.XPathAPI;
import net.hep.cms.xdaqctl.XPathAPIException;
//import static rcms.fm.ajaxFM.MyEventHandler.logger;
import rcms.fm.resource.CommandException;
import rcms.statemachine.StateMachineException;
import rcms.statemachine.definition.StateMachineDefinitionException;

public class CSC {
    /**
     * <code>logger</code>: log4j logger.
     */
    static RCMSLogger logger = new RCMSLogger(CSC.class);

    private MyFunctionManager functionManager = null;

    public XdaqApplication supervisor = null;

    public CSC(MyFunctionManager functionManager) {
	super();
	this.functionManager = functionManager;
    }


    @SuppressWarnings("unchecked")
    public void initAction(Object obj) throws UserActionException {
        // get the parameters of the incoming command
        ParameterSet<CommandParameter> inCommandParSet  = functionManager.getLastInput().getParameterSet();
        String msg = inCommandParSet.getParameters().size()+" initialize command parameters received:";
        msg = inCommandParSet.getParameters().stream().map( (cp) -> " "+cp.getName() +"="+cp.getValue() ).reduce( msg, String::concat );
        logger.info( msg );

	findSupervisor();
        if ( supervisor != null ){
            // Add Supervisor-related parameters:
            functionManager.getParameterSet().get(MyParameterSet.CSC_RUN_TYPES_AVAILABLE_V).setValue( getRunTypesAvailableVector() );
            functionManager.getParameterSet().get(MyParameterSet.CSC_RUN_TYPE             ).setValue( new StringT( getRunType() )  );
        }
    }

    @SuppressWarnings("unchecked")
    public void configureAction(Object obj) throws UserActionException {
        //((FunctionManagerParameter<StringT>)functionManager.getParameterSet().get(MyParameterSet.STATE)).setValue( new StringT( functionManager.getState().getStateString() ) );
        //if (true) return;
        logger.info( getClass().getName() +  ".configureAction called for "+obj.getClass().getName()+". FM state is " + functionManager.getState().getStateString() );
	if (obj instanceof StateNotification) {
            if ( supervisor != null ){
                if ( supervisor.getCacheState().equals( MyStates.CONFIGURED ) ){                    
                    try {
                        // go to CONFIGURED
                        // Do not define SETCONFIGURED transition explicitly so that it doesn't appear on the GUI.
                        // functionManager.fireEvent( MyInputs.SETCONFIGURED );
                        // Simply set the state to CONFIGURED instead:
                        functionManager.setState(MyStates.CONFIGURED);
                    } catch (StateMachineException ex) {
                        logger.error( "Failed to set CONFIGURED" , ex );
                    }
                }
                else if ( supervisor.getCacheState().equals( MyStates.ERROR      ) ){
                    String errorMsg = "Failed to Configure " + supervisor.getName() + " ";
                    errorMsg += getXdaqAppsReasonForFailure( supervisor );
                    logger.error( getClass().getName() + ": " + errorMsg );
                    sendCMSError( errorMsg );
                    logger.info( "About to go into error from FM state "+functionManager.getState().getStateString() );
                    try{
                        functionManager.setState(MyStates.ERROR);
                    } catch (StateMachineException ex) {
                        logger.error( "Failed to set ERROR" , ex );
                    }
                }
            }
            ((FunctionManagerParameter<StringT>)functionManager.getParameterSet().get(MyParameterSet.STATE)).setValue( new StringT( functionManager.getState().getStateString() ) );
	}
	else if (obj instanceof StateEnteredEvent) {
	    if ( supervisor == null ){
		// Nothing to control, go immediately to steady state.
                // TODO: Make no state transition at all. If we have no Supervisor, we're not supposed to have a state machine either.
		// functionManager.fireEvent( MyInputs.SETCONFIGURED );
		return;
	    }    
    
	    // get the parameters of the incoming command
	    ParameterSet<CommandParameter> inCommandParSet  = functionManager.getLastInput().getParameterSet();
            // create a parameter set for the outgoing command
	    ParameterSet<CommandParameter> outCommandParSet = new ParameterSet<>();

            String msg = inCommandParSet.getParameters().size()+" configure command parameters received:";
            msg = inCommandParSet.getParameters().stream().map( (cp) -> " "+cp.getName() +"="+cp.getValue() ).reduce( msg, String::concat );
            logger.info( msg );
            

            // TODO: move this to MyEventHandler as this is not CSC-specific:
	    // Check received command parameter set for TCDS HW instance to use
            boolean usePrimaryTCDS = false;
	    if (inCommandParSet.contains( MyParameterSet.USE_PRIMARY_TCDS )){
		usePrimaryTCDS = ( (CommandParameter<BooleanT>)inCommandParSet.get(MyParameterSet.USE_PRIMARY_TCDS) ).getValue().booleanValue();
		logger.info( getClass().getName() + ": Got USE_PRIMARY_TCDS " + usePrimaryTCDS );
            }
	    else {
		String errMsg = getClass().getName() + ": Error reading parameter USE_PRIMARY_TCDS of received Configure command. Using default: " + usePrimaryTCDS;
		logger.warn( errMsg );
	    }
            
            // Check received command parameter set for run type 
            String runType = "Global";
	    if (inCommandParSet.contains( MyParameterSet.CSC_RUN_TYPE )){
		runType = ((CommandParameter<StringT>)inCommandParSet.get(MyParameterSet.CSC_RUN_TYPE)).getValue().toString();
		logger.info( getClass().getName() + ": Got CSC_RUN_TYPE " + runType );
            }
	    else {
		String errMsg = getClass().getName() + ": Error reading parameter CSC_RUN_TYPE of received Configure command. Using default: " + runType;
//                                ((StringT)functionManager.getParameterSet().get(MyParameterSet.CSC_RUN_TYPE).getValue()).toString();
		logger.warn( errMsg );
	    }

            try{
                // In the Supervisor, these are called "usePrimaryTCDS" and "RunType"
                outCommandParSet.add( new CommandParameter<>( "usePrimaryTCDS", new BooleanT( usePrimaryTCDS ) ) );
                outCommandParSet.add( new CommandParameter<>( "RunType"       , new StringT ( runType        ) ) );
            }
            catch( ParameterException e ){
                String errMsg = getClass().getName() + ": Failed to add " + MyParameterSet.USE_PRIMARY_TCDS + " and " + MyParameterSet.CSC_RUN_TYPE
                              + " to outgoing 'Configure' command.";
                logger.error( errMsg, e );
                errMsg += " Exception caught: " + e.getMessage() + "\n\n" + stackTraceString( e );                
                sendCMSError( errMsg );
                return;
            }
	    
	    // set run type and TCDS HW instance (primary or secondary) in the Supervisor (Not needed, these will be sent along with the Configure SOAP.)
//	    try {
//		XDAQParameter xdaqParam = supervisor.getXDAQParameter();
//		String[] parametersToSet = {"RunType","usePrimaryTCDS"};
//		xdaqParam.select( parametersToSet );
//		xdaqParam.setValue("RunType", runType);
//		xdaqParam.setValue("usePrimaryTCDS", String.valueOf( usePrimaryTCDS ));
//		xdaqParam.send();
//	    } catch (Exception e) {
//                String errorMsg = "Failed to set RunType and usePrimaryTCDS in " + supervisor.getName() + ".";
//		logger.error( getClass().getName() + errorMsg, e );
//		functionManager.fireEvent(MyInputs.SETERROR);
//                sendCMSError( errorMsg );
//                return;
//	    }
            
	    // send Configure
	    try {
                MyInputs.CONFIGURE.setParameters( outCommandParSet );
        	supervisor.execute(MyInputs.CONFIGURE);
	    } catch (CommandException e) {
                String errorMsg = "Failed to Configure " + supervisor.getName() + ".";
                errorMsg += getXdaqAppsReasonForFailure( supervisor );
		logger.error( getClass().getName() + errorMsg, e );
                sendCMSError( errorMsg );
                try{
                    functionManager.setState(MyStates.ERROR);
                } catch (StateMachineException ex) {
                    logger.error( "Failed to set ERROR" , ex );
                }
                return;
	    }
            
            msg = functionManager.getParameterSet().getParameters().size()+" global parameters before:";
            msg = functionManager.getParameterSet().getParameters().stream().map((p) -> " "+p.getName()+"="+p.getValue()).reduce(msg, String::concat);
            logger.info( msg );
            
            // Set the configuration parameters in the Function Manager parameters
            functionManager.getParameterSet().get( MyParameterSet.CONFIGURED_WITH_CSC_RUN_TYPE     ).setValue( new StringT ( runType        ) );
            functionManager.getParameterSet().get( MyParameterSet.CONFIGURED_WITH_USE_PRIMARY_TCDS ).setValue( new BooleanT( usePrimaryTCDS ) );
            functionManager.getParameterSet().get( MyParameterSet.CONFIGURED_WITH_CSC_RUN_TYPE     ).setExported( FunctionManagerParameter.Exported.READONLY );
            functionManager.getParameterSet().get( MyParameterSet.CONFIGURED_WITH_USE_PRIMARY_TCDS ).setExported( FunctionManagerParameter.Exported.READONLY );

            msg = functionManager.getParameterSet().getParameters().size()+" global parameters after:";
            msg = functionManager.getParameterSet().getParameters().stream().map((p) -> " "+p.getName()+"="+p.getValue()).reduce(msg, String::concat);
            logger.info( msg );
        }    
    }

    @SuppressWarnings("unchecked")
    public void startAction(Object obj) throws UserActionException {
//        ((FunctionManagerParameter<StringT>)functionManager.getParameterSet().get(MyParameterSet.STATE)).setValue( new StringT( functionManager.getState().getStateString() ) );
        logger.info( getClass().getName() +  ".startAction called for "+obj.getClass().getName()+". FM state is " + functionManager.getState().getStateString() );
	if (obj instanceof StateNotification) {
            if ( supervisor != null ){
                if ( supervisor.getCacheState().equals( MyStates.RUNNING ) ){
                    try {
                        // go to RUNNING
                        // Do not define SETRUNNING transition explicitly so that it doesn't appear on the GUI.
                        // functionManager.fireEvent( MyInputs.SETRUNNING );
                        // Simply set the state to RUNNING instead:
                        functionManager.setState(MyStates.RUNNING);
                    } catch (StateMachineException ex) {
                        logger.error( "Failed to set RUNNING" , ex );
                    }
                }
                else if ( supervisor.getCacheState().equals( MyStates.ERROR   ) ){
                    String errorMsg = "Failed to Start " + supervisor.getName() + " ";
                    errorMsg += getXdaqAppsReasonForFailure( supervisor );
                    logger.error( getClass().getName() + ": " + errorMsg );
                    sendCMSError( errorMsg );
//                    if ( !functionManager.getState().equals( MyStates.ERROR ) ) functionManager.fireEvent( MyInputs.SETERROR );
                    try{
                        functionManager.setState(MyStates.ERROR);
                    } catch (StateMachineException ex) {
                        logger.error( "Failed to set ERROR" , ex );
                    }
                }
            }
            ((FunctionManagerParameter<StringT>)functionManager.getParameterSet().get(MyParameterSet.STATE)).setValue( new StringT( functionManager.getState().getStateString() ) );
	}
	else if (obj instanceof StateEnteredEvent) {
	    if ( supervisor == null ){
		// Nothing to control, go immediately to steady state.
		// functionManager.fireEvent( MyInputs.SETRUNNING );
		return;
	    }

            ParameterSet<CommandParameter> inCommandParSet  = functionManager.getLastInput().getParameterSet();
	    ParameterSet<CommandParameter> outCommandParSet = new ParameterSet<>();

            String msg = inCommandParSet.getParameters().size()+" start command parameters received:";
            msg = inCommandParSet.getParameters().stream().map( (cp) -> " "+cp.getName() +"="+cp.getValue() ).reduce( msg, String::concat );
            logger.info( msg );

            // Check received command parameter set for run number. If none found, book one instead.
            Integer runNumber = -1;
	    if (inCommandParSet.contains( MyParameterSet.RUN_NUMBER )){
		runNumber = ((CommandParameter<IntegerT>)inCommandParSet.get(MyParameterSet.RUN_NUMBER)).getValue().intValue();
		logger.info( getClass().getName() + ": Got RUN_NUMBER " + runNumber );
            }
	    else {
		String errMsg = getClass().getName() + ": Error reading parameter RUN_NUMBER of received Start command. Will book one now instead.";
		logger.warn( errMsg );
                try{
                    runNumber = bookRunNumber();
                }
                catch( Exception e ){
                    String errorMessage = functionManager.getClass().getName() + ": Failed to book run number. ";
                    logger.error( errorMessage, e );
                }
	    }
            // Check sanity of run number
            if ( runNumber <= 0 ){
                logger.warn( getClass().getName() + ": Suspicious run number (" + runNumber + ") not sent to " + supervisor.getName() + ". Sending 1 instead." );
                runNumber = 1;
            }
            functionManager.getParameterSet().get( MyParameterSet.RUN_NUMBER ).setValue( new IntegerT( runNumber ) );
            // Add run number to the outgoing command's paremeter set
            try{
                // In the Supervisor, this is called "RunNumber"
                outCommandParSet.add( new CommandParameter<>( "RunNumber", new UnsignedIntegerT( runNumber ) ) );
            } catch( ParameterException e ){
                String errMsg = getClass().getName() + ": Failed to add " + MyParameterSet.RUN_NUMBER  + " to outgoing 'Start' command.";
                logger.error( errMsg, e );
                sendCMSError( errMsg );
		if ( !functionManager.getState().equals( MyStates.ERROR ) ) functionManager.fireEvent(MyInputs.SETERROR);
                return;
            }
            // Send Start command with run number to Supervisor
            try {
                MyInputs.START.setParameters( outCommandParSet );
		supervisor.execute(MyInputs.START);
	    } catch (CommandException e) {
                String errMsg = getClass().getName() + ": Failed to Start " + supervisor.getName() + ". ";
                errMsg += getXdaqAppsReasonForFailure( supervisor );
		logger.error( errMsg, e );
                errMsg += " Exception caught: " + e.getMessage() + "\n\n" + stackTraceString( e );
                sendCMSError( errMsg );
		if ( !functionManager.getState().equals( MyStates.ERROR ) ) functionManager.fireEvent(MyInputs.SETERROR);
	    }
            functionManager.getParameterSet().get( MyParameterSet.STARTED_WITH_RUN_NUMBER ).setValue( new IntegerT( runNumber ) );
	}
    }    

    public void stopAction(Object obj) throws UserActionException {
        logger.info( getClass().getName() +  ".stopAction called for "+obj.getClass().getName()+". FM state is " + functionManager.getState().getStateString() );
	if (obj instanceof StateNotification) {
            if ( supervisor != null ){
                if      ( supervisor.getCacheState().equals( MyStates.CONFIGURED ) ){
                    try {
                        // go to CONFIGURED
                        // Do not define SETCONFIGURED transition explicitly so that it doesn't appear on the GUI.
                        // functionManager.fireEvent( MyInputs.SETCONFIGURED );
                        // Simply set the state to CONFIGURED instead:
                        functionManager.setState(MyStates.CONFIGURED);
                    } catch (StateMachineException ex) {
                        logger.error( "Failed to set CONFIGURED" , ex );
                    }
                }
                else if ( supervisor.getCacheState().equals( MyStates.ERROR   ) ){
                    String errorMsg = getClass().getName() + ": Failed to Stop " + supervisor.getName() + " ";
                    errorMsg += getXdaqAppsReasonForFailure( supervisor );
                    logger.error( errorMsg );
                    sendCMSError( errorMsg );
//                    if ( !functionManager.getState().equals( MyStates.ERROR ) ) functionManager.fireEvent( MyInputs.SETERROR );
                    try{
                        functionManager.setState(MyStates.ERROR);
                    } catch (StateMachineException ex) {
                        logger.error( "Failed to set ERROR" , ex );
                    }
                }
            }
            ((FunctionManagerParameter<StringT>)functionManager.getParameterSet().get(MyParameterSet.STATE)).setValue( new StringT( functionManager.getState().getStateString() ) );
	}
	else if (obj instanceof StateEnteredEvent) {
	    if ( supervisor == null ){
		// Nothing to control, go immediately to steady state.
		// functionManager.fireEvent( MyInputs.SETCONFIGURED );
		return;
	    }
	    try {
		supervisor.execute(MyInputs.STOP);
	    } catch (Exception e) {
                String errorMsg = getClass().getName() + ": Failed to Stop CSC supervisor xdaq application.";
                errorMsg += getXdaqAppsReasonForFailure( supervisor );
		logger.error( errorMsg, e );
                sendCMSError( errorMsg );
		if ( !functionManager.getState().equals( MyStates.ERROR ) ) functionManager.fireEvent(MyInputs.SETERROR);
	    }
	}
    }    

    public void haltAction(Object obj) throws UserActionException {
        logger.info( getClass().getName() + ".haltAction called for "+obj.getClass().getName()+". FM state is " + functionManager.getState().getStateString() );
	if (obj instanceof StateNotification) {
            if ( supervisor != null ){
                if ( supervisor.getCacheState().equals( MyStates.HALTED ) ){
                    try {
                        // go to HALTED
                        // Do not define SETHALTED transition explicitly so that it doesn't appear in the GUI.
                        // functionManager.fireEvent( MyInputs.SETHALTED );
                        // Simply set the state to HALTED instead:
                        functionManager.setState(MyStates.HALTED);
                    } catch (StateMachineException ex) {
                        //java.util.logging.Logger.getLogger(MyEventHandler.class.getName()).log(Level.SEVERE, null, ex);
                        logger.error( "Failed to set HALTED" , ex );
                    }
                }
                else if ( supervisor.getCacheState().equals( MyStates.ERROR ) ){
                    String errorMsg = "Failed to Halt " + supervisor.getName() + " ";
                    errorMsg += getXdaqAppsReasonForFailure( supervisor );
                    logger.error( getClass().getName() + ": " + errorMsg );
                    sendCMSError( errorMsg );
                    try{
                        functionManager.setState(MyStates.ERROR);
                    } catch (StateMachineException ex) {
                        logger.error( "Failed to set ERROR" , ex );
                    }
                }
            }
            ((FunctionManagerParameter<StringT>)functionManager.getParameterSet().get(MyParameterSet.STATE)).setValue( new StringT( functionManager.getState().getStateString() ) );
	}
	else if (obj instanceof StateEnteredEvent) {
	    if ( supervisor == null ){
		// Nothing to control, go immediately to steady state.
		// functionManager.fireEvent( MyInputs.SETHALTED );
		return;
	    }
	    try {
		supervisor.execute(MyInputs.HALT);
	    } catch (Exception e) {
                String errorMsg = getClass().getName() + ": Failed to Halt CSC supervisor xdaq application.";
                errorMsg += getXdaqAppsReasonForFailure( supervisor );
		logger.error( errorMsg, e );
                sendCMSError( errorMsg );
		if ( !functionManager.getState().equals( MyStates.ERROR ) ) functionManager.fireEvent(MyInputs.SETERROR);
	    }
	}
    }    

    @SuppressWarnings("unchecked")
    private void sendCMSError(String errMessage){
	// create a new error notification msg
	CMSError error = functionManager.getErrorFactory().getCMSError();
	error.setDateTime(new Date().toString());
	error.setMessage(errMessage);
	
	// update error msg parameter for GUI
	functionManager.getParameterSet().get(MyParameterSet.ERROR_MSG).setValue(new StringT(errMessage));
	
	// send error
	try {
	    functionManager.getParentErrorNotifier().sendError(error);
	} catch (Exception e) {
	    logger.warn( functionManager.getClass().getName() + ": Failed to send the following error message\n\n" + errMessage 
                                                               + "\nThe exception caught while trying to send it:\nMessage: " + e.getMessage()
                                                               + "\n" + stackTraceString( e ) );
	}
    }


    public void preparingTTSTestModeAction(Object obj) throws UserActionException {
	if (obj instanceof StateNotification) {
	    StateNotification sn = (StateNotification)obj;
	    if (sn.getToState().equals(MyStates.CONFIGURED.getStateString())) {
		// "Configure" has been sent in StateEnteredEvent part (below).
		// Here we should receive "Configured" from supervisor. If we have, send "Start".
		try {                                                                                           
		    supervisor.execute(MyInputs.START);
		} catch (Exception e) {
		    logger.error("Could not Start csc supervisor.",e);
		    functionManager.fireEvent(MyInputs.SETERROR);
		}
	    }
	    else if ( sn.getToState().equals(MyStates.RUNNING.getStateString()) ) {
		// Received "Running" state. Now we can leave the intermediate state.
		functionManager.fireEvent( MyInputs.SETTTSTEST_MODE );
	    }
	    else {
		// We don't understand state, go to error.
		logger.error("Unexpected state notification in state " 
			     + functionManager.getState().toString() 
			     + ", received state :" 
			     + sn.getToState());
		functionManager.fireEvent(MyInputs.SETERROR);
	    }
	    return;
	}
	else if (obj instanceof StateEnteredEvent) {
	    if ( supervisor == null ){
		// Nothing to control, go immediately to steady state.
		functionManager.fireEvent( MyInputs.SETTTSTEST_MODE );
		return;
	    }
	    try {
		XDAQParameter xdaqParam = supervisor.getXDAQParameter();
		xdaqParam.select("RunType");
		xdaqParam.setValue("RunType", "sTTS_Test");
		xdaqParam.send();
	    } catch (Exception e) {
		logger.error(getClass().getName() + "Failed to set CSC supervisor run type to sTTS_Test.", e);
		functionManager.fireEvent(MyInputs.SETERROR);
	    }
	    try {
		supervisor.execute(MyInputs.CONFIGURE);
	    } catch (Exception e) {
		logger.error(getClass().getName() + "Failed to Configure CSC supervisor for TTS Test.", e);
		functionManager.fireEvent(MyInputs.SETERROR);
	    }
	}
    }

    public void testingTTSAction(Object obj) throws UserActionException {
    }

    
    
//    public void setParameterFromProperties() throws UserActionException {
//	List<ConfigProperty> lp = functionManager.getGroup().getThisResource().getProperties();
//	logger.info("start properties");
//	for (ConfigProperty p : lp) {
//	    logger.info("geting properties" + p.getName());
//	    if (p.getName().equals("Calibration_Keys")) {
//		logger.info("ends properties");
//		functionManager.getParameterSet().get(MyParameterSet.CSC_CALIB_KEYS_AVAILABLE).setValue(new StringT( p.getValue() ));
//	    }
//	}
//    }
    
    private void findSupervisor() throws UserActionException {
        logger.info( "findSupervisor called" );
	List<XdaqApplication> supervisors = null;
	try{
	    supervisors = functionManager.containerXdaqApplication.getApplicationsOfClass("emu::supervisor::Application");
	}
	catch (Exception e) {
            String errorMess = "Failed to get applications of class 'emu::supervisor::Application'";
	    logger.error( getClass().getName() + " " + errorMess, e );
	    throw new UserActionException( errorMess );
	}

	if ( supervisors.isEmpty() ){
	    logger.warn( "No emu::supervisor::Application found in the configuration." );
	    return;
	}

	if ( supervisors.size() > 1 ){
	    String errorMess = supervisors.size() + " instances of emu::supervisor::Application found in the configuration. There should be no more than one.";
	    logger.error( errorMess );
	    throw new UserActionException( errorMess );
	}

	supervisor = supervisors.get( 0 );

	if ( supervisor == null ){
	    String errorMess = "No emu::supervisor::Application found in the configuration.";
	    logger.error( errorMess );
	    throw new UserActionException( errorMess );
	}

	logger.info( "Supervisor emu::supervisor::Application found in the configuration." );

//        logger.info( "Available run types: " + getRunTypesAvailable() );
    }

    public String getConfigXmlForXdaqApp( XdaqApplication app ){
	String configXML = null;
	logger.info("XdaqApplication '"  +app.getName()
		    // +"' of id '"  +app.getId()
		    +"' has URL '"+app.getURL()
		    +"' role '"   +app.getRole()
		    // +"' config file \n'"   +app.getConfigFile()
		    +"'.");
        // Find the XdaqExecutive for this XdaqApplication, and get its configFile
        XdaqExecutive executive = null;
        List<QualifiedResource> executives = functionManager.getQualifiedGroup().seekQualifiedResourcesOfType(new XdaqExecutive());
        for (QualifiedResource exe : executives) {
            if ( app.getURL().getHost().equals( exe.getURL().getHost() ) && app.getURL().getPort() == exe.getURL().getPort() ) {
                executive = (XdaqExecutive) exe;
                break;
            }
        }
        if ( executive == null ){
            logger.error("XDAQ Application '"  +app.getName()
		    +"' at '"+app.getURL()
		    +"' has no corresponding XDAQ Executive in the .duck file?!.");
            return configXML;
        }
        XdaqExecutiveConfiguration conf = executive.getXdaqExecutiveConfiguration();
        if ( conf == null ){
            logger.error("XDAQ Executive of XDAQ Application '"  +app.getName()
		    +"' at '"+app.getURL()
		    +"' has no configuration?!.");
            return configXML;
        }
        configXML = conf.getXml();
	return configXML;
    }

    public String getRunTypesAvailable() {
        String runTypes = new String();
        StringReader configXML = new StringReader( getConfigXmlForXdaqApp( supervisor ) );
        XDAQDocument doc;    
        try{
            doc = new XDAQDocument( configXML );
        }catch( XDAQException e ){
            logger.error( "Failed to create XDAQDocument out of Supervisor's configuration XML. " + e.getMessage() );
            return runTypes;
        }
        
        NodeList runTypeNodes;
        // String xPath = "//xc:Application[@class='emu::supervisor::Application']/properties/calibParams/item/key"; // TTC
        String xPath = "//xc:Application[@class='emu::supervisor::Application']/app:properties/app:runParameters/app:item/app:key"; // TCDS
        DefaultNamespaceContext ns = new DefaultNamespaceContext();
        ns.put( "xc" , "http://xdaq.web.cern.ch/xdaq/xsd/2004/XMLConfiguration-30" );
        ns.put( "app", "urn:xdaq-application:emu::supervisor::Application"         );
        try {
            runTypeNodes = XPathAPI.selectNodeList( doc.getDOMDocument(), xPath, ns );
        } catch ( XPathAPIException e ) {
            logger.error( "Failed to extract run types from Supervisor's configuration XML." + e.getMessage() );
            return runTypes;
        }
        for ( int i=0; i < runTypeNodes.getLength(); ++i ){
            Element rt = (Element) runTypeNodes.item(i);
            runTypes += rt.getTextContent() + ( i+1 < runTypeNodes.getLength() ? "," : "" );
        }
        logger.info("Found " + runTypeNodes.getLength() + " available run types in Supervisor's configuration XML: " + runTypes );
       
        return runTypes;
    }

    public VectorT<StringT> getRunTypesAvailableVector() {
        VectorT<StringT> rta = new VectorT<StringT>();
        StringReader configXML = new StringReader( getConfigXmlForXdaqApp( supervisor ) );
        XDAQDocument doc;    
        try{
            doc = new XDAQDocument( configXML );
        }catch( XDAQException e ){
            logger.error( "Failed to create XDAQDocument out of Supervisor's configuration XML. " + e.getMessage() );
            return rta;
        }
        
        NodeList runTypeNodes;
        // First try assuming supervisor is to configure TCDS (as opposed to legacy TTC)
        String xPath = "//xc:Application[@class='emu::supervisor::Application']/app:properties/app:runParameters/app:item/app:key"; // TCDS
        DefaultNamespaceContext ns = new DefaultNamespaceContext();
        ns.put( "xc" , "http://xdaq.web.cern.ch/xdaq/xsd/2004/XMLConfiguration-30" );
        ns.put( "app", "urn:xdaq-application:emu::supervisor::Application"         );
        try {
            runTypeNodes = XPathAPI.selectNodeList( doc.getDOMDocument(), xPath, ns );
        } catch ( XPathAPIException e ) {
            logger.error( "Failed to extract run types from Supervisor's configuration XML." + e.getMessage() );
            return rta;
        }
        if ( runTypeNodes.getLength() == 0 ){
            // Looks like we've found none, so now try assuming supervisor is to configure legacy TTC
            xPath = "//xc:Application[@class='emu::supervisor::Application']/app:properties/app:calibParams/app:item/app:key"; // TTC
            try {
                runTypeNodes = XPathAPI.selectNodeList( doc.getDOMDocument(), xPath, ns );
            } catch ( XPathAPIException e ) {
                logger.error( "Failed to extract run types from Supervisor's configuration XML." + e.getMessage() );
                return rta;
            }
        }
        for ( int i=0; i < runTypeNodes.getLength(); ++i ){
            Element rt = (Element) runTypeNodes.item(i);
            rta.add( new StringT( rt.getTextContent() ) );
        }
        logger.info("Found " + rta.size() + " available run types in Supervisor's configuration XML: " + rta.toString() );
       
        return rta;
    }
    
    public String getRunType() {
        String runType = new String();
        StringReader configXML = new StringReader( getConfigXmlForXdaqApp( supervisor ) );
        XDAQDocument doc;    
        try{
            doc = new XDAQDocument( configXML );
        }catch( XDAQException e ){
            logger.error( "Failed to create XDAQDocument out of Supervisor's configuration XML. " + e.getMessage() );
            return runType;
        }
        
        Node runTypeNode = null;
        String xPath = "//xc:Application[@class='emu::supervisor::Application']/app:properties/app:RunType"; // TCDS
        DefaultNamespaceContext ns = new DefaultNamespaceContext();
        ns.put( "app", "urn:xdaq-application:emu::supervisor::Application"         );
        ns.put( "xc" , "http://xdaq.web.cern.ch/xdaq/xsd/2004/XMLConfiguration-30" );
//        logger.info("NS URI for 'xc': " + ns.getNamespaceURI( "xc" ) );
//        logger.info("NS URI for 'app': " + ns.getNamespaceURI( "app" ) );
        try {
            runTypeNode = XPathAPI.selectSingleNode( doc.getDOMDocument(), xPath, ns );
            if ( runTypeNode != null ){
                runType = runTypeNode.getTextContent();
                logger.info("Found run type node: " + runTypeNode.getNodeName() );
            }
            else{
                logger.error("Failed to find run type node by " + xPath );
            }
        } catch ( XPathAPIException e ) {
            logger.error( "Failed to extract run types from Supervisor's configuration XML." + e.getMessage() );
            return runType;
        }
        logger.info("Found run type in Supervisor's configuration XML: " + runType );
       
        return runType;
    }
    
    public String getRscConfProperty( String name ) {
        logger.info("getRscConfProperty called");
	String value = null;

	List<ConfigProperty> l = functionManager.getGroup().getThisResource().getProperties();

	List<Resource> rlist = functionManager.getGroup().getChildrenResources();
	if ( rlist!=null ){
	    for (Resource r : rlist) {
		logger.info("Resource '"  +r.getName()
			    +"' of id '"  +r.getId()
			    +"' has URL '"+r.getURL()
			    +"' role '"   +r.getRole()
			    +"' config file \n'"   +r.getConfigFile()+"'.");
		
	    }
	}
	logger.info("Resource '"  +functionManager.getGroup().getThisResource().getName()
		    +"' of id '"  +functionManager.getGroup().getThisResource().getId()
		    +"' has URL '"+functionManager.getGroup().getThisResource().getURL()+"'.");

	if (l!=null) {
	    for (ConfigProperty p : l) {
		logger.info("FM property '"+p.getName()+"' of type '"+p.getType()+"' has value '"+p.getValue()+"'.");
		if (p.getName().equals( name )) {
		    value = p.getValue();
		    break;
		}
	    }
	}

	return value;
    }
    
    public String getXdaqAppsReasonForFailure( XdaqApplication app ){
        String reasonForFailure = "";
        try {
            reasonForFailure = app.getXDAQParameter().getValue( "reasonForFailure" );
            logger.info( getClass().getName() + " Retrieved " + app.getName() + "'s reason for failure: " + reasonForFailure );
            reasonForFailure = reasonForFailure.replace("<![CDATA[", "" ).replace( "]]>", "" ).replace( "]]&gt;", "" );
            logger.info( getClass().getName() + " Retrieved and cleaned up " + app.getName() + "'s reason for failure: " + reasonForFailure );
        } catch( XDAQException e ) {
            reasonForFailure = "Failed to find out the reason for " + app.getName() + "'s failure, too. Perhaps it's crashed? The exception caught: " + e.getMessage();
        }
        return reasonForFailure;
    }
    
    public Integer bookRunNumber() {
        RunSequenceNumber runNumberGenerator = null;
        RunNumberData     runNumberData = null;
        Integer           runNumber = null;
        RunInfo           runInfo = null;

        RunInfoConnectorIF ric = functionManager.getRunInfoConnector();

        if (ric != null) {
            if (runNumberGenerator == null) {
                logger.info("Creating RunNumberGenerator with SEQ_NAME " + functionManager.getParameterSet().
                        get(MyParameterSet.SEQ_NAME).getValue().toString() );
                runNumberGenerator = new RunSequenceNumber(ric, functionManager.getOwner(), functionManager.getParameterSet().
                        get(MyParameterSet.SEQ_NAME).getValue().toString());
            }

            if (runNumberGenerator != null) {
                logger.info("Getting run number data with SID " + functionManager.getParameterSet().get(MyParameterSet.SID).
                        getValue().toString() );
                runNumberData = runNumberGenerator.createRunSequenceNumber(new Integer(functionManager.getParameterSet().get(MyParameterSet.SID).
                        getValue().toString()));
            }

            if (runNumberData != null) {
                runNumber = runNumberData.getRunNumber();
                logger.info("Generated run number " + runNumberData.toString());
            } else {
                logger.error("Error generating RunNumber");
            }
        } else {
            logger.error("Failed to create RunNumberGenerator");
        }
        return runNumber;
    }

	
    /**
     * Kill orphaned XDAQ executives
     */
    public void killOrphanedExecutives() {
	String pathToOrphanHandler = getRscConfProperty( "pathToOrphanHandler" );
	if ( pathToOrphanHandler == null ){
	    logger.warn("No orphan handler script found in the configuration of this Function Manager.");
	    return;
	}
	if ( pathToOrphanHandler.equals("") ){
	    logger.warn("No orphan handler script found in the configuration of this Function Manager.");
	    return;
	}

	System.out.println("Orphan handler script '"+pathToOrphanHandler+"' will be used.");
	logger.info("Orphan handler script '"+pathToOrphanHandler+"' will be used.");
	    
	List<QualifiedResource> jobcontrols = functionManager.getQualifiedGroup().seekQualifiedResourcesOfType(new JobControl());
	for (QualifiedResource jc: jobcontrols) {
	    try{
		List<String> orphanHandlingJIDs = new ArrayList<String>();
		List<QualifiedResource> executives = functionManager.getQualifiedGroup().seekQualifiedResourcesOfType(new XdaqExecutive());
		for (QualifiedResource exe: executives){
		    // We assume a single jobcontrol on each host and compare host names only:
		    if ( jc.getURL().getHost().equals( exe.getURL().getHost() ) ){
			// This doesn't seem to work: String user = ((XdaqExecutive)exe).getJobUserName();
			// Get the user name indirectly::
			XdaqExecutiveConfiguration conf = ((XdaqExecutive)exe).getXdaqExecutiveConfiguration();
			if ( conf != null ){
			    String user = conf.getUserName();
			    String args = Integer.toString( exe.getURL().getPort() );
			    String environment = "USER=" + user;
			    String jid = pathToOrphanHandler + "_" + Integer.toString( exe.getURL().getPort() );
			    ((JobControl)jc).start(pathToOrphanHandler,args,environment,user,jid);
			    logger.info("Issued to JobControl " + jc.getURL() + " the command: " + environment + " " + pathToOrphanHandler + " " + args + ", jid=" + jid );
			    orphanHandlingJIDs.add( jid );
			}
			else{
			    logger.error( "XdaqExecutiveConfiguration is null for " + exe.toString() ); 
			}
		    }
		}
		// Allow the script some time to do its dirty job:
		wait( 1000 );
		// Clean up orphan-handling job entries
		for ( String jid : orphanHandlingJIDs ){
		    logger.info("Killing orphan handling job " + jid + " of JobControl " + jc.getURL() );
		    ((JobControl)jc).killJid( jid );
		}
	    } catch (Exception e) {
		logger.error( "Failed to execute orphan handler on JobControl " + jc.getURL(), e);
	    }
	}
    }

    
    public String stackTraceString( Exception e ){
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        e.printStackTrace( new PrintStream( os ) );
	return os.toString();
    } 
}
