import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.1

import "components"

Item {
	id: rbConfigContentPage

	property int currentConfig: -1
	
	visible: currentConfig != -1

	TextInputDialog {
		id: dlgAddHeader
		title: qsTr("Add header")
		onAccepted: {
			modelRBConfigItems.addHeader(outputText, -1)
			lvItems.positionViewAtEnd();
		}
	}

	EditItemsListDialog {
		id: dlgAddRecipes
		useScrollBars: false
		title: qsTr("Add recipe")
		onListChanged: {
			lvItems.currentIndex = -1;
			lvItems.positionViewAtEnd();
			headerSubpageSpace.item.btnAddRecipe.enabled = modelRBConfigItems.canRecipesBeAdded();
		}
	}
    
	TextInputDialog {
		id: dlgRenameHeader
		title: qsTr("Rename header")
		onAccepted: modelRBConfigItems.renameHeader(lvItems.currentIndex, outputText)
	}

	TextMessageDialog {
		id: dlgRemoveItem
		title: qsTr("Remove item")
		onAccepted: {
			modelRBConfigItems.removeItem(lvItems.currentIndex)
			lvItems.currentIndex = -1
			headerSubpageSpace.item.btnAddRecipe.enabled = modelRBConfigItems.canRecipesBeAdded()
		}
	}

	// Header Component

	onVisibleChanged: {
		if(visible)
		{
			modelRBConfigItems.setCurrentConfig(currentConfig)
			headerSubpageSpace.sourceComponent = rbConfigsHeaderComponent;
		}
	}

	Component {
		id: rbConfigsHeaderComponent

		RowLayout {
			anchors.right: parent.right
			anchors.rightMargin: 10

			property alias btnAddRecipe: btnAddRecipeInternal

			ToolButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"
				icon.color: "yellow"

				onClicked: dlgAddHeader.open()
			}

			ToolButton {
				id: btnAddRecipeInternal

				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				onVisibleChanged: {
					if(visible)
					{
						enabled = modelRBConfigItems.canRecipesBeAdded()
					}
				}
				onClicked: {
					filterModelUnusedRecipes.setRecipeBookConfiguration(currentConfig)

					dlgAddRecipes.editListModel = modelRBConfigItems;
					dlgAddRecipes.allValuesFilterModel = filterModelUnusedRecipes;

					dlgAddRecipes.open()
				}
			}
		}
	}

	// Rearrange recipe items and headers

	component MyDragDelegate : Item {

		MouseArea {
			id: dragArea

			property bool held: false

			anchors.left: parent != null ? parent.left : undefined;
			width: reorderImage.height
			height: content.height

			drag.target: held ? content : undefined
			drag.axis: Drag.YAxis

			onPressed: {
				if(!isHeader)
				{
					held = true
					modelRBConfigItems.beginMove(index);
				}
			}
			onReleased: {
				if(!isHeader)
				{
					held = false
					modelRBConfigItems.applyMove();
				}
			}

			Rectangle {
				id: content

				anchors.left: parent.left
				anchors.verticalCenter: parent.verticalCenter

				width: lvItems.width - lvItems.leftMargin - lvItems.rightMargin
				height: column.implicitHeight + 47

				color: dragArea.held ? "lightgray" : "transparent"

				Drag.active: dragArea.held
				Drag.source: swipeDelegate
				Drag.hotSpot.x: dragArea.width / 2
				Drag.hotSpot.y: dragArea.height / 2

				states: State {
					when: dragArea.held

					ParentChange { target: content; parent: rbConfigContentPage }
					AnchorChanges {
						target: content
						anchors { verticalCenter: undefined; left: undefined; }
					}
				}

				Item {
					id: column
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.verticalCenter: parent.verticalCenter

					// Move image
					Image {
						id: reorderImage
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: parent.left
						height: labelColumn.implicitHeight + 10
						verticalAlignment: Image.AlignVCenter

						visible: !isHeader

						fillMode: Image.PreserveAspectFit
						source: "qrc:/images/reorder.svg"
					}

					// Item name
					Label {
						id: labelColumn
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: !isHeader ? reorderImage.right : parent.left
						anchors.leftMargin: 15

						verticalAlignment: Text.AlignVCenter
						font.bold: isHeader
						text: name
					}

					SpinBox {
						anchors.verticalCenter: parent.verticalCenter
						anchors.right: parent.right
						anchors.rightMargin: 24

						from: 1
						to: maxHeaderLevel+1
						visible: isHeader
			
						value: headerLevel+1
						onValueModified: headerLevel = value - 1
					}
				}
			}

			DropArea {
				anchors { fill: parent }

				onEntered: {
					modelRBConfigItems.updateMoveTarget(swipeDelegate.itemsIndex);
					rearrangeCurrentItemDelegateModel.items.move(drag.source.itemsIndex,
														swipeDelegate.itemsIndex);
				}
			}
		}
	}

	// Rearrange recipe item list view
	ListView {
		id: lvItems
		anchors.fill: parent
		anchors.topMargin: 24
		anchors.bottomMargin: 24
		
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

		model: DelegateModel {
			id: rearrangeCurrentItemDelegateModel

			model: modelRBConfigItems
			delegate: SwipeDelegate {
				id: swipeDelegate
				property int itemsIndex : DelegateModel.itemsIndex

				width: lvItems.width - lvItems.leftMargin - lvItems.rightMargin

				onPressAndHold: {
					if(!isHeader)
					{
						mouse.accept = false
						return;
					}

					lvItems.currentIndex = index
					dlgRenameHeader.initialText = modelRBConfigItems.name(lvItems.currentIndex);
					dlgRenameHeader.open()
				}

				contentItem: MyDragDelegate {
					id: dragDelegate

				}

				Timer {
					id: undoTimer
					interval: 2750
					onTriggered: {
						modelRBConfigItems.removeItem(index)
						lvItems.currentIndex = -1
						headerSubpageSpace.item.btnAddRecipe.enabled = modelRBConfigItems.canRecipesBeAdded()
					}
				}

				swipe.onCompleted: {
					undoTimer.start()
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

						visible: !swipeDelegate.swipe.complete

						fillMode: Image.PreserveAspectFit
						source: "qrc:/images/remove.svg"
					}
					Label {
						anchors.left: parent.left
						anchors.leftMargin: 10
						anchors.verticalCenter: parent.verticalCenter
						text: qsTr("Item \"%1\" removed").arg(name)

						visible: swipeDelegate.swipe.complete
					}
					RoundButton {
						text: qsTr("Undo")

						anchors.right: parent.right
						anchors.verticalCenter: parent.verticalCenter

						onClicked: {
							undoTimer.stop()
							swipeDelegate.swipe.close()
						}

						visible: swipeDelegate.swipe.complete
					}
				}
			}
		}
	}
}
