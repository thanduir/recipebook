import QtQuick 2.14
import QtQuick.Controls 2.14

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
			
			var activeListOrdering = recipeBookSettings.getActiveListOrderingGoShopping() == shoppingListOrderingNames[1];
			btnSeparateCollectedItems.checked = activeListOrdering;

			modelGoShopping.setSortOrder(cbxSortOrder.currentText, btnSeparateCollectedItems.checked);
		}
	}
	ComboBox {
		id: cbxSortOrder
        
		anchors.left: scrollView.left
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
        
		anchors.right: scrollView.right
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

		onClicked: {
			recipeBookSettings.setActiveListOrderingGoShopping(checked ? shoppingListOrderingNames[1] : shoppingListOrderingNames[0]);
			modelGoShopping.setSortOrder(cbxSortOrder.currentText, checked);
		}
	}

	ScrollView {
		id: scrollView
		anchors.left: parent.left
		anchors.top: cbxSortOrder.bottom 
		anchors.bottom: parent.bottom
		anchors.topMargin: 48
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		width: 400

		ListView {
			id: lvShoppingList
			anchors.fill: parent
			topMargin: 5
			leftMargin: 5
			bottomMargin: 5
			rightMargin: 5
			boundsBehavior: Flickable.StopAtBounds

			spacing: 5
			model: modelGoShopping
			delegate: ItemDelegate {
				width: lvShoppingList.width - lvShoppingList.leftMargin - lvShoppingList.rightMargin
				highlighted: ListView.isCurrentItem
				onClicked: lvShoppingList.currentIndex = index
                
				Item {
					anchors.fill: parent

					Label {
						id: listItemName
						anchors.left: parent.left
						anchors.leftMargin: 10
                        
						visible: !normalItem
						font.bold: true
						color: topLevelHeader ? "blue" : (incompatiblesHeader ? "red" : "black")

						text: name
						verticalAlignment: Text.AlignVCenter
						height: parent.height
					}

					CheckBox {
						id: cbxItemName
						anchors.left: parent.left
						anchors.leftMargin: 10
                        
						visible: normalItem						
						text: name

						checked: itemChecked
						onClicked: itemChecked = checked
					}
				}
			}
		}
	}
}
