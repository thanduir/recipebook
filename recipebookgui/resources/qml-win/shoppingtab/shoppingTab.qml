import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {
	Component.onCompleted: {
		bar.currentIndex = 1
	}

	TabBar {
		id: bar
		width: (button1.implicitWidth + 20) * 3
		height: button1.height
		transform: [
			Rotation { origin.x: 0; origin.y: 0; angle: -90},
			Translate { y: parent.height / 2 + bar.width / 2; x: 0 }
		]
        
		TabButton {
			font.capitalization: Font.MixedCase
			text: qsTr("Go shopping")

			background: Rectangle {
				color: bar.currentIndex == 0 ? "lightgray" : "transparent"
			}
		}
		TabButton {
			id: button1
			font.capitalization: Font.MixedCase
			text: qsTr("Shopping list")

			background: Rectangle {
				color: bar.currentIndex == 1 ? "lightgray" : "transparent"
			}
		}
	}

	StackLayout {
		id: recipePages
		anchors.fill: parent        
		anchors.leftMargin: bar.height
		currentIndex: bar.currentIndex
              		
		GoShoppingPage {
			id: goShoppingPage
		}
		ShoppingListPage {
			id: shoppingListPage
		}
	}
}
