import QtQuick 2.14
import QtQuick.Controls 2.14

Dialog {
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2
	width: 300

	property string initialText: ""
	property alias outputText: dlgTextField.text
	property alias validator: dlgTextField.validator
	property bool currentTextAllowed: true
	signal currentTextChanged

	TextField { 
		id: dlgTextField
		anchors.centerIn: parent
		width: parent.width       
		validator: RegularExpressionValidator { regularExpression: /[0-9a-zA-ZéèçàäöüÄÖÜ ()._-]+$/ }

		onTextEdited: {
			currentTextChanged();
			buttons.standardButton(Dialog.Ok).enabled = text != "" && currentTextAllowed;
		}

		onAccepted: {
			if(buttons.standardButton(Dialog.Ok).enabled)
			{
				buttons.standardButton(Dialog.Ok).clicked();
			}
		}
	}

	footer: DialogButtonBox {
		id: buttons
		standardButtons: Dialog.Ok | Dialog.Cancel
	}

	onAboutToShow: {
		dlgTextField.text = initialText;
		buttons.standardButton(Dialog.Ok).enabled = false;
		dlgTextField.forceActiveFocus();
	}  
}
