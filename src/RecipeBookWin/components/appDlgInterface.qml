import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
	objectName: "appDlgInterface"

	function showMessageBox(title: string, msg: string, bErrorIcon: bool) {
        dlgMsgBox.msgText = msg;
		dlgMsgBox.title = title;
		dlgMsgBox.okOnly = true
		dlgMsgBox.errorIcon = bErrorIcon
		dlgMsgBox.open();
    }

	TextMessageDialog {
		id: dlgMsgBox
	}

	function lockUI() {
		if(popupLock.visible) {
			return;
		}

		popupLock.open();
	}

	function unlockUI() {
		if(!popupLock.visible) {
			return;
		}

		popupLock.close();
	}

	Popup {
		id: popupLock

		modal: true

		BusyIndicator {
			running: popupLock.opend
		}

		background: Rectangle {
			color: "transparent"
		}
	}
}