import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {
    
    TextInputDialog {
        id: dlgAddIngredient
        title: qsTr("Add ingredient")
        onAccepted: {
            listView.currentIndex = modelIngredients.addIngredient(outputText)
            listView.positionViewAtIndex(listView.currentIndex, ListView.Center)
        }
    }

    TextInputDialog {
        id: dlgRenameIngredient
        title: qsTr("Rename ingredient")
        initialText: modelIngredients.name(listView.currentIndex)
        onAccepted: {
            listView.currentIndex = modelIngredients.renameIngredient(listView.currentIndex, outputText)
            listView.positionViewAtIndex(listView.currentIndex, ListView.Center)
        }
    }
    
    TextMessageDialog {
        id: dlgRemoveIngredient
        title: qsTr("Remove ingredient")
        msgText: qsTr("This will remove the ingredient \"" + modelIngredients.name(listView.currentIndex) + "\". Proceed?")
        onAccepted: {
            modelIngredients.removeIngredient(listView.currentIndex)
            listView.incrementCurrentIndex()
            listView.decrementCurrentIndex()
        }
    }


    ScrollView {
        id: scrollView
        anchors.left: parent.left
        anchors.top: parent.top 
        anchors.bottom: labelAdd.top
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

    Button {
        id: labelAdd
            
        anchors.top: listView.bottom
        anchors.bottom: parent.bottom
        anchors.right: labelRemove.left
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        // TODO: Icon instead of text?
        text: qsTr("Add ingredient")
        onClicked: dlgAddIngredient.open()
    }

    Button {
        id: labelRemove
            
        anchors.top: listView.bottom
        anchors.bottom: parent.bottom
        anchors.right: scrollView.right
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        // TODO: Icon instead of text?
        text: qsTr("Remove ingredient")
        enabled: listView.count > 0 && modelIngredients.canIngredientBeRemoved(listView.currentIndex)
        onClicked: dlgRemoveIngredient.open()
    }

    GridLayout {
        id: grid
        anchors.left: scrollView.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 48
        anchors.leftMargin: 48
        anchors.bottomMargin: 48
        anchors.rightMargin: 48
        
        visible: listView.count > 0
        columns: 2

        Label { 
            text: qsTr("Name") 
        }
        RowLayout {
            Layout.fillWidth: true

            TextField { 
                Layout.fillWidth: true
                readOnly: true
                text: modelIngredients.name(listView.currentIndex)
            }
            Button {
                // TODO: Icon instead of text?
                text: "Edit"
                onClicked: dlgRenameIngredient.open()
            }
        }

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

        Label { 
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft

            text: qsTr("Used in Recipes") 
        }
        Label {
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            lineHeight: 1.25

            text: modelIngredients.listUsedInRecipes(listView.currentIndex)
        }

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
