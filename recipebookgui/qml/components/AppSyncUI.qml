import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
	objectName: "appSyncUI"
	anchors.fill: parent

	function askForAccessCode() {
		dlgAccessCode.open();
    }

	TextInputDialog {
		id: dlgAccessCode
		title: qsTr("Enter Dropbox access code")
		onAccepted: recipeBookSynchronization.setAccessCode(outputText)
	}

	function askForSync() {
        dlgAskForSync.open();
    }

	TextMessageDialog {
		id: dlgAskForSync

		title: qsTr("Synchronization reminder");
		msgText: qsTr("It's time for another synchronization. Do this now?");
		okOnly: false
		errorIcon: false

		onAccepted: recipeBookSynchronization.execute()
	}
}