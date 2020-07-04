import QtQuick 2.14
import QtQuick.Controls 2.14

Dialog {
	id: dlgRemoveIngredient
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2
	width: parent.width > 380 ? 360 : parent.width - 20

	property alias msgText: dlgAskLabel.text
	property bool errorIcon: false
	property bool okOnly: false
    
	Label { 
		id: dlgAskLabel
		anchors.centerIn: parent
		width: parent.width
		wrapMode: Text.WordWrap

		visible: !errorIcon
	}

	Item {
		visible: errorIcon
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.verticalCenter: parent.verticalCenter
		width: 360
		implicitHeight: dlgAskLabelWarn.implicitHeight

		Image {
			id: imgWarn
			anchors.left: parent.left
			anchors.verticalCenter: parent.verticalCenter
			height: 48
			fillMode: Image.PreserveAspectFit
			source: "qrc:/images/error-black.svg"
		}
		Label { 
			id: dlgAskLabelWarn
			
			anchors.left: imgWarn.right
			anchors.right: parent.right
			anchors.leftMargin: 24
			anchors.verticalCenter: parent.verticalCenter

			wrapMode: Text.WordWrap
			text: dlgAskLabel.text
		}
	}

	footer: DialogButtonBox {
		id: buttons
		standardButtons: okOnly ? Dialog.Ok : Dialog.Yes | Dialog.No
	}
}
