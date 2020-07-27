import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14

Dialog {
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2

	property var allValuesFilterModel: []
	property var editListModel: []
	property int initialyHighlightedIndex: -1
	property bool useScrollBars: true
    
	signal listChanged

	Item {
		implicitWidth: 300
		implicitHeight: lvValues.implicitHeight + textFilter.implicitHeight

		TextField { 
			id: textFilter
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.top: parent.top
			anchors.topMargin: 0
			selectByMouse: true

			onTextEdited: {
				allValuesFilterModel.setFilterString(text);
				forceActiveFocus();
			}

			Image {
					anchors { top: parent.top; right: parent.right }
					id: clearText
					fillMode: Image.PreserveAspectFit
					visible: textFilter.text
					source: "qrc:/images/backspace.svg"
					height: parent.height - 5

					MouseArea {
						id: clear
						anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
						height: textFilter.height; width: textFilter.height
						onClicked: {
							textFilter.text = ""
							allValuesFilterModel.setFilterString(textFilter.text);
							textFilter.forceActiveFocus()
						}
					}
				}
		}

		ListView {
			id: lvValues
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.top: textFilter.bottom
			anchors.topMargin: 24
			anchors.bottomMargin: 24

			implicitHeight: 400

			ScrollBar.vertical: ScrollBar { visible: useScrollBars }
			ScrollIndicator.vertical: ScrollIndicator { visible: !useScrollBars }

			model: allValuesFilterModel
			delegate: CheckDelegate {
				checkState: editListModel.itemSelected(name) ? Qt.Checked : Qt.Unchecked
				onClicked: editListModel.changeState(name, checked)
				width: lvValues.width - lvValues.leftMargin - lvValues.rightMargin
                
				text: name
			}
		}
	}

	footer: DialogButtonBox {
		id: buttons
		standardButtons: Dialog.Ok | Dialog.Cancel
	}

	onAboutToShow: {
		textFilter.text = "";
		allValuesFilterModel.setFilterString("");

		editListModel.beginEditList();

		if(initialyHighlightedIndex != -1)
		{
			lvValues.positionViewAtIndex(initialyHighlightedIndex, ListView.Center)
		}
	}

	onAccepted: {
		if(editListModel.applyEditList())
		{
			listChanged();
		}
	}
	onRejected: editListModel.cancelEditList()
}
