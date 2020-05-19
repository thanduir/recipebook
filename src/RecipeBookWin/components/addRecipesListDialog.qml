import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14

Dialog {
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2

	property var allValuesFilterModel: []
	property var editListModel: []
    
	signal listChanged

	Item {
		implicitWidth: 400
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

			ScrollBar.vertical: ScrollBar { }

			delegate: ItemDelegate {
				width: lvValues.width - lvValues.leftMargin - lvValues.rightMargin
					
				Item {
					anchors.fill: parent

					Label {
						anchors.left: parent.left
						anchors.leftMargin: 10
                        
						width: 210
						elide: Text.ElideRight

						text: name
						verticalAlignment: Text.AlignVCenter
						height: parent.height

						font.bold: spinBoxListItem.value > 0
					}

					SpinBox {
						id: spinBoxListItem
						anchors.right: parent.right
						anchors.rightMargin: 24

						from: 0
						to: 50
						width: 150

						value: editListModel.getInsertionsCount(name)
						onValueModified: editListModel.changeState(name, value)
					}
				}
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
		lvValues.model = allValuesFilterModel;

		editListModel.beginAddList();
	}

	onAccepted: {
		if(editListModel.applyAddList())
		{
			listChanged();
		}
	}
	onRejected: editListModel.cancelAddList()
}
