import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {
    SwipeView {
        id: recipePages
        currentIndex: pageIndicator.currentIndex
        anchors.fill: parent

        ShoppingListPage {
            id: shoppingListPage
        }
        GoShoppingPage {
            id: goShoppingPage
        }
    }

    PageIndicator {
        id: pageIndicator
        interactive: true
        count: recipePages.count
        currentIndex: recipePages.currentIndex

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
