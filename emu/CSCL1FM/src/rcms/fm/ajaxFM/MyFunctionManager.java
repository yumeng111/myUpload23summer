package rcms.fm.ajaxFM;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

//import org.apache.log4j.Logger;

//import rcms.fm.context.RCMSConstants;

import rcms.fm.ajaxFM.myParameters.MyParameterSet;
import rcms.fm.ajaxFM.notificationsToGUI.parameters.ChangedParameterSender;
import rcms.fm.ajaxFM.updatesFromGUI.MySetParameterHandler;
import rcms.fm.fw.EventHandlerException;
import rcms.fm.fw.parameter.CommandParameter;
import rcms.fm.fw.parameter.FunctionManagerParameter;
import rcms.fm.fw.parameter.ParameterException;
import rcms.fm.fw.parameter.ParameterSet;
import rcms.fm.fw.parameter.type.IntegerT;
import rcms.fm.fw.parameter.type.StringT;
//import rcms.fm.fw.parameter.type.BooleanT;
import rcms.fm.fw.user.UserActionException;
import rcms.fm.fw.user.UserFunctionManager;
import rcms.fm.resource.StateVectorCalculation;
import rcms.statemachine.definition.State;
import rcms.statemachine.definition.StateMachineDefinitionException;

import rcms.fm.fw.user.UserStateMachineDefinition;

import rcms.util.logger.RCMSLogger;
import rcms.fm.resource.qualifiedresource.XdaqApplicationContainer;




import rcms.fm.resource.QualifiedResource;
import rcms.fm.resource.qualifiedresource.XdaqApplication;
import java.util.List;



public class MyFunctionManager extends UserFunctionManager {

	//static Logger logger = Logger.getLogger(MyFunctionManager.class);
        static RCMSLogger logger = new RCMSLogger(MyFunctionManager.class);

        public XdaqApplicationContainer containerXdaqApplication = null;

        public StateVectorCalculation svCalc = null;

	public State calcState = null;

        // CSC stuff
        public CSC csc=null;

        private MyParameterSet parameterSet;
	
	private ScheduledExecutorService integerParameterCounter;

	/**
	 * This class instance runs periodically and sends updated FM parameters to
	 * the GUI. To indicate that a parameter update is required, the method
	 * requireParameterUpdate() needs to be called every time a GUI-relevant
	 * parameter has changed.
	 */
	private ChangedParameterSender changedParameterSender;

	private boolean _isDestroyed = false;
//	private boolean _isDestroyed = true;

	public MyFunctionManager() {
		this.parameterSet = MyParameterSet.getInstance();
	}
        
	public boolean isDestroyed() {
		return this._isDestroyed;
	}

	public ChangedParameterSender getChangedParameterSender() {
		return this.changedParameterSender;
	}

	@Override
	public MyParameterSet getParameterSet() {
		return this.parameterSet;
	}

	@Override
	public void init() throws StateMachineDefinitionException, EventHandlerException {

                System.out.println("init called.");
                logger.debug("init called.");
        
                        
            // Instantiate CSC stuff
                csc = new CSC( this );

                // Adapt FSM to the configuration. If no Supervisor, we only control the life cycle.
                // This could probably also have been done by setting MyInputs.*.setVisualizable(false) ...
                boolean isReducedFSM = controlLifeCycleOnly();
                this.setStateMachineDefinition(new MyStateMachineDefinition( isReducedFSM ));
                
		this.addEventHandler(new MyEventHandler());
		
		// processes parameter changes by the control servlet
		this.addEventHandler(new MySetParameterHandler());

	}

        /*
         * (non-Javadoc)
         * 
         * @see rcms.statemachine.user.UserStateMachine#createAction()
         */
	@SuppressWarnings("rawtypes")
	@Override
	public void createAction(ParameterSet<CommandParameter> parameters) throws UserActionException {
                // This method is called by the framework when the Function Manager is
                // created.

                System.out.println("createAction called.");
                logger.debug("createAction called.");

                csc.killOrphanedExecutives();

                this.getParameterSet().put(new FunctionManagerParameter<IntegerT>(MyParameterSet.FM_GROUP_ID,
				new IntegerT(this.getGroup().getThisResource().getId())));

		this.changedParameterSender = new ChangedParameterSender(this);
		this.changedParameterSender.start();
		logger.debug("ChangedParameterSender created and started");
		
		this.integerParameterCounter = Executors.newSingleThreadScheduledExecutor();
//		this.integerParameterCounter.scheduleAtFixedRate(new IntegerParameterCounter(this), 500, 500, TimeUnit.MILLISECONDS);
		this.integerParameterCounter.scheduleAtFixedRate(new IntegerParameterCounter(this), 500, 2000, TimeUnit.MILLISECONDS);

		// set Running state
		this.calcState = MyStates.RUNNING;

	}


       /*
         * (non-Javadoc)
         * 
         * @see rcms.statemachine.user.UserStateMachine#destroyAction()
         */
	@Override
	public void destroyAction() throws UserActionException {
                //
                // This method is called by the framework when the Function Manager is
                // destroyed.
                //
		this.integerParameterCounter.shutdownNow();
		this.changedParameterSender.shutdown();

                getQualifiedGroup().destroy();

                this._isDestroyed = true;
	}

	@Override
	public State getUpdatedState() {
		return this.getState();
	}

	@Override
	public boolean hasCustomGUI() {
		return true;
	}
	
        private boolean controlLifeCycleOnly(){
            // Check if we have a Supervisor. If so, we'll need to drive its FSM, not just control the apps' life cycle.
            List<QualifiedResource> xdaqApps = getQualifiedGroup().seekQualifiedResourcesOfType(new XdaqApplication());
            logger.info( "XDAQ apps: "+xdaqApps.size() );
            for (QualifiedResource app: xdaqApps ){
                logger.info( "XDAQ app name: "+app.getName()+", URI: "+app.getURI() );
                if ( app.getName().contains( "emu::supervisor::Application" ) ) return false;
            }
            return true;
        }
        
        public void updateSTATE(){
                ((FunctionManagerParameter<StringT>)getParameterSet().get(MyParameterSet.STATE)).setValue( new StringT( getState().getStateString() ) );
        }

//        public boolean isGUIAccessAllowed(){
//            if ( this.fm != null && !( (BooleanT)this.fm.getParameter().get(RCMSConstants.GUI_ACCESS_ALLOWED).getValue() ).booleanValue() ){
//                return false;
//            }
//            return true;
//        }
        
        public void resetAllParameters() {
		try {
			this.getParameterSet().initializeParameters();
		} catch (ParameterException ex) {
			logger.error("Error reinitializig parameters.", ex);
		}
		this.changedParameterSender.requireParameterUpdate();
	}

	
	private class IntegerParameterCounter implements Runnable {
		
		private MyParameterSet parameterSet;
		private ChangedParameterSender parameterSender;
		
		public IntegerParameterCounter(MyFunctionManager parent) {
			this.parameterSet = parent.getParameterSet();
			this.parameterSender = parent.getChangedParameterSender();
		}

		@Override
		public void run() {
			int currentCount = ((IntegerT) this.parameterSet.get(MyParameterSet.INTEGER_PARAMETER).getValue()).intValue();
			this.parameterSet.put(new FunctionManagerParameter<IntegerT>(MyParameterSet.INTEGER_PARAMETER, new IntegerT(++currentCount)));
			this.parameterSender.requireParameterUpdate();
		}
		
	}

}
