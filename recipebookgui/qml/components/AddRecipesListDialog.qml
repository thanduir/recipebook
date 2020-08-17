import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14

Dialog {
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2

	property var allValuesFilterModel: []
	property var editListModel: []
	property bool useScrollBars: true

	property var largeScreen : parent.width > 400

	signal listChanged

	Item {
		implicitWidth: largeScreen ? 400 : 300
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

			delegate: ItemDelegate {
				width: lvValues.width - lvValues.leftMargin - lvValues.rightMargin
					
				Item {
					anchors.fill: parent

					Label {
						anchors.left: parent.left
						anchors.leftMargin: 10
                        
						width: largeScreen ? 210 : 180
						elide: Text.ElideRight

						text: name
						verticalAlignment: Text.AlignVCenter
						height: parent.height

						font.bold: spinBoxListItem.value > 0
					}

					SpinBox {
						id: spinBoxListItem
						anchors.right: parent.right
						anchors.rightMargin: largeScreen ? 24 : 0

						from: 0
						to: 50
						width: largeScreen ? 150 : 120

						value: editListModel.getInsertionsCount(name)
						onValueModified: editListModel.changeState(name, value)
					}
				}
			}
		}
	}

	footer: DialogButtonBox {
		Button {
			text: qsTr("Close")
			DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
			flat: true
		}
	}

	onAboutToShow: {
		textFilter.text = "";
		allValuesFilterModel.setFilterString("");
		lvValues.model = allValuesFilterModel;

		editListModel.beginAddList();
	}

	onAboutToHide: {
		if(editListModel.applyAddList())
		{
			listChanged();
		}
	}
}
