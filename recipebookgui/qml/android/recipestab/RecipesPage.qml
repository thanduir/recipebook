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
		onCurrentTextChanged: currentTextAllowed = !modelRecipes.existsRecipe(outputText)
		onAccepted: {
			var recipeIndex = modelRecipes.addRecipe(outputText)
			modelRecipeItems.setRecipe(recipeIndex);
			recipeItems.currentRecipe = recipeIndex;
			recipeDetails.currentRecipe = recipeIndex;
			btnChangeCurrentRecipe.text = modelRecipes.name(recipeIndex)
		}
	}

	TextInputDialog {
		id: dlgCopyRecipe
		title: qsTr("Copy recipe")
		onCurrentTextChanged: currentTextAllowed = !modelRecipes.existsRecipe(outputText)
		onAccepted: {
			var recipeIndex = modelRecipes.copyRecipe(recipeItems.currentRecipe, outputText);
			modelRecipeItems.setRecipe(recipeIndex);
			recipeItems.currentRecipe = recipeIndex;
			recipeDetails.currentRecipe = recipeIndex;
			btnChangeCurrentRecipe.text = modelRecipes.name(recipeIndex)
		}
	}

	TextInputDialog {
		id: dlgRenameRecipe
		title: qsTr("Rename recipe")
		onCurrentTextChanged: currentTextAllowed = !modelRecipes.existsRecipe(outputText)
		onAccepted: {
			var recipeIndex = modelRecipes.renameRecipe(recipeItems.currentRecipe, outputText);
			modelRecipeItems.setRecipe(recipeIndex);
			recipeItems.currentRecipe = recipeIndex;
			recipeDetails.currentRecipe = recipeIndex;
			btnChangeCurrentRecipe.text = modelRecipes.name(recipeIndex)
		}
	}

	// Recipe item dialog

	EditItemsListDialog {
		id: dlgEditRecipeItemsList
		useScrollBars: false
		title: qsTr("Edit ingredients list")

		onListChanged: {
			modelRecipeItems.modelReset();
		}
	}

	// Header Component

	onVisibleChanged: {
		if(visible)
		{
			headerSubpageSpace.sourceComponent = recipesHeaderComponent;
		}
	}

	Component {
		id: recipesHeaderComponent

		RowLayout {
			anchors.right: parent.right
			anchors.rightMargin: 10

			ToolButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				enabled: filterModelRecipes.canRecipesBeAdded()
				onClicked: dlgAddRecipe.open()
			}

			ToolButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/copy-black.svg"

				enabled: recipeItems.currentRecipe != -1
				onClicked: {
					dlgCopyRecipe.initialText = modelRecipes.name(recipeItems.currentRecipe);
					dlgCopyRecipe.open();
				}
			}
		}
	}

	// Recipes list

	Dialog {
		id: popupRecipes

		width: parent.width - 20
		height: parent.height - 20
		x: (parent.width - width) / 2
		y: (parent.height - height) / 2

		onAboutToShow: {
			lvRecipes.currentIndex = -1
			textFilterRecipes.text = ""
			filterModelRecipes.setFilterString("");
		}

		TextField {
			id: textFilterRecipes
			anchors.left: lvRecipes.left
			anchors.right: lvRecipes.right
			anchors.top: parent.top
			selectByMouse: true
			placeholderText: qsTr("Filter recipes")

			onTextEdited: {
				filterModelRecipes.setFilterString(text);
				lvRecipes.currentIndex = -1
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
						textFilterRecipes.forceActiveFocus()
					}
				}
			}
		}

		ListView {
			id: lvRecipes
			anchors.left: parent.left
			anchors.right: parent.right
			anchors.top: textFilterRecipes.bottom
			anchors.bottom: parent.bottom
			anchors.topMargin: 24
			anchors.bottomMargin: 24

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

			model: filterModelRecipes
			delegate: SwipeDelegate {
				id: listItemRecipe

				highlighted: ListView.isCurrentItem
				onClicked: {
					forceActiveFocus();
					lvRecipes.currentIndex = index;
					var recipeIndex = filterModelRecipes.getRecipeIndex(index);
					modelRecipeItems.setRecipe(recipeIndex)
					recipeItems.currentRecipe = recipeIndex;
					recipeDetails.currentRecipe = recipeIndex;
					editIngredientsListButton.enabled = modelRecipeItems.canRecipeItemsBeAdded();
					btnChangeCurrentRecipe.text = modelRecipes.name(recipeIndex)
					popupRecipes.close();
				}

				width: lvRecipes.width - lvRecipes.leftMargin - lvRecipes.rightMargin

				text: isEverythingSet ? name : name + " (*)"

				Timer {
					id: undoTimer
					interval: 2750
					onTriggered: {
						filterModelRecipes.removeRecipe(index);
						if(lvRecipes.currentIndex == index)
						{
							lvRecipes.incrementCurrentIndex();
							lvRecipes.decrementCurrentIndex();
							modelRecipeItems.setRecipe(-1);
							recipeItems.currentRecipe = -1;
							recipeDetails.currentRecipe = -1;

							var recipeIndex = filterModelRecipes.getRecipeIndex(lvRecipes.currentIndex);
							modelRecipeItems.setRecipe(recipeIndex);
							recipeItems.currentRecipe = recipeIndex;
							recipeDetails.currentRecipe = recipeIndex;
							btnChangeCurrentRecipe.text = modelRecipes.name(recipeIndex)
						}
					}
				}

				swipe.onCompleted: {
					undoTimer.start()
					listItemRecipe.contentItem.visible = false
					listItemRecipe.background.visible = false
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

						visible: !listItemRecipe.swipe.complete

						fillMode: Image.PreserveAspectFit
						source: "qrc:/images/remove.svg"
					}
					Label {
						anchors.left: parent.left
						anchors.leftMargin: 10
						anchors.verticalCenter: parent.verticalCenter
						text: qsTr("Recipe \"%1\" removed").arg(name)

						visible: listItemRecipe.swipe.complete
					}
					RoundButton {
						text: qsTr("Undo")

						anchors.right: parent.right
						anchors.verticalCenter: parent.verticalCenter

						SwipeDelegate.onClicked: {
							undoTimer.stop()
							listItemRecipe.swipe.close()
							listItemRecipe.contentItem.visible = true
							listItemRecipe.background.visible = true
						}

						visible: listItemRecipe.swipe.complete
					}
				}
			}

			Component.onCompleted: {
				if(lvRecipes.count > 0) {
					lvRecipes.currentIndex = 0;
					modelRecipeItems.setRecipe(0);
					recipeItems.currentRecipe = 0;
					recipeDetails.currentRecipe = 0;
					editIngredientsListButton.enabled = modelRecipeItems.canRecipeItemsBeAdded();
				}
			}

			Connections {
				target: modelRecipes
				function onModelReset() {
					if(modelRecipes.rowCount() === 0) {
						lvRecipes.currentIndex = -1;
					}
					var recipeIndex = filterModelRecipes.getRecipeIndex(lvRecipes.currentIndex);
					modelRecipeItems.setRecipe(recipeIndex);
					recipeItems.currentRecipe = recipeIndex;
					recipeDetails.currentRecipe = recipeIndex;
					editIngredientsListButton.enabled = modelRecipeItems.canRecipeItemsBeAdded()
				}
			}
		}

		footer: DialogButtonBox {
			Button {
				text: qsTr("Close")
				DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
				flat: true
			}
		}
	}

	RowLayout {
		id: rowCurrentRecipe

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top

		RoundButton {
			id: editIngredientsListButton
			display: AbstractButton.IconOnly
			icon.source: "qrc:/images/list-black.svg"

			onVisibleChanged: {
				if(visible)
				{
					if(recipeItems.currentRecipe === -1 && lvRecipes.count > 0)
					{
						modelRecipeItems.setRecipe(0);
						recipeItems.currentRecipe = 0;
						recipeDetails.currentRecipe = 0;
					}
					enabled = modelRecipeItems.canRecipeItemsBeAdded()
				}
			}

			onClicked: {
				dlgEditRecipeItemsList.editListModel = modelRecipeItems;
				dlgEditRecipeItemsList.allValuesFilterModel = filterModelIngredients;
				dlgEditRecipeItemsList.initialyHighlightedIndex = -1

				dlgEditRecipeItemsList.open();
			}
		}

		Button {
			id: btnChangeCurrentRecipe

			Layout.fillWidth: true

			enabled: modelRecipes.rowCount() > 0

			text: modelRecipes.name(recipeItems.currentRecipe)
			font.bold: true
			font.capitalization: Font.Capitalize

			onClicked: popupRecipes.open()

			onPressAndHold: {
				if(recipeItems.currentRecipe != -1)
				{
					dlgRenameRecipe.initialText = modelRecipes.name(recipeItems.currentRecipe);
					dlgRenameRecipe.open();
				}
			}
		}
	}

	SwipeView {
		id: swipeView

		currentIndex: 0
		anchors.top: rowCurrentRecipe.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: indicator.top

		RecipeItemsPane {
			id: recipeItems
		}
		RecipeDetailsPane {
			id: recipeDetails
		}
	}

	PageIndicator {
		id: indicator

		anchors.bottom: parent.bottom

		count: swipeView.count
		currentIndex: swipeView.currentIndex

		anchors.horizontalCenter: parent.horizontalCenter
	}
}
