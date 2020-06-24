import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.14

import "components"

Item {

	property int currentRecipe : -1

	// Recipe item dialog

	TextMessageDialog {
		id: dlgRemoveIngredient
		title: qsTr("Remove ingredient")
		onAccepted: {
			modelRecipeItems.removeItem(lvCurrentRecipe.currentIndex)
			lvCurrentRecipe.currentIndex = -1
		}
	}

	// Pane

	RowLayout {
		id: grid
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.leftMargin: 10
		anchors.rightMargin: 10

		visible: currentRecipe != -1

		Label {
			text: qsTr("Number of persons")
		}
		Item {
			Layout.fillWidth: true
		}
		SpinBox {
			from: 1
			to: 50
			editable: true
			wheelEnabled: true
			value: filterModelRecipes.numberOfPersons(currentRecipe)

			onValueModified: filterModelRecipes.setNumberOfPersons(currentRecipe, value)
		}
	}

	// TODO?
	/*Switch {
		id: idRearrangeCurrentItems
		anchors.right: lvCurrentRecipe.right
		anchors.top: parent.top
		anchors.topMargin: 24

		visible: currentRecipe != -1

		text: qsTr("Rearrange recipe items")
	}*/

	// Edit recipe item list view
	ListView {
		id: lvCurrentRecipe
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: grid.bottom
		anchors.bottom: parent.bottom
		anchors.topMargin: 24
		anchors.bottomMargin: 24
		// TODO: visible: !idRearrangeCurrentItems.checked

		spacing: 0
		model: modelRecipeItems
		delegate: ItemDelegate {
			id: listItemRecipeItem
			highlighted: ListView.isCurrentItem
			onClicked: lvCurrentRecipe.currentIndex == index ? lvCurrentRecipe.currentIndex = -1 : lvCurrentRecipe.currentIndex = index
			width: lvCurrentRecipe.width - lvCurrentRecipe.leftMargin - lvCurrentRecipe.rightMargin
			height: listItemRecipeItemGroup.height

			Item {
				id: listItemRecipeItemGroup
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.top: parent.top
				anchors.topMargin: 10

				height: listItemRecipeItemName.height + 30 + (highlighted ? listItemGridRecipeItem.height : 0)

				// Group bar
				Rectangle {
					id: groupBar
					anchors.left: parent.left
					anchors.top: parent.top
					anchors.bottom: parent.bottom
					anchors.topMargin: -10
					anchors.bottomMargin: modelRecipeItems.lastInGroup(index) ? 10 : 0

					visible: hasGroup
					color: groupColor
					width: 5
				}

				// Ingredient / group name
				Label {
					id: listItemRecipeItemName
					anchors.left: hasGroup ? groupBar.right : parent.left
					anchors.leftMargin: 10

					font.bold: !optional
					font.italic: optional
					text: name
					verticalAlignment: Text.AlignVCenter
				}

				// Summary for inactive ingredients
				Label {
					function recipeItemSmallDesc(n){
						var text = "";
						if(amountUnit !== modelRecipeItems.indexUnitUnitless())
						{
							text += amountMin;
							if(amountIsRange)
							{
								text += "-" + amountMax;
							}
							text += " " + uiStrings.getUnitShortName(amountUnit);
						}
						if(sizeIndex != 1)
						{
							if(text !== "")
							{
								text = text + ", ";
							}
							text += uiStrings.getSizeName(sizeIndex, amountUnit)
						}
						if(additionalInfo != "")
						{
							if(text !== "")
							{
								text = text + ", ";
							}
							text += additionalInfo;
						}

						if(text !== "")
						{
							text = " (" + text + ")";
						}
						return text;
					}

					anchors.left: listItemRecipeItemName.right
					color: "gray"
					text: recipeItemSmallDesc(index)
					verticalAlignment: Text.AlignVCenter
					font.italic: optional
					width: parent.width - listItemRecipeItemName.width - 10
					wrapMode: Label.WordWrap
					visible: !listItemRecipeItem.highlighted
				}

				// Extended information for active ingredient elements
				GridLayout {
					id: listItemGridRecipeItem
					visible: listItemRecipeItem.highlighted

					anchors.left: parent.left
					anchors.right: parent.right
					anchors.top: listItemRecipeItemName.bottom
					anchors.leftMargin: 20
					anchors.rightMargin: 10
					anchors.topMargin: 10

					columns: 2
					columnSpacing: 10

					Label {
						text: qsTr("Amount")
					}
					RowLayout {
						Layout.fillWidth: true

						ComboBox {
							Layout.fillWidth: true
							model: uiStrings.getAllUnitNames()
							currentIndex: amountUnit
							onActivated: amountUnit = currentIndex
						}

						CheckBox {
							visible: amountUnit !== modelRecipeItems.indexUnitUnitless()

							text: qsTr("Range")
							checked: amountIsRange

							onClicked: amountIsRange = checked
						}
					}

					Label {
						visible: amountUnit !== modelRecipeItems.indexUnitUnitless()
						text: " "
					}
					RowLayout {
						visible: amountUnit !== modelRecipeItems.indexUnitUnitless()
						spacing: 20

						Label {
							visible: amountIsRange
							text: qsTr("Min.")
						}
						TextField {
							Layout.preferredWidth: 75
							selectByMouse: true
							horizontalAlignment: TextInput.AlignRight
							onFocusChanged: {
								if(focus)
									selectAll()
							}

							text: amountMin
							validator: DoubleValidator { bottom: 0; top: 9999; decimals: 3; locale: "en_US" }
							onEditingFinished: {
								if(amountIsRange && text > amountMax)
								{
									text = amountMax;
								}
								amountMin = text;
							}
						}

						Label {
							visible: amountIsRange
							text: qsTr("Max.")
						}
						TextField {
							Layout.preferredWidth: 75
							visible: amountIsRange

							selectByMouse: true
							horizontalAlignment: TextInput.AlignRight
							onFocusChanged: {
								if(focus)
									selectAll()
							}

							validator: DoubleValidator { bottom: 0; top: 9999; decimals: 3; locale: "en_US" }
							text: amountMax
							onEditingFinished: {
								if(amountIsRange && text < amountMin)
								{
									text = amountMin;
								}
								amountMax = text
							}
						}
					}

					Label {
						text: qsTr("Size")
					}
					SpinBox {
						from: 0
						to: uiStrings.getSizesCount() - 1

						textFromValue: function(value, locale) {
							return uiStrings.getSizeName(value, amountUnit);
						}
						valueFromText: function(text, locale) {
							return uiStrings.getSizeIndex(text);
						}

						value: sizeIndex
						onValueModified: sizeIndex = value
					}

					Label {
						text: qsTr("Add. Info")
					}
					TextField {
						Layout.fillWidth: true
						selectByMouse: true

						text: additionalInfo
						onEditingFinished: additionalInfo = text
					}

					Label {
						text: " "
					}
					CheckBox {
						text: qsTr("Optional")
						checked: optional

						onClicked: optional = checked
					}

					Label { text: qsTr("Group") }
					ComboBox {
						Layout.fillWidth: true

						model: alternativesGroups
						textRole: "name"
						valueRole: "name"

						currentIndex: indexOfValue(group)
						onActivated: {
							lvCurrentRecipe.currentIndex = modelRecipeItems.setGroup(lvCurrentRecipe.currentIndex, currentText)
						}

						onVisibleChanged: {
							if(visible)
								currentIndex = indexOfValue(group)
						}
					}
				}
			}
		}
	}

	/*
	// TODO: Is this compatible with the SwipeView?
	RoundButton {
		display: AbstractButton.IconOnly
		icon.source: "qrc:/images/remove.svg"

		enabled: lvCurrentRecipe.visible && lvCurrentRecipe.count > 0 && lvCurrentRecipe.currentIndex != -1
		onClicked: {
			dlgRemoveIngredient.msgText = qsTr("This will remove the ingredient \"%1\". Proceed?").arg(modelRecipeItems.name(lvCurrentRecipe.currentIndex));
			dlgRemoveIngredient.open();
		}
	}
	*/

	// TODO: Can i combine these or should the remain similar to before? (problem: move whole groups instead of single elements and how to show this...)
	// Rearrange recipe item list moveable delegate
	/*Component {
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
				filterModelRecipeItems.beginMove(index);
				
			}
			onReleased: {
				held = false
				filterModelRecipeItems.applyMove();
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

					ParentChange { target: content; parent: recipesTab }
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

						fillMode: Image.PreserveAspectFit
						source: "qrc:/images/reorder.svg"
					}

					Rectangle {
						id: reorderGroupBar
						anchors.left: reorderImage.right
						anchors.top: parent.top
						anchors.leftMargin: 10
						anchors.topMargin: -20
						anchors.bottomMargin: 20

						visible: hasGroup
						color: groupColor
						height: content.height
						width: 5
					}

					// Item name
					Label {
						id: labelColumn
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: hasGroup ? reorderGroupBar.right : reorderImage.right

						anchors.margins: 15

						verticalAlignment: Text.AlignVCenter
						font.bold: !optional
						font.italic: optional
						text: groupOrItemName
					}
				}
			}

			DropArea {
				anchors { fill: parent; margins: 15 }

				onEntered: {
					filterModelRecipeItems.updateMoveTarget(dragArea.DelegateModel.itemsIndex);
					rearrangeCurrentItemDelegateModel.items.move(drag.source.DelegateModel.itemsIndex,
																 dragArea.DelegateModel.itemsIndex);
				}
			}
		}
	}

	// Rearrange recipe item list view
	ListView {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: idRearrangeCurrentItems.bottom
		anchors.bottom: paneCurrentRecipe.top
		anchors.topMargin: 24
		anchors.leftMargin: 24
		anchors.bottomMargin: 48
		width: 400
		visible: idRearrangeCurrentItems.checked

		ScrollBar.vertical: ScrollBar { }
		boundsBehavior: Flickable.StopAtBounds

		spacing: 0
		model: DelegateModel {
			id: rearrangeCurrentItemDelegateModel

			model: filterModelRecipeItems
			delegate: dragDelegate
		}
	}*/


}
