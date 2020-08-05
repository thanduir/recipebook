import QtQuick 2.14
import QtQuick.Controls 2.14

Dialog {
	id: dlgRemoveIngredient
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2
	width: parent.width > 380 ? 360 : parent.width - 20

	property alias msgText: dlgAskLabelWarn.text
	property bool errorIcon: false
	property bool okOnly: false

	Item {
		id: dlgErrorLabel
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.verticalCenter: parent.verticalCenter
		implicitHeight: dlgAskLabelWarn.implicitHeight

		Image {
			id: imgWarn
			anchors.left: parent.left
			anchors.verticalCenter: parent.verticalCenter
			height: 48
			fillMode: Image.PreserveAspectFit
			source: "qrc:/images/error-black.svg"

			visible: errorIcon
		}
		Label { 
			id: dlgAskLabelWarn
			
			anchors.left: errorIcon ? imgWarn.right : parent.left
			anchors.right: parent.right
			anchors.leftMargin: errorIcon ? 24 : 0

			wrapMode: Text.WordWrap
		}
	}

	footer: DialogButtonBox {
		id: buttons
		standardButtons: okOnly ? Dialog.Ok : Dialog.Yes | Dialog.No
	}
}
