import QtQuick.Dialogs 1.3
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14

import "components"

// TODO: Crash on import old file (from einkaufsliste)
// TODO: Adjust to small screens (e.g. my phone)
ApplicationWindow {
    visible: true
    title: qsTr("RecipeBook")

    TextMessageDialog {
        id: dlgConfirmImportFile
        title: qsTr("Import file")
        msgText: qsTr("This will load the selected file, replacing all current data irrevocably. Proceed?");
        onAccepted: fileDialogImport.onImport(fileDialogImport.fileUrls)
    }

    AppDlgInterface {
        anchors.centerIn: parent
    }

    signal onClosingRecipeBook()
    // TODO: This might not work on phones (at least data changes didn't seem to get saved on phone...)
    onClosing: onClosingRecipeBook()

    // TODO: Add left / right navigation buttons to switch between items in a submenu?
    header: ToolBar {
        id: mainBar
        width: parent.width

        ToolButton {
            id: btnOpenDrawer
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            display: AbstractButton.IconOnly
            icon.source: "qrc:/images/open-drawer.svg"

            onClicked: drawerMainMenu.open()
        }

        Label {
            id: lblCurrentTabName
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: btnOpenDrawer.right
            anchors.leftMargin: 20

            font.bold: true
            text: pageShoppingList.text
        }

        Loader {
            anchors.right: parent.right
            //anchors.left: lblCurrentTabName.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            id: headerSubpageSpace
        }
    }

    function selectPage(myIndex: int, text: string) {
        headerSubpageSpace.sourceComponent = undefined
        stackMain.currentIndex = myIndex;
        lblCurrentTabName.text = text;
        drawerMainMenu.close();
    }

    Drawer {
        id: drawerMainMenu
        width: 0.66 * parent.width
        height: parent.height

        Label {
            id: lblMainMenuHeader
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 24

            font.bold: true
            text: qsTr("RecipeBook")
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

                onClicked: selectPage(2, pageRecipes.text)
            }

            ItemDelegate {
                Layout.fillWidth: true
                Layout.leftMargin: 24
                text: qsTr("Sort orders")

                onClicked: selectPage(5, text)
            }
            ItemDelegate {
                Layout.fillWidth: true
                Layout.leftMargin: 24
                text: qsTr("Alternatives types")

                onClicked: selectPage(4, text)
            }
            ItemDelegate {
                Layout.fillWidth: true
                Layout.leftMargin: 24
                text: qsTr("Ingredients")

                onClicked: selectPage(3, text)
            }
            ItemDelegate {
                id: pageRecipes
                Layout.fillWidth: true
                Layout.leftMargin: 24
                text: qsTr("Recipes")

                onClicked: selectPage(2, text)
            }

            ItemDelegate {
                Layout.fillWidth: true
                font.bold: true
                text: qsTr("Shopping")

                onClicked: selectPage(0, pageShoppingList.text)
            }

            ItemDelegate {
                id: pageShoppingList
                Layout.fillWidth: true
                Layout.leftMargin: 24
                text: qsTr("Shopping list")

                onClicked: selectPage(0, text)
            }
            ItemDelegate {
                Layout.fillWidth: true
                Layout.leftMargin: 24
                text: qsTr("Go shopping")

                onClicked: selectPage(1, text)
            }

            ItemDelegate {
                Layout.fillWidth: true
                font.bold: true
                text: qsTr("Recipe books")

                onClicked: selectPage(6, text)
            }

            Item { Layout.fillHeight: true }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                Layout.bottomMargin: 12

                spacing: 10

                RoundButton {
                    display: AbstractButton.IconOnly

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
                        selectExisting: true
                        selectMultiple: false
                        selectFolder: false
                        signal onImport(filename: string)
                        onAccepted: dlgConfirmImportFile.open()
                    }
                }

                RoundButton {
                    display: AbstractButton.IconOnly

                    text: qsTr("Merge file")
                    icon.source: "qrc:/images/merge.svg"
                    enabled: false
                }

                RoundButton {
                    display: AbstractButton.IconOnly

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
                        selectExisting: false
                        selectMultiple: false
                        selectFolder: false
                        signal onExport(filename: string)
                        onAccepted: onExport(fileDialogExport.fileUrls)
                    }
                }

                RoundButton {
                    display: AbstractButton.IconOnly

                    text: qsTr("Settings")
                    icon.source: "qrc:/images/settings-white.svg"
                    onClicked: selectPage(7, text)
                }

            }
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
}
