import QtQuick 2.14

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
}