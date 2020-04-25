import QtQuick.Dialogs 1.3
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14

ApplicationWindow {
	width: 1300
	height: 825
	visible: true
	title: qsTr("RecipeBook")
	Component.onCompleted: {
		x = Screen.width / 2 - width / 2
		y = Screen.height / 2 - height / 2
	}

	signal onClosingRecipeBook()
	onClosing: onClosingRecipeBook()

	header: ToolBar {
		RowLayout {
			ToolButton {
				// TODO: Icon (instead of text?)
				text: qsTr("Import file")
				onClicked: {
					fileDialogImport.folder = "file:///" + recipeBookSettings.lastUsedImportFolder()
					fileDialogImport.open()
				}

				FileDialog {
					id: fileDialogImport
					objectName: "fileDialogImport"

					title: qsTr("Export data")
					modality: Qt.WindowModal
					nameFilters: [ qsTr("Json files (*.json)") ]
					selectExisting: true
					selectMultiple: false
					selectFolder: false
					signal onImport(filename: string)
					onAccepted: onImport(fileDialogImport.fileUrls)
				}
			}

			ToolSeparator {}

			ToolButton {
				// TODO: Icon (instead of text?)
				text: qsTr("Merge file")
				// TODO: implementieren und enablen!
				enabled: false
			}

			ToolSeparator {}

			ToolButton {
				// TODO: Icon (instead of text?)
				text: qsTr("Export")
				onClicked: {
					fileDialogExport.folder = "file:///" + recipeBookSettings.lastUsedExportFolder()
					fileDialogExport.open()
				}

				FileDialog {
					id: fileDialogExport
					objectName: "fileDialogExport"

					title: qsTr("Export data")
					modality: Qt.WindowModal
					nameFilters: [ qsTr("Json files (*.json)") ]
					selectExisting: false
					selectMultiple: false
					selectFolder: false
					signal onExport(filename: string)
					onAccepted: onExport(fileDialogExport.fileUrls)
				}
			}

			ToolSeparator {}

			ToolButton {
				// TODO: Icon (instead of text?)
				text: qsTr("Settings")
				enabled: true
				onClicked: dlgSettings.open()
			}
		}
	}

	SettingsDialog {
		id: dlgSettings
		title: qsTr("Settings")
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
			text: qsTr("Cookbook")
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
		CookbookTab {
			id: cookbookTab
		}
	}
}
