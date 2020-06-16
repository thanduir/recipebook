import QtQuick.Dialogs 1.3
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14

import "components"

ApplicationWindow {
    visible: true
    title: qsTr("RecipeBook")

    TextMessageDialog {
        id: dlgConfirmImportFile
        title: qsTr("Import file")
        msgText: qsTr("This will load the selected file, replacing all current data irrevocably. Proceed?");
        onAccepted: fileDialogImport.onImport(fileDialogImport.fileUrls)
    }

    SettingsDialog {
        id: dlgSettings
        title: qsTr("Settings")
    }

    AppDlgInterface {
        anchors.centerIn: parent
    }

    signal onClosingRecipeBook()
    onClosing: onClosingRecipeBook()

    header: ToolBar {
        id: bar
        width: parent.width

        ToolButton {
            id: btnOpenDrawer
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            display: AbstractButton.IconOnly
            icon.source: "qrc:/images/open-drawer.svg"

            onClicked: drawer.open()
        }

        Label {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: btnOpenDrawer.right
            anchors.leftMargin: 20

            font.bold: true
            text: qsTr("RecipeBook")
        }
    }

    ListModel {
        id: mainTabs

        ListElement { name: qsTr("Recipes") }
        ListElement { name: qsTr("Shopping") }
        ListElement { name: qsTr("Recipe books") }
    }

    Drawer {
        id: drawer
        width: 0.66 * parent.width
        height: parent.height

        ListView {
            id: mainTab
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: colBar.bottom

            spacing: 5

            model: mainTabs
            delegate: ItemDelegate {
                highlighted: ListView.isCurrentItem
                text: name
                width: parent.width

                onClicked: {
                    mainTab.currentIndex = index
                }
            }

            Component.onCompleted: {
                mainTab.currentIndex = 1;
            }
        }

        ColumnLayout {
            id: colBar

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            ToolSeparator {
                Layout.fillWidth: true
                orientation: Qt.Horizontal
            }

            ToolButton {
                Layout.fillWidth: true

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

            ToolSeparator {
                Layout.fillWidth: true
                orientation: Qt.Horizontal
            }

            ToolButton {
                Layout.fillWidth: true

                text: qsTr("Merge file")
                icon.source: "qrc:/images/merge.svg"
                enabled: false
            }

            ToolSeparator {
                Layout.fillWidth: true
                orientation: Qt.Horizontal
            }

            ToolButton {
                Layout.fillWidth: true

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

            ToolSeparator {
                Layout.fillWidth: true
                orientation: Qt.Horizontal
            }

            ToolButton {
                Layout.fillWidth: true

                text: qsTr("Settings")
                icon.source: "qrc:/images/settings-white.svg"
                onClicked: dlgSettings.open()
            }
        }
    }

    StackLayout {
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        currentIndex: mainTab.currentIndex

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
