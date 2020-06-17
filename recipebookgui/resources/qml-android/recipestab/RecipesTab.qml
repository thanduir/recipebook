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

    Component.onCompleted: {
        recipePages.currentIndex = 3;
    }

    function currentTabName(myIndex: int) {
        if(myIndex === 0)
        {
            return qsTr("Sort orders");
        }
        else if(myIndex === 1)
        {
            return qsTr("Alternatives types");
        }
        else if(myIndex === 2)
        {
            return qsTr("Ingredients");
        }
        else if(myIndex === 3)
        {
            return qsTr("Recipes");
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

        CategoriesPage {
            id: categoriesPage
        }
        AlternativesTypesPage {
            id: alternativesTypesPage
        }
        IngredientsPage {
            id: ingredientsPage
        }
        RecipesPage {
			id: recipesPage
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
