import QtQuick 2.14
import QtQuick.Controls 2.14

Dialog {
	modal: true

	x: (parent.width - width) / 2
	y: Qt.inputMethod.keyboardRectangle.y === 0 ? (parent.height - height) / 2 : 50
	width: 300

	property string initialText: ""
	property alias outputText: dlgTextField.text
	property alias validator: dlgTextField.validator
	property bool listInputOnly: false
	property var listItems: []

	contentItem: Item {
		implicitHeight: lvItems.implicitHeight + (listInputOnly ? 0 : dlgTextField.implicitHeight)

		ListView {
			id: lvItems

			anchors.top: parent.top
			anchors.left: parent.left
			anchors.right: parent.right

			implicitHeight: 200 < lvItems.contentItem.childrenRect.height ? 200 : lvItems.contentItem.childrenRect.height

			delegate: ItemDelegate {
				width: lvItems.width - lvItems.leftMargin - lvItems.rightMargin
				onClicked: {
					lvItems.currentIndex = index;
					dlgTextField.text = text;
					buttons.standardButton(Dialog.Ok).enabled = true;
				}

				highlighted: ListView.isCurrentItem

				text: modelData
			}
		}

		TextField {
			id: dlgTextField
			anchors.top: lvItems.bottom
			anchors.topMargin: 5
			width: parent.width

			visible: !listInputOnly

			validator: RegularExpressionValidator { regularExpression: /[0-9a-zA-ZéèçàäöüÄÖÜ ()._-]+$/ }

			onTextEdited: {
				lvItems.currentIndex = -1
				buttons.standardButton(Dialog.Ok).enabled = text != ""
			}

			onAccepted: {
				if(buttons.standardButton(Dialog.Ok).enabled)
				{
					buttons.standardButton(Dialog.Ok).clicked();
				}
			}
		}
	}

	footer: DialogButtonBox {
		id: buttons
		standardButtons: Dialog.Ok | Dialog.Cancel
	}

	onAboutToShow: {
		dlgTextField.text = initialText;
		lvItems.currentIndex = -1
		lvItems.model = listItems;
		buttons.standardButton(Dialog.Ok).enabled = false;
		if(!listInputOnly)
		{
			dlgTextField.forceActiveFocus();
		}
	}  
}
