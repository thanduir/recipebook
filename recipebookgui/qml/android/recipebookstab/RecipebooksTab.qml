import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "components"

Item {
	TextInputDialog {
		id: dlgAddConfig
		title: qsTr("Add recipe book configuration")
		onCurrentTextChanged: currentTextAllowed = !modelRecipeBookConfigurations.existsConfiguration(outputText)
		onAccepted: {
			cbxCurrentConfiguration.currentIndex = modelRecipeBookConfigurations.addConfiguration(outputText)
			modelRBConfigItems.setCurrentConfig(cbxCurrentConfiguration.currentIndex)
		}
	}

	TextInputDialog {
		id: dlgRenameConfig
		title: qsTr("Rename recipe book configuration")
		onCurrentTextChanged: currentTextAllowed = !modelSortOrders.existsSortOrder(outputText)
		onAccepted: {
			cbxCurrentConfiguration.currentIndex = modelRecipeBookConfigurations.renameConfiguration(cbxCurrentConfiguration.currentIndex, outputText)
			modelRBConfigItems.setCurrentConfig(cbxCurrentConfiguration.currentIndex)
		}
	}
    
	TextMessageDialog {
		id: dlgRemoveConfig
		title: qsTr("Remove recipe book configuration")
		onAccepted: {
			modelRecipeBookConfigurations.removeConfiguration(cbxCurrentConfiguration.currentIndex)
			cbxCurrentConfiguration.incrementCurrentIndex()
			cbxCurrentConfiguration.decrementCurrentIndex()
			modelRBConfigItems.setCurrentConfig(cbxCurrentConfiguration.currentIndex)
		}
	}

	RowLayout {
		id: rowCurrentConfiguration

		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top

		ComboBox {
			id: cbxCurrentConfiguration
			Layout.fillWidth: true

			model: modelRecipeBookConfigurations
			textRole: "name"
			onActivated: modelRBConfigItems.setCurrentConfig(index)

			Component.onCompleted: {
				if(cbxCurrentConfiguration.count > 0) {
					cbxCurrentConfiguration.currentIndex = 0;
					modelRBConfigItems.setCurrentConfig(0);
				}
			}
		}

		RoundButton {
			id: btnAddConfiguration

			display: AbstractButton.IconOnly
			icon.source: "qrc:/images/more-vert.svg"

			onClicked: menuConfigurations.open()

			Menu {
				id: menuConfigurations
				y: btnAddConfiguration.y

				MenuItem {
					text: qsTr("Add recipe book configuration")
					enabled: modelRecipeBookConfigurations.canConfigurationBeAdded()
					onClicked: {
						dlgAddConfig.open()
						highlighted = false
					}
				}

				MenuItem {
					text: qsTr("Rename recipe book configuration")
					enabled: cbxCurrentConfiguration.currentIndex !== -1
					onClicked: {
						dlgRenameConfig.initialText = modelRecipeBookConfigurations.name(cbxCurrentConfiguration.currentIndex);
						dlgRenameConfig.open();
						highlighted = false
					}
				}

				MenuItem {
					text: qsTr("Remove recipe book configuration")
					enabled: cbxCurrentConfiguration.currentIndex !== -1
					onClicked: {
						dlgRemoveConfig.msgText = qsTr("This will remove the recipe book configuration \"%1\". Proceed?").arg(modelRecipeBookConfigurations.name(cbxCurrentConfiguration.currentIndex));
						dlgRemoveConfig.open();
						highlighted = false
					}
				}
			}
		}
	}

	SwipeView {
		id: swipeView

		currentIndex: 0
		anchors.top: rowCurrentConfiguration.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: indicator.top

		RbConfigContentPage {
			id: configContent

			currentConfig: cbxCurrentConfiguration.currentIndex
		}
		RbConfigDetailsPage {
			id: configDetails

			currentConfig: cbxCurrentConfiguration.currentIndex
		}
	}

	PageIndicator {
		id: indicator

		anchors.bottom: parent.bottom

		count: swipeView.count
		currentIndex: swipeView.currentIndex

		anchors.horizontalCenter: parent.horizontalCenter
	}
}
