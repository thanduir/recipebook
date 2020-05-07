import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "components"

Item {

	// Recipes dialogs

	AddRecipesListDialog {
		id: dlgAddExistingRecipes
		title: qsTr("Add existing recipes")

		onListChanged: {
			lvRecipes.currentIndex = -1;
			modelShoppingListItems.setShoppingRecipe(-1);
		}
	}

	TextInputDialog {
		id: dlgAddNewRecipe
		title: qsTr("Add new recipe")
		onCurrentTextChanged: currentTextAllowed = !modelShoppingRecipes.existsRecipe(outputText)
		onAccepted: {
			lvRecipes.currentIndex = modelShoppingRecipes.addNewRecipe(outputText)
			lvRecipes.positionViewAtIndex(lvRecipes.currentIndex, ListView.Center)
		}
	}

	TextInputDialog {
		id: dlgRenameRecipe
		title: qsTr("Rename recipe")
		onCurrentTextChanged: currentTextAllowed = !modelShoppingRecipes.existsRecipe(outputText)
		onAccepted: {
			lvRecipes.currentIndex = modelShoppingRecipes.renameRecipe(lvRecipes.currentIndex, outputText)
			lvRecipes.positionViewAtIndex(lvRecipes.currentIndex, ListView.Center)
		}
	}
    
	TextMessageDialog {
		id: dlgRemoveRecipe
		title: qsTr("Remove recipe")
		onAccepted: {
			modelShoppingRecipes.removeRecipe(lvRecipes.currentIndex)
			lvRecipes.incrementCurrentIndex()
			lvRecipes.decrementCurrentIndex()
		}
	}

	TextMessageDialog {
		id: dlgClearShoppingList
		title: qsTr("Clear shopping list")
		onAccepted: {
			modelShoppingRecipes.resetShoppingList()
			lvRecipes.incrementCurrentIndex()
			lvRecipes.decrementCurrentIndex()
		}
	}

	// Recipe dialogs

	DateSelectionDialog {
		id: dlgDateSelection
		title: qsTr("Select date")
		onAccepted: {
			modelShoppingRecipes.setDueDate(lvRecipes.currentIndex, selectedDate)
		}
	}

	// Recipe items dialogs

	EditItemsListDialog {
		id: dlgEditShoppingListItemsList
		title: qsTr("Edit ingredients list")

		onListChanged: {
			lvCurrentRecipe.currentIndex = -1;
			modelShoppingListItems.modelReset();
		}
	}

	TextMessageDialog {
		id: dlgRemoveIngredient
		title: qsTr("Remove ingredient")
		onAccepted: {
			modelShoppingListItems.removeItem(lvCurrentRecipe.currentIndex)
			lvCurrentRecipe.currentIndex = -1
		}
	}

	// Recipes list

	Label {
		id: labelRecipes
        
		anchors.left: parent.left
		anchors.top: parent.top
		anchors.topMargin: 24
		anchors.leftMargin: 48
        
		text: qsTr("Recipes")
		font.bold: true
	}

	ToolButton {
		id: buttonClearList

		anchors.verticalCenter: labelRecipes.verticalCenter
		anchors.right: scrollViewRecipes.right

		display: AbstractButton.IconOnly
		icon.source: "qrc:/images/clear-black.svg"

		ToolTip.delay: 1000
		ToolTip.timeout: 5000
		ToolTip.visible: hovered
		ToolTip.text: qsTr("Clear all shopping recipe")

		enabled: lvRecipes.currentIndex < lvRecipes.count
		onClicked: {
			dlgClearShoppingList.msgText = qsTr("This will remove all recipes from the list. Proceed?");
			dlgClearShoppingList.open();
		}
	}

	ScrollView {
		id: scrollViewRecipes
		anchors.left: parent.left
		anchors.top: labelRecipes.bottom 
		anchors.bottom: paneRecipes.top
		anchors.topMargin: 48
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		width: 300

		ListView {
			id: lvRecipes
			anchors.fill: parent
			topMargin: 5
			leftMargin: 5
			bottomMargin: 5
			rightMargin: 5
			boundsBehavior: Flickable.StopAtBounds

			spacing: 5
			model: modelShoppingRecipes
			delegate: ItemDelegate {
				highlighted: ListView.isCurrentItem
				onClicked: {
					forceActiveFocus()
					lvRecipes.currentIndex = index
					modelShoppingListItems.setShoppingRecipe(index)
					lvCurrentRecipe.currentIndex = -1
				}
				width: lvRecipes.width - lvRecipes.leftMargin - lvRecipes.rightMargin
                
				text: name
			}

			Component.onCompleted: {
				if(lvRecipes.count > 0) {
					lvRecipes.currentIndex = 0;
					modelShoppingListItems.setShoppingRecipe(0);
				}
			}
		}
	}

	Pane {
		id: paneRecipes
		anchors.bottom: parent.bottom
		anchors.left: scrollViewRecipes.left
		anchors.right: scrollViewRecipes.right
		anchors.leftMargin: 10
		anchors.rightMargin: 10
		anchors.bottomMargin: 10

		RowLayout {
			anchors.fill: parent
        
			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add from recipe")

				onClicked: {
					dlgAddExistingRecipes.editListModel = modelShoppingRecipes;
					dlgAddExistingRecipes.allValuesFilterModel = filterModelRecipes;
					dlgAddExistingRecipes.open();
				}
			}

			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"
				icon.color: "blue"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add new shopping recipe")

				onClicked: dlgAddNewRecipe.open()
			}

			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/edit.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Rename shopping recipe")

				onClicked: {
					dlgRenameRecipe.initialText = modelShoppingRecipes.name(lvRecipes.currentIndex);
					dlgRenameRecipe.open();
				}
			}

			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/remove.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Remove shopping recipe")

				enabled: lvRecipes.currentIndex < lvRecipes.count && lvRecipes.currentIndex >= 0
				onClicked: {
					dlgRemoveRecipe.msgText = qsTr("This will remove the recipe \"" + modelShoppingRecipes.name(lvRecipes.currentIndex) + "\". Proceed?");
					dlgRemoveRecipe.open();
				}
			}
		}
	}

	Label {
		id: labelCurrentRecipe
        
		anchors.left: grid.left
		anchors.top: parent.top
		anchors.topMargin: 24
        
		visible: lvRecipes.count > 0 && lvRecipes.currentIndex != -1

		text: qsTr("Recipe \"" + modelShoppingRecipes.name(lvRecipes.currentIndex) + "\"")
		font.bold: true
	}

	GridLayout {
		id: grid
		anchors.left: scrollViewRecipes.right
		anchors.top: labelCurrentRecipe.bottom
		anchors.topMargin: 24
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		anchors.rightMargin: 24
        
		width: 375

		visible: lvRecipes.count > 0 && lvRecipes.currentIndex != -1
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

			value: modelShoppingRecipes.scalingFactor(lvRecipes.currentIndex) * 100
			onValueModified: modelShoppingRecipes.setScalingFactor(lvRecipes.currentIndex, value / 100)
			
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
			Layout.rightMargin: 50
			text: qsTr("Due date")
		}
		RowLayout { 
			Layout.fillWidth: true

			Label {
				Layout.fillWidth: true
				horizontalAlignment: Text.AlignRight
				rightPadding: 10
				// TODO: This is not updated on changes (even though dataChanged is set in model)!
				text: modelShoppingRecipes.dueDate(lvRecipes.currentIndex).toLocaleDateString(Qt.locale(), "dddd, dd.MM.yyyy")
			}

			RoundButton {
				id: buttonEditDate

				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/date-range.svg"

				onClicked: {
					var currentDate = new Date();
					var selDate = modelShoppingRecipes.dueDate(lvRecipes.currentIndex);
					dlgDateSelection.minimumDate = selDate < currentDate ? selDate : currentDate;
					dlgDateSelection.selectedDate = selDate;
					dlgDateSelection.open();
				}
			}
		}
	}

	// TODO: Idea: Use this empty space in middle column for alternatives groups (combo box with groups present + ListView with checkboxes below to select items to use?)
	//		Or is there a better way to do this? Would it be better to use a button for this that opens a dialog with the checkboxes? 
	//		Or add checkboxes to the list on the right?
	//		Idea: left/right buttons on item to switch between them.
	//		Do i need a status "hidden" anyways (independent of how exactly i show these group items)?
	//			=> YES!

	// Edit recipe item list view
	ScrollView {
		id: scrollViewCurrentRecipe
		anchors.left: grid.right
		anchors.top: parent.top
		anchors.bottom: paneCurrentRecipe.top
		anchors.topMargin: 24
		anchors.leftMargin: 24
		anchors.bottomMargin: 48
		width: 400
		
		ListView {
			id: lvCurrentRecipe
			anchors.fill: parent
			topMargin: 5
			leftMargin: 5
			bottomMargin: 5
			rightMargin: 5
			boundsBehavior: Flickable.StopAtBounds
			keyNavigationEnabled: false
            
			spacing: 5
			model: modelShoppingListItems
			delegate: ItemDelegate {
				id: listItemRecipeItem
				highlighted: ListView.isCurrentItem
				onPressed: lvCurrentRecipe.currentIndex == index ? lvCurrentRecipe.currentIndex = -1 : lvCurrentRecipe.currentIndex = index
				width: lvCurrentRecipe.width - lvCurrentRecipe.leftMargin - lvCurrentRecipe.rightMargin
				height: listItemRecipeItemGroup.height

				Item {
					id: listItemRecipeItemGroup
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.top: parent.top
					anchors.topMargin: 10

					height: listItemRecipeItemName.height + 30 + (highlighted ? listItemGridRecipeItem.height : 0)

					// Ingredient / group name
					Label {
						id: listItemRecipeItemName
						anchors.left: parent.left
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
							if(group != alternativesGroups.stringNoAlternativesGroup())
							{
								text += group;
							}
							if(amountUnit != modelShoppingListItems.indexUnitUnitless())
							{
								if(text != "")
								{
									text = text + ", ";
								}
								text += amountMin;
								if(amountIsRange)
								{
									text += "-" + amountMax;
								}
								text += " " + unitNamesShort[amountUnit];
							}
							if(sizeIndex != 1)
							{
								if(text != "")
								{
									text = text + ", ";
								}
								text += sizeNames[sizeIndex]
							}
							if(additionalInfo != "")
							{
								if(text != "")
								{
									text = text + ", ";
								}
								text += additionalInfo;
							}

							if(text != "")
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

					// Group symbol
					Rectangle {
						id: rowGroupHeaderButtons
						anchors.right: parent.right
						anchors.top: parent.top
						anchors.topMargin: 0
						anchors.rightMargin: 10
						width: 10
						height: width
						radius: 0.5 * width

						color: groupColor
						visible: group != alternativesGroups.stringNoAlternativesGroup()

						ToolTip.delay: 1000
						ToolTip.timeout: 5000
						ToolTip.visible: hovered && group != alternativesGroups.stringNoAlternativesGroup()
						ToolTip.text: group
					}

					// Extended information for active ingredient elements
					GridLayout {
						id: listItemGridRecipeItem
						visible: listItemRecipeItem.highlighted

						anchors.left: parent.left
						anchors.right: parent.right
						anchors.top: listItemRecipeItemName.bottom
						anchors.leftMargin: 10
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
								model: unitNames
								currentIndex: amountUnit
								onActivated: amountUnit = currentIndex
							}

							CheckBox {
								visible: amountUnit != modelShoppingListItems.indexUnitUnitless()

								text: qsTr("Range")
								checked: amountIsRange

								onClicked: amountIsRange = checked
							}
						}

						Label { 
							visible: amountUnit != modelShoppingListItems.indexUnitUnitless()
							text: " "
						}
						RowLayout {
							visible: amountUnit != modelShoppingListItems.indexUnitUnitless()
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
							to: sizeNames.length - 1
                            
							textFromValue: function(value, locale) {
								return sizeNames[value];
							}
							valueFromText: function(text, locale) {
								return sizeNames.indexOf(text)
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
		}
	}

	Pane {
		id: paneCurrentRecipe
		anchors.left: scrollViewCurrentRecipe.left
		anchors.right: scrollViewCurrentRecipe.right
		anchors.bottom: parent.bottom
		anchors.topMargin: 48

		RowLayout {
			anchors.fill: parent
        
			RoundButton {
				enabled: lvRecipes.count > 0 && lvRecipes.currentIndex != -1

				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/list-black.svg"

				onClicked: {
					dlgEditShoppingListItemsList.editListModel = modelShoppingListItems;
					dlgEditShoppingListItemsList.allValuesFilterModel = filterModelIngredients;
					dlgEditShoppingListItemsList.initialyHighlightedIndex = modelIngredients.indexOfIngredient(modelShoppingListItems.name(lvCurrentRecipe.currentIndex))
					
					dlgEditShoppingListItemsList.open();
				}
			}

			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/remove.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Remove ingredient")

				enabled: lvCurrentRecipe.count > 0 && lvCurrentRecipe.currentIndex != -1
				onClicked: {
					dlgRemoveIngredient.msgText = qsTr("This will remove the ingredient \"" + modelShoppingListItems.name(lvCurrentRecipe.currentIndex) + "\". Proceed?");
					dlgRemoveIngredient.open();
				}
			}
		}
	}
}
