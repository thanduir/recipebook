import QtQuick.Dialogs 1.3
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "components"

Dialog {
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2 - 25

	width: 500
	height: parent.height

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
			id: groupSettings

			title: qsTr("Settings")
			width: groupDefaultValues.width

			GridLayout {
				anchors.fill: parent
        
				columns: 2
				columnSpacing: 50
				rowSpacing: 10

				Label { 
					text: qsTr("Current language")
				}
				ComboBox { 
					id: cbxLanguages
					Layout.fillWidth: true
					model: RBLanguageManager.availableLanguages()

					onActivated: RBLanguageManager.setCurrentLanguageIndex(currentIndex)
				}
			}
		}

		GroupBox {
			id: groupDefaultValues
			anchors.top: groupSettings.bottom
			anchors.topMargin: 18

			title: qsTr("Default values")

			GridLayout {
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
			anchors.topMargin: 18

			width: groupDefaultValues.width

			title: qsTr("Data")

			ColumnLayout {
				anchors.centerIn: parent

				Button {
					text: qsTr("Load default data")
					font.capitalization: Font.MixedCase
					Layout.alignment: Qt.AlignHCenter
					onClicked: dlgLoadDefaultData.open()
				}

				Button {
					text: qsTr("Reset all data")
					font.capitalization: Font.MixedCase
					Layout.alignment: Qt.AlignHCenter
					onClicked: dlgResetData.open()
				}
			}
		}

		GroupBox {
			id: groupPdfExport
			anchors.top: groupData.bottom
			anchors.topMargin: 18

			width: groupDefaultValues.width

			title: qsTr("PDF Export")

			RowLayout {
				anchors.fill: parent
				spacing: 10

				Label {
					text: qsTr("PDF Latex path")
				}

				TextField {
					id: txtPdfLatex
					Layout.fillWidth: true
					readOnly: true

					placeholderText: "PDF Latex executable"
					text: recipeBookSettings.getPdfLatexFile()

					ToolTip.delay: 1000
					ToolTip.timeout: 3000
					ToolTip.visible: hovered
					ToolTip.text: recipeBookSettings.getPdfLatexFile()
				}

				RoundButton {
					display: AbstractButton.IconOnly
					icon.source: "qrc:/images/folder-black.svg"

					onClicked: {
						fileDialogPdfLatex.folder = recipeBookSettings.getPdfLatexFile()
						fileDialogPdfLatex.open()
					}

					FileDialog {
						id: fileDialogPdfLatex

						title: qsTr("Generate recipe book pdf")
						modality: Qt.WindowModal
						nameFilters: recipeBookSettings.getPdfLatexExeNameFilter()
						selectExisting: true
						selectMultiple: false
						selectFolder: false
						onAccepted: {
							recipeBookSettings.setPdfLatexFile(fileUrls)
							txtPdfLatex.text = recipeBookSettings.getPdfLatexFile()
						}
					}
				}
			}
		}

		RowLayout {
			id: rowUID
			anchors.top: groupPdfExport.bottom
			anchors.topMargin: 18

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

		cbxLanguages.currentIndex = cbxLanguages.indexOfValue(RBLanguageManager.getCurrentLanguageName());
	}

	footer: DialogButtonBox {
		Button {
			text: qsTr("Close")
			DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
			flat: true
		}
	}
}
