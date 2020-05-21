import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

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
			modelShoppingListItems.setShoppingRecipe(lvRecipes.currentIndex);
		}
	}

	TextMessageDialog {
		id: dlgClearShoppingList
		title: qsTr("Clear shopping list")
		onAccepted: {
			modelShoppingRecipes.resetShoppingList()
			lvRecipes.currentIndex = -1
			modelShoppingListItems.setShoppingRecipe(-1);
		}
	}

	// Recipe dialogs

	DateSelectionDialog {
		id: dlgDateSelection
		title: qsTr("Select date")
		onAccepted: {
			modelShoppingRecipes.setDueDate(lvRecipes.currentIndex, selectedDate)
			var oldIndex = lvRecipes.currentIndex
			lvRecipes.currentIndex = -1
			lvRecipes.currentIndex = oldIndex
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
		anchors.right: lvRecipes.right

		display: AbstractButton.IconOnly
		icon.source: "qrc:/images/clear-black.svg"

		ToolTip.delay: 1000
		ToolTip.timeout: 5000
		ToolTip.visible: hovered
		ToolTip.text: qsTr("Clear all shopping recipe")

		enabled: lvRecipes.currentIndex != -1
		onClicked: {
			dlgClearShoppingList.msgText = qsTr("This will remove all recipes from the list. Proceed?");
			dlgClearShoppingList.open();
		}
	}

	ListView {
		id: lvRecipes
		anchors.left: parent.left
		anchors.top: labelRecipes.bottom 
		anchors.bottom: paneRecipes.top
		anchors.topMargin: 48
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		width: 300

		ScrollBar.vertical: ScrollBar { }
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
 			height: listItemRecipesName.height + 30

			Label {
				id: listItemRecipesName
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.top: parent.top
				anchors.topMargin: 15
				anchors.leftMargin: 15
                        
				text: name
				verticalAlignment: Text.AlignVCenter

				color: everythingSet ? "black" : "red"
				font.bold: !everythingSet
			}
		}

		Component.onCompleted: {
			if(lvRecipes.count > 0) {
				lvRecipes.currentIndex = 0;
				modelShoppingListItems.setShoppingRecipe(0);
			}
		}

		Connections {
			target: modelShoppingRecipes
			onModelReset: {
				if(modelShoppingRecipes.rowCount() <= lvRecipes.currentIndex)
				{
					lvRecipes.currentIndex = -1;
					buttonAddFromRecipe.enabled = modelShoppingRecipes.canShoppingRecipesBeAddedFromRecipes()
				}
				else
				{
					modelShoppingListItems.setShoppingRecipe(-1);
				}
				modelShoppingListItems.setShoppingRecipe(lvRecipes.currentIndex);
			}
		}
	}

	Pane {
		id: paneRecipes
		anchors.bottom: parent.bottom
		anchors.left: lvRecipes.left
		anchors.right: lvRecipes.right
		anchors.leftMargin: 10
		anchors.rightMargin: 10

		RowLayout {
			anchors.centerIn: parent
			spacing: 20
        
			RoundButton {
				id: buttonAddFromRecipe
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add from recipe")

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

			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"
				icon.color: "blue"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add new shopping recipe")

				onVisibleChanged: {
					if(visible)
					{
						enabled = modelShoppingRecipes.canNewShoppingRecipesBeAdded()
					}
				}
				onClicked: dlgAddNewRecipe.open()
			}

			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/edit.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Rename shopping recipe")

				enabled: lvRecipes.currentIndex < lvRecipes.count && lvRecipes.currentIndex >= 0
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
		anchors.left: lvRecipes.right
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

			RoundButton {
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
			
			Label {
				Layout.leftMargin: 10
				Layout.fillWidth: true
				rightPadding: 10
				text: modelShoppingRecipes.dueDate(lvRecipes.currentIndex).toLocaleDateString(Qt.locale(), "ddd, dd.MM.yyyy")
			}

			Item {
				height: parent.height
				Layout.fillWidth: true
				Layout.rightMargin: 20

				RoundButton {
					anchors.centerIn: parent
					display: AbstractButton.IconOnly
					icon.source: "qrc:/images/cancel-black.svg"
					height: 40
					width: height				
					padding: 10

					visible: modelShoppingRecipes.isDueDateSet(lvRecipes.currentIndex)

					onClicked: {
						modelShoppingRecipes.resetDueDate(lvRecipes.currentIndex)
						var oldIndex = lvRecipes.currentIndex
						lvRecipes.currentIndex = -1
						lvRecipes.currentIndex = oldIndex
					}
				}
			}
		}
	}

	// Edit recipe item list view
	ListView {
		id: lvCurrentRecipe
		anchors.left: grid.right
		anchors.top: labelCurrentRecipe.bottom
		anchors.bottom: paneCurrentRecipe.top
		anchors.topMargin: 24
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		width: 400

		ScrollBar.vertical: ScrollBar { }
		boundsBehavior: Flickable.StopAtBounds

		keyNavigationEnabled: false
            
		spacing: 0
		model: modelShoppingListItems
		delegate: ItemDelegate {
			id: listItemRecipeItem
			highlighted: ListView.isCurrentItem 
			onPressed: lvCurrentRecipe.currentIndex == index || !listItemRecipeItemName.checked ? lvCurrentRecipe.currentIndex = -1 : lvCurrentRecipe.currentIndex = index
			width: lvCurrentRecipe.width - lvCurrentRecipe.leftMargin - lvCurrentRecipe.rightMargin
			height: listItemRecipeItemGroup.height

			Item {
				id: listItemRecipeItemGroup
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.top: parent.top

				height: listItemRecipeItemName.height + (highlighted ? listItemGridRecipeItem.height : 0)

				Rectangle {
					id: groupBar
					anchors.left: parent.left
					anchors.top: parent.top
					anchors.bottom: parent.bottom

					visible: hasGroup
					color: groupColor
					width: 5

					ToolTip.delay: 1000
					ToolTip.timeout: 5000
					ToolTip.visible: hasGroup && hovered
					ToolTip.text: group
				}

				// Ingredient / group name
				CheckBox {
					id: listItemRecipeItemName
					anchors.top: parent.top
					anchors.left: hasGroup ? groupBar.right : parent.left
					anchors.topMargin: 0
					anchors.leftMargin: 10
                        
					font.bold: checked
					font.italic: optional
					text: name

					checked: itemEnabled
					onClicked: {
						itemEnabled = checked
							
						if(!checked && lvCurrentRecipe.currentIndex == index)
						{
							lvCurrentRecipe.currentIndex = -1;
						}
					}
				}

				// Summary for inactive ingredients
				Label {
					function recipeItemSmallDesc(n){
						var text = "";
						if(amountUnit != modelShoppingListItems.indexUnitUnitless())
						{
							text += roundValue(amountMin);
							if(amountIsRange)
							{
								text += "-" + roundValue(amountMax);
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
				GridLayout {
					id: listItemGridRecipeItem
					visible: listItemRecipeItem.highlighted

					anchors.left: parent.left
					anchors.right: parent.right
					anchors.top: listItemRecipeItemName.bottom
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
								amountMax = roundValue(text)
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

	Pane {
		id: paneCurrentRecipe
		anchors.left: lvCurrentRecipe.left
		anchors.right: lvCurrentRecipe.right
		anchors.bottom: parent.bottom
		anchors.topMargin: 48

		visible: lvRecipes.currentIndex != -1

		RowLayout {
			anchors.centerIn: parent
			spacing: 20
        
			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/list-black.svg"

				onVisibleChanged: {
					if(visible)
					{
						enabled = modelShoppingListItems.canItemsBeAdded()
					}
				}
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
