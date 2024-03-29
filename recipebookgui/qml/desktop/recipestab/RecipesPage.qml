import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.14

import "components"

Item {

	// Recipe dialogs

	TextInputDialog {
		id: dlgAddRecipe
		title: qsTr("Add recipe")
		onCurrentTextChanged: currentTextAllowed = !filterModelRecipes.existsRecipe(outputText)
		onAccepted: {
			lvRecipes.currentIndex = filterModelRecipes.addRecipe(outputText)
			lvRecipes.positionViewAtIndex(lvRecipes.currentIndex, ListView.Center)
		}
	}

	TextInputDialog {
		id: dlgCopyRecipe
		title: qsTr("Copy recipe")
		onCurrentTextChanged: currentTextAllowed = !filterModelRecipes.existsRecipe(outputText)
		onAccepted: {
			lvRecipes.currentIndex = filterModelRecipes.copyRecipe(lvRecipes.currentIndex, outputText)
			lvRecipes.positionViewAtIndex(lvRecipes.currentIndex, ListView.Center)
		}
	}

	TextInputDialog {
		id: dlgRenameRecipe
		title: qsTr("Rename recipe")
		onCurrentTextChanged: currentTextAllowed = !filterModelRecipes.existsRecipe(outputText)
		onAccepted: {
			lvRecipes.currentIndex = filterModelRecipes.renameRecipe(lvRecipes.currentIndex, outputText)
			lvRecipes.positionViewAtIndex(lvRecipes.currentIndex, ListView.Center)
		}
	}
    
	TextMessageDialog {
		id: dlgRemoveRecipe
		title: qsTr("Remove recipe")
		onAccepted: {
			filterModelRecipes.removeRecipe(lvRecipes.currentIndex);
			lvRecipes.incrementCurrentIndex();
			lvRecipes.decrementCurrentIndex();
			modelRecipeItems.setRecipe(lvRecipes.currentIndex);
			editIngredientsListButton.enabled = modelRecipeItems.canRecipeItemsBeAdded()
		}
	}

	// Recipe item dialog
    
	EditItemsListDialog {
		id: dlgEditRecipeItemsList
		useScrollBars: true
		title: qsTr("Edit ingredients list")

		onListChanged: {
			lvCurrentRecipe.currentIndex = -1;
			modelRecipeItems.modelReset();
		}
	}

	TextMessageDialog {
		id: dlgRemoveIngredient
		title: qsTr("Remove ingredient")
		onAccepted: {
			modelRecipeItems.removeItem(lvCurrentRecipe.currentIndex)
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

	TextField { 
		id: textFilterRecipes
		anchors.left: lvRecipes.left
		anchors.right: lvRecipes.right
		anchors.top: labelRecipes.bottom
		anchors.topMargin: 24
		selectByMouse: true
		placeholderText: qsTr("Filter recipes")

		onTextEdited: {
			filterModelRecipes.setFilterString(text);
			lvRecipes.currentIndex = -1
			modelRecipeItems.setRecipe(-1)
			forceActiveFocus();
		}

		Image {
			anchors { top: parent.top; right: parent.right }
			id: clearText
			fillMode: Image.PreserveAspectFit
			visible: textFilterRecipes.text
			source: "qrc:/images/backspace.svg"
			height: parent.height - 5

			MouseArea {
				id: clear
				anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
				height: textFilterRecipes.height; width: textFilterRecipes.height
				onClicked: {
					textFilterRecipes.text = ""
					filterModelRecipes.setFilterString(textFilterRecipes.text);
					lvRecipes.currentIndex = -1
					modelRecipeItems.setRecipe(-1)
					textFilterRecipes.forceActiveFocus()
				}
			}
		}
	}

	ListView {
		id: lvRecipes
		anchors.left: parent.left
		anchors.top: textFilterRecipes.bottom 
		anchors.bottom: paneRecipes.top
		anchors.topMargin: 48
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		width: 300

		ScrollBar.vertical: ScrollBar { }
		boundsBehavior: Flickable.StopAtBounds

		spacing: 5
		model: filterModelRecipes
		delegate: ItemDelegate {
			highlighted: ListView.isCurrentItem
			onClicked: {
				forceActiveFocus()
				lvRecipes.currentIndex = index
				modelRecipeItems.setRecipe(filterModelRecipes.getRecipeIndex(index))
				editIngredientsListButton.enabled = modelRecipeItems.canRecipeItemsBeAdded()
				lvCurrentRecipe.currentIndex = -1
			}
			width: lvRecipes.width - lvRecipes.leftMargin - lvRecipes.rightMargin
                
			text: isEverythingSet ? name : name + " (*)"
		}

		Component.onCompleted: {
			if(lvRecipes.count > 0) {
				lvRecipes.currentIndex = 0;
				modelRecipeItems.setRecipe(0);
				editIngredientsListButton.enabled = modelRecipeItems.canRecipeItemsBeAdded();
			}
		}

		Connections {
			target: modelRecipes
			function onModelReset() {
				if(modelRecipes.rowCount() == 0) {
					lvRecipes.currentIndex = -1;
				}
				modelRecipeItems.setRecipe(lvRecipes.currentIndex);
				editIngredientsListButton.enabled = modelRecipeItems.canRecipeItemsBeAdded()
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
				id: buttonAdd

				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add recipe")

				enabled: filterModelRecipes.canRecipesBeAdded()
				onClicked: dlgAddRecipe.open()
			}

			RoundButton {
				id: buttonCopy

				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/copy-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Copy recipe")

				enabled: lvRecipes.currentIndex != -1
				onClicked: {
					dlgCopyRecipe.initialText = filterModelRecipes.name(lvRecipes.currentIndex);
					dlgCopyRecipe.open();
				}
			}

			RoundButton {
				id: buttonRename

				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/edit.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Rename recipe")

				enabled: lvRecipes.currentIndex != -1
				onClicked: {
					dlgRenameRecipe.initialText = filterModelRecipes.name(lvRecipes.currentIndex);
					dlgRenameRecipe.open();
				}
			}

			RoundButton {
				id: buttonRemove

				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/remove.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Remove recipe")

				enabled: lvRecipes.currentIndex < lvRecipes.count && lvRecipes.currentIndex >= 0
				onClicked: {
					dlgRemoveRecipe.msgText = qsTr("This will remove the recipe \"%1\". Proceed?").arg(filterModelRecipes.name(lvRecipes.currentIndex));
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

		text: qsTr("Recipe \"%1\"").arg(filterModelRecipes.name(lvRecipes.currentIndex))
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
			from: 1
			to: 50
			editable: true
			wheelEnabled: true
			value: filterModelRecipes.numberOfPersons(lvRecipes.currentIndex)

			onValueModified: filterModelRecipes.setNumberOfPersons(lvRecipes.currentIndex, value)
		}

		Label { 
			Layout.rightMargin: 50
			text: qsTr("Cooking time")
		}
		SpinBox { 
			from: 1
			to: 359
			editable: true
			wheelEnabled: true
			inputMethodHints: Qt.ImhTime
			validator: RegularExpressionValidator { regularExpression: /^([0][0-5]):([0-5][0-9])$/ }
			textFromValue: function(value, locale) {
				var hour = Math.floor(value / 60);
				var min = value % 60;
				var hourStr = hour < 10 ? "0" + hour : hour;
				var minStr = min < 10 ? "0" + min : min;
				return hourStr + ":" + minStr;
			}
			valueFromText: function(text, locale) {
				var cookingTime = new Date()
				cookingTime = Date.fromLocaleString(locale, text, "HH:mm");
				return cookingTime.getHours() * 60 + cookingTime.getMinutes();
			}
			value: filterModelRecipes.cookingTime(lvRecipes.currentIndex)
                        
			onValueModified: filterModelRecipes.setCookingTime(lvRecipes.currentIndex, value)

			ToolTip.delay: 1000
			ToolTip.timeout: 5000
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Format: \"HH:mm\"")
		}
	}

	TextField { 
		id: textShortDesc
		anchors.top: grid.bottom
		anchors.left: lvRecipes.right
		anchors.right: grid.right
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		anchors.rightMargin: 24

		visible: lvRecipes.count > 0 && lvRecipes.currentIndex != -1

		selectByMouse: true

		ToolTip.delay: 1000
		ToolTip.timeout: 3000
		ToolTip.visible: hovered
		ToolTip.text: qsTr("Short description")
		placeholderText: qsTr("Recipe short description")

		text: filterModelRecipes.shortDescription(lvRecipes.currentIndex)
		onEditingFinished: filterModelRecipes.setShortDescription(lvRecipes.currentIndex, text)
	}

	ScrollView {
		anchors.top: textShortDesc.bottom
		anchors.left: lvRecipes.right
		anchors.right: grid.right
		anchors.bottom: paneRecipes.bottom
		anchors.leftMargin: 48
		anchors.bottomMargin: 12
		anchors.rightMargin: 24

		width: 375

		visible: lvRecipes.count > 0 && lvRecipes.currentIndex != -1
        
		TextArea { 
			wrapMode: TextEdit.Wrap
			selectByMouse: true

			ToolTip.delay: 1000
			ToolTip.timeout: 3000
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Recipe text")
			placeholderText: qsTr("Recipe text")

			text: filterModelRecipes.recipeText(lvRecipes.currentIndex)
			onEditingFinished: filterModelRecipes.setRecipeText(lvRecipes.currentIndex, text)
		}
	}

	Switch {
		id: idRearrangeCurrentItems
		anchors.right: lvCurrentRecipe.right
		anchors.top: parent.top
		anchors.topMargin: 24

		visible: lvRecipes.currentIndex != -1

		text: qsTr("Rearrange recipe items")
	}

	// Edit recipe item list view
	ListView {
		id: lvCurrentRecipe
		anchors.left: grid.right
		anchors.top: idRearrangeCurrentItems.bottom
		anchors.bottom: paneCurrentRecipe.top
		anchors.topMargin: 24
		anchors.leftMargin: 24
		anchors.bottomMargin: 48
		width: 400
		visible: !idRearrangeCurrentItems.checked

		ScrollBar.vertical: ScrollBar { }
		boundsBehavior: Flickable.StopAtBounds
		keyNavigationEnabled: false
            
		spacing: 0
		model: modelRecipeItems
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

					ToolTip.delay: 1000
					ToolTip.timeout: 5000
					ToolTip.visible: hasGroup && hovered
					ToolTip.text: group
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
						if(amountUnit != modelRecipeItems.indexUnitUnitless())
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
							if(text != "")
							{
								text = text + ", ";
							}
							text += uiStrings.getSizeName(sizeIndex, amountUnit)
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
							visible: amountUnit != modelRecipeItems.indexUnitUnitless()

							text: qsTr("Range")
							checked: amountIsRange

							onClicked: amountIsRange = checked
						}
					}

					Label { 
						visible: amountUnit != modelRecipeItems.indexUnitUnitless()
						text: " "
					}
					RowLayout {
						visible: amountUnit != modelRecipeItems.indexUnitUnitless()
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

						ToolTip.delay: 1000
						ToolTip.timeout: 5000
						ToolTip.visible: hovered
						ToolTip.text: qsTr("Alternatives group")                        

						onVisibleChanged: {
							if(visible)
								currentIndex = indexOfValue(group)
						}
					}
				}
			}
		}
	}

	// Rearrange recipe item list moveable delegate
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
		anchors.left: grid.right
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
				id: editIngredientsListButton
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/list-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Edit ingredients list")

				onVisibleChanged: {
					if(visible)
					{
						enabled = modelRecipeItems.canRecipeItemsBeAdded()
					}
				}

				onClicked: {
					dlgEditRecipeItemsList.editListModel = modelRecipeItems;
					dlgEditRecipeItemsList.allValuesFilterModel = filterModelIngredients;
					if(lvCurrentRecipe.visible)
						dlgEditRecipeItemsList.initialyHighlightedIndex = modelIngredients.indexOfIngredient(modelRecipeItems.name(lvCurrentRecipe.currentIndex))
					else
						dlgEditRecipeItemsList.initialyHighlightedIndex = -1

					dlgEditRecipeItemsList.open();
				}
			}

			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/remove.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Remove ingredient")

				enabled: lvCurrentRecipe.visible && lvCurrentRecipe.count > 0 && lvCurrentRecipe.currentIndex != -1
				onClicked: {
					dlgRemoveIngredient.msgText = qsTr("This will remove the ingredient \"%1\". Proceed?").arg(modelRecipeItems.name(lvCurrentRecipe.currentIndex));
					dlgRemoveIngredient.open();
				}
			}
		}
	}
}
