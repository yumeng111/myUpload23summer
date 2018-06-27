<%@ page language="java" contentType="text/html" %>
<%@ page import="rcms.rs.ws.stubs.GroupInfo" %>
<%@ page import="java.util.List" %>
<%@ page import="java.util.Date" %>
<%@ page import="java.util.Iterator" %>
<%@ page import="rcms.gui.servlet.pilot.FMPilotConstants" %>
<%@ page import="rcms.gui.servlet.tree.StateTreeConstants" %>
<%@ page import="rcms.gui.servlet.tree.StateTableBean" %>
<%@ page import="rcms.gui.servlet.util.Util" %>
<%@ page import="rcms.gui.servlet.ns.NotificationConstants" %>

<%
    response.setHeader("Cache-Control","no-cache"); //HTTP 1.1
    response.setHeader("Pragma","no-cache"); //HTTP 1.0
    response.setDateHeader ("Expires", 0); //prevents caching at the proxyserver
%>

<%
  StateTableBean stateTreeBean = (StateTableBean) request.getAttribute(StateTreeConstants.STATE_TABLE_BEAN);
  String filterSelected = stateTreeBean.getFilter();
  String displayModeSelected = stateTreeBean.getDisplayMode();
  GroupInfo currentGroup = stateTreeBean.getCurrentGroup();
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html>

<head>
	<META Http-Equiv="Cache-Control" Content="no-cache">
	<META Http-Equiv="Pragma" Content="no-cache">
	<META Http-Equiv="Expires" Content="0">
	
	<title>Status Display</title>

	<link rel="StyleSheet" href="../css/stateTable.css" type="text/css" />
	<script type="text/javascript" src="../js/common.js"></script>
	<script>
		var url = null;
		
		function onLoad() {
		
		<% if (filterSelected != null && filterSelected.length() > 0) { %> 
			selectComboOption('filter', '<%=filterSelected%>');
		<% } %> 
		<% if (displayModeSelected != null && displayModeSelected.length() > 0) { %> 
			selectComboOption('displayMode', '<%=displayModeSelected%>');
		<% } %> 
		<% if (stateTreeBean.getInformationBuffer() != null && stateTreeBean.getInformationBuffer().length() > 0) { %>
			var warning = '<%=Util.escapeForAlertJavaScript(stateTreeBean.getInformationBuffer().toString())%>';
			alert(warning);
		<%}%>

		}
		
		/* Handles the client behaviour when XML data come from XMLHttpRequest object's connection */
		function handleResponse(response) {
			if (response != null) {
				var timestampNode = response.getElementsByTagName('<%= NotificationConstants.TIMESTAMP %>')[0];
				var timestamp = timestampNode.childNodes[0].nodeValue;
				var typeNode = response.getElementsByTagName('<%= NotificationConstants.TYPE %>')[0];
				var type = typeNode.childNodes[0].nodeValue;
				var messageNode = response.getElementsByTagName('<%= NotificationConstants.MESSAGE %>')[0];
				var message = messageNode.childNodes[0].nodeValue;
				if (message != null) {
					if (timestamp != null) {
						doAction(timestamp, message, type);
					}
				} else {
					//alert('Extracted message is null');
				}
			} else {
				closeXMLHttpConnection();
				alert('Abnormal condition in the Notification System');
			}
		}
	
		/* Function called by asynchronous notification*/
		function doAction(timestamp, message, type) {
			if (timestamp != null) {
				setTimestamp(timestamp);
				closeXMLHttpConnection();
				onClickRefreshTree();
			}
		}

		/* Set the current timestamp */
		function setTimestamp(timestamp) {
			if (timestamp != null) {
				var timestampObject = document.getElementById('<%= NotificationConstants.TIMESTAMP %>');
				timestampObject.value = timestamp;
			}
		}
		
		function onClickRefreshTree() {
			submitForm('stateTreeForm', '<%=StateTreeConstants.REFRESH_TREE%>');
		}
		
		function onClickRefreshUpdateTree() {
			submitForm('stateTreeForm', '<%=StateTreeConstants.REFRESH_UPDATE_TREE%>');
		}
		
		function onClickCloseTree() {
			window.close();
		}
		
		function onChangeFilter() {
			submitForm('stateTreeForm', '<%=StateTreeConstants.FILTER_TREE%>');
		}
		
		function onChangeDisplayMode() {
			submitForm('stateTreeForm', '<%=StateTreeConstants.DISPLAY_MODE%>');
		}
		
		function submitForm(formId, message) {
			closeXMLHttpConnection();
		  var formObject = document.getElementById(formId);
		  formObject.<%=StateTreeConstants.ACTION%>.value = message;
		  formObject.submit();
		}
	</script>

</head>

<body onLoad="onLoad()">
	<form id="stateTreeForm" method="post" action="StateTableObserverServlet">
	  <input type="hidden" id="<%=StateTreeConstants.ACTION%>" name="<%=StateTreeConstants.ACTION%>" value="">
	  <% if (currentGroup != null) { %>
		<input type="hidden" id="<%=FMPilotConstants.GROUPID%>" name="<%=FMPilotConstants.GROUPID%>" value="<%=currentGroup.getResourceGroupID()%>">
		<input type="hidden" id="<%= NotificationConstants.TIMESTAMP %>" name="<%= NotificationConstants.TIMESTAMP %>" value=""/>
		<% } %> 
		<div class="stateTable">
	  	<input type="button" id="updateTree" value="Update" name="Update" onclick="onClickRefreshUpdateTree()"/>
	  	<input type="button" id="refreshTree" value="Refresh" name="Refresh" onclick="onClickRefreshTree()"/>
  				
  	    <select size="1" id="displayMode" name="<%=StateTreeConstants.DISPLAY_MODE%>" onChange="onChangeDisplayMode()">
   	    <option value="full">full</option>
		<option value="summary">summary</option>
        </select>

        <select size="1" id="filter" name="<%=StateTreeConstants.FILTER%>" onChange="onChangeFilter()">
    	    <option value="">-- All Application Types --</option>
						<% List options = stateTreeBean.getApplicationTypes();
						   if ((options != null)) { 
						   	Iterator i = options.iterator();
						   	String option;
						     while (i.hasNext()) {
						     	option = (String) i.next(); 
						%>
			<option value="<%=option%>"><%=option%></option>
						<%   }
						   }
						%>
          </select>
	  <input type="button" id="closeTree" value="Exit" name="Exit" onclick="onClickCloseTree()"/>	
	  Updated: <%= new java.util.Date() %>
	  
	  </div>
	   
	</form>
		
	<table class="stateTable" width="100%" border="1" cellpadding="1" cellspacing="1" bordercolor="#CC6600" bgcolor="#EEEEEE">
		<%= stateTreeBean.getTableHtml()%>	
	</table>			

</body>

</html>