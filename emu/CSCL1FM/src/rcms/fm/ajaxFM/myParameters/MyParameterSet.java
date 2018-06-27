package rcms.fm.ajaxFM.myParameters;

import java.util.Map;

import org.apache.log4j.Logger;

import rcms.fm.fw.parameter.FunctionManagerParameter;
import rcms.fm.fw.parameter.ParameterException;
import rcms.fm.fw.parameter.ParameterSet;
import rcms.fm.fw.parameter.type.BooleanT;
import rcms.fm.fw.parameter.type.IntegerT;
import rcms.fm.fw.parameter.type.StringT;
import rcms.fm.fw.parameter.type.DoubleT;
import rcms.fm.fw.parameter.type.VectorT;
import rcms.fm.fw.parameter.FunctionManagerParameter.Exported;

@SuppressWarnings("rawtypes")
public class MyParameterSet extends ParameterSet<FunctionManagerParameter> {

	static Logger logger = Logger.getLogger(MyParameterSet.class);

	private static MyParameterSet instance;

	public static final String FM_GROUP_ID = "FM_GROUP_ID";

	public static final String GUI_COMMAND = "GUI_COMMAND";

	public static final String STRING_PARAMETER1 = "STRING_PARAMETER1";
	public static final String INTEGER_PARAMETER = "INTEGER_PARAMETER";
	public static final String BOOLEAN_PARAMETER = "BOOLEAN_PARAMETER";
        
	
	/**
	 * standard parameter definitions for Level 1 Function Manager
	 */	

	public static final String STATE = "STATE";


	public static final String ACTION_MSG = "ACTION_MSG";
	public static final String ERROR_MSG = "ERROR_MSG";
	public static final String COMPLETION = "COMPLETION";
        
        // For run number nooking
        public static final String SEQ_NAME = "SEQ_NAME";


	// for HCAL LUMI only
 	// public static final String HCAL_LUMI_STATE = "HCAL_LUMI_STATE";
	// public static final String HCAL_LUMI_KEY = "HCAL_LUMI_KEY";
	// public static final String HCAL_LUMI_COMMAND = "HCAL_LUMI_COMMAND";
	
	
	// To be exported after initialize
	public static final String INITIALIZED_WITH_SID = "INITIALIZED_WITH_SID";
	public static final String INITIALIZED_WITH_GLOBAL_CONF_KEY = "INITIALIZED_WITH_GLOBAL_CONF_KEY";
        public static final String CSC_RUN_TYPES_AVAILABLE  = "CSC_RUN_TYPES_AVAILABLE";
        public static final String CSC_RUN_TYPES_AVAILABLE_V  = "CSC_RUN_TYPES_AVAILABLE_V";

	// To be exported after configure
	public static final String CONFIGURED_WITH_FED_ENABLE_MASK = "CONFIGURED_WITH_FED_ENABLE_MASK";
	public static final String CONFIGURED_WITH_GLOBAL_CONF_KEY = "CONFIGURED_WITH_GLOBAL_CONF_KEY";
	public static final String CONFIGURED_WITH_RUN_KEY = "CONFIGURED_WITH_RUN_KEY";
	public static final String CONFIGURED_WITH_RUN_NUMBER = "CONFIGURED_WITH_RUN_NUMBER";
	public static final String CONFIGURED_WITH_RUN_TYPE = "CONFIGURED_WITH_RUN_TYPE";
	public static final String CONFIGURED_WITH_CSC_RUN_TYPE = "CONFIGURED_WITH_CSC_RUN_TYPE";
	public static final String CONFIGURED_WITH_USE_PRIMARY_TCDS = "CONFIGURED_WITH_USE_PRIMARY_TCDS";
        public static final String CONFIGURED_WITH_TF_KEY = "CONFIGURED_WITH_TF_KEY";
	
	// To be exported after start
	public static final String STARTED_WITH_RUN_NUMBER = "STARTED_WITH_RUN_NUMBER";
	
	
	// Command parameters
	public static final String SID = "SID";
	public static final String GLOBAL_CONF_KEY = "GLOBAL_CONF_KEY";

	public static final String RUN_NUMBER = "RUN_NUMBER";
	public static final String RUN_KEY = "RUN_KEY";
	public static final String RUN_TYPE = "RUN_TYPE";

	public static final String FED_ENABLE_MASK = "FED_ENABLE_MASK";
	public static final String TRIGGER_NUMBER_AT_PAUSE = "TRIGGER_NUMBER_AT_PAUSE";

	// Command parameters for TTS testing
	public static final String TTS_TEST_FED_ID = "TTS_TEST_FED_ID";
	
	public static final String TTS_TEST_MODE = "TTS_TEST_MODE";
	
	public static final String TTS_TEST_PATTERN = "TTS_TEST_PATTERN";
	
	public static final String TTS_TEST_SEQUENCE_REPEAT = "TTS_TEST_SEQUENCE_REPEAT";
	
        // CSC
//        public static final String CSC_CALIB_KEYS_AVAILABLE = "CSC_CALIB_KEYS_AVAILABLE";
//        public static final String CSC_CALIB_KEY            = "CSC_CALIB_KEY";
        public static final String CSC_RUN_TYPE             = "CSC_RUN_TYPE";
        public static final String TF_KEY                   = "TF_KEY";
	public static final String USE_PRIMARY_TCDS         = "USE_PRIMARY_TCDS";
        
        
	public static boolean isForGUI(String parameterName) {
		boolean isForGUI = false;

//		if (   parameterName.equals(STRING_PARAMETER1)
//			|| parameterName.equals(INTEGER_PARAMETER)
//			|| parameterName.equals(BOOLEAN_PARAMETER)
//                        || parameterName.equals(CSC_RUN_TYPES_AVAILABLE_V)
//		) {
			isForGUI = true;
//		}

		return isForGUI;
	}

	public static MyParameterSet getInstance() {
		if (instance == null) {
			synchronized (MyParameterSet.class) {
				if (instance == null) {
					instance = new MyParameterSet();
				}
			}
		}
		return instance;
	}
	
	private MyParameterSet() {
		super();

		try {
			this.initializeParameters();
		} catch (ParameterException ex) {
			logger.error("Encountered ParameterException while initializing parameter set.", ex);
		}
	}

	public synchronized void initializeParameters() throws ParameterException {
		this.put(new FunctionManagerParameter<IntegerT>(FM_GROUP_ID, new IntegerT(0)));

		this.put(new FunctionManagerParameter<StringT>(GUI_COMMAND, new StringT("")));

		this.put(new FunctionManagerParameter<StringT>(STRING_PARAMETER1, new StringT("")));
		this.put(new FunctionManagerParameter<IntegerT>(INTEGER_PARAMETER, new IntegerT(0)));
		this.put(new FunctionManagerParameter<BooleanT>(BOOLEAN_PARAMETER, new BooleanT(false)));
                
		/**
		 * State of the Function Manager is currently in
		 */
		this.put(new FunctionManagerParameter<StringT>(STATE, new StringT(""), Exported.READONLY));

		/**
		 * parameters for monitoring
		 */
		this.put(new FunctionManagerParameter<StringT>(ACTION_MSG, new StringT(""), Exported.READONLY));
		this.put(new FunctionManagerParameter<StringT>(ERROR_MSG, new StringT(""), Exported.READONLY));
		this.put(new FunctionManagerParameter<DoubleT>(COMPLETION, new DoubleT(-1), Exported.READONLY));
		
		this.put(new FunctionManagerParameter<IntegerT>(RUN_NUMBER, new IntegerT(-1), Exported.READONLY));

                /**
		 * Session Identifier
		 */                
		this.put(new FunctionManagerParameter<StringT>(SEQ_NAME, new StringT("DEFAULT"),Exported.READONLY));
		this.put(new FunctionManagerParameter<IntegerT>(SID, new IntegerT(-1)));
                
		this.put(new FunctionManagerParameter<IntegerT>(INITIALIZED_WITH_SID, new IntegerT(-1), Exported.READONLY));
		this.put(new FunctionManagerParameter<StringT>(INITIALIZED_WITH_GLOBAL_CONF_KEY, new StringT(""), Exported.READONLY));
				
		this.put(new FunctionManagerParameter<StringT>(CONFIGURED_WITH_FED_ENABLE_MASK, new StringT(""), Exported.READONLY));
		this.put(new FunctionManagerParameter<StringT>(CONFIGURED_WITH_GLOBAL_CONF_KEY, new StringT(""), Exported.READONLY));
		this.put(new FunctionManagerParameter<StringT>(CONFIGURED_WITH_RUN_KEY, new StringT(""), Exported.READONLY));
		this.put(new FunctionManagerParameter<StringT>(CONFIGURED_WITH_RUN_TYPE, new StringT(""), Exported.READONLY));
		this.put(new FunctionManagerParameter<IntegerT>(CONFIGURED_WITH_RUN_NUMBER, new IntegerT(-1), Exported.READONLY));
		
		this.put(new FunctionManagerParameter<IntegerT>(STARTED_WITH_RUN_NUMBER, new IntegerT(-1), Exported.READONLY));

		/** special - for HCAL only */
		// this.put(new FunctionManagerParameter<StringT>(HCAL_LUMI_STATE, new StringT(""), Exported.READWRITE));
		// this.put(new FunctionManagerParameter<StringT>(HCAL_LUMI_COMMAND, new StringT(""), Exported.READWRITE));
		// this.put(new FunctionManagerParameter<StringT>(HCAL_LUMI_KEY, new StringT(""), Exported.READWRITE));
		
//              CSC
                this.put(new FunctionManagerParameter<StringT>(CSC_RUN_TYPE, new StringT(""), Exported.READWRITE));
                this.put(new FunctionManagerParameter<BooleanT>(USE_PRIMARY_TCDS, new BooleanT(true), Exported.READWRITE));

                VectorT<StringT> rta = new VectorT<StringT>();
//                rta.add( new StringT( "Local" ) );
//                rta.add( new StringT( "Global" ) );
                this.put(new FunctionManagerParameter<VectorT<StringT>>(CSC_RUN_TYPES_AVAILABLE_V, rta ));
                this.put(new FunctionManagerParameter<StringT>(CSC_RUN_TYPES_AVAILABLE, new StringT(""), Exported.READONLY));
                this.put(new FunctionManagerParameter<VectorT<StringT>>(CSC_RUN_TYPES_AVAILABLE_V, new VectorT<StringT>(), Exported.READONLY));
                this.put(new FunctionManagerParameter<StringT>(CONFIGURED_WITH_CSC_RUN_TYPE, new StringT(""), Exported.READONLY));
                this.put(new FunctionManagerParameter<BooleanT>(CONFIGURED_WITH_USE_PRIMARY_TCDS, new BooleanT(true), Exported.READONLY));
        
        }

	/** Returns a copy of this ParameterSet. Copied from LV0. **/
	@SuppressWarnings("unchecked")
	public synchronized MyParameterSet getClonedParameterSet() {
		MyParameterSet cloned = new MyParameterSet();
		for (Map.Entry<String, FunctionManagerParameter> pair : this.getMap().entrySet()) {
			if (pair.getValue() instanceof FunctionManagerParameter) {
				cloned.put(new FunctionManagerParameter((FunctionManagerParameter) pair.getValue()));
			}
		}
		return cloned;
	}

	/**
	 * ASSUMES NO PARAMETERS CAN BE ADDED UPON MYPARAMETERSET'S CREATION.
	 * Returns a ParameterSet containing only changed parameters. Copied from
	 * LV0.
	 **/
	@SuppressWarnings("unchecked")
	public synchronized ParameterSet<FunctionManagerParameter> getChanged(
			ParameterSet<FunctionManagerParameter> earlier) {
		ParameterSet<FunctionManagerParameter> changed = new ParameterSet<FunctionManagerParameter>();
		for (Map.Entry<String, FunctionManagerParameter> pair : this.getMap().entrySet()) {
			try {
				if (earlier == null || earlier.get(pair.getKey()) == null
						|| !pair.getValue().getValue().equals(earlier.get(pair.getKey()).getValue())) {
					changed.put(new FunctionManagerParameter((FunctionManagerParameter) pair.getValue()));
				}
			} catch (Exception ex) {
				logger.error(String.format("Failed to determine if parameter %s changed", pair.getKey()), ex);
			}
		}
		return changed;
	}

}
