import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls.Material 2.4

import "components"

Item {

	// General rounding method
	function roundValue(value) {
		if(Math.abs(value - Math.round(value)) <= 0.0001)
		{
			return Number(value).toLocaleString(Qt.locale("en_US"), 'f', 0);
		}
		else if(Math.abs(value*10 - Math.round(value * 10)) <= 0.0001)
		{
			return Number(value).toLocaleString(Qt.locale("en_US"), 'f', 1);
		}
		else if(Math.abs(value*100 - Math.round(value * 100)) <= 0.0001)
		{
			return Number(value).toLocaleString(Qt.locale("en_US"), 'f', 2);
		}
		else
		{
			return Number(value).toLocaleString(Qt.locale("en_US"), 'f', 3);
		}
	}

	// Recipes dialogs

	AddRecipesListDialog {
		id: dlgAddExistingRecipes
		title: qsTr("Add existing recipes")
	}

	TextInputDialog {
		id: dlgAddNewRecipe
		title: qsTr("Add new recipe")
		onCurrentTextChanged: currentTextAllowed = !modelShoppingRecipes.existsRecipe(outputText)
		onAccepted: {
			var index = modelShoppingRecipes.addNewRecipe(outputText)
			lvRecipes.positionViewAtIndex(index, ListView.Center)
		}
	}

	TextInputDialog {
		id: dlgRenameRecipe
		title: qsTr("Rename recipe")
		onCurrentTextChanged: currentTextAllowed = !modelShoppingRecipes.existsRecipe(outputText)

		property int myIndex : -1
		onAccepted: {
			var index = modelShoppingRecipes.renameRecipe(myIndex, outputText)
			lvRecipes.positionViewAtIndex(index, ListView.Center)
		}
	}

	TextMessageDialog {
		id: dlgClearShoppingList
		title: qsTr("Clear shopping list")
		onAccepted: {
			modelShoppingRecipes.resetShoppingList()
		}
	}

	// Recipe dialogs

	DateSelectionDialog {
		id: dlgDateSelection
		
		property int myIndex : -1
		onAccepted: {
			modelShoppingRecipes.setDueDate(myIndex, selectedDate)
		}
		onReset: {
			modelShoppingRecipes.resetDueDate(myIndex);
			close();
		}
	}

	// Recipe items dialogs

	EditItemsListDialog {
		id: dlgEditShoppingListItemsList
		title: qsTr("Edit ingredients list")

		onListChanged: {
			editListModel.modelReset();
		}
	}

	// Header Component

	Component.onCompleted: {
		onVisibleChanged(true)
	}

	onVisibleChanged: {
		if(visible)
		{
			headerSubpageSpace.sourceComponent = ingredientHeaderComponent;
		}
	}

	Component {
		id: ingredientHeaderComponent

		RowLayout {
			anchors.right: parent.right
			anchors.rightMargin: 10

			ToolButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/undo.svg"

				onClicked: {
					dlgClearShoppingList.msgText = qsTr("This will remove all recipes from the list. Proceed?");
					dlgClearShoppingList.open();
				}
			}
		}
	}

	// Floating action buttons

	RoundButton {
		id: buttonAddFromRecipe
		anchors.right: parent.right
		anchors.rightMargin: 5
		anchors.bottom: parent.bottom
		z: 1
		scale: 1.5

		Material.background: Material.Indigo

		display: AbstractButton.IconOnly
		icon.source: "qrc:/images/add-black.svg"
		icon.color: "white"

		onVisibleChanged: {
			if(visible)
			{
				enabled = modelShoppingRecipes.canShoppingRecipesBeAddedFromRecipes()
			}
		}
		onClicked: {
			dlgAddExistingRecipes.editListModel = modelShoppingRecipes;
			dlgAddExistingRecipes.allValuesFilterModel = filterModelRecipes;
			dlgAddExistingRecipes.open();
		}
	}

	// TODO: Different symbol?
	RoundButton {
		anchors.right: parent.right
		anchors.rightMargin: 5
		anchors.bottom: buttonAddFromRecipe.top
		anchors.bottomMargin: 10
		z: 1
		scale: 1.33

		Material.background: Material.Indigo

		display: AbstractButton.IconOnly
		icon.source: "qrc:/images/add-black.svg"
		icon.color: "white"

		onVisibleChanged: {
			if(visible)
			{
				enabled = modelShoppingRecipes.canNewShoppingRecipesBeAdded()
			}
		}
		onClicked: dlgAddNewRecipe.open()
	}

	// Main page

	ListView {
		id: lvRecipes
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.bottom: parent.bottom

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

		spacing: 0
		model: modelShoppingRecipes
		delegate: SwipeDelegate {
			id: listRecipeItem
			width: lvRecipes.width - lvRecipes.leftMargin - lvRecipes.rightMargin
			implicitHeight: listRecipesItemGroup.implicitHeight

			contentItem: Item {
				id: listRecipesItemGroup
				implicitHeight: listItemRecipesName.height + grid.height + lvRecipeItems.height + 5

				// TODO: Change to make it more obvious that this is actually clickable
				RoundButton {
					id: buttonEditRecipeItems
					anchors.top: parent.top
					anchors.left: parent.left
					anchors.leftMargin: -20
					anchors.topMargin: -16

					display: AbstractButton.IconOnly
					icon.source: "qrc:/images/list-black.svg"
					flat: true

					onVisibleChanged: {
						if(visible)
						{
							enabled = lvRecipeItems.model.canItemsBeAdded()
						}
					}
					onClicked: {
						dlgEditShoppingListItemsList.editListModel = lvRecipeItems.model;
						dlgEditShoppingListItemsList.allValuesFilterModel = filterModelIngredients;
						dlgEditShoppingListItemsList.open();
					}
				}

				Label {
					id: listItemRecipesName
					anchors.left: buttonEditRecipeItems.right
					anchors.right: parent.right
					anchors.top: parent.top

					text: name
					verticalAlignment: Text.AlignVCenter

					color: everythingSet ? "black" : "red"
					font.bold: true

					MouseArea {
						anchors.fill: parent
						propagateComposedEvents: true
						onPressAndHold: {
							dlgRenameRecipe.initialText = modelShoppingRecipes.name(index);
							dlgRenameRecipe.myIndex = index;
							dlgRenameRecipe.open();
						}
					}
				}



				// Additional recipe data

				GridLayout {
					id: grid
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.top: listItemRecipesName.bottom
					anchors.topMargin: -5

					columns: 2

					Label {
						text: qsTr("Number of persons")
					}
					SpinBox {
						id: spinboxScalingFactor
						from: 1 * 100
						to: 50 * 100
						stepSize: 100
						editable: true
						wheelEnabled: true

						property int decimals: 2
						property real realValue: value / 100

						value: scalingFactor * 100
						onValueModified: modelShoppingRecipes.setScalingFactor(index, value / 100)

						validator: DoubleValidator {
							bottom: Math.min(spinboxScalingFactor.from, spinboxScalingFactor.to)
							top:  Math.max(spinboxScalingFactor.from, spinboxScalingFactor.to)
							locale: "en_US"
						}

						textFromValue: function(value, locale) {
							return Number(value / 100).toLocaleString(Qt.locale("en_US"), 'f', spinboxScalingFactor.decimals)
						}

						valueFromText: function(text, locale) {
							return Number.fromLocaleString(Qt.locale("en_US"), text) * 100
						}
					}

					Label {
						Layout.rightMargin: 20
						text: qsTr("Due date")
					}
					Label {
						Layout.leftMargin: 10
						Layout.fillWidth: true
						text: dueDateSet ? dueDate.toLocaleDateString(Qt.locale(), "ddd, dd.MM.yyyy") : qsTr("None")
						font.underline: true

						MouseArea {
							anchors.fill: parent

							onClicked: {
								var currentDate = new Date();
								var selDate = modelShoppingRecipes.dueDate(index);
								dlgDateSelection.minimumDate = selDate < currentDate ? selDate : currentDate;
								dlgDateSelection.selectedDate = selDate;
								dlgDateSelection.myIndex = index;
								dlgDateSelection.open();
							}
						}
					}
				}

				// Recipe items

				// TODO: First item always active at first. Why?
				ListView {
					id: lvRecipeItems
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.top: grid.bottom
					anchors.topMargin: 10

					interactive: false
					implicitHeight: lvRecipeItems.contentItem.childrenRect.height + 20

					keyNavigationEnabled: false

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

					spacing: 0
					model: modelShoppingRecipes.getItemsModel(index)
					delegate: SwipeDelegate {
						id: listItemRecipeItem
						highlighted: ListView.isCurrentItem
						width: lvRecipeItems.width - lvRecipeItems.leftMargin - lvRecipeItems.rightMargin
						implicitHeight: listItemRecipeItemGroup.implicitHeight

						onClicked: {
							if(lvRecipeItems.currentIndex == index)
							{
								lvRecipeItems.currentIndex = -1;
								laoderExtendedInfo.sourceComponent = undefined;
							}
							else
							{
								lvRecipeItems.currentIndex = index;
								laoderExtendedInfo.sourceComponent = componentExtendedInfo;
							}
						}

						contentItem: Item {
							id: listItemRecipeItemGroup
							implicitHeight: listItemRecipeItemName.height + (highlighted ? laoderExtendedInfo.height : -20)

							Rectangle {
								id: groupBar
								anchors.left: parent.left
								anchors.top: parent.top
								anchors.bottom: parent.bottom
								anchors.topMargin: -8
								anchors.bottomMargin: -8
								anchors.leftMargin: -10

								visible: hasGroup
								color: groupColor
								width: 5
							}

							// Ingredient / group name
							CheckBox {
								id: listItemRecipeItemName
								anchors.top: parent.top
								anchors.left: hasGroup ? groupBar.right : parent.left
								anchors.topMargin: -18
								anchors.leftMargin: hasGroup ? 0 :  -15

								font.italic: optional
								text: checked ? name : "<font color=\"gray\">" + name + "</font>"

								checked: itemEnabled
								onClicked: {
									itemEnabled = checked

									if(!checked && lvRecipeItems.currentIndex == index)
									{
										lvRecipeItems.currentIndex = -1;
									}
								}
							}

							// Summary for inactive ingredients
							Label {
								function recipeItemSmallDesc(n){
									var text = "";
									if(amountUnit !== modelShoppingListItems.indexUnitUnitless())
									{
										text += roundValue(amountMin);
										if(amountIsRange)
										{
											text += "-" + roundValue(amountMax);
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
								anchors.verticalCenter: listItemRecipeItemName.verticalCenter
								color: "gray"
								text: recipeItemSmallDesc(index)
								verticalAlignment: Text.AlignVCenter
								font.italic: optional
								width: parent.width - listItemRecipeItemName.width - 10
								wrapMode: Label.WordWrap
								visible: !listItemRecipeItem.highlighted
							}

							// Extended information for active ingredient elements
							Loader {
								anchors.left: parent.left
								anchors.right: parent.right
								anchors.top: listItemRecipeItemName.bottom

								id: laoderExtendedInfo
							}

							// Extended information for active ingredient
							Component {
								id: componentExtendedInfo

								GridLayout {
									id: listItemGridRecipeItem

									anchors.left: parent.left
									anchors.right: parent.right
									anchors.top: parent.top
									anchors.leftMargin: 20
									anchors.rightMargin: 10

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
											visible: amountUnit !== modelShoppingListItems.indexUnitUnitless()

											text: qsTr("Range")
											checked: amountIsRange

											onClicked: amountIsRange = checked
										}
									}

									Label {
										visible: amountUnit !== modelShoppingListItems.indexUnitUnitless()
										text: " "
									}
									RowLayout {
										visible: amountUnit !== modelShoppingListItems.indexUnitUnitless()
										spacing: 20

										Label {
											visible: amountIsRange
											text: qsTr("Min.")
										}
										TextField {
											Layout.preferredWidth: 32
											selectByMouse: true
											horizontalAlignment: TextInput.AlignRight
											onFocusChanged: {
												if(focus)
													selectAll()
											}

											text: roundValue(amountMin)
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
											Layout.preferredWidth: 32
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
												amountMax = roundValue(text)
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
								}
							}
						}

						Timer {
							id: undoTimerRecipeItems
							interval: 2750
							onTriggered: lvRecipeItems.model.removeItem(index)
						}

						swipe.onCompleted: {
							undoTimerRecipeItems.start()
							if(index === lvRecipeItems.currentIndex)
							{
								lvRecipeItems.currentIndex = -1
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
								text: qsTr("Recipe item \"%1\" removed").arg(name)

								visible: listItemRecipeItem.swipe.complete
							}
							RoundButton {
								text: qsTr("Undo")

								anchors.right: parent.right
								anchors.verticalCenter: parent.verticalCenter

								onClicked: {
									undoTimerRecipeItems.stop()
									listItemRecipeItem.swipe.close()
								}

								visible: listItemRecipeItem.swipe.complete
							}
						}
					}
				}
			}

			Timer {
				id: undoTimerRecipes
				interval: 2750
				onTriggered: modelShoppingRecipes.removeRecipe(index)
			}

			swipe.onCompleted: undoTimerRecipes.start()

			swipe.left: Rectangle {
				width: parent.width
				height: parent.height

				clip: true
				color: "darkgray"

				Image {
					anchors.left: parent.left
					anchors.leftMargin: 10
					anchors.verticalCenter: parent.verticalCenter

					visible: !listRecipeItem.swipe.complete

					fillMode: Image.PreserveAspectFit
					source: "qrc:/images/remove.svg"
				}
				Label {
					anchors.left: parent.left
					anchors.leftMargin: 10
					anchors.verticalCenter: parent.verticalCenter
					text: qsTr("Recipe \"%1\" removed").arg(name)

					visible: listRecipeItem.swipe.complete
				}
				RoundButton {
					text: qsTr("Undo")

					anchors.right: parent.right
					anchors.verticalCenter: parent.verticalCenter

					onClicked: {
						undoTimerRecipes.stop()
						listRecipeItem.swipe.close()
					}

					visible: listRecipeItem.swipe.complete
				}
			}
		}
	}
}
