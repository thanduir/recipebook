import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {
	property int currentRecipe : -1

	RowLayout {
		id: grid
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.leftMargin: 10
		anchors.rightMargin: 10

		visible: currentRecipe != -1

		Label {
			text: qsTr("Cooking time")
		}
		Item {
			Layout.fillWidth: true
		}
		SpinBox {
			from: 1
			to: 359
			editable: true
			wheelEnabled: true
			inputMethodHints: Qt.ImhTime
			validator: RegularExpressionValidator { regularExpression: /^([0][0-5]):([0-5][0-9])$/ }
			textFromValue: function(value, locale) {
				var hour = Math.floor(value / 60);
				var min = value % 60;
				var hourStr = hour < 10 ? "0" + hour : hour;
				var minStr = min < 10 ? "0" + min : min;
				return hourStr + ":" + minStr;
			}
			valueFromText: function(text, locale) {
				var cookingTime = new Date()
				cookingTime = Date.fromLocaleString(locale, text, "HH:mm");
				return cookingTime.getHours() * 60 + cookingTime.getMinutes();
			}
			value: modelRecipes.cookingTime(currentRecipe)

			onValueModified: modelRecipes.setCookingTime(currentRecipe, value)
		}
	}

	TextField {
		id: textShortDesc
		anchors.top: grid.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.leftMargin: 10
		anchors.rightMargin: 10

		visible: currentRecipe !== -1

		selectByMouse: true

		placeholderText: qsTr("Recipe short description")

		text: modelRecipes.shortDescription(currentRecipe)
		onEditingFinished: modelRecipes.setShortDescription(currentRecipe, text)
	}

	ScrollView {
		anchors.top: textShortDesc.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.leftMargin: 10
		anchors.rightMargin: 10

		visible: currentRecipe !== -1
        
		contentWidth: -1

		TextArea { 
			wrapMode: TextEdit.Wrap
			selectByMouse: true

			placeholderText: qsTr("Recipe text")

			text: modelRecipes.recipeText(currentRecipe)
			onEditingFinished: modelRecipes.setRecipeText(currentRecipe, text)
		}
	}
}
