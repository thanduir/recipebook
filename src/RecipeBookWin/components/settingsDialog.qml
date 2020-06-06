import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "components"

Dialog {
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2

	width: 500
	height: parent.height - 100

	TextMessageDialog {
		id: dlgLoadDefaultData
		title: qsTr("Load default data")
		msgText: qsTr("This will replace current data with the default data. Current data will be lost. Proceed?");
		onAccepted: recipeBookSettings.loadDefaultData()
	}

	TextMessageDialog {
		id: dlgResetData
		title: qsTr("Reset all data")
		msgText: qsTr("This will reset all data. Current data will be lost. Proceed?");
		onAccepted: recipeBookSettings.resetAllData()
	}

	Pane {
		id: paneSettings

		anchors.fill: parent

		GroupBox {
			id: groupDefaultValues

			title: qsTr("Default values")

			GridLayout {
				id: grid
				anchors.fill: parent
        
				columns: 2
				columnSpacing: 50
				rowSpacing: 10

				Label { 
					text: qsTr("Default unit")
				}
				ComboBox { 
					id: cbxDefaultUnit
					Layout.fillWidth: true
					model: uiStrings.getAllUnitNames()

					onActivated: recipeBookSettings.setDefaultUnit(currentIndex)
				}

				Label { 
					text: qsTr("Default number of persons")
				}
				SpinBox { 
					id: spinDefaultNrPersons
					from: 1
					to: 50
					editable: true
					wheelEnabled: true
                    
					onValueModified: recipeBookSettings.setDefaultRecipeNrPersons(value)
				}

				Label { 
					text: qsTr("Default category")
				}
				ComboBox { 
					id: cbxDefaulCategory
					Layout.fillWidth: true
					model: modelCategories
 
					onActivated: recipeBookSettings.setDefaultCategory(currentText)
				}

				Label { 
					text: qsTr("Default sort order")
				}
				ComboBox { 
					id: cbxDefaultSortOrder
					Layout.fillWidth: true
					model: modelSortOrders

					onActivated: recipeBookSettings.setDefaultSortOrder(currentText)
				}
			}
		}

		GroupBox {
			id: groupData
			anchors.top: groupDefaultValues.bottom
			anchors.topMargin: 24

			width: groupDefaultValues.width

			title: qsTr("Data")

			ColumnLayout {
				anchors.centerIn: parent

				Button {
					text: qsTr("Load default data")
					Layout.alignment: Qt.AlignHCenter
					onClicked: dlgLoadDefaultData.open()
				}

				Button {
					text: qsTr("Reset all data")
					Layout.alignment: Qt.AlignHCenter
					onClicked: dlgResetData.open()
				}
			}
		}

		RowLayout {
			id: rowUID
			anchors.top: groupData.bottom
			anchors.topMargin: 48

			Label { 
				text: qsTr("Application instance UID: ")
			}
			Label { 
				id: textUID
			}
		}
	}

	onAboutToShow: {
		textUID.text = recipeBookSettings.getApplicationInstanceUID();
		cbxDefaultUnit.currentIndex = recipeBookSettings.getDefaultUnit();
		spinDefaultNrPersons.value = recipeBookSettings.getDefaultRecipeNrPersons();

		cbxDefaulCategory.currentIndex = cbxDefaulCategory.indexOfValue(recipeBookSettings.getDefaultCategory());
		cbxDefaultSortOrder.currentIndex = cbxDefaultSortOrder.indexOfValue(recipeBookSettings.getDefaultSortOrder());
	}

	footer: DialogButtonBox {
		Button {
			text: qsTr("Close")
			DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
			flat: true
		}
	}
}
