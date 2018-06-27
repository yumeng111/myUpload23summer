function onCreateButton() {
	submitForm('FMPilotForm', 'Create');
}

function onAttachButton() {
	submitForm('FMPilotForm', 'Attach');
}

function onDetachButton() {
	submitForm('FMPilotForm', 'Detach');
}

function onDestroyButton() {
        alert("onDestroyButton");
        if (confirm("Everything will be terminated!\nAre you sure you want to destroy?") == false) {
            return;
        }
	submitForm('FMPilotForm', 'Destroy');
}

/*
function onInitializeButton() {
	submitForm('FMPilotForm', 'Initialize');
}

function onConfigureButton() {
	submitForm('FMPilotForm', 'Configure');
}
*/
