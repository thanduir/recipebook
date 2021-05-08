import QtQuick 2.14
import QtQuick.Controls 2.14
import Qt.labs.platform 1.1

Item {
	onVisibleChanged: {
		if(visible)
		{
			modelGoShopping.updateList();

			var activeSortOrderIndex = cbxSortOrder.indexOfValue(recipeBookSettings.getActiveSortOrderGoShopping());
			if(activeSortOrderIndex == -1)
			{
				activeSortOrderIndex = 0;
			}
			cbxSortOrder.currentIndex = activeSortOrderIndex;
			
			var activeListOrdering = recipeBookSettings.getActiveListOrderingGoShopping() == uiStrings.getShoppingListOrderingNameSeparate();
			btnSeparateCollectedItems.checked = activeListOrdering;

			modelGoShopping.setSortOrder(cbxSortOrder.currentText, btnSeparateCollectedItems.checked);
		}
	}

	ComboBox {
		id: cbxSortOrder
        
		anchors.left: lvShoppingList.left
		anchors.right: btnSeparateCollectedItems.left
		anchors.top: parent.top
		anchors.topMargin: 24
		anchors.rightMargin: 10

		model: modelSortOrders
		onActivated: {
			recipeBookSettings.setActiveSortOrderGoShopping(currentText);
			modelGoShopping.setSortOrder(currentText, btnSeparateCollectedItems.checked);
		}
	}

	RoundButton {
		id: btnSeparateCollectedItems
        
		anchors.right: lvShoppingList.right
		anchors.verticalCenter: cbxSortOrder.verticalCenter
		
		height: 50
		width: height

		checkable: true

		display: AbstractButton.IconOnly
		icon.source: "qrc:/images/sort-black.svg"

		ToolTip.delay: 1000
		ToolTip.timeout: 5000
		ToolTip.visible: hovered
		ToolTip.text: qsTr("List collected items separately")

		enabled: cbxSortOrder.count > 0
		onClicked: {
			recipeBookSettings.setActiveListOrderingGoShopping(checked ? uiStrings.getShoppingListOrderingNameSeparate() : uiStrings.getShoppingListOrderingNameCombined());
			modelGoShopping.setSortOrder(cbxSortOrder.currentText, checked);
		}
	}

	ListView {
		id: lvShoppingList
		anchors.left: parent.left
		anchors.top: cbxSortOrder.bottom 
		anchors.bottom: btnExport.visible ? btnExport.top : parent.bottom
		anchors.topMargin: 48
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		width: 400

		ScrollBar.vertical: ScrollBar { }
		boundsBehavior: Flickable.StopAtBounds

		move: Transition {
			NumberAnimation { properties: "x,y"; duration: 600; easing.type: Easing.InCubic }
		}

		displaced: Transition {
			NumberAnimation { properties: "x,y"; duration: 300; easing.type: Easing.InCubic }
		}

		spacing: 5
		model: modelGoShopping
		delegate: ItemDelegate {
			width: lvShoppingList.width - lvShoppingList.leftMargin - lvShoppingList.rightMargin
			highlighted: ListView.isCurrentItem
			height: listItemParent.height

			Item {
				id: listItemParent
				anchors.left: parent.left
				anchors.right: parent.right
				anchors.top: parent.top
				anchors.topMargin: 10

				height: listItemName.height + 20 + (normalItem && itemMultiline ? listItemAdditionalText.height - 20 : 0)

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered && normalItem
				ToolTip.text: itemRecipeInfo

				Label {
					id: listItemName
					anchors.left: parent.left
					anchors.leftMargin: 10
                        
					visible: !normalItem
					font.bold: true
					color: topLevelHeader ? "blue" : (incompatiblesHeader ? "red" : "black")

					text: name
					verticalAlignment: Text.AlignVCenter
				}

				CheckBox {
					id: cbxItemName
					anchors.top: parent.top
					anchors.topMargin: -15
					anchors.left: parent.left
					anchors.leftMargin: 10
                        
					visible: normalItem
					font.strikeout: itemChecked
					font.italic: itemOptional
					text: itemHeader + (itemMultiline ? "" : " <font color=\"gray\">" + itemAdditionalText + "</font>")

					checked: itemChecked
					onClicked: itemChecked = checked
				}

				Label {
					id: listItemAdditionalText
					anchors.top: cbxItemName.bottom
					anchors.topMargin: -10
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.leftMargin: 10

					visible: normalItem && itemMultiline
					font.strikeout: itemChecked
					color: "gray"
					text: itemAdditionalText
				}
			}
		}
	}

	Button {
		id: btnExport
		anchors.bottom: parent.bottom
		anchors.horizontalCenter: lvShoppingList.horizontalCenter

		visible: recipeBookSettings.pdfLatexExporterAvailable()

		font.capitalization: Font.MixedCase

		text: "Export shopping list"

		onClicked: {
			fileDialogExport.folder = "file:///" + recipeBookSettings.lastUsedShoppingListExportFolder()
			fileDialogExport.open()
		}

		FileDialog {
			id: fileDialogExport

			title: qsTr("Export shopping list")
			modality: Qt.WindowModal
			nameFilters: shoppingListExporter.getDlgNameFilters()
			fileMode: FileDialog.SaveFile
			onAccepted: shoppingListExporter.exportShoppingList(file, cbxSortOrder.currentText)
		}
	}
}
