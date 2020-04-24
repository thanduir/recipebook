import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {

  TabBar {
        id: bar
        width: (button1.implicitWidth + 50) * 3
        height: button1.height
        transform: [
            Rotation { origin.x: 0; origin.y: 0; angle: -90},
            Translate { y: parent.height / 2 + bar.width / 2; x: 0 }
        ]
        
        TabButton {
            font.capitalization: Font.MixedCase
            text: qsTr("Recipes")

            background: Rectangle {
                color: bar.currentIndex == 0 ? "lightgray" : "transparent"
            }
        }
        TabButton {
            font.capitalization: Font.MixedCase
            text: qsTr("Ingredients")            

            background: Rectangle {
                color: bar.currentIndex == 1 ? "lightgray" : "transparent"
            }
        }
        TabButton {
            id: button1
            font.capitalization: Font.MixedCase
            text: qsTr("Sort orders")

            background: Rectangle {
                color: bar.currentIndex == 2 ? "lightgray" : "transparent"
            }
        }
    }

    StackLayout {
        id: recipePages
        anchors.fill: parent        
        anchors.leftMargin: bar.height
        currentIndex: bar.currentIndex
                
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
}
