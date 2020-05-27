import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "components"

Item {
    
	TextInputDialog {
		id: dlgAddIngredient
		title: qsTr("Add ingredient")
		onCurrentTextChanged: currentTextAllowed = !filterModelIngredients.existsIngredient(outputText)
		onAccepted: {
			lvIngredients.currentIndex = filterModelIngredients.addIngredient(outputText)
			lvIngredients.positionViewAtIndex(lvIngredients.currentIndex, ListView.Center)
		}
	}

	TextInputDialog {
		id: dlgRenameIngredient
		title: qsTr("Rename ingredient")
		onCurrentTextChanged: currentTextAllowed = !filterModelIngredients.existsIngredient(outputText)
		onAccepted: {
			lvIngredients.currentIndex = filterModelIngredients.renameIngredient(lvIngredients.currentIndex, outputText)
			lvIngredients.positionViewAtIndex(lvIngredients.currentIndex, ListView.Center)
		}
	}
    
	TextMessageDialog {
		id: dlgRemoveIngredient
		title: qsTr("Remove ingredient")
		onAccepted: {
			filterModelIngredients.removeIngredient(lvIngredients.currentIndex)
			lvIngredients.currentIndex = -1
		}
	}

	Label {
		id: labelIngredients
        
		anchors.left: parent.left
		anchors.top: parent.top
		anchors.topMargin: 24
		anchors.leftMargin: 48
        
		text: qsTr("Ingredients")
		font.bold: true
	}

	TextField { 
		id: textFilterIngredients
		anchors.left: lvIngredients.left
		anchors.right: lvIngredients.right
		anchors.top: labelIngredients.bottom
		anchors.topMargin: 24
		selectByMouse: true

		onTextEdited: {
			filterModelIngredients.setFilterString(text);
			lvIngredients.currentIndex = -1
			forceActiveFocus();
		}

		Image {
			anchors { top: parent.top; right: parent.right }
			id: clearText
			fillMode: Image.PreserveAspectFit
			visible: textFilterIngredients.text
			source: "qrc:/images/backspace.svg"
			height: parent.height - 5

			MouseArea {
				id: clear
				anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
				height: textFilterIngredients.height; width: textFilterIngredients.height
				onClicked: {
					textFilterIngredients.text = ""
					filterModelIngredients.setFilterString(textFilterIngredients.text);
					lvIngredients.currentIndex = -1
					textFilterIngredients.forceActiveFocus()
				}
			}
		}
	}

	ListView {
		id: lvIngredients
		anchors.left: parent.left
		anchors.top: textFilterIngredients.bottom 
		anchors.bottom: paneIngredients.top
		anchors.topMargin: 48
		anchors.leftMargin: 48
		anchors.bottomMargin: 48

		width: 400

		boundsBehavior: Flickable.StopAtBounds
		ScrollBar.vertical: ScrollBar { }

		spacing: 5
		model: filterModelIngredients
		delegate: ItemDelegate {
			width: lvIngredients.width - lvIngredients.leftMargin - lvIngredients.rightMargin
			highlighted: ListView.isCurrentItem
			onClicked: lvIngredients.currentIndex = index
                
			Item {
				anchors.fill: parent

				Label {
					id: listItemDelegateName
					anchors.left: parent.left
					anchors.leftMargin: 10
                        
					text: name
					verticalAlignment: Text.AlignVCenter
					height: parent.height
				}

				Label {
					anchors.left: listItemDelegateName.right
					color: "gray"
					text: " (" + category + ", " + defaultUnit + ")"
					verticalAlignment: Text.AlignVCenter
					height: parent.height
					width: parent.width - listItemDelegateName.width - 10
					wrapMode: Label.WordWrap
				}
			}
		}

		Connections {
			target: modelIngredients
			function onModelReset() {
				if(modelIngredients.rowCount() == 0) {
					lvIngredients.currentIndex = -1
				}
			}
		}
	}

	Pane {
		id: paneIngredients
		anchors.bottom: parent.bottom
		anchors.left: lvIngredients.left
		anchors.right: lvIngredients.right
		anchors.leftMargin: 48
		anchors.rightMargin: 48
		anchors.bottomMargin: 10

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
				ToolTip.text: qsTr("Add ingredient")
				
				onVisibleChanged: {
					if(visible)
					{
						enabled = filterModelIngredients.canIngredientsBeAdded();
					}
				}
				onClicked: dlgAddIngredient.open()
			}

			RoundButton {
				id: buttonRename

				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/edit.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Rename ingredient")

				enabled: lvIngredients.currentIndex != -1
				onClicked: {
					dlgRenameIngredient.initialText = filterModelIngredients.name(lvIngredients.currentIndex);
					dlgRenameIngredient.open();
				}
			}

			RoundButton {
				id: buttonRemove

				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/remove.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Remove ingredient")

				enabled: lvIngredients.count > 0 && filterModelIngredients.canIngredientBeRemoved(lvIngredients.currentIndex)
				onClicked: {
					dlgRemoveIngredient.msgText = qsTr("This will remove the ingredient \"" + filterModelIngredients.name(lvIngredients.currentIndex) + "\". Proceed?");
					dlgRemoveIngredient.open();
				}
			}
		}
	}

	Label {
		id: labelCurrentSortOrder
        
		anchors.left: grid.left
		anchors.top: parent.top
		anchors.topMargin: 24
        
		visible: lvIngredients.count > 0 && lvIngredients.currentIndex != -1
		text: qsTr("Ingredient \"" + filterModelIngredients.name(lvIngredients.currentIndex) + "\"")
		font.bold: true
	}

	GridLayout {
		id: grid
		anchors.left: lvIngredients.right
		anchors.top: labelCurrentSortOrder.bottom
		anchors.right: parent.right
		anchors.topMargin: 48
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		anchors.rightMargin: 48
        
		visible: lvIngredients.count > 0 && lvIngredients.currentIndex != -1
		columns: 2

		Label { 
			text: qsTr("Category")
		}
		ComboBox {
			Layout.fillWidth: true
			model: modelCategories
			currentIndex: indexOfValue(filterModelIngredients.category(lvIngredients.currentIndex))
			onActivated: filterModelIngredients.setCategory(lvIngredients.currentIndex, currentText)
		}

		Label { 
			Layout.rightMargin: 50
			Layout.topMargin: 15
			Layout.alignment: Qt.AlignTop
			text: qsTr("Provenance")
		}
		GridView {
			id: lvSortOrders
			Layout.fillWidth: true
			height: 100
			cellWidth: 155
			cellHeight: 40
			flow: GridView.FlowTopToBottom

			ScrollBar.horizontal: ScrollBar { }

			model: modelSortOrders
			delegate: CheckBox {
				id: cbxItemName                        
				text: name

				width: lvSortOrders.cellWidth

				checked: modelIngredients.provenanceAvailable(lvIngredients.currentIndex, name)
				onClicked: modelIngredients.setProvenanceAvailable(lvIngredients.currentIndex, name, checked)
				
			}			
		}

		Label { text: qsTr("Unit") }
		ComboBox {
			id: cbxUnit
			Layout.fillWidth: true
			model: unitNames
			currentIndex: indexOfValue(filterModelIngredients.defaultUnit(lvIngredients.currentIndex))
			onActivated: filterModelIngredients.setDefaultUnit(lvIngredients.currentIndex, currentText)
		}

		Item { height: 10; Layout.columnSpan: 2 }

		Label { 
			Layout.topMargin: 24
			Layout.alignment: Qt.AlignTop | Qt.AlignLeft

			text: qsTr("Used in recipes") 
		}
		GridView {
			Layout.topMargin: 24
			Layout.fillWidth: true
			height: 200

			cellWidth: 275
			cellHeight: 30
			flow: GridView.FlowLeftToRight
			ScrollBar.vertical: ScrollBar { }

			model: filterModelIngredients.listUsedInRecipes(lvIngredients.currentIndex)
			delegate: Label {
				text: modelData
			}
		}

		Label { 
			Layout.topMargin: 24
			Layout.alignment: Qt.AlignTop | Qt.AlignLeft

			text: qsTr("Used in shopping list") 
		}
		GridView {
			Layout.topMargin: 24
			Layout.fillWidth: true
			height: 150

			cellWidth: 275
			cellHeight: 40
			flow: GridView.FlowLeftToRight
			ScrollBar.vertical: ScrollBar { }

			model: filterModelIngredients.listUsedInShoppingRecipes(lvIngredients.currentIndex)
			delegate: Label {
				text: modelData
			}
		}
	}
}
