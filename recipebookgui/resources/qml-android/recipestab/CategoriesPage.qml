import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.1

import "components"

Item {
	id: categoriesPage

	// TODO: Dialogs with text input should be placed differently on android (avoid the keyboard, i.e. pos should be updated when the keyboard is (in)visible)!
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
	TextMessageDialog {
		id: dlgCantRemoveIngredient
		title: qsTr("Category can't be removed")
		okOnly: true
	}

	// Header Component

	onVisibleChanged: {
		if(visible)
		{
			headerSubpageSpace.sourceComponent = categoriesHeaderComponent;
		}
	}

	Component {
		id: categoriesHeaderComponent

		ToolButton {
			anchors.right: parent.right
			anchors.rightMargin: 10

			display: AbstractButton.IconOnly
			icon.source: "qrc:/images/add-black.svg"

			onVisibleChanged: {
				if(visible)
				{
					enabled = modelSortOrder.canCategoriesBeAdded()
				}
			}
			onClicked: dlgAddCategory.open()
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

	component MyDragDelegate : Item {

		MouseArea {
			id: dragArea

			property bool held: false

			anchors.left: parent != null ? parent.left : undefined;
			anchors.leftMargin: 15
			width: reorderImage.height
			height: content.height

			drag.target: held ? content : undefined
			drag.axis: Drag.YAxis

			onPressed: {
				held = true
				modelSortOrder.beginMoveCategory(index);
			}
			onReleased: {
				held = false
				modelSortOrder.applyMoveCategory();
			}

			Rectangle {
				id: content

				anchors.left: parent.left
				anchors.verticalCenter: parent.verticalCenter

				anchors.leftMargin: -15
				width: lvCategories.width - lvCategories.leftMargin - lvCategories.rightMargin
				height: column.implicitHeight + 40

				color: dragArea.held ? "lightgray" : "transparent"

				Drag.active: dragArea.held
				Drag.source: swipeDelegate
				Drag.hotSpot.x: dragArea.width / 2
				Drag.hotSpot.y: dragArea.height / 2

				states: State {
					when: dragArea.held

					ParentChange { target: content; parent: categoriesPage }
					AnchorChanges {
						target: content
						anchors { verticalCenter: undefined; left: undefined; }
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
						anchors.leftMargin: 15
						height: labelColumn.implicitHeight + 10
						verticalAlignment: Image.AlignVCenter

						fillMode: Image.PreserveAspectFit
						source: "qrc:/images/reorder.svg"
					}

					Label {
						id: labelColumn
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: reorderImage.right
						anchors.leftMargin: 15

						verticalAlignment: Text.AlignVCenter
						text: modelData

						MouseArea {
							anchors.fill: parent
							onPressAndHold: {
								lvCategories.currentIndex = index
								dlgRenameCategory.initialText = modelSortOrder.name(lvCategories.currentIndex);
								dlgRenameCategory.open();

							}
						}
					}
				}
			}

			DropArea {
				anchors { fill: parent; }

				onEntered: {
					modelSortOrder.updateMoveTarget(swipeDelegate.itemsIndex);
					categoriesDelegateModel.items.move(drag.source.itemsIndex,
														swipeDelegate.itemsIndex);
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

		visible: cbxSortOrders.currentIndex != -1

		ScrollBar.vertical: ScrollBar { }
		boundsBehavior: Flickable.StopAtBounds

		model: DelegateModel {
			id: categoriesDelegateModel

			model: modelSortOrder
			delegate: SwipeDelegate {
				id: swipeDelegate
				property int itemsIndex : DelegateModel.itemsIndex

				width: lvCategories.width - lvCategories.leftMargin - lvCategories.rightMargin

				contentItem: MyDragDelegate {
					id: dragDelegate

				}

				Timer {
					id: undoTimer
					interval: 2750
					onTriggered: {
						modelSortOrder.removeCategory(index)
					}
				}

				swipe.onCompleted: {
					if(!modelSortOrder.canCategoryBeRemoved(index))
					{
						swipeDelegate.swipe.close()
						dlgCantRemoveIngredient.msgText = qsTr("Category \"%1\" is still in use and can't be removed.").arg(name);
						dlgCantRemoveIngredient.open();
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

						visible: !swipeDelegate.swipe.complete

						fillMode: Image.PreserveAspectFit
						source: "qrc:/images/remove.svg"
					}
					Label {
						anchors.left: parent.left
						anchors.leftMargin: 10
						anchors.verticalCenter: parent.verticalCenter
						text: qsTr("Category \"%1\" removed").arg(name)

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
