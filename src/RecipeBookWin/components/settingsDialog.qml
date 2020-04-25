import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Dialog {
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2

	width: 500
	height: parent.height - 100

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
					model: unitNames

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

			Button {
				anchors.centerIn: parent
				text: qsTr("reset all data")

				onClicked: recipeBookSettings.resetAllData();
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
