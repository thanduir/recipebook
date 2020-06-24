import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.1

import "components"

Item {
	id: rbConfigContentPage

	property int currentConfig: -1
	
	visible: currentConfig != -1

	onVisibleChanged: {
		if(visible)
		{
			modelRBConfigItems.setCurrentConfig(currentConfig)
			// TODO: btnAddRecipe.enabled = modelRBConfigItems.canRecipesBeAdded()
		}
	}

	TextInputDialog {
		id: dlgAddHeader
		title: qsTr("Add header")
		onAccepted: {
			lvItems.currentIndex = modelRBConfigItems.addHeader(outputText, lvItems.currentIndex)
			lvItems.positionViewAtIndex(lvItems.currentIndex, ListView.Center)
		}
	}

	EditItemsListDialog {
		id: dlgAddRecipes
		title: qsTr("Add recipe")
		onListChanged: {
			lvItems.currentIndex = -1;
			btnAddRecipe.enabled = modelRBConfigItems.canRecipesBeAdded();
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
			btnAddRecipe.enabled = modelRBConfigItems.canRecipesBeAdded()
		}
	}

	// Rearrange recipe items and headers

	Component {
		id: dragDelegate

		MouseArea {
			id: dragArea

			property bool held: false

			anchors { left: parent != null ? parent.left : undefined; right: parent != null ? parent.right : undefined }
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

				anchors.horizontalCenter: parent.horizontalCenter
				anchors.verticalCenter: parent.verticalCenter

				width: dragArea.width; 
				height: column.implicitHeight + 47

				color: dragArea.held ? "lightgray" : "transparent"
                
				Drag.active: dragArea.held
				Drag.source: dragArea
				Drag.hotSpot.x: width / 2
				Drag.hotSpot.y: height / 2

				states: State {
					when: dragArea.held

					ParentChange { target: content; parent: rbConfigContentPage }
					AnchorChanges {
						target: content
						anchors { horizontalCenter: undefined; verticalCenter: undefined }
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
						anchors.margins: 15
						visible: !isHeader

						fillMode: Image.PreserveAspectFit
						source: "qrc:/images/reorder.svg"
					}

					// Item name
					Label {
						id: labelColumn
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: !isHeader ? reorderImage.right : parent.left

						anchors.margins: 15

						verticalAlignment: Text.AlignVCenter
						font.bold: isHeader
						text: name

						// TODO: This prevents swipe on label!
						MouseArea {
							anchors.fill: parent
							enabled: isHeader
							onPressAndHold: {
								lvItems.currentIndex = index
								dlgRenameHeader.initialText = modelRBConfigItems.name(lvItems.currentIndex);
								dlgRenameHeader.open()
							}
						}
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
				anchors { fill: parent; margins: 15 }

				onEntered: {
					modelRBConfigItems.updateMoveTarget(dragArea.DelegateModel.itemsIndex);
					rearrangeCurrentItemDelegateModel.items.move(drag.source.DelegateModel.itemsIndex,
																 dragArea.DelegateModel.itemsIndex);
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
		
		spacing: 0
		model: DelegateModel {
			id: rearrangeCurrentItemDelegateModel

			model: modelRBConfigItems
			delegate: dragDelegate
		}
	}

	// TODO: THESE!
	/*
	RoundButton {
		display: AbstractButton.IconOnly
		icon.source: "qrc:/images/add-black.svg"
		icon.color: "blue"

		ToolTip.delay: 1000
		ToolTip.timeout: 5000
		ToolTip.visible: hovered
		ToolTip.text: qsTr("Add header")

		onClicked: dlgAddHeader.open()
	}
	RoundButton {
		id: btnAddRecipe
		display: AbstractButton.IconOnly
		icon.source: "qrc:/images/add-black.svg"

		ToolTip.delay: 1000
		ToolTip.timeout: 5000
		ToolTip.visible: hovered
		ToolTip.text: qsTr("Add recipe")

		enabled: modelRBConfigItems.canRecipesBeAdded()
		onClicked: {
			filterModelUnusedRecipes.setRecipeBookConfiguration(currentConfig)

			dlgAddRecipes.editListModel = modelRBConfigItems;
			dlgAddRecipes.allValuesFilterModel = filterModelUnusedRecipes;

			dlgAddRecipes.open()
		}
	}

	RoundButton {
		display: AbstractButton.IconOnly
		icon.source: "qrc:/images/remove.svg"

		ToolTip.delay: 1000
		ToolTip.timeout: 5000
		ToolTip.visible: hovered
		ToolTip.text: qsTr("Remove item")

		enabled: lvItems.count > 0
		onClicked: {
			dlgRemoveItem.msgText = qsTr("This will remove the item \"%1\". Proceed?").arg(modelRBConfigItems.name(lvItems.currentIndex));
			dlgRemoveItem.open();
		}
	}
	*/
}
