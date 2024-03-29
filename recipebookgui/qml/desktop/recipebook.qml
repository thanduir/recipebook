import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14
import Qt.labs.platform 1.1

import "components"

ApplicationWindow {
	width: 1300
	height: 875
	visible: true
	title: qsTr("RecipeBook")
	Component.onCompleted: {
		x = Screen.width / 2 - width / 2
		y = Screen.height / 2 - height / 2
	}

	TextMessageDialog {
		id: dlgConfirmImportFile
		title: qsTr("Import file")
		msgText: qsTr("This will load the selected file, replacing all current data irrevocably. Proceed?");
		onAccepted: fileDialogImport.onImport(fileDialogImport.file)
	}

	AppDlgInterface {
		anchors.centerIn: parent
	}

	AppSyncUI {
		anchors.centerIn: parent
	}

	signal onClosingRecipeBook()
	onClosing: onClosingRecipeBook()

	header: ToolBar {
		RowLayout {
			ToolButton {
				text: qsTr("Import file")
				icon.source: "qrc:/images/import-file.svg"
				onClicked: {
					fileDialogImport.folder = "file:///" + recipeBookSettings.lastUsedImportFolder()
					fileDialogImport.open()
				}

				FileDialog {
					id: fileDialogImport
					objectName: "fileDialogImport"

					title: qsTr("Import file")
					modality: Qt.WindowModal
					nameFilters: importExportFilters
					fileMode: FileDialog.OpenFile
					signal onImport(filename: string)
					onAccepted: dlgConfirmImportFile.open()
				}
			}

			ToolSeparator {}

			ToolButton {
				text: qsTr("Sync with server")
				icon.source: "qrc:/images/merge.svg"
				enabled: false
				onClicked: recipeBookSynchronization.execute()
			}

			ToolSeparator {}

			ToolButton {
				text: qsTr("Export")
				icon.source: "qrc:/images/export-file.svg"
				onClicked: {
					fileDialogExport.folder = "file:///" + recipeBookSettings.lastUsedExportFolder()
					fileDialogExport.open()
				}

				FileDialog {
					id: fileDialogExport
					objectName: "fileDialogExport"

					title: qsTr("Export data")
					modality: Qt.WindowModal
					nameFilters: importExportFilters
					fileMode: FileDialog.SaveFile
					signal onExport(filename: string)
					onAccepted: onExport(fileDialogExport.file)
				}
			}

			ToolSeparator {}

			ToolButton {
				text: qsTr("Settings")
				icon.source: "qrc:/images/settings-white.svg"
				onClicked: dlgSettings.open()
			}
		}
	}

	SettingsDialog {
		id: dlgSettings
	}

	TabBar {
		id: bar
		width: parent.width
		topPadding: 20

		TabButton {
			text: qsTr("Recipes")
		}
		TabButton {
			text: qsTr("Shopping")
		}
		TabButton {
			text: qsTr("Recipe books")
		}
	}

	StackLayout {
		anchors.top: bar.bottom
		anchors.topMargin: 10
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 20
		anchors.left: parent.left
		anchors.leftMargin: 10
		anchors.right: parent.right
		anchors.rightMargin: 10

		currentIndex: bar.currentIndex

		RecipesTab {
			id: recipesTab
		}
		ShoppingTab {
			id: shoppingTab
		}
		RecipebooksTab {
			id: recipebooksTab
		}
	}
}
