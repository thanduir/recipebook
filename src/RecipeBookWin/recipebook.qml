import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14

// TODO: REPLACE WITH dialogs from Qt.labs.platform 1.1
//  e.g. https://doc.qt.io/qt-5/qml-qt-labs-platform-filedialog.html
import QtQuick.Dialogs 1.3

import "components"

ApplicationWindow {
    width: 1250
    height: 825
    visible: true
    title: qsTr("RecipeBook")
    Component.onCompleted: {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2
    }

    header: ToolBar {
        RowLayout {
            ToolButton {
                objectName: "toolBarButtonSave"

                // TODO: Icon
                text: qsTr("Save")
                // TODO: implementieren und enablen!
                enabled: false
            }
            
            ToolSeparator {}

            ToolButton {
                objectName: "toolBarButtonSaveAs"

                // TODO: Icon
                text: qsTr("Save as")
                onClicked: fileDialogSaveAs.open()

                FileDialog {
                    id: fileDialogSaveAs
                    objectName: "fileDialogSaveAs"

                    title: qsTr("Save As")
                    modality: Qt.WindowModal
                    nameFilters: [ qsTr("Json files (*.json)") ]
                    selectExisting: false
                    selectMultiple: false
                    selectFolder: false
                    folder: shortcuts.documents
                    signal onSaveAs(filename: string)
                    onAccepted: fileDialogSaveAs.onSaveAs(fileDialogSaveAs.fileUrls)
                }
            }
            
            ToolSeparator {}

            ToolButton {
                objectName: "toolBarButtonExport"

                // TODO: Icon
                text: qsTr("Export")
                // TODO: implementieren und enablen!
                enabled: false
                //onClicked: menu.open()
            }

            ToolSeparator {}

            ToolButton {
                objectName: "toolBarButtonSettings"

                // TODO: Icon
                text: qsTr("Settings")
                // TODO: implementieren und enablen!
                enabled: false
                //onClicked: menu.open()
            }
        }
    }

    TabBar {
        id: bar
        width: parent.width
        topPadding: 20

        TabButton {
            text: qsTr("Categories")
        }
        TabButton {
            text: qsTr("Ingredients")
        }
        TabButton {
            text: qsTr("Recipes")
        }
    }

    StackLayout {
        currentIndex: bar.currentIndex
        anchors.top: bar.bottom
        anchors.topMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 20

        CategoriesTab {
            id: categoriesTab            
        }
        IngredientsTab {
            id: ingredientsTab
        }
        RecipesTab {
            id: recipesTab
        }
    }
}
