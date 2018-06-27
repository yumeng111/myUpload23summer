<%@ page language="java" contentType="text/html"%>
<%@ page import="java.util.*"%>
<%@ page import="rcms.gui.servlet.pilot.FMPilotConstants"%>
<%@ page isELIgnored="false"%>

<%@ page import="rcms.gui.servlet.pilot.FMPilotBean"%>
<%@ page import="rcms.gui.common.FMPilotState"%>

<%@ taglib prefix="rcms.menu" uri="rcms.menu"%>
<%@ taglib prefix="rcms.control" uri="rcms.control"%>
<%@ taglib prefix="rcms.globalParameter" uri="rcms.globalParameter"%>
<%@ taglib prefix="rcms.notification" uri="rcms.notification"%>

<%!public static boolean isDetachable(HttpServletRequest request) {
		return ((FMPilotBean) request.getAttribute(FMPilotConstants.FM_PILOT_BEAN)).getSessionState()
				.isInputAllowed(FMPilotState.DETACH);
	}%>
<rcms.control:menuCreator />

<!DOCTYPE html>
<html>

<head>
<meta Http-Equiv="Cache-Control" Content="no-cache">
<meta Http-Equiv="Pragma" Content="no-cache">
<meta Http-Equiv="Expires" Content="0">
<meta http-equiv="Content-type" content="text/html;charset=UTF-8">

<title>CSC FM</title>

<link rel="StyleSheet" href="../css/common.css" type="text/css" />
<link rel="StyleSheet" href="../css/control.css" type="text/css" />

<rcms.control:customResourceRenderer indentation="0" type="css"
	path="/css/csc.css" />

<!--  Java script -->
<rcms.control:customResourceRenderer indentation="0" type="js"
	path="/js/GUI.js" />

<script type="text/javascript">
	var guiInst = new GUI();
</script>

<rcms.control:customResourceRenderer indentation="0" type="js"
	path="/js/jquery-2.1.4.min.js" />
<rcms.control:customResourceRenderer indentation="0" type="js"
	path="/js/ajaxRequest.js" />

<rcms.control:customResourceRenderer indentation="0" type="js"
	path="/js/ajaxRequestFunctions.js" />
<rcms.control:customResourceRenderer indentation="0" type="js"
	path="/js/notifications.js" />
<!--rcms.control:customResourceRenderer indentation="0" type="js" TODO:remove button.js
	path="/js/buttons.js" /-->
<rcms.control:customResourceRenderer indentation="0" type="js"
	path="/js/control.js" />

<rcms.globalParameter:getParameterMap fmParameterContainer="pars" />

<script type="text/javascript" src="../js/stateNotification.js"></script>
<script type="text/javascript" src="../js/common.js"></script>
<script type="text/javascript" src="../js/globalParameters.js"></script>


<!-- Custom javascript section begin -->
<script type="text/javascript">
	<rcms.control:onLoadJSRenderer 
		reloadOnStateChange="false" 
		commandButtonCssClass="MyControlButton" 
		commandParameterCheckBoxTitle="&nbsp;Show Command Parameter Section"
		commandParameterCssClass="label_left_black" 
		indentation="2"/>
		
	<rcms.control:buttonsJSRenderer indentation="2"/>

	<rcms.notification:jSRenderer indentation="2"/>

	<rcms.globalParameter:jSRenderer indentation="2"/>

//	function drawMyCommandButtons(currentState) {
	function drawCommandButtons(currentState) {
            // This function is called by render() on receiving notification of type "Status"
                console.log('Oops, drawCommandButtons (dummy) called...');
		// do nothing
		// placeholder for custom function
	}
        
        // This overrides that in js/control.js
        //function drawState(state) {
        //    console.log("drawState 2 called for "+state.value);
        //    $('#State').text(state.value);
        //}

        var previousStateName=""; // global parameter to keep track of state changes
        // This overrides function drawCommandButtons(state) written by framework/gui/src/rcms/gui/tag/control/OnLoadJSRenderer.java
//        function drawCommandButtons(state) {
        function drawMyCommandButtons(state) {
                // function drawMyCommandButtons is called by render upon receiving a notification message of type "Status".
                // It's also called by myUpdateParameters, which in turn is called by render upon receiving a notification message of type "Parameter".
                console.log('drawMyCommandButtons called');
                var text = '';
                if (commands != null && commands.length != 0) {
                        for (var index = 0; index < commands.length; index++) {
//                                console.log('drawMyCommandButtons checked '+commands[index].name+' for '+state.value);
                                if (isCommandEnabled(transitions, commands[index], state)) {
                                        text += '<input type="button" id="command' + index + '" class="MyControlButton my'+commands[index].name+'Button" value="' + commands[index].name + '" name="command' + index + '" onClick="onClickCommandButton(\'' + commands[index].input + '\')">\n';
//                                        console.log('              ...OK');
                                }
                        }
                }
//                setFieldInnerHtml('commandSection', text);
                setFieldInnerHtml('myCommandSection', text);
                // Update command parameters on state change only. 
                if ( state.name != previousStateName ){
                    drawMyCommandParameters(state);
                    previousStateName = state.name;
                }
        }
		
        // This overrides function drawCommandParameters(state) written by framework/gui/src/rcms/gui/tag/control/OnLoadJSRenderer.java
        function drawCommandParameters(state) {
                console.log('Oops, drawCommandParameters (dummy) called...');
        }
                        
        var cscRunTypeSelectTagId=null;
        function drawMyCommandParameters(state) {
                var htmlObject = null;
                var text = '';
                var enabledCommandParameterNumber = 0;
                if (commands != null && commands.length != 0 && commandParameters != null && commandParameters.length != 0) {
                        for (var index = 0; index < commands.length; index++) {
                                if (isCommandEnabled(transitions, commands[index], state)) {
                                        for (var j = 0; j < commandParameters.length; j++) {
                                                if (commandParameters[j].commandName == commands[index].input) {
                                                        enabledCommandParameterNumber++;
                                                        text +=	'<div class="flexRow">'
                                                        text +=	'  <input type="hidden" id="parameterCommand' + enabledCommandParameterNumber + '" name="parameterCommand' + enabledCommandParameterNumber + '" value="' + commands[index].input + '" >\n';
                                                        text +=	'  <input type="hidden" id="parameterName' + enabledCommandParameterNumber + '" name="parameterName' + enabledCommandParameterNumber + '" value="' + commandParameters[j].parameterName + '" >\n';
                                                        text +=	'  <input type="hidden" id="parameterType' + enabledCommandParameterNumber + '" name="parameterType' + enabledCommandParameterNumber + '" value="' + commandParameters[j].parameterType + '" >\n';
                                                        text +=	'  <input type="checkbox" id="parameterCheckBox' + enabledCommandParameterNumber + '" onclick="onClickParameterCheckBox(\'' + enabledCommandParameterNumber + '\')" checked="checked"/>\n';
                                                        text +=	commandParameters[j].parameterName+'&nbsp;';
                                                        if ( commandParameters[j].parameterType.search("BooleanT") >=0 ){
                                                            text +='   <select id="parameterValue' + enabledCommandParameterNumber + '" name="parameterValue' + enabledCommandParameterNumber + '" class="label_left_black_disabled"  />\n';
                                                            text +='      <option selected="selected" value="true">true</option>\n<option value="false">false</option>\n';
                                                            text +='   </select>';
                                                        }
                                                        else if( commandParameters[j].parameterName == "CSC_RUN_TYPE"){
                                                            cscRunTypeSelectTagId = "parameterValue" + enabledCommandParameterNumber;
                                                            text +='    <select id="parameterValue' + enabledCommandParameterNumber + '" name="parameterValue' + enabledCommandParameterNumber + '" class="label_left_black_disabled"  />\n';
                                                        }
                                                        else{
                                                            text +='  <input type="text" id="parameterValue' + enabledCommandParameterNumber + '" name="parameterValue' + enabledCommandParameterNumber + '" value="" class="label_left_black_disabled" size="50" maxlength="100"  />\n';
                                                        }
                                                        text +=	'</div>'
                                               }
                                        }
                                }
                        }
                }
                setFieldInnerHtml('commandParameterSection', text);
                text = '';
                if (enabledCommandParameterNumber > 0) {
                        text += '<input type="checkbox" id="commandParameterCheckBox" name="commandParameterCheckBox" value="" onclick="onClickCommandParameterCheckBox()"/>';
                        text += '&nbsp;Show Command Parameters';
                }
                setFieldInnerHtml('commandParameterCheckBoxSection', text);
        }

        // This overrides function onDestroyButton() written by generateHtml() in RunControl/framework/gui/src/rcms/gui/tag/control/ButtonsJSRenderer.java
        function onDestroyButton() {
            if (confirm("Everything will be terminated!\nAre you sure you want to destroy?") == false) {
                return;
            }
            submitForm('FMPilotForm', 'Destroy');
        }

       
</script>
<!-- Custom javascript section end -->

</head>
<body class="body">
    <table width="100%" border="0" cellpadding="0" cellspacing="0">
	<!-- Header fragment -->
	<jsp:include page="./header.jsp"/>
            <tr>
		<td width="17%" valign="top" bgcolor="#EEEEEE">
			<br>
			<rcms.menu:menuRenderer indentation="3"/>
		</td>
		<td id="customizedArea" height="259" valign="top" colspan="2">

	<!-- This is the main FMPilotForm, which is required for non-AJAX requests and can be submitted using AJAXRequest. -->
<!--	<form name="FMPilotForm" id="FMPilotForm" method="POST"
		action="../../gui/servlet/FMPilotServlet?PAGE=/gui/jsp/controlPanel.jsp">

		<rcms.control:actionHiddenInputRenderer indentation="1" />
		<rcms.control:commandHiddenInputRenderer indentation="1" />
		<rcms.notification:hiddenInputRenderer indentation="1" />
		<rcms.control:configurationKeyRenderer titleClass="control_label1"
			hidden="true" label="Configuration Keys:&nbsp;"
			contentClass="control_label2" indentation="1" />

		 The following part is required for submitting the form using the AJAX Request Library or without AJAX. 
		<input type="hidden" id="globalParameterName1"
			name="globalParameterName1" value="" />
		<input type="hidden"
			id="globalParameterValue1" name="globalParameterValue1" value="" />
		<input type="hidden" id="globalParameterType1"
			name="globalParameterType1" value="" />
		<input type="hidden"
			id="NO_RESPONSE" name="NO_RESPONSE" value="" />
	</form>-->

	<%
		// this means that the FMPilot is attached
		if (!isDetachable(request)) {
	%>

	<!--rcms.control:configurationPathRenderer titleClass="control_label1"
		label="Configuration : &nbsp;" contentClass="control_label2"
		indentation="10" /-->

	<%
		}
	%>

	<!-- FMPilot action buttons -->
<!--        <p>
            <rcms.control:refreshButtonRenderer cssClass="button1"
		onClickFunction="onUpdatedRefreshButton()" name="Refresh"
		indentation="10" />
            <input type="button" id="command0" class="button1" value="Status Table" name="command1" onClick="onShowStatusTableButton()">
        </p>
-->

<!--        <p>
	<rcms.control:createButtonRenderer cssClass="button1"
		onClickFunction="onCreateButton()" name="Create" indentation="10" />
	<rcms.control:attachButtonRenderer cssClass="button1"
		onClickFunction="onAttachButton()" name="Attach" indentation="10" />
	<rcms.control:detachButtonRenderer cssClass="button1"
		onClickFunction="onDetachButton()" name="Detach" indentation="10" />
	<rcms.control:destroyButtonRenderer cssClass="button1"
		onClickFunction="onDestroyButton()" name="Destroy" indentation="10" />
        </p>-->
        
	<%
		if (!isDetachable(request)) {
//			out.println("</body></html>");
//			return; // don't show full page if not attached
		}
	%>
	
        <!--<p>-->


			<form id="FMPilotForm" method="POST" action="FMPilotServlet">
				<rcms.control:actionHiddenInputRenderer indentation="4"/>
				<rcms.control:commandHiddenInputRenderer indentation="4"/>
				<rcms.notification:hiddenInputRenderer indentation="4"/>
				<rcms.control:configurationKeyRenderer titleClass="control_label1" label="Configuration Keys:&nbsp;" contentClass="control_label2" indentation="10"/>
                                    <div id="controlLayout">
                                            <div id="properties">
                                                <div id="location">
                                                    <div>
										<rcms.control:configurationPathRenderer titleClass="propertyName" label="Path:&nbsp;" contentClass="propertyValue" indentation="10"/>
                                                    </div>
                                                    <div>
                                                                                <rcms.control:configurationNameRenderer titleClass="propertyName" label="Group:&nbsp;" contentClass="propertyValue" indentation="10"/>
                                                    </div>
                                                </div>
                                                <div id="FSMState">
										<rcms.control:stateRenderer titleClass="control_label1" label="" contentClass="stateName" indentation="10"/>
                                                </div>
                                            </div>
                                            <div id="lifeCycle">
										<rcms.control:createButtonRenderer cssClass="button1 myCreateButton" onClickFunction="onCreateButton()" name="Create" indentation="10"/>
										<rcms.control:attachButtonRenderer cssClass="button1 myAttachButton" onClickFunction="onAttachButton()" name="Attach" indentation="10"/>
										<rcms.control:detachButtonRenderer cssClass="button1 myDetachButton" onClickFunction="onDetachButton()" name="Detach" indentation="10"/>
										<rcms.control:destroyButtonRenderer cssClass="button1 myDestroyButton" onClickFunction="onDestroyButton()" name="Destroy" indentation="10"/>
                                            </div>
                                            <div id="status">
										<rcms.control:refreshButtonRenderer cssClass="button1" onClickFunction="onRefreshButton()" name="Refresh" indentation="10"/>
										<rcms.control:showTreeButtonRenderer cssClass="button1" onClickFunction="onShowTreeButton()" name="Status Display" indentation="10"/>
										<rcms.control:showStatusTableButtonRenderer cssClass="button1" onClickFunction="onShowStatusTableButton()" name="Status Table" indentation="10"/>
	<%
		if (isDetachable(request)) {
                    // Only show diagnostics checkbox if we're attached
                    out.println("<label for='showFMParametersCheckbox'>Show&nbsp;diagnostics&nbsp;</label>");
                    out.println("<input id='showFMParametersCheckbox' type='checkbox'/>");
		}
	%>
                                            </div>                              
                                                  
                                            <div id="FSM">
										<div id="myCommandSection" class="flexRow">
											<rcms.control:commandButtonsRenderer cssClass="button1" indentation="11"/>
                                                                                </div>
										<div id="commandParameterCheckBoxSection" class="control_label">
											<rcms.control:commandParameterCheckboxRenderer title="&nbsp;Show Command Parameter Section" indentation="11"/>
										</div>
                                                <div id="parameterTableSection">
                                                    <div id="commandParameterSection"></div>
                                                </div>
                                            </div>
                                            <div id="FMParameters">
                                                <table id="sortedParametersTable"><tbody></tbody></table>
                                            </div>
                                    </div>
                        </form>




	<!-- You should definitely check the values your are updating for valid types
		(only send an actual integer for an IntegerT parameter etc.).
		For example with JavaScript's parseInt, isNaN, isNumeric etc. for integers and string comparison for booleans.
		Also check for valid value ranges (for example numbers that are too big to fit into a Java integer). -->

<!--
	<p>
		The following text box displays a simple IntegerT FM parameter that can not be modified directly.<br />
		The function manager increases this parameter by one every 500 milliseconds and the page is updated using notifications.<br />
		Clicking the button sets the FM parameter to zero using AJAX.
		<br />
		<br />integerParameter:
		<input readonly="readonly" type="text" id="integerParameter"
			value="${pars.INTEGER_PARAMETER}" />
		<button
			onclick="guiInst.setFMParameter('INTEGER_PARAMETER', 0, guiInst.PARAMETER_TYPE_INTEGER); return false;">reset
			counter</button>
		(using jQuery)
	</p>
	<br />
<p>
		The following text box displays a simple BooleanT FM parameter that can be toggled using the button next to it.<br />
		The new boolean is determined on the client and then sent as a simple parameter update. With many concurrent users it might be
		a better idea to use a GUI command instead and let the FM toggle the parameter.
		<br />
		<br />booleanParameter:
		<input readonly="readonly" type="text" id="booleanParameter"
			value="${pars.BOOLEAN_PARAMETER}" />
		<button
			onclick="ajaxRequestSetFMParameter('BOOLEAN_PARAMETER', !($('#booleanParameter').val() == 'true'), guiInst.PARAMETER_TYPE_BOOLEAN); return false;">toggle</button>
		(using AJAXLibrary)
	</p>
        -->
	
	<script type="text/javascript">
		guiInst.attach(document);

		// a call to onLoad is needed since it starts the notification system
		$(document).ready(function() {
			onLoad();
		});
	</script>
        </td></tr></table>
</body>
</html>
