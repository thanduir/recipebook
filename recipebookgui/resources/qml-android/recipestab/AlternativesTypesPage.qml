import QtQuick.Dialogs 1.3
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "components"

Item {
	TextInputDialog {
		id: dlgAddType
		title: qsTr("Add alternatives group")
		onCurrentTextChanged: currentTextAllowed = !alternativesTypes.existsType(outputText)
		onAccepted: {
			lvValues.currentIndex = alternativesTypes.addType(outputText)
			lvValues.positionViewAtIndex(lvValues.currentIndex, ListView.Center)
		}
	}

	TextInputDialog {
		id: dlgRenameType
		title: qsTr("Rename alternatives group")
		onCurrentTextChanged: currentTextAllowed = !alternativesTypes.existsType(outputText)
		onAccepted: {
			lvValues.currentIndex = alternativesTypes.renameType(lvValues.currentIndex, outputText)
			lvValues.positionViewAtIndex(lvValues.currentIndex, ListView.Center)
		}
	}
    
	TextMessageDialog {
		id: dlgCantRemoveAlternativesGroup
		title: qsTr("Ingredient can't be removed")
		okOnly: true
	}

	// TODO: This crashes on android tablet (click on ok / cancel) and is wrongly positioned!
	ColorDialog {
		id: colorDialog

		x: (parent.width - width) / 2
		y: (parent.height - height) / 2

		title: "Choose alternatives group color"
		modality: Qt.ApplicationModal // Qt.WindowModal
		property int currentIndex: -1
		onAccepted: {
			alternativesTypes.setColor(currentIndex, colorDialog.color)
		}
	}

	// Header Component

	onVisibleChanged: {
		if(visible)
		{
			headerSubpageSpace.sourceComponent = alternativesTypesHeaderComponent;
		}
	}

	Component {
		id: alternativesTypesHeaderComponent

		ToolButton {
			anchors.right: parent.right
			anchors.rightMargin: 10

			display: AbstractButton.IconOnly
			icon.source: "qrc:/images/add-black.svg"

			onVisibleChanged: {
				if(visible)
				{
					enabled = alternativesTypes.canTypesBeAdded()
				}
			}
			onClicked: dlgAddType.open()
		}
	}

	// Main page

	ListView {
		id: lvValues
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		anchors.topMargin: 24
		anchors.bottomMargin: 24

		ScrollBar.vertical: ScrollBar { }
		boundsBehavior: Flickable.StopAtBounds

		remove: Transition {
			SequentialAnimation {
				PauseAnimation { duration: 125 }
				NumberAnimation { property: "height"; to: 0; easing.type: Easing.InOutQuad }
			}
		}

		displaced: Transition {
			SequentialAnimation {
				PauseAnimation { duration: 125 }
				NumberAnimation { property: "y"; easing.type: Easing.InOutQuad }
			}
		}

		model: alternativesTypes

		delegate: SwipeDelegate {
			id: listItemRecipeItem
			width: lvValues.width - lvValues.leftMargin - lvValues.rightMargin
			height: listItem.height

			contentItem: Item {
				id: listItem
				anchors.top: listItemRecipeItem.top
				anchors.topMargin: 10

				height: labelGroupName.height + 20
					
				// group name
				Label {
					id: labelGroupName
					anchors.left: parent.left
					anchors.leftMargin: 10
                        
					text: name
					verticalAlignment: Text.AlignVCenter

					MouseArea {
						anchors.fill: parent
						onPressAndHold: {
							dlgRenameType.initialText = alternativesTypes.name(index);
							dlgRenameType.open();
						}
					}
				}

				// group symbol
				Rectangle {
					id: rowGroupHeaderButtons
					anchors.right: listItem.right
					anchors.top: parent.top
					anchors.topMargin: 0
					anchors.rightMargin: 10
					width: 20
					height: width
					radius: 0.5 * width

					color: typeColor

					MouseArea {
						anchors.fill: parent
						onClicked: { 
							colorDialog.currentColor = typeColor
							colorDialog.currentIndex = index
							colorDialog.open();
						}
					}
				}
			}

			Timer {
				id: undoTimer
				interval: 2750
				onTriggered: {
					alternativesTypes.removeType(index)
				}
			}

			swipe.onCompleted: {
				if(!alternativesTypes.canTypeBeRemoved(index))
				{
					listItemRecipeItem.swipe.close()
					dlgCantRemoveAlternativesGroup.msgText = qsTr("Alternatives group \"%1\" is still in use and can't be removed.").arg(name);
					dlgCantRemoveAlternativesGroup.open();
				}
				else
				{
					undoTimer.start()
				}
			}

			swipe.left: Rectangle {
				width: parent.width
				height: parent.height

				clip: true
				color: "darkgray"

				Image {
					anchors.left: parent.left
					anchors.leftMargin: 10
					anchors.verticalCenter: parent.verticalCenter

					visible: !listItemRecipeItem.swipe.complete

					fillMode: Image.PreserveAspectFit
					source: "qrc:/images/remove.svg"
				}
				Label {
					anchors.left: parent.left
					anchors.leftMargin: 10
					anchors.verticalCenter: parent.verticalCenter
					text: qsTr("Alternatives group \"%1\" removed").arg(name)

					visible: listItemRecipeItem.swipe.complete
				}
				RoundButton {
					text: qsTr("Undo")

					anchors.right: parent.right
					anchors.verticalCenter: parent.verticalCenter

					onClicked: {
						undoTimer.stop()
						listItemRecipeItem.swipe.close()
					}

					visible: listItemRecipeItem.swipe.complete
				}
			}
		}
	}
}
