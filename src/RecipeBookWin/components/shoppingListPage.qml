import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "components"

Item {

	// Dialogs

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

		enabled: lvRecipes.currentIndex < lvRecipes.count && lvRecipes.currentIndex >= 0
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
					//modelRecipeItems.setRecipe(index)
					//lvCurrentRecipe.currentIndex = -1
				}
				width: lvRecipes.width - lvRecipes.leftMargin - lvRecipes.rightMargin
                
				text: name
			}

			Component.onCompleted: {
				if(lvRecipes.count > 0) {
					lvRecipes.currentIndex = 0;
					//modelRecipeItems.setRecipe(0);
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
				id: buttonAddExisting

				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add from recipe")

				// TODO! (similar to edit recipe items list, but how to make it possible to add the same recipe several times?)
				//onClicked: dlgAddRecipe.open()
			}

			RoundButton {
				id: buttonAddNew

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
				id: buttonRename

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
				id: buttonRemove

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

	// TODO: * Add alternatives groups to ShoppingListItems (for those that come from a recipe)!
	//		 * Also change ShoppingRecipe class to be sorted by name (and remove the move-method)!
	//		 To be changed: ShoppingListItem! SERIALIZATION (incl. UnitTest)! ListModel!

	// TODO: Layout similar to recipesPage. Middle col with duedate and scaling factor
	
	// TODO: Idea: Use empty space in middle column for alternatives groups (combo box with groups present + ListView with checkboxes below to select items to use?)
	//		Or is there a better way to do this? Would it be better to use a button for this that opens a dialog with the checkboxes? 
	//		Or add checkboxes to the list on the right?
}
