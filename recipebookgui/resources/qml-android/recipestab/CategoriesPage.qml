import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.1

import "components"

// TODO: ANPASSEN!
Item {
	id: categoriesTab

	TextInputDialog {
		id: dlgAddSortOrder
		title: qsTr("Add sort order")
		onCurrentTextChanged: currentTextAllowed = !modelSortOrders.existsSortOrder(outputText)
		onAccepted: {
			lvSortOrders.currentIndex = modelSortOrders.addSortOrder(outputText)
			lvSortOrders.positionViewAtIndex(lvSortOrders.currentIndex, ListView.Center)
			modelSortOrder.setSortOrder(lvSortOrders.currentIndex)
		}
	}

	TextInputDialog {
		id: dlgRenameSortOrder
		title: qsTr("Rename sort order")
		onCurrentTextChanged: currentTextAllowed = !modelSortOrders.existsSortOrder(outputText)
		onAccepted: {
			lvSortOrders.currentIndex = modelSortOrders.renameSortOrder(lvSortOrders.currentIndex, outputText)
			lvSortOrders.positionViewAtIndex(lvSortOrders.currentIndex, ListView.Center)
			modelSortOrder.setSortOrder(lvSortOrders.currentIndex)
		}
	}
    
	TextMessageDialog {
		id: dlgRemoveSortOrder
		title: qsTr("Remove sort order")
		onAccepted: {
			modelSortOrders.removeSortOrder(lvSortOrders.currentIndex)
			lvSortOrders.incrementCurrentIndex()
			lvSortOrders.decrementCurrentIndex()
			modelSortOrder.setSortOrder(lvSortOrders.currentIndex)
		}
	}

	TextInputDialog {
		id: dlgAddCategory
		title: qsTr("Add category")
		onCurrentTextChanged: currentTextAllowed = !modelSortOrder.existsCategory(outputText)
		onAccepted: {
			lvCategories.currentIndex = modelSortOrder.addCategory(outputText)
			lvCategories.positionViewAtIndex(lvCategories.currentIndex, ListView.Center)
		}
	}

	TextInputDialog {
		id: dlgRenameCategory
		title: qsTr("Rename category")
		onCurrentTextChanged: currentTextAllowed = !modelSortOrder.existsCategory(outputText)
		onAccepted: {
			lvCategories.currentIndex = modelSortOrder.renameCategory(lvCategories.currentIndex, outputText)
		}
	}
    
	TextMessageDialog {
		id: dlgRemoveCategory
		title: qsTr("Remove category")
		onAccepted: {
			modelSortOrder.removeCategory(lvCategories.currentIndex)
			lvCategories.incrementCurrentIndex()
			lvCategories.decrementCurrentIndex()
		}
	}

	Label {
		id: labelSortOrders
        
		anchors.left: parent.left
		anchors.top: parent.top
		anchors.topMargin: 24
		anchors.leftMargin: 48
        
		text: qsTr("Sort orders")
		font.bold: true
	}

	ListView {
		id: lvSortOrders
		anchors.left: parent.left
		anchors.top: labelSortOrders.bottom 
		anchors.bottom: groupSortOrders.top
		anchors.topMargin: 48
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		width: 300

		ScrollBar.vertical: ScrollBar { }
		boundsBehavior: Flickable.StopAtBounds

		spacing: 5
		model: modelSortOrders
		delegate: ItemDelegate {
			highlighted: ListView.isCurrentItem
			onClicked: {
				lvSortOrders.currentIndex = index
				modelSortOrder.setSortOrder(index)
			}
			width: lvSortOrders.width - lvSortOrders.leftMargin - lvSortOrders.rightMargin
                
			text: name
		}

		Component.onCompleted: {
			if(lvSortOrders.count > 0) {
				lvSortOrders.currentIndex = 0;
				modelSortOrder.setSortOrder(0);
			}
		}

		Connections {
			target: modelSortOrders
			function onModelReset() {
				if(modelSortOrders.rowCount() <= lvSortOrders.currentIndex) {
					lvSortOrders.currentIndex = -1;
				}
				else if(modelSortOrders.rowCount() > 0 && lvSortOrders.currentIndex == -1) {
					lvSortOrders.currentIndex = 0;
				}
				modelSortOrder.setSortOrder(lvSortOrders.currentIndex);
			}
		}
	}

	Pane {
		id: groupSortOrders
		anchors.left: lvSortOrders.left
		anchors.right: lvSortOrders.right
		anchors.bottom: parent.bottom

		RowLayout {
			anchors.centerIn: parent
			spacing: 20

			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add sort order")

				enabled: modelSortOrders.canSortOrdersBeAdded()
				onClicked: dlgAddSortOrder.open()
			}
			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/edit.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Rename sort order")

				enabled: lvSortOrders.currentIndex != -1
				onClicked: {
					dlgRenameSortOrder.initialText = modelSortOrders.name(lvSortOrders.currentIndex);
					dlgRenameSortOrder.open();
				}
			}
			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/remove.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Remove sort order")

				enabled: lvSortOrders.count > 0
				onClicked: {
					dlgRemoveSortOrder.msgText = qsTr("This will remove the sort order \"%1\". Proceed?").arg(modelSortOrders.name(lvSortOrders.currentIndex));
					dlgRemoveSortOrder.open();
				}
			}
		}
	}

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
				held = true
				modelSortOrder.beginMoveCategory(index);
				lvCategories.currentIndex = -1
			}
			onReleased: {
				held = false
				modelSortOrder.applyMoveCategory();
			}
			onClicked: lvCategories.currentIndex = index
            
			Rectangle {
				id: content

				anchors.horizontalCenter: parent.horizontalCenter
				anchors.verticalCenter: parent.verticalCenter

				width: dragArea.width; 
				height: column.implicitHeight + 50

				color: dragArea.held || lvCategories.currentIndex == index ? "lightgray" : "transparent"
                
				Drag.active: dragArea.held
				Drag.source: dragArea
				Drag.hotSpot.x: width / 2
				Drag.hotSpot.y: height / 2

				states: State {
					when: dragArea.held

					ParentChange { target: content; parent: categoriesTab }
					AnchorChanges {
						target: content
						anchors { horizontalCenter: undefined; verticalCenter: undefined }
					}
				}

				Item {
					id: column
					anchors.left: parent.left
					anchors.verticalCenter: parent.verticalCenter

					Image {
						id: reorderImage
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: parent.left
						height: labelColumn.implicitHeight + 10
						verticalAlignment: Image.AlignVCenter
						anchors.margins: 15

						fillMode: Image.PreserveAspectFit
						source: "qrc:/images/reorder.svg"
					}

					Label {
						id: labelColumn
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: reorderImage.right

						anchors.margins: 15

						verticalAlignment: Text.AlignVCenter
						text: modelData

                        
					}
				}
			}

			DropArea {
				anchors { fill: parent; margins: 15 }

				onEntered: {
					modelSortOrder.updateMoveTarget(dragArea.DelegateModel.itemsIndex);
					categoriesDelegateModel.items.move(drag.source.DelegateModel.itemsIndex,
														dragArea.DelegateModel.itemsIndex);
				}
			}
		}
	}

	Label {
		id: labelCurrentSortOrder
        
		anchors.left: lvCategories.left
		anchors.top: parent.top
		anchors.topMargin: 24
        
		visible: lvSortOrders.currentIndex != -1

		text: qsTr("Sort order \"%1\"").arg(modelSortOrders.name(lvSortOrders.currentIndex))
		font.bold: true
	}

	ListView {
		id: lvCategories
		anchors.left: lvSortOrders.right
		anchors.top: labelCurrentSortOrder.bottom
		anchors.bottom: groupCategories.top
		anchors.topMargin: 48
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		width: 400

		visible: lvSortOrders.currentIndex != -1

		ScrollBar.vertical: ScrollBar { }
		boundsBehavior: Flickable.StopAtBounds

		spacing: 5
		model: DelegateModel {
			id: categoriesDelegateModel

			model: modelSortOrder
			delegate: dragDelegate
		}
	}

	Pane {
		id: groupCategories
		anchors.left: lvCategories.left
		anchors.right: lvCategories.right
		anchors.bottom: parent.bottom
		anchors.topMargin: 48
		
		visible: lvSortOrders.currentIndex != -1
		
		RowLayout {
			anchors.centerIn: parent
			spacing: 20

			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add category")

				enabled: modelSortOrder.canCategoriesBeAdded()
				onClicked: dlgAddCategory.open()
			}
			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/edit.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Rename category")

				enabled: lvCategories.count > 0 && lvCategories.currentIndex != -1 && lvCategories.currentIndex >= 0
				onClicked: {
					dlgRenameCategory.initialText = modelSortOrder.name(lvCategories.currentIndex);
					dlgRenameCategory.open();
				}
			}
			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/remove.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Remove category")

				enabled: lvCategories.count > 0 && lvCategories.currentIndex != -1 && modelSortOrder.canCategoryBeRemoved(lvCategories.currentIndex)
				onClicked: {
					dlgRemoveCategory.msgText = qsTr("This will remove the category \"%1\" from all sort orders. Proceed?").arg(modelSortOrder.name(lvCategories.currentIndex));
					dlgRemoveCategory.open();
				}
			}
		}
	}

	ToolSeparator {
		anchors.left: lvCategories.right
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.topMargin: 24
		anchors.leftMargin: 12
		anchors.bottomMargin: 24
	}

	Label {
		id: labelAlternativesGroups
        
		anchors.left: paneAlternativesGroups.left
		anchors.top: parent.top
		anchors.topMargin: 24
        
		text: qsTr("Alternatives types")
		font.bold: true
	}

	EditAlternativesGroupPane {
		id: paneAlternativesGroups
		anchors.left: lvCategories.right
		anchors.top: labelAlternativesGroups.bottom
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		anchors.topMargin: 48
		anchors.leftMargin: 48
		anchors.bottomMargin: 0
		anchors.rightMargin: 48
	}
}
