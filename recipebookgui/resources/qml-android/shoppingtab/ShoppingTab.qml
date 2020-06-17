import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {
    onVisibleChanged: {
        if(visible)
        {
            lblCurrentTabName.text = currentTabName(recipePages.currentIndex);
        }
    }

    function currentTabName(myIndex: int) {
        if(myIndex === 0)
        {
            return qsTr("Shopping list");
        }
        else if(myIndex === 1)
        {
            return qsTr("Go shopping");
        }
        return "";
    }

    SwipeView {
        id: recipePages
        currentIndex: pageIndicator.currentIndex
        anchors.fill: parent

        onCurrentIndexChanged: {
            lblCurrentTabName.text = currentTabName(currentIndex);
        }

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
