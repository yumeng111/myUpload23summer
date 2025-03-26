package rcms.fm.ajaxFM;

import rcms.fm.ajaxFM.myParameters.MyParameterSet;

import org.apache.log4j.Logger;

import java.util.Date;
import java.util.List;

import rcms.fm.context.RCMSConstants;

import rcms.fm.fw.EventHandlerException;
import rcms.fm.fw.StateEnteredEvent;
import rcms.fm.fw.user.UserActionException;
import rcms.fm.fw.user.UserStateNotificationHandler;
import rcms.fm.fw.parameter.FunctionManagerParameter;
import rcms.fm.fw.parameter.CommandParameter;
import rcms.fm.fw.parameter.type.StringT;
import rcms.fm.fw.parameter.type.IntegerT;
import rcms.fm.fw.parameter.type.BooleanT;
import rcms.fm.fw.parameter.ParameterSet;
import rcms.stateFormat.StateNotification;
import rcms.errorFormat.CMS.CMSError;
import rcms.fm.resource.QualifiedGroup;
import rcms.fm.resource.QualifiedResource;
import rcms.fm.resource.qualifiedresource.XdaqApplication;
import rcms.fm.resource.qualifiedresource.XdaqApplicationContainer;
import rcms.statemachine.StateMachineException;

public class MyEventHandler extends UserStateNotificationHandler {

	static Logger logger = Logger.getLogger(MyEventHandler.class);

	MyFunctionManager functionManager = null;

	public MyEventHandler() throws EventHandlerException {
		super();

		this.subscribeForEvents(StateEnteredEvent.class);

                this.addAction(MyStates.INITIALIZING,          "initAction");          
                this.addAction(MyStates.CONFIGURING, 	  "configureAction");
		this.addAction(MyStates.STARTING,             "startAction");
		this.addAction(MyStates.STOPPING,              "stopAction");
                this.addAction(MyStates.HALTING,               "haltAction");
                this.addAction(MyStates.ERROR,                "errorAction");
                
                this.addAction(MyStates.HALTED,         "steadyStateAction");
                this.addAction(MyStates.CONFIGURED,     "steadyStateAction");
                this.addAction(MyStates.RUNNING,        "steadyStateAction");
	}

	@Override
	public void init() throws EventHandlerException {
		this.functionManager = (MyFunctionManager) this.getUserFunctionManager();

                int session_id = functionManager.getQualifiedGroup().getGroup().getDirectory().getId();
		functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.SID, new IntegerT(session_id)));
                // functionManager.getParameterSet().get(MyParameterSet.SID).setValue(new IntegerT(session_id));
                logger.info( getClass().getName() +  ".init: SID = " + session_id );
        }

        public void initAction(Object obj) throws UserActionException {
                logger.info( getClass().getName() +  ".initAction called for "+obj.getClass().getName() );
                if (obj instanceof StateNotification) {
                    // We don't get state notification when the XDAQ app is initialized, so this clause is kind of redundant...
                        
                        // triggered by State Notification from child resource
                        
                        /************************************************
                         * PUT YOUR CODE HERE                                                   
                         ***********************************************/
                        functionManager.csc.initAction( obj );

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

                        logger.info( getClass().getName() +  ".initAction executed for "+obj.getClass().getName() );
                       
                        // set action
                        functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("Initializing done.")));

                        functionManager.updateSTATE();

                }
                 else if (obj instanceof StateEnteredEvent) {
                        
                        // triggered by entered state action
                        // let's command the child resources

                        functionManager.updateSTATE();
                                                
                        // set action
                        functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("Initializing")));
                        
                        // get the parameters of the command
                        // 2 initialize command parameters received: GLOBAL_CONF_KEY=/GLOBAL_CONFIGURATION_MAP/904_COMBINED/COMBINED_RUN SID=1000020148
                        Integer sid = -1;
                        String globalConfKey = "";
                        
                        try {
                                ParameterSet<CommandParameter> parameterSet = getUserFunctionManager().getLastInput().getParameterSet();
                                sid = ((CommandParameter<IntegerT>)parameterSet.get(MyParameterSet.SID)).getValue().getInteger();
                                globalConfKey = ((CommandParameter<StringT>)parameterSet.get(MyParameterSet.GLOBAL_CONF_KEY)).getValue().toString();
                        }
                        catch (Exception e) {
                                // go to error, we require parameters
                                String errMsg = "initAction: error reading command parameters of Initialize command.";
                                
                                // log error
                                logger.error(errMsg, e);
                                
                                // notify error if GUI access is not allowed, as then the command is sure from the L0 FM
                                // Doesn't really work because RCMSConstants.GUI_ACCESS_ALLOWED is apparently _false_ in stand-alone mode, too...
                                // if ( ! isGUIAccessAllowed() ) sendCMSError(errMsg);
                                
//                              // go to error state
//                              functionManager.fireEvent( MyInputs.SETERROR );
//                              return;
                        }
                        
                                
                        // 
                        // initialize qualified group
                        
                        //
                        logger.debug("Initializing QualifiedGroup");
                        QualifiedGroup qg = functionManager.getQualifiedGroup();
                        logger.debug("QualifiedGroup: "+qg.print());

                        try {
                                qg.init();
                        } catch (Exception e) {
                                // failed to init
                                String errMsg = this.getClass().getName() + " failed to initialize resources. " + e.getMessage();
                        
                                // send error notification
                                sendCMSError(errMsg);
                
                                //log error
                                logger.error(errMsg,e);
                        
                                // go to error state
                                functionManager.fireEvent(MyInputs.SETERROR);
                                functionManager.updateSTATE();
                                
                                // set action
                                functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("Going to ERROR...")));

                                return;
                        }
                        // find xdaq applications
                        List<QualifiedResource> xdaqList = qg.seekQualifiedResourcesOfType(new XdaqApplication());
                        functionManager.containerXdaqApplication = new XdaqApplicationContainer(xdaqList);
                        logger.debug("Application list : " + xdaqList.size() );

                        // Example: find "your" applications
                        // functionManager.containerYourClass = new XdaqApplicationContainer( 
                        //              functionManager.containerXdaqApplication.getApplicationsOfClass("yourClass"));

                        /************************************************
                         * PUT YOUR CODE HERE                                                   
                         ***********************************************/
                        functionManager.csc.initAction( obj );

                        // set exported parameters
                        ((FunctionManagerParameter<IntegerT>)functionManager.getParameterSet().get(MyParameterSet.INITIALIZED_WITH_SID)).setValue(new IntegerT(sid));
                        ((FunctionManagerParameter<StringT>)functionManager.getParameterSet().get(MyParameterSet.INITIALIZED_WITH_GLOBAL_CONF_KEY)).setValue(new StringT(globalConfKey));
       
                        // We won't get state notification when the XDAQ app is initialized, so let's make the state change here
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
                        
                        functionManager.updateSTATE();
                        
                        // set action
                        functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("Initializing done.")));
                        
                        logger.info("initAction Executed");
                }
        }
               

                
//        public void configureAction(Object obj) throws UserActionException {
//		this.functionManager.fireEvent(MyInputs.SETCONFIGURED);
//                logger.info("configureAction");
//	}

        public void configureAction(Object obj) throws UserActionException {
                logger.info( getClass().getName() +  ".configureAction called for "+obj.getClass().getName() );
               
                if (obj instanceof StateNotification) {
                        // triggered by State Notification from child resource
                        
                        /************************************************
                         * PUT YOUR CODE HERE                                                   
                         ***********************************************/
                        functionManager.csc.configureAction( obj );
                        logger.info( getClass().getName() +  ".configureAction executed for "+obj.getClass().getName() );

                        functionManager.updateSTATE();

                        // set action
//                        if ( functionManager.getState().getStateString().compareTo( MyStates.CONFIGURED.getStateString() ) == 0 ){
                        if ( functionManager.getState() == MyStates.CONFIGURED ){
                           functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("Configuring done.")));                        
                        }
                        
                }
                
                else if (obj instanceof StateEnteredEvent) {

                        functionManager.updateSTATE();

                        // set action
                        functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("configuring")));
                        
                        // get the parameters of the command
                        Integer runNumber = 0;
                        String runType = "";
                        String runKey = "";
                        String fedEnableMask = "";
                        String globalConfKey = "";
                        Boolean usePrimaryTCDS = false;
                       // 6 configure command parameters: RUN_TYPE=GLOBAL USE_PRIMARY_TCDS=true FED_ENABLE_MASK= RUN_KEY= RUN_NUMBER=1000028554 GLOBAL_CONF_KEY=/GLOBAL_CONFIGURATION_MAP/904_COMBINED/COMBINED_RUN
                        try {
                                ParameterSet<CommandParameter> parameterSet = getUserFunctionManager().getLastInput().getParameterSet();
                                runNumber = ((CommandParameter<IntegerT>)parameterSet.get(MyParameterSet.RUN_NUMBER)).getValue().getInteger();
                                runType = ((CommandParameter<StringT>)parameterSet.get(MyParameterSet.RUN_TYPE)).getValue().toString();
                                runKey = ((CommandParameter<StringT>)parameterSet.get(MyParameterSet.RUN_KEY)).getValue().toString();
                                fedEnableMask = ((CommandParameter<StringT>)parameterSet.get(MyParameterSet.FED_ENABLE_MASK)).getValue().toString();
                                globalConfKey = ((CommandParameter<StringT>)parameterSet.get(MyParameterSet.GLOBAL_CONF_KEY)).getValue().toString();
                 		usePrimaryTCDS = ((CommandParameter<BooleanT>)parameterSet.get(MyParameterSet.USE_PRIMARY_TCDS) ).getValue().booleanValue();
                       }
                        catch (Exception e) {
                                // go to error, we require parameters
                                String errMsg = "configureAction: error reading command parameters of Configure command.";
                                
                                // log error
                                logger.error(errMsg, e);
                                
                                // notify error if GUI access is not allowed, as then the command is sure from the L0 FM
                                // Doesn't really work because RCMSConstants.GUI_ACCESS_ALLOWED is apparently _false_ in stand-alone mode, too...
                                // if ( ! isGUIAccessAllowed() ) sendCMSError(errMsg);
                                
                                //go to error state
//                              functionManager.fireEvent( MyInputs.SETERROR );
//                              return;
                        }                       
                        
                        /************************************************
                         * PUT YOUR CODE HERE                                                   
                         ***********************************************/
                        functionManager.csc.configureAction( obj );
                        
                        // Set the configuration parameters in the Function Manager parameters
                        functionManager.getParameterSet().getParameters().forEach( (p)->{ logger.warn( p.getName()+" "+p.getValue() ); } );
                       
                        // These below are not really used in CSC, but let's keep track of them anyway:
                        ((FunctionManagerParameter<IntegerT>)functionManager.getParameterSet().get(MyParameterSet.CONFIGURED_WITH_RUN_NUMBER)).setValue(new IntegerT(runNumber));
                        ((FunctionManagerParameter<StringT>)functionManager.getParameterSet().get(MyParameterSet.CONFIGURED_WITH_RUN_TYPE)).setValue(new StringT(runType));
                        ((FunctionManagerParameter<StringT>)functionManager.getParameterSet().get(MyParameterSet.CONFIGURED_WITH_RUN_KEY)).setValue(new StringT(runKey));
                        ((FunctionManagerParameter<StringT>)functionManager.getParameterSet().get(MyParameterSet.CONFIGURED_WITH_GLOBAL_CONF_KEY)).setValue(new StringT(globalConfKey));
                        ((FunctionManagerParameter<StringT>)functionManager.getParameterSet().get(MyParameterSet.CONFIGURED_WITH_FED_ENABLE_MASK)).setValue(new StringT(fedEnableMask));

                        // leave intermediate state
//                        functionManager.fireEvent( MyInputs.SETCONFIGURED );
                      
                        functionManager.updateSTATE();

                         // set action
//                        functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("Configuring done.")));                        
                        
                        logger.info( getClass().getName() +  ".configureAction executed for "+obj.getClass().getName() );
                }
        }
        
        public void startAction(Object obj) throws UserActionException {
                logger.info( getClass().getName() +  ".startAction called for "+obj.getClass().getName() );
                if (obj instanceof StateNotification) {
                        
                        // triggered by State Notification from child resource

                        functionManager.updateSTATE();
                        
                        /************************************************
                         * PUT YOUR CODE HERE                                                   
                         ***********************************************/
                        functionManager.csc.startAction( obj );
                        
                        functionManager.updateSTATE();

                        // set action
                        functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("Starting done.")));

                }
                
                else if (obj instanceof StateEnteredEvent) {
                        System.out.println("Executing startAction");
                        logger.info("Executing startAction");

                        functionManager.updateSTATE();
                        
                        // set action
                        functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("starting")));

                        // get the parameters of the command
                        Integer runNumber = 1;
                        ParameterSet<CommandParameter> parameterSet = getUserFunctionManager().getLastInput().getParameterSet();

                        // check parameter set
                        if (parameterSet.size()==0 || parameterSet.get(MyParameterSet.RUN_NUMBER) == null )  {

                                // go to error, we require parameters
                                String errMsg = "startAction: no parameters given with start command.";
                                
                                // log error
                                logger.error(errMsg);
                                
                                // notify error if GUI access is not allowed, as then the command is sure from the L0 FM
                                // Doesn't really work because RCMSConstants.GUI_ACCESS_ALLOWED is apparently _false_ in stand-alone mode, too...
                                // if ( ! isGUIAccessAllowed() ) sendCMSError(errMsg);
                                
                                // go to error state
//                                functionManager.fireEvent( MyInputs.SETERROR );
//                                return;
                        }
                        else{
                            // get the run number from the start command
                            runNumber = ((IntegerT)parameterSet.get(MyParameterSet.RUN_NUMBER).getValue()).getInteger();
                        }
                        
                        /************************************************
                         * PUT YOUR CODE HERE                                                   
                         ***********************************************/
                        functionManager.csc.startAction( obj );
                        
                        // Set the run number in the Function Manager parameters
                        ((FunctionManagerParameter<IntegerT>)functionManager.getParameterSet().get(MyParameterSet.STARTED_WITH_RUN_NUMBER)).setValue(new IntegerT(runNumber));

                        // leave intermediate state
//                        functionManager.fireEvent( MyInputs.SETRUNNING );
                        
                        // set action
//                        functionManager.getParameterSet().put(new FunctionManagerParameter<StringT>(MyParameterSet.ACTION_MSG,new StringT("")));
                        
                        functionManager.updateSTATE();

                        logger.debug("startAction Executed");
                        
                }
	}

	public void stopAction(Object obj) throws UserActionException {
                logger.info( getClass().getName() +  ".stopAction called for "+obj.getClass().getName() );
                if (obj instanceof StateNotification) {
                        
                        // triggered by State Notification from child resource
                        
                        functionManager.updateSTATE();

                        /************************************************
                         * PUT YOUR CODE HERE                                                   
                         ***********************************************/
                        functionManager.csc.stopAction( obj );
                        
                        functionManager.updateSTATE();

                        // set action
                        functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("Stopping done.")));

                }
                
                else if (obj instanceof StateEnteredEvent) {    
                        System.out.println("Executing stopAction");
                        logger.info("Executing stopAction");

                        functionManager.updateSTATE();
                       
                        // set action
                        functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("stopping")));
                        
                        /************************************************
                         * PUT YOUR CODE HERE                                                   
                         ***********************************************/
                        functionManager.csc.stopAction( obj );
                        
                        // leave intermediate state
//                        functionManager.fireEvent( MyInputs.SETCONFIGURED );
                        
                        // set action
//                        functionManager.getParameterSet().put(new FunctionManagerParameter<StringT>(MyParameterSet.ACTION_MSG,new StringT("")));
                        
                        functionManager.updateSTATE();

                        logger.debug("stopAction Executed");
                        
                }
	}

        public void haltAction(Object obj) throws UserActionException {
                logger.info( getClass().getName() +  ".haltAction called for "+obj.getClass().getName() );
                
                if (obj instanceof StateNotification) {
                        
                        // triggered by State Notification from child resource
                        
                        functionManager.updateSTATE();

                        /************************************************
                         * PUT YOUR CODE HERE                                                   
                         ***********************************************/
                        functionManager.csc.haltAction( obj );
                       
                        functionManager.updateSTATE();

                        // set action
                        functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("Halting done.")));

                }
                
                else if (obj instanceof StateEnteredEvent) {
                        System.out.println("Executing haltAction");
                        logger.info("Executing haltAction");
                        
                        functionManager.updateSTATE();

                        // set action
                        functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("halting")));
                        
                        /************************************************
                         * PUT YOUR CODE HERE                                                   
                         ***********************************************/
                         functionManager.csc.haltAction( obj );

                        // check from which state we came.
                        if (functionManager.getPreviousState().equals(MyStates.TTSTEST_MODE)) {
                                // when we came from TTSTestMode we need to
                                // 1. give back control of sTTS to HW
                        }
                        
                        
                        // leave intermediate state
//                        functionManager.fireEvent( MyInputs.SETHALTED );
                        
                        // Clean-up of the Function Manager parameters
                        cleanUpFMParameters();
                        
                        functionManager.updateSTATE();

                        logger.debug("haltAction Executed");
                }
	}

        public void errorAction(Object obj) throws UserActionException { // TODO: remove altogether?
                logger.info( getClass().getName() +  ".errorAction called for "+obj.getClass().getName()+" while in "+functionManager.getState().getStateString() );
                        
                if (obj instanceof StateNotification) {
                        functionManager.updateSTATE();
                }                
                else if (obj instanceof StateEnteredEvent) {
                        functionManager.updateSTATE();
                }
        }
        
        public void steadyStateAction(Object obj) throws UserActionException {
            logger.info(getClass().getName() + ".steadyStateAction called for " + obj.getClass().getName() + " while in " + functionManager.getState().getStateString());

                functionManager.updateSTATE();

            if (obj instanceof StateNotification) {
                String reason = ((StateNotification) obj).getReason().replace("<![CDATA[", "").replace("]]>", "");
                logger.warn("Unexpected state notification received while in " + functionManager.getState().getStateString()
                        + " state: destination " + ((StateNotification) obj).getDestination()
                        + ", type " + ((StateNotification) obj).getCommandType()
                        + ", from " + ((StateNotification) obj).getFromState()
                        + ", to " + ((StateNotification) obj).getToState()
                        + ", reason: " + reason);
                // What we do depends on the state and run type:
                String runType = functionManager.getParameterSet().get(MyParameterSet.CSC_RUN_TYPE).getValue().toString();
                if ( ( runType.toLowerCase().equals( "global" ) || runType.toLowerCase().equals( "local" ) ) && 
                     functionManager.getState().equals( MyStates.RUNNING ) ){
                    // If we're running in global or local, we just warn the user, but we don't go into error state.
                    String msg = "WARNING: Unexpected state notification received. Reason: " + (reason.length() == 0 ? "UNKNOWN." : reason);
                    if ( ((StateNotification) obj).getToState().equals( "Crashed" ) ) msg += " (A process crashed.)";
                    msg += " The next state transition will likely fail.";
                    functionManager.getParameterSet().put( new FunctionManagerParameter<>( MyParameterSet.ACTION_MSG, new StringT( msg ) ) );
                }
                else if ( runType.toLowerCase().startsWith( "calib" ) && 
                          functionManager.getState().equals( MyStates.RUNNING ) &&
                          ((StateNotification) obj).getToState().equals( MyStates.HALTED.getStateString() )
                        ){
                    // The automatic calibration sequencer halts by itself. That's normal, we just follow it.
                    try {
                        functionManager.setState( MyStates.HALTED );
                    } catch (StateMachineException ex) {
                        logger.error("Failed to set HALTED on receiving unexpected state notification.", ex);
                    }
                }
                else{
                    // In all other cases we go into error.
                    String msg = "Unexpected state notification received. Reason: " + (reason.length() == 0 ? "UNKNOWN." : reason);
                    if ( ((StateNotification) obj).getToState().equals( "Crashed" ) ) msg += " (A process crashed.)";
                    sendCMSError( msg );
                    try {
                        functionManager.setState(MyStates.ERROR);
                    } catch (StateMachineException ex) {
                        logger.error("Failed to set ERROR on receiving unexpected state notification.", ex);
                    }
                }
            }
            else if (obj instanceof StateEnteredEvent) {}

            functionManager.updateSTATE();
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
                        logger.warn(functionManager.getClass().getName() + ": Failed to send error message " + errMessage);
                }
        }
 
        private void cleanUpFMParameters() {
                // Clean-up of the Function Manager parameters
                functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ACTION_MSG,new StringT("")));
                functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.ERROR_MSG,new StringT("")));
                functionManager.getParameterSet().put(new FunctionManagerParameter<>(MyParameterSet.TTS_TEST_FED_ID,new IntegerT(-1)));
        }

        public boolean isGUIAccessAllowed(){
            return ( (BooleanT) functionManager.getParameterSet().get( RCMSConstants.GUI_ACCESS_ALLOWED).getValue() ).booleanValue();
        }   
}
