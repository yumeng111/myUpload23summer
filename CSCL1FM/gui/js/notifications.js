/**
 * This function is called by the notification system to handle the received
 * notification.
 * 
 * @param message
 *            The received notification (XML).
 */
function myUpdateParameters(message) {
        // This function (myUpdateParameters) is called by render() on receiving notification of type "Parameter".
        // We redraw the command buttons here, too, or else they won't get customized.
         
        // currentState is presumably up to date, so let's take its value:
        var state = $('#currentState').text();
        drawMyCommandButtons( new State(state,state) );

//        console.debug('myUpdateParameters called with message:\n' + message);
	// only process notification of PARAMETER type
	var pArray = message.getElementsByTagName('PARAMETER');

	if (pArray != null) {
		// loop through the array of parameters contained in this notification
		for (var i = 0; i < pArray.length; i++) {
			if (pArray[i] != null) {
				pNameNode = pArray[i].getElementsByTagName('NAME')[0];
				pValueNode = pArray[i].getElementsByTagName('VALUE')[0];
				if (pNameNode != null && pValueNode != null) {
					// get the parameter name and its value
					pName = pNameNode.childNodes[0].nodeValue;
					pValue = pValueNode.textContent;

//					console.debug('Notification parameter '+i+': '+pName+' = '+pValue);

					// decode and handle JSON encoded parameters
					if (pName.indexOf('JSON_') == 0) {
						pDecoded = JSON.parse(pValue);
						realName = pName.substring(5);

						console.log('Decoded from JSON: '+realName+' = '+pDecoded);
                                                
                                                if (realName == 'CSC_RUN_TYPES_AVAILABLE_V') {
                                                        var areAllCalib = true;
                                                        pDecoded.forEach( function(rt){ areAllCalib &= rt.toString().startsWith( "Calib_" ); } );
                                                        var items = [];
                                                        if ( areAllCalib ){
                                                            items.push( "<option value='Calib_RunAllInOneGo'>Calib_RunAllInOneGo</option>" );
                                                        }
							pDecoded.forEach(function(rt){ items.push( "<option value='" + rt + "'>" + rt + "</option>" ); });
                                                        if ( $( "#"+cscRunTypeSelectTagId+" > option" ).length == 0 ){
                                                            $( "#"+cscRunTypeSelectTagId ).html( items.join( "" ) );
                                                        }
                                                        //console.log("cscRunTypeSelectTagId=");
                                                        console.log("cscRunTypeSelectTagId="+cscRunTypeSelectTagId);
                                                        // Find the index of command parameter CSC_RUN_TYPES with anonymous function. (Use the "this" argument of Array.findIndex to pass parameter to it.)
                                                        //var index=commandParameters.findIndex( function(a){return a.parameterName==this;}, "CSC_RUN_TYPE" );
                                                        //console.log("Index of CSC_RUN_TYPE is "+index);
                                                        //if ( index > 0 ){
                                                        //    $( "#parameterValue"+index ).html( items.join( "" ) );
                                                        //}
                                                }
					}
					/*
					 * handle regular parameters by name
					 * 
					 * again, different methods of updating the document are
					 * used (regular JS and jQuery)
					 */
					else {
//						if (pName === 'STRING_PARAMETER1') {
//							$('#stringParameter1').val(pValue);
//						} else if (pName === 'INTEGER_PARAMETER') {
//							document.getElementById('integerParameter').value = pValue;
//						} else if (pName === 'BOOLEAN_PARAMETER') {
//							$('#booleanParameter').val(pValue);
//						} else if (pName === 'State') {
//							$('#State').text(pValue);
//						}
						if (pName === 'STATE') {
							$('#State').text(pValue);
                                                        //$('#properties').addClass(pValue);
                                                        $('#properties').attr('class',pValue);
						}
					}
				}
			}
		}
	}
        console.log("$('#showFMParametersCheckbox').checked="+$('#showFMParametersCheckbox').is(':checked'));
        // Always show diagnostics in global runs. Otherwise only if requested.
        if ( $('#showFMParametersCheckbox').is(':checked') || isRunTypeGlobal(message) ){
            tabulateParameters( message );
            //$('#FMParameters').toggle( true );
            $('#FMParameters').css('visibility', 'visible');
        }
        else{
            //tabulateParameters( message );
            //$('#FMParameters').toggle( false );
            $('#FMParameters').css('visibility', 'hidden');
        }
}

function isRunTypeGlobal(message){
    var pArray = message.getElementsByTagName('PARAMETER');
    if (pArray != null) {
        // loop through the array of parameters contained in this notification
        for (var i = 0; i < pArray.length; i++) {
            if (pArray[i] != null) {
                pNameNode = pArray[i].getElementsByTagName('NAME')[0];
                pValueNode = pArray[i].getElementsByTagName('VALUE')[0];
                if (pNameNode != null && pValueNode != null) {
                    if ( pNameNode.childNodes[0].nodeValue.toString()    == 'CSC_RUN_TYPE' && 
                         pValueNode.textContent.toString().toLowerCase() == 'global'           ) return true;
                }
            }
        }
    }
    return false;
}

function tabulateParameters(message){
    var sortedParameters = new Array();

    // only process notification of PARAMETER type
    var pArray = message.getElementsByTagName('PARAMETER');

    if (pArray != null) {
        // loop through the array of parameters contained in this notification
        for (var i = 0; i < pArray.length; i++) {
            if (pArray[i] != null) {
                pNameNode = pArray[i].getElementsByTagName('NAME')[0];
                pValueNode = pArray[i].getElementsByTagName('VALUE')[0];
                if (pNameNode != null && pValueNode != null) {
                    // get the parameter name and its value
                    sortedParameters.push( {n: pNameNode.childNodes[0].nodeValue, v: pValueNode.textContent } );
                }
            }
        }
    }

    // Sort the parameters by name
    sortedParameters.sort( function(a,b){
        if(a.n < b.n) return -1;
        if(a.n > b.n) return 1;
        return 0;
    } );
    // Add them to the table
    $("#sortedParametersTable tbody").empty();
    sortedParameters.forEach(function(item,index){
        $("#sortedParametersTable tbody").append("<tr><td>" + item.n + ": </td><td"+(item.n=="ERROR_MSG"?" style='color:red;'":"")+">" + item.v + "</td></tr>");
    });
}