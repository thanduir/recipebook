import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.1

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
			filterModelRecipes.removeRecipe(lvRecipes.currentIndex)
			lvRecipes.incrementCurrentIndex()
			lvRecipes.decrementCurrentIndex()
		}
	}

	// Recipe item dialog
    
	EditItemsListDialog {
		id: dlgEditRecipeItemsList
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

	// Alternatives groups

	EditAlternativesGroupDialog {
		id: editAlternativesGroupDialog
		title: qsTr("Edit alternatives groups")
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
		anchors.left: scrollViewRecipes.left
		anchors.right: scrollViewRecipes.right
		anchors.top: labelRecipes.bottom
		anchors.topMargin: 24
		selectByMouse: true

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

	ScrollView {
		id: scrollViewRecipes
		anchors.left: parent.left
		anchors.top: textFilterRecipes.bottom 
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
			model: filterModelRecipes
			delegate: ItemDelegate {
				highlighted: ListView.isCurrentItem
				onClicked: {
					forceActiveFocus()
					lvRecipes.currentIndex = index
					modelRecipeItems.setRecipe(filterModelRecipes.getRecipeIndex(index))
					lvCurrentRecipe.currentIndex = -1
				}
				width: lvRecipes.width - lvRecipes.leftMargin - lvRecipes.rightMargin
                
				text: isEverythingSet ? name : name + " (*)"
			}

			Component.onCompleted: {
				if(lvRecipes.count > 0) {
					lvRecipes.currentIndex = 0;
					modelRecipeItems.setRecipe(0);
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
				id: buttonAdd

				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add recipe")

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
					dlgRemoveRecipe.msgText = qsTr("This will remove the recipe \"" + filterModelRecipes.name(lvRecipes.currentIndex) + "\". Proceed?");
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

		text: qsTr("Recipe \"" + filterModelRecipes.name(lvRecipes.currentIndex) + "\"")
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
		anchors.left: scrollViewRecipes.right
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

		text: filterModelRecipes.shortDescription(lvRecipes.currentIndex)
		onEditingFinished: filterModelRecipes.setShortDescription(lvRecipes.currentIndex, text)
	}

	ScrollView {
		anchors.top: textShortDesc.bottom
		anchors.left: scrollViewRecipes.right
		anchors.right: grid.right
		anchors.bottom: parent.bottom
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		anchors.rightMargin: 24

		visible: lvRecipes.count > 0 && lvRecipes.currentIndex != -1
        
		TextArea { 
			anchors.top: parent.top
			anchors.left: parent.left
			anchors.bottom: parent.bottom
			width: 375

			wrapMode: TextEdit.Wrap
			selectByMouse: true

			ToolTip.delay: 1000
			ToolTip.timeout: 3000
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Recipe text")

			text: filterModelRecipes.recipeText(lvRecipes.currentIndex)
			onEditingFinished: filterModelRecipes.setRecipeText(lvRecipes.currentIndex, text)
		}
	}

	Switch {
		id: idRearrangeCurrentItems
		anchors.right: scrollViewCurrentRecipe.right
		anchors.top: parent.top
		anchors.topMargin: 24

		text: "Rearrange recipe items"
	}

	// Edit recipe item list view
	ScrollView {
		id: scrollViewCurrentRecipe
		anchors.left: grid.right
		anchors.top: idRearrangeCurrentItems.bottom
		anchors.bottom: paneCurrentRecipe.top
		anchors.topMargin: 24
		anchors.leftMargin: 24
		anchors.bottomMargin: 48
		width: 400
		visible: !idRearrangeCurrentItems.checked

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
							if(amountUnit != modelRecipeItems.indexUnitUnitless())
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
	}

	// Rearrange recipe item list moveable delegate
	Component {
		id: dragDelegate

		MouseArea {
			id: dragArea

			property bool held: false

			anchors { left: parent.left; right: parent.right }
			height: content.height

			drag.target: held ? content : undefined
			drag.axis: Drag.YAxis

			onPressed: {
				held = true
			}
			onReleased: held = false
            
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

					// Item name
					Label {
						id: labelColumn
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: reorderImage.right

						anchors.margins: 15

						verticalAlignment: Text.AlignVCenter
						font.bold: !optional
						font.italic: optional
						text: name
					}
                    
					// Group symbol
					Rectangle {
						id: rowRearrangeGroupHeaderButtons
						anchors.right: parent.right
						anchors.top: parent.top
						anchors.topMargin: -5
						anchors.rightMargin: 10
						width: 10
						height: width
						radius: 0.5 * width

						color: groupColor
						visible: group != alternativesGroups.stringNoAlternativesGroup()
					}
				}
			}

			DropArea {
				anchors { fill: parent; margins: 15 }

				onEntered: {
					modelRecipeItems.moveItem(drag.source.DelegateModel.itemsIndex,
												dragArea.DelegateModel.itemsIndex);
					rearrangeCurrentRecipeDelegateModel.items.move(drag.source.DelegateModel.itemsIndex,
																	dragArea.DelegateModel.itemsIndex);
				}
			}
		}
	}

	// Rearrange recipe item list view
	ScrollView {
		id: scrollViewRearrangeCurrentRecipe
		anchors.left: grid.right
		anchors.top: idRearrangeCurrentItems.bottom
		anchors.bottom: paneCurrentRecipe.top
		anchors.topMargin: 24
		anchors.leftMargin: 24
		anchors.bottomMargin: 48
		width: 400
		visible: idRearrangeCurrentItems.checked

        
		ListView {
			anchors.fill: parent
			topMargin: 5
			leftMargin: 5
			bottomMargin: 5
			rightMargin: 5
			boundsBehavior: Flickable.StopAtBounds

			spacing: 5
			model: DelegateModel {
				id: rearrangeCurrentRecipeDelegateModel

				model: modelRecipeItems
				delegate: dragDelegate
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

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Edit ingredients list")

				onClicked: {
					dlgEditRecipeItemsList.editListModel = modelRecipeItems;
					dlgEditRecipeItemsList.allValuesFilterModel = filterModelIngredients;
					if(scrollViewCurrentRecipe.visible)
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

				enabled: lvCurrentRecipe.count > 0 && lvCurrentRecipe.currentIndex != -1
				onClicked: {
					dlgRemoveIngredient.msgText = qsTr("This will remove the ingredient \"" + modelRecipeItems.name(lvCurrentRecipe.currentIndex) + "\". Proceed?");
					dlgRemoveIngredient.open();
				}
			}

			Button {
				id: buttonEditAlternativesGroup

				text: qsTr("Edit alternatives groups") 
				icon.source: "qrc:/images/edit.svg"

				onClicked: {
					editAlternativesGroupDialog.open();
				}
			}
		}
	}
}