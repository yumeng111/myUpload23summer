package rcms.fm.ajaxFM.updatesFromGUI;

import org.apache.log4j.Logger;

import rcms.fm.ajaxFM.MyFunctionManager;
import rcms.fm.ajaxFM.myParameters.MyParameterSet;
import rcms.fm.fw.EventHandlerException;
import rcms.fm.fw.parameter.FunctionManagerParameter;
import rcms.fm.fw.parameter.Parameter;
import rcms.fm.fw.parameter.ParameterSet;
import rcms.fm.fw.parameter.type.ParameterType;
import rcms.fm.fw.parameter.type.StringT;
import rcms.fm.fw.user.UserActionException;
import rcms.fm.fw.user.UserEventHandler;

public class MySetParameterHandler extends UserEventHandler {

	static Logger logger = Logger.getLogger(MySetParameterHandler.class);

	private MyFunctionManager functionManager;

	@Override
	public void init() throws EventHandlerException {
		this.functionManager = (MyFunctionManager) this.getUserFunctionManager();
	}

	public MySetParameterHandler() throws EventHandlerException {
		subscribeForEvents(ParameterSet.class);
		addAnyStateAction("onParameterSet");
	}

	private static final FunctionManagerParameter<StringT> emptyGUICommandParameter = new FunctionManagerParameter<StringT>(
			MyParameterSet.GUI_COMMAND, new StringT(""));

	@SuppressWarnings("rawtypes")
	public void onParameterSet(ParameterSet parameters) throws UserActionException {
		boolean guiParameterUpdateRequired = false;
		
		StringBuilder sb = new StringBuilder("updated parameter(s):");

		for (Object obj : parameters.getParameters()) {

			Parameter<?> parameter = (Parameter<?>) obj;
			String parameterName = parameter.getName();
			ParameterType<?> parameterValue = parameter.getValue();

//			logger.debug(parameterName);
//			logger.debug(parameter.getType());
//			logger.debug(parameterValue);
                        logger.debug( "parameter name '" +  parameterName + "' type '" + parameter.getType() + "' value '" + parameterValue + "'");
			
			sb.append(" ");
			sb.append(parameterName);

			if (parameterName.equals(MyParameterSet.GUI_COMMAND)) {
				this.processGUICommand(parameterValue.toString());
				// reset gui command just in case
				functionManager.getParameterSet().put(emptyGUICommandParameter);
			}
			// process parameter updates relevant for the GUI
			else if (parameterName.equals(MyParameterSet.STRING_PARAMETER1)
					|| parameterName.equals(MyParameterSet.INTEGER_PARAMETER)
					|| parameterName.equals(MyParameterSet.BOOLEAN_PARAMETER)) {
				guiParameterUpdateRequired = true;
			}
		}

		if (guiParameterUpdateRequired) {
			this.functionManager.getParameterSet().put(new FunctionManagerParameter<StringT>(
					MyParameterSet.STRING_PARAMETER1, new StringT(sb.toString())));
			this.functionManager.getChangedParameterSender().requireParameterUpdate();
		}
	}

	private void processGUICommand(String command) {
		if (command.equals("resetAllParameters")) 
			this.functionManager.resetAllParameters();
	}
}
