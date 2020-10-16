import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
	objectName: "appSyncUI"

	function askForAccessCode() {
		dlgAccessCode.open();
    }

	TextInputDialog {
		id: dlgAccessCode
		title: qsTr("Enter Dropbox access code")
		onAccepted: recipeBookSynchronization.setAccessCode(outputText)
	}
}