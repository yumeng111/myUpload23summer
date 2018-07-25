package rcms.fm.ajaxFM;

import rcms.fm.ajaxFM.myParameters.MyParameterSet;

import rcms.fm.fw.parameter.CommandParameter;
import rcms.fm.fw.parameter.ParameterException;
import rcms.fm.fw.parameter.ParameterSet;
import rcms.fm.fw.parameter.type.IntegerT;
import rcms.fm.fw.parameter.type.LongT;
import rcms.fm.fw.parameter.type.StringT;
import rcms.fm.fw.parameter.type.BooleanT;
import rcms.fm.fw.user.UserStateMachineDefinition;
import rcms.statemachine.definition.StateMachineDefinitionException;

public class MyStateMachineDefinition extends UserStateMachineDefinition {

	public MyStateMachineDefinition( boolean controlLifeCycleOnly ) throws StateMachineDefinitionException {
		super();
		
//		addState(MyStates.RUNNING);
//		addState(MyStates.HALTED);
		
                //
                // INITIALIZE
                //
                addState(MyStates.INITIAL);
                addState(MyStates.INITIALIZING);
		addState(MyStates.HALTED);
                addInput(MyInputs.INITIALIZE);
//                addInput(MyInputs.SETHALTED);
		addTransition(MyInputs.INITIALIZE, MyStates.INITIAL, MyStates.INITIALIZING);
//		addTransition(MyInputs.SETHALTED, MyStates.INITIALIZING, MyStates.HALTED);

                //
                // ERROR
                //
                addState(MyStates.ERROR);
                addInput(MyInputs.SETERROR);
                MyInputs.SETERROR.setVisualizable(false); // To prevent it from being displayed in the GUI
		addTransition(MyInputs.SETERROR, MyStates.INITIALIZING, MyStates.ERROR);

                
                setInitialState(MyStates.INITIAL);
                

                if ( !controlLifeCycleOnly ) extend();
        }
        
        public void extend() throws StateMachineDefinitionException {
                // Only invoked if we have a full FSM, i.e. we don't just control the life cycle.


//		addInput(MyInputs.START);
//		addInput(MyInputs.STOP);
//		
//		addInput(MyInputs.STARTED);
//		addInput(MyInputs.STOPPED);
//		
//		addTransition(MyInputs.START, MyStates.HALTED, MyStates.STARTING);
//		addTransition(MyInputs.STARTED, MyStates.STARTING, MyStates.RUNNING);
//		addTransition(MyInputs.STOP, MyStates.RUNNING, MyStates.STOPPING);
//		addTransition(MyInputs.STOPPED, MyStates.STOPPING, MyStates.HALTED);

                //
                // CONFIGURE
                //
                addState(MyStates.CONFIGURING);
                addState(MyStates.CONFIGURED);
                addInput(MyInputs.CONFIGURE);
//		addInput(MyInputs.SETCONFIGURED);
		addTransition(MyInputs.CONFIGURE, MyStates.HALTED, MyStates.CONFIGURING);
//		addTransition(MyInputs.SETCONFIGURED, MyStates.CONFIGURING, MyStates.CONFIGURED);
		//
		// define parameters for Configure command
		//
		CommandParameter<StringT> configureFedEnableMask = new CommandParameter<StringT>(MyParameterSet.FED_ENABLE_MASK, new StringT(""));
		CommandParameter<StringT> configureCSCRunType = new CommandParameter<StringT>(MyParameterSet.CSC_RUN_TYPE, new StringT(""));
		CommandParameter<BooleanT> configureUsePrimaryTCDS = new CommandParameter<BooleanT>(MyParameterSet.USE_PRIMARY_TCDS, new BooleanT(true));
		// define parameter set
		ParameterSet<CommandParameter> configureParameters = new ParameterSet<CommandParameter>();
		try {
			// configureParameters.add(configureFedEnableMask);
                        configureParameters.add(configureCSCRunType);
                        configureParameters.add(configureUsePrimaryTCDS);
                } catch (ParameterException nothing) {
			// Throws an exception if a parameter is duplicate
			throw new StateMachineDefinitionException( "Could not add to configureParameters. Duplicate Parameter?", nothing );
		}
		MyInputs.CONFIGURE.setParameters(configureParameters);

                //
                // START
                //
                addState(MyStates.STARTING);
                addState(MyStates.RUNNING);
                addInput(MyInputs.START);
//		addInput(MyInputs.SETRUNNING);
		addTransition(MyInputs.START, MyStates.CONFIGURED, MyStates.STARTING);
//		addTransition(MyInputs.SETRUNNING, MyStates.STARTING, MyStates.RUNNING);
		//
		// define parameters for Start command
		//
		CommandParameter<IntegerT> startRunNumber = new CommandParameter<>(MyParameterSet.RUN_NUMBER, new IntegerT(0));
		// define parameter set
		ParameterSet<CommandParameter> startParameters = new ParameterSet<CommandParameter>();
		try {
			startParameters.add(startRunNumber);
                } catch (ParameterException nothing) {
			// Throws an exception if a parameter is duplicate
			throw new StateMachineDefinitionException( "Could not add to startParameters. Duplicate Parameter?", nothing );
		}
		// MyInputs.START.setParameters(startParameters);

                
                //
                // STOP
                //
                addState(MyStates.STOPPING);
                addInput(MyInputs.STOP);
		addTransition(MyInputs.STOP, MyStates.RUNNING, MyStates.STOPPING);
//		addTransition(MyInputs.SETCONFIGURED, MyStates.STOPPING, MyStates.CONFIGURED);

                //
                // HALT
                //
                addState(MyStates.HALTING);
                addInput(MyInputs.HALT);
		addTransition(MyInputs.HALT, MyStates.CONFIGURING, MyStates.HALTING); // TODO: review whether needed
		addTransition(MyInputs.HALT, MyStates.CONFIGURED, MyStates.HALTING);
		addTransition(MyInputs.HALT, MyStates.STARTING, MyStates.HALTING); // TODO: review whether needed
		addTransition(MyInputs.HALT, MyStates.RUNNING, MyStates.HALTING);
//		addTransition(MyInputs.SETHALTED, MyStates.HALTING, MyStates.HALTED);

       }

}
