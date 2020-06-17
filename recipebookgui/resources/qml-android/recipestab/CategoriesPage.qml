import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.1

import "components"

// TODO: ANPASSEN!
Item {
	id: categoriesTab

    // TODO: Dialogs should be placed differently on android (avoid the keyboard, i.e. pos should be updated when the keyboard is (in)visible)!
	TextInputDialog {
		id: dlgAddSortOrder
        title: qsTr("Add sort order")
        onCurrentTextChanged: currentTextAllowed = !modelSortOrders.existsSortOrder(outputText)
		onAccepted: {
            cbxSortOrders.currentIndex = modelSortOrders.addSortOrder(outputText)
            cbxSortOrders.positionViewAtIndex(cbxSortOrders.currentIndex, ListView.Center)
            modelSortOrder.setSortOrder(cbxSortOrders.currentIndex)
		}
	}

	TextInputDialog {
		id: dlgRenameSortOrder
		title: qsTr("Rename sort order")
        // TODO: This doesn't quite work (enabled not always updated correctly)!
		onCurrentTextChanged: currentTextAllowed = !modelSortOrders.existsSortOrder(outputText)
		onAccepted: {
            cbxSortOrders.currentIndex = modelSortOrders.renameSortOrder(cbxSortOrders.currentIndex, outputText)
            cbxSortOrders.positionViewAtIndex(cbxSortOrders.currentIndex, ListView.Center)
            modelSortOrder.setSortOrder(cbxSortOrders.currentIndex)
		}
	}
    
	TextMessageDialog {
		id: dlgRemoveSortOrder
		title: qsTr("Remove sort order")
		onAccepted: {
            modelSortOrders.removeSortOrder(cbxSortOrders.currentIndex)
            cbxSortOrders.incrementCurrentIndex()
            cbxSortOrders.decrementCurrentIndex()
            modelSortOrder.setSortOrder(cbxSortOrders.currentIndex)
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

    ComboBox {
        id: cbxSortOrders
		anchors.left: parent.left
        anchors.right: btnAddSortOrder.left
        anchors.top: parent.top

        model: modelSortOrders
        textRole: "name"
        onActivated: modelSortOrder.setSortOrder(currentIndex)

		Component.onCompleted: {
            if(cbxSortOrders.count > 0) {
                cbxSortOrders.currentIndex = 0;
				modelSortOrder.setSortOrder(0);
			}
        }

		Connections {
			target: modelSortOrders
			function onModelReset() {
                if(modelSortOrders.rowCount() <= cbxSortOrders.currentIndex) {
                    cbxSortOrders.currentIndex = -1;
				}
                else if(modelSortOrders.rowCount() > 0 && cbxSortOrders.currentIndex === -1) {
                    cbxSortOrders.currentIndex = 0;
				}
                modelSortOrder.setSortOrder(cbxSortOrders.currentIndex);
			}
		}
	}

    RoundButton {
        id: btnAddSortOrder
        anchors.top: parent.top
        anchors.right: parent.right

        display: AbstractButton.IconOnly
        icon.source: "qrc:/images/more-vert.svg"

        onClicked: menuSortOrders.open()

        Menu {
            id: menuSortOrders
            y: btnAddSortOrder.y

            MenuItem {
                text: qsTr("Add sort order")
                enabled: modelSortOrders.canSortOrdersBeAdded()
                onClicked: {
                    dlgAddSortOrder.open()
                    highlighted = false
                }
            }

            MenuItem {
                text: qsTr("Rename sort order")
                enabled: cbxSortOrders.currentIndex != -1
                onClicked: {
                    dlgRenameSortOrder.initialText = modelSortOrders.name(cbxSortOrders.currentIndex);
                    dlgRenameSortOrder.open();
                    highlighted = false
                }
            }

            MenuItem {
                text: qsTr("Remove sort order")
                enabled: cbxSortOrders.currentIndex != -1
                onClicked: {
                    dlgRemoveSortOrder.msgText = qsTr("This will remove the sort order \"%1\". Proceed?").arg(modelSortOrders.name(cbxSortOrders.currentIndex));
                    dlgRemoveSortOrder.open();
                    highlighted = false
                }
            }
        }
    }

    // TODO: Dragging should only be possible when grabbing the image!
    // TODO: Scrolling should work everywhere except on the image (not only on the scrollbar)
    // TODO: SwipeView with pages might collide with the swiping gestures i want to have here...
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

	ListView {
		id: lvCategories
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: cbxSortOrders.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 24
        anchors.bottomMargin: 24
        anchors.leftMargin: 24
        anchors.rightMargin: 24

        visible: cbxSortOrders.currentIndex != -1

		ScrollBar.vertical: ScrollBar { }
		boundsBehavior: Flickable.StopAtBounds

		spacing: 5
		model: DelegateModel {
			id: categoriesDelegateModel

			model: modelSortOrder
			delegate: dragDelegate
		}
	}

    // TODO: Add these methods somewhere!
    /*
        // TODO: How to add categories? Button at the top (might be confusing, though)? Hovering button (as in einkaufsliste) might be the best option here.
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
        // TODO: Rename after long click on name!
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
        // TODO: Remove by swipe!
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
    }*/
}
