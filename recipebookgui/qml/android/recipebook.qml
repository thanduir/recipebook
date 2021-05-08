import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14

import "components"

ApplicationWindow {
	visible: true
	title: qsTr("RecipeBook")

	InputFromListDialog {
		id: fileDialogExport
		objectName: "fileDialogExport"
		title: qsTr("Export data")
		listInputOnly: false
		signal onExport(filename: string)
		onAccepted: {
			if(listItems.includes(outputText))
			{
				dlgConfirmExportFileOverwrite.open()
			}
			else
			{
				onExport(outputText);
			}
		}
	}

	TextMessageDialog {
		id: dlgConfirmExportFileOverwrite
		title: qsTr("Export file")
		msgText: qsTr("The file \"%1\" already exists. Overwrite?").arg(fileDialogExport.outputText);
		onAccepted: fileDialogExport.onExport(fileDialogExport.outputText)
	}

	InputFromListDialog {
		id: fileDialogImport
		objectName: "fileDialogImport"

		title: qsTr("Import file")
		listInputOnly: true

		signal onImport(filename: string)
		onAccepted: dlgConfirmImportFile.open()
	}

    TextMessageDialog {
        id: dlgConfirmImportFile
        title: qsTr("Import file")
        msgText: qsTr("This will load the selected file, replacing all current data irrevocably. Proceed?");
		onAccepted: fileDialogImport.onImport(fileDialogImport.outputText)
    }

    AppDlgInterface {
        anchors.centerIn: parent
    }

	AppSyncUI {
		anchors.centerIn: parent
	}

    signal onClosingRecipeBook()
	onClosing: {
		if(stackMain.currentIndex != 0)
		{
			selectPage(0, lblMainMenuHeader.text)
			close.accepted = false;
		}
		else
		{
			onClosingRecipeBook()
		}
	}

	header: ToolBar {
        id: mainBar
        width: parent.width

        ToolButton {
            id: btnOpenDrawer
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

			enabled: stackMain.currentIndex != 0

            display: AbstractButton.IconOnly
            icon.source: "qrc:/images/open-drawer.svg"

            onClicked: drawerMainMenu.open()
        }

        Label {
            id: lblCurrentTabName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: btnOpenDrawer.right

            font.bold: true
			text: lblMainMenuHeader.text
        }

        Loader {
			anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

			id: headerSubpageSpace
        }
    }

	function selectPage(myIndex: int, text: string) {
		if(myIndex !== stackMain.currentIndex)
		{
			headerSubpageSpace.sourceComponent = undefined
			stackMain.currentIndex = myIndex;
		}
		lblCurrentTabName.text = text;
        drawerMainMenu.close();
    }

    Drawer {
        id: drawerMainMenu
		width: 0.75 * parent.width
        height: parent.height
		interactive: stackMain.currentIndex != 0

		Label {
            id: lblMainMenuHeader
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 24

            font.bold: true
            text: qsTr("RecipeBook")

			MouseArea {
				anchors.fill: parent
				onClicked: selectPage(0, lblMainMenuHeader.text)
			}
        }

        ColumnLayout {
            id: sideBar
            anchors.top: lblMainMenuHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: 24

            ItemDelegate {
                Layout.fillWidth: true
                font.bold: true
                text: qsTr("Recipes")

				onClicked: selectPage(3, pageRecipes.text)
            }

            ItemDelegate {
                Layout.fillWidth: true
                Layout.leftMargin: 24
				font.bold: false
                text: qsTr("Sort orders")

				onClicked: selectPage(6, text)
            }
            ItemDelegate {
                Layout.fillWidth: true
                Layout.leftMargin: 24
				font.bold: false
                text: qsTr("Alternatives types")

				onClicked: selectPage(5, text)
            }
            ItemDelegate {
                Layout.fillWidth: true
                Layout.leftMargin: 24
				font.bold: false
                text: qsTr("Ingredients")

				onClicked: selectPage(4, text)
            }
            ItemDelegate {
                id: pageRecipes
                Layout.fillWidth: true
                Layout.leftMargin: 24
				font.bold: false
                text: qsTr("Recipes")

				onClicked: selectPage(3, text)
            }

            ItemDelegate {
                Layout.fillWidth: true
                font.bold: true
                text: qsTr("Shopping")

				onClicked: selectPage(1, pageShoppingList.text)
            }

            ItemDelegate {
                id: pageShoppingList
                Layout.fillWidth: true
                Layout.leftMargin: 24
				font.bold: false
                text: qsTr("Shopping list")

				onClicked: selectPage(1, text)
            }
            ItemDelegate {
                Layout.fillWidth: true
                Layout.leftMargin: 24
				font.bold: false
                text: qsTr("Go shopping")

				onClicked: selectPage(2, text)
            }

            ItemDelegate {
                Layout.fillWidth: true
                font.bold: true
                text: qsTr("Recipe books")

				onClicked: selectPage(7, text)
            }

            Item { Layout.fillHeight: true }

			MenuButtonRow {
				Layout.alignment: Qt.AlignHCenter
				Layout.fillWidth: true
				Layout.leftMargin: 12
				Layout.rightMargin: 12
				Layout.bottomMargin: 12
			}
        }
    }

	component MenuButtonRow: RowLayout {
		spacing: 10

		RoundButton {
			display: AbstractButton.IconOnly

			text: qsTr("Import file")
			icon.source: "qrc:/images/import-file.svg"
			onClicked: {
				fileDialogImport.listItems = recipeBookSettings.getCurrentFilenamesAndroid()
				fileDialogImport.open()
			}
		}

		RoundButton {
			display: AbstractButton.IconOnly

			text: qsTr("Sync with server")
			icon.source: "qrc:/images/merge.svg"
			enabled: false
			onClicked: recipeBookSynchronization.execute()
		}

		RoundButton {
			display: AbstractButton.IconOnly

			text: qsTr("Export")
			icon.source: "qrc:/images/export-file.svg"
			onClicked: {
				fileDialogExport.listItems = recipeBookSettings.getCurrentFilenamesAndroid()
				fileDialogExport.open()
			}
		}

		RoundButton {
			display: AbstractButton.IconOnly

			text: qsTr("Settings")
			icon.source: "qrc:/images/settings-white.svg"
			onClicked: selectPage(8, text)
		}
	}

   StackLayout {
        id: stackMain
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

		StartPage {
			id: startPage
		}

        ShoppingListPage {
            id: shoppingListPage
        }
        GoShoppingPage {
            id: goShoppingPage
        }

        RecipesPage {
            id: recipesPage
        }
        IngredientsPage {
            id: ingredientsPage
        }
        AlternativesTypesPage {
            id: alternativesTypesPage
        }
        CategoriesPage {
            id: categoriesPage
        }

        RecipebooksTab {
            id: recipebooksTab
        }

        SettingsDialog {
            id: settingsTab
        }
	}

	component StartPage : Item {
	   ColumnLayout {
			anchors.horizontalCenter: parent.horizontalCenter
			spacing: 15

			GroupBox {
				Layout.alignment: Qt.AlignHCenter
				title: qsTr("Recipes")
				font.bold: true
				implicitWidth: 200

				ColumnLayout {
					anchors.horizontalCenter: parent.horizontalCenter

					RoundButton {
						Layout.alignment: Qt.AlignHCenter
						display: AbstractButton.TextOnly
						font.bold: false
						text: qsTr("Sort orders")
						font.capitalization: Font.Capitalize

						onClicked: selectPage(6, text)
					}
					RoundButton {
						Layout.alignment: Qt.AlignHCenter
						display: AbstractButton.TextOnly
						font.bold: false
						text: qsTr("Alternatives types")
						font.capitalization: Font.Capitalize

						onClicked: selectPage(5, text)
					}
					RoundButton {
						Layout.alignment: Qt.AlignHCenter
						display: AbstractButton.TextOnly
						font.bold: false
						text: qsTr("Ingredients")
						font.capitalization: Font.Capitalize

						onClicked: selectPage(4, text)
					}
					RoundButton {
						Layout.alignment: Qt.AlignHCenter
						display: AbstractButton.TextOnly
						font.bold: false
						text: qsTr("Recipes")
						font.capitalization: Font.Capitalize

						onClicked: selectPage(3, text)
					}
				}
			}

			GroupBox {
				title: qsTr("Shopping")
				Layout.alignment: Qt.AlignHCenter
				font.bold: true
				implicitWidth: 200

				ColumnLayout {
					anchors.horizontalCenter: parent.horizontalCenter

					RoundButton {
						Layout.alignment: Qt.AlignHCenter
						display: AbstractButton.TextOnly
						font.bold: false
						text: qsTr("Shopping list")
						font.capitalization: Font.Capitalize

						onClicked: selectPage(1, text)
					}
					RoundButton {
						Layout.alignment: Qt.AlignHCenter
						display: AbstractButton.TextOnly
						font.bold: false
						text: qsTr("Go shopping")
						font.capitalization: Font.Capitalize

						onClicked: selectPage(2, text)
					}
				}
			}

			GroupBox {
				title: qsTr("Recipe books")
				Layout.alignment: Qt.AlignHCenter
				font.bold: true
				implicitWidth: 200

				ColumnLayout {
					anchors.horizontalCenter: parent.horizontalCenter

					RoundButton {
						Layout.alignment: Qt.AlignHCenter
						display: AbstractButton.TextOnly
						font.bold: false
						text: qsTr("Recipe books")
						font.capitalization: Font.Capitalize

						onClicked: selectPage(7, text)
					}
				}
			}

			MenuButtonRow {
				Layout.alignment: Qt.AlignHCenter
			}
	   }
   }
}
