import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {
    SwipeView {
        id: recipePages
        currentIndex: pageIndicator.currentIndex
        anchors.fill: parent

		RecipesPage {
			id: recipesPage
		}
		IngredientsPage {
			id: ingredientsPage
		}
		CategoriesPage {
			id: categoriesPage
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
