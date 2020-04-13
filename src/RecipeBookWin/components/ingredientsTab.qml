import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {
    
    TextInputDialog {
        id: dlgAddIngredient
        title: qsTr("Add ingredient")
        onCurrentTextChanged: currentTextAllowed = !modelIngredients.existsIngredient(outputText)
        onAccepted: {
            listView.currentIndex = modelIngredients.addIngredient(outputText)
            listView.positionViewAtIndex(listView.currentIndex, ListView.Center)
        }
    }

    TextInputDialog {
        id: dlgRenameIngredient
        title: qsTr("Rename ingredient")
        onCurrentTextChanged: currentTextAllowed = !modelIngredients.existsIngredient(outputText)
        onAccepted: {
            listView.currentIndex = modelIngredients.renameIngredient(listView.currentIndex, outputText)
            listView.positionViewAtIndex(listView.currentIndex, ListView.Center)
        }
    }
    
    TextMessageDialog {
        id: dlgRemoveIngredient
        title: qsTr("Remove ingredient")
        onAccepted: {
            modelIngredients.removeIngredient(listView.currentIndex)
            listView.incrementCurrentIndex()
            listView.decrementCurrentIndex()
        }
    }

    Label {
        id: labelIngredients
        
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 48
        anchors.leftMargin: 48
        
        text: qsTr("Ingredients")
        font.bold: true
    }

    ScrollView {
        id: scrollView
        anchors.left: parent.left
        anchors.top: labelIngredients.bottom 
        anchors.bottom: paneIngredients.top
        anchors.topMargin: 48
        anchors.leftMargin: 48
        anchors.bottomMargin: 48
        width: 400

        ListView {
            id: listView
            anchors.fill: parent
            topMargin: 5
            leftMargin: 5
            bottomMargin: 5
            rightMargin: 5
            boundsBehavior: Flickable.StopAtBounds

            spacing: 5
            model: modelIngredients
            delegate: ItemDelegate {
                width: listView.width - listView.leftMargin - listView.rightMargin
                highlighted: ListView.isCurrentItem
                onClicked: listView.currentIndex = index
                
                Item {
                    anchors.fill: parent

                    Label {
                        id: listItemDelegateName
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        
                        text: name
                        verticalAlignment: Text.AlignVCenter
                        height: parent.height
                    }

                    Label {
                        anchors.left: listItemDelegateName.right
                        color: "gray"
                        text: " (" + category + (provenance != modelProvenance.provenanceEverywhere() ? ", " + provenance : "") + ", " + defaultUnit + ")"
                        verticalAlignment: Text.AlignVCenter
                        height: parent.height
                        width: parent.width - listItemDelegateName.width - 10
                        wrapMode: Label.WordWrap
                    }
                }
            }
        }
    }

    Pane {
        id: paneIngredients
        anchors.bottom: parent.bottom
        anchors.left: scrollView.left
        anchors.right: scrollView.right
        anchors.leftMargin: 48
        anchors.rightMargin: 48
        anchors.bottomMargin: 10

        RowLayout {
            anchors.fill: parent
        
            Button {
                id: buttonAdd

                text: qsTr("Add")
                onClicked: dlgAddIngredient.open()
            }

            Button {
                id: buttonRename

                text: qsTr("Rename")
                onClicked: {
                    dlgRenameIngredient.initialText = modelIngredients.name(listView.currentIndex);
                    dlgRenameIngredient.open();
                }
            }

            Button {
                id: buttonRemove

                text: qsTr("Remove")
                enabled: listView.count > 0 && modelIngredients.canIngredientBeRemoved(listView.currentIndex)
                onClicked: {
                    dlgRemoveIngredient.msgText = qsTr("This will remove the ingredient \"" + modelIngredients.name(listView.currentIndex) + "\". Proceed?");
                    dlgRemoveIngredient.open();
                }
            }
        }
    }

    Label {
        id: labelCurrentSortOrder
        
        anchors.left: grid.left
        anchors.top: parent.top
        anchors.topMargin: 48
        
        visible: listView.count > 0
        text: qsTr("Category \"" + modelIngredients.name(listView.currentIndex) + "\"")
        font.bold: true
    }

    GridLayout {
        id: grid
        anchors.left: scrollView.right
        anchors.top: labelCurrentSortOrder.bottom
        anchors.right: parent.right
        anchors.topMargin: 48
        anchors.leftMargin: 48
        anchors.bottomMargin: 48
        anchors.rightMargin: 48
        
        visible: listView.count > 0
        columns: 2

        Label { 
            text: qsTr("Category")
        }
        ComboBox {
            Layout.fillWidth: true
            model: modelCategories
            currentIndex: indexOfValue(modelIngredients.category(listView.currentIndex))
            onActivated: modelIngredients.setCategory(listView.currentIndex, currentText)
        }

        Label { 
            Layout.rightMargin: 50
            text: qsTr("Provenance")
        }
        ComboBox {
            Layout.fillWidth: true
            model: modelProvenance
            currentIndex: indexOfValue(modelIngredients.provenance(listView.currentIndex))
            onActivated: modelIngredients.setProvenance(listView.currentIndex, currentText)
        }

        Label { text: qsTr("Unit") }
        ComboBox {
            id: cbxUnit
            Layout.fillWidth: true
            model: unitNames
            currentIndex: indexOfValue(modelIngredients.defaultUnit(listView.currentIndex))
            onActivated: modelIngredients.setDefaultUnit(listView.currentIndex, currentText)
        }

        Item { height: 10; Layout.columnSpan: 2 }

        // TODO: Ausblenden wenn leer?
        Label { 
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft

            text: qsTr("Used in Recipes") 
        }
        Label {
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            lineHeight: 1.25

            text: modelIngredients.listUsedInRecipes(listView.currentIndex)
        }

        // TODO: Ausblenden wenn leer?
        Label { 
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft

            text: qsTr("Used in ShoppingRecipes") 
        }
        Label {
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            lineHeight: 1.25

            text: modelIngredients.listUsedInShoppingRecipes(listView.currentIndex)
        }
    }
}
