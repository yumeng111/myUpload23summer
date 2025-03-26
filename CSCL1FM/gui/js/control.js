/* Function to manage the checkbox list of command parameters */
function onClickParameterCheckBox(counter) {
	var parameterNameFieldId = 'parameterName' + counter;
	var checkBoxFieldId = 'parameterCheckBox' + counter;
	var parameterValueFieldId = 'parameterValue' + counter;
	var parameterCommandFieldId = 'parameterCommand' + counter;
	var parameterTypeFieldId = 'parameterType' + counter;
	var checkBoxObj = document.getElementById(checkBoxFieldId);
	
	if (checkBoxObj != null) {
		if (checkBoxObj.checked) {
			enableField(parameterNameFieldId);
			enableParameterField(parameterValueFieldId);
			enableField(parameterCommandFieldId);
			enableField(parameterTypeFieldId);
			var commandName = document.getElementById(parameterCommandFieldId).value;
			manageCommandParameterSection(commandName);
		} else {
			disableField(parameterNameFieldId);
			disableParameterField(parameterValueFieldId);
			disableField(parameterCommandFieldId);
			disableField(parameterTypeFieldId);
		}
	}
}

/* Function to hide or disclose parameters command parameter section */
function onClickCommandParameterCheckBox() {
        console.log('onClickCommandParameterCheckBox called. commandParameterCheckBox='+document.getElementById('commandParameterCheckBox').checked);
	var isChecked = document.getElementById('commandParameterCheckBox').checked;
	if (isChecked) {
		disclose('parameterTableSection');
	} else {
		disableCommandParameterSection();
		hide('parameterTableSection');
	}
}

/* Function to enable a command parameter field */
function enableParameterField(fieldId) {
		enableField(fieldId);
		var object = document.getElementById(fieldId);
		object.className = 'label_left_black_enabled';
}

/* Function to disable a command parameter field */
function disableParameterField(fieldId) {
		disableField(fieldId);
		var object = document.getElementById(fieldId);
		object.value = "";
		object.className = 'label_left_black_disabled';
}

/* Function to disable all command parameters */
function disableCommandParameterSection() {
	for (i = 0; i < commandParameterNumber; i++) {
		var checkBoxFieldId = 'parameterCheckBox' + i;
		var checkBoxObj = document.getElementById(checkBoxFieldId);
			if (checkBoxObj != null) {
				checkBoxObj.checked = false;
				var parameterValueFieldId = 'parameterValue' + i;
				var parameterNameFieldId = 'parameterName' + i;
				var parameterCommandFieldId = 'parameterCommand' + i;
				var parameterTypeFieldId = 'parameterType' + i;
				
				disableParameterField(parameterValueFieldId);
				disableField(parameterNameFieldId);
				disableField(parameterCommandFieldId);
				disableField(parameterTypeFieldId);
		}
	}
}

/* Function to manage command parameter section */
function manageCommandParameterSection(commandName) {
		for (i = 0; i < commandParameterNumber; i++) {
			var parameterCommandFieldId = 'parameterCommand' + i;
			var parameterCommandObject = document.getElementById(parameterCommandFieldId);
			
			var parameterValueFieldId = 'parameterValue' + i;
			var parameterNameFieldId = 'parameterName' + i;
			var parameterTypeFieldId = 'parameterType' + i;
			
			var checkBoxFieldId = 'parameterCheckBox' + i;
			var checkBoxObject = document.getElementById(checkBoxFieldId);

			if (parameterCommandObject != null && parameterCommandObject.value != commandName) {
				checkBoxObject.checked = false;
				disableField(parameterNameFieldId);
				disableParameterField(parameterValueFieldId);
				disableField(parameterCommandFieldId);
				disableField(parameterTypeFieldId);
			}
	}
}

/* Draws the state label value */
// This can be overriden by the same function defined in controlPanel.jsp
function drawState(state) {
        console.log("drawState 1 called for "+state.value);
	if (state != null) {
		setFieldInnerHtml('currentState', state.value)
		setFieldInnerHtml('State', state.value)
	}
}
