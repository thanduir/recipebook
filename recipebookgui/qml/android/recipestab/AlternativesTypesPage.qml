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

	// TODO: Use alternative dialog when it is available (qt labs variant needs Qt widgets at the moment, however)
	/*ColorDialog {
		id: colorDialog

		title: "Choose alternatives group color"
		modality: Qt.WindowModal
		property int currentIndex: -1
		onAccepted: {
			alternativesTypes.setColor(currentIndex, colorDialog.color)
		}
	}*/

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

		ScrollIndicator.vertical: ScrollIndicator { }
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

			onPressAndHold: {
				dlgRenameType.initialText = alternativesTypes.name(index);
				dlgRenameType.open();
			}

			implicitHeight: listItem.implicitHeight

			contentItem: Item {
				id: listItem
				anchors.top: listItemRecipeItem.top
				implicitHeight: labelGroupName.implicitHeight + 20

				// group name
				Label {
					id: labelGroupName
					anchors.left: parent.left
					anchors.leftMargin: 10
					anchors.top: parent.top
					anchors.topMargin: 10
                        
					text: name
				}

				// group symbol
				Rectangle {
					id: rowGroupHeaderButtons
					anchors.right: listItem.right
					anchors.rightMargin: 10
					anchors.top: parent.top
					anchors.topMargin: 8
					width: 20
					height: width
					radius: 0.5 * width

					color: typeColor

					// TODO
					/*MouseArea {
						anchors.fill: parent
						onClicked: { 
							colorDialog.currentColor = typeColor
							colorDialog.currentIndex = index
							colorDialog.open();
						}
					}*/
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
