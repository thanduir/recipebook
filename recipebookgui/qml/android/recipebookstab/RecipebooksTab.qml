import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "components"

// TODO: ANPASSEN!
Item {
	TextInputDialog {
		id: dlgAddConfig
		title: qsTr("Add recipe book configuration")
		onCurrentTextChanged: currentTextAllowed = !modelRecipeBookConfigurations.existsConfiguration(outputText)
		onAccepted: {
			bar.currentIndex = modelRecipeBookConfigurations.addConfiguration(outputText)
			modelRBConfigItems.setCurrentConfig(bar.currentIndex)
		}
	}

	TextInputDialog {
		id: dlgRenameConfig
		title: qsTr("Rename recipe book configuration")
		onCurrentTextChanged: currentTextAllowed = !modelSortOrders.existsSortOrder(outputText)
		onAccepted: {
			bar.currentIndex = modelRecipeBookConfigurations.renameConfiguration(bar.currentIndex, outputText)
			modelRBConfigItems.setCurrentConfig(bar.currentIndex)
		}
	}
    
	TextMessageDialog {
		id: dlgRemoveConfig
		title: qsTr("Remove recipe book configuration")
		onAccepted: {
			modelRecipeBookConfigurations.removeConfiguration(bar.currentIndex)
			bar.incrementCurrentIndex()
			bar.decrementCurrentIndex()
			modelRBConfigItems.setCurrentConfig(bar.currentIndex)
		}
	}

	TabBar {
		id: bar
		width: parent.height - groupConfigurations.height
		transform: [
			Rotation { origin.x: 0; origin.y: 0; angle: -90},
			Translate { y: parent.height / 2 - groupConfigurations.height / 2 + bar.width / 2; x: 0 }
		]
        
		Repeater {
			id: tabBtnRepeater

			model: modelRecipeBookConfigurations

			TabButton {
				font.capitalization: Font.MixedCase
				text: name

				background: Rectangle {
					color: bar.currentIndex == index ? "lightgray" : "transparent"
				}

				onClicked: {
					modelRBConfigItems.setCurrentConfig(index)
				}
			}
		}
	}

	Pane {
		id: groupConfigurations
		anchors.left: parent.left
		anchors.bottom: parent.bottom
		anchors.leftMargin: -16
		anchors.bottomMargin: -16

		ColumnLayout {
			anchors.centerIn: parent
			spacing: 0

			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add recipe book configuration")

				enabled: modelRecipeBookConfigurations.canConfigurationBeAdded()
				onClicked: dlgAddConfig.open()
			}
			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/edit.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Rename recipe book configuration")

				enabled: bar.currentIndex != -1
				onClicked: {
					dlgRenameConfig.initialText = modelRecipeBookConfigurations.name(bar.currentIndex);
					dlgRenameConfig.open();
				}
			}
			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/remove.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Remove recipe book configuration")

				enabled: bar.count > 0
				onClicked: {
					dlgRemoveConfig.msgText = qsTr("This will remove the recipe book configuration \"%1\". Proceed?").arg(modelRecipeBookConfigurations.name(bar.currentIndex));
					dlgRemoveConfig.open();
				}
			}
		}
	}

	RbConfigPage {
		anchors.fill: parent        
		anchors.leftMargin: bar.height

		currentConfig: bar.currentIndex
	}
}
