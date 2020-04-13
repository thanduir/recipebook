import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.1

Item {  
    id: categoriesTab

    TextInputDialog {
        id: dlgAddSortOrder
        title: qsTr("Add sort order")
        onCurrentTextChanged: currentTextAllowed = !modelSortOrders.existsSortOrder(outputText)
        onAccepted: {
            lvSortOrders.currentIndex = modelSortOrders.addSortOrder(outputText)
            lvSortOrders.positionViewAtIndex(lvSortOrders.currentIndex, ListView.Center)
            modelSortOrder.setSortOrder(lvSortOrders.currentIndex)
        }
    }

    TextInputDialog {
        id: dlgRenameSortOrder
        title: qsTr("Rename sort order")
        onCurrentTextChanged: currentTextAllowed = !modelSortOrders.existsSortOrder(outputText)
        onAccepted: {
            lvSortOrders.currentIndex = modelSortOrders.renameSortOrder(lvSortOrders.currentIndex, outputText)
            lvSortOrders.positionViewAtIndex(lvSortOrders.currentIndex, ListView.Center)
            modelSortOrder.setSortOrder(lvSortOrders.currentIndex)
        }
    }
    
    TextMessageDialog {
        id: dlgRemoveSortOrder
        title: qsTr("Remove sort order")
        onAccepted: {
            modelSortOrders.removeSortOrder(lvSortOrders.currentIndex)
            lvSortOrders.incrementCurrentIndex()
            lvSortOrders.decrementCurrentIndex()
            modelSortOrder.setSortOrder(lvSortOrders.currentIndex)
        }
    }

    TextInputDialog {
        id: dlgAddCategory
        title: qsTr("Add category")
        onCurrentTextChanged: currentTextAllowed = !modelSortOrder.existsCategory(outputText)
        onAccepted: {
            lvCategories.currentIndex = modelSortOrder.addCategory(outputText)
            lvCategories.positionViewAtIndex(lvCategories.currentIndex, ListView.Center)
        }
    }

    TextInputDialog {
        id: dlgRenameCategory
        title: qsTr("Rename category")
        onCurrentTextChanged: currentTextAllowed = !modelSortOrder.existsCategory(outputText)
        onAccepted: {
            lvCategories.currentIndex = modelSortOrder.renameCategory(lvCategories.currentIndex, outputText)
        }
    }
    
    TextMessageDialog {
        id: dlgRemoveCategory
        title: qsTr("Remove category")
        onAccepted: {
            modelSortOrder.removeCategory(lvCategories.currentIndex)
            lvCategories.incrementCurrentIndex()
            lvCategories.decrementCurrentIndex()
        }
    }

    Label {
        id: labelSortOrders
        
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 48
        anchors.leftMargin: 48
        
        text: qsTr("Sort orders")
        font.bold: true
    }

    ScrollView {
        id: scrollViewSortOrders
        anchors.left: parent.left
        anchors.top: labelSortOrders.bottom
        anchors.topMargin: 48
        anchors.leftMargin: 48
        anchors.bottomMargin: 48
        width: 300

        ListView {
            id: lvSortOrders
            anchors.fill: parent
            topMargin: 5
            leftMargin: 5
            bottomMargin: 5
            rightMargin: 5
            boundsBehavior: Flickable.StopAtBounds

            spacing: 5
            model: modelSortOrders
            delegate: ItemDelegate {
                highlighted: ListView.isCurrentItem
                onClicked: {
                    lvSortOrders.currentIndex = index
                    modelSortOrder.setSortOrder(index)
                }
                width: lvSortOrders.width - lvSortOrders.leftMargin - lvSortOrders.rightMargin
                
                text: name
            }

            Component.onCompleted: {
                if(lvSortOrders.count > 0) {
                    lvSortOrders.currentIndex = 0;
                    modelSortOrder.setSortOrder(0);
                }
            }
        }
    }

    Pane {
        id: groupSortOrders
        anchors.left: scrollViewSortOrders.left
        anchors.right: scrollViewSortOrders.right
        anchors.top: scrollViewSortOrders.bottom
        anchors.topMargin: 48

        RowLayout {
            anchors.fill: parent
            Button { 
                text: qsTr("Add") 
                onClicked: dlgAddSortOrder.open()
            }
            Button { 
                text: qsTr("Rename") 
                onClicked: {
                    dlgRenameSortOrder.initialText = modelSortOrders.name(lvSortOrders.currentIndex);
                    dlgRenameSortOrder.open();
                }
            }
            Button { 
                text: qsTr("Remove") 
                enabled: lvSortOrders.count > 0 && modelSortOrders.canSortOrderBeRemoved(lvSortOrders.currentIndex)
                onClicked: {
                    dlgRemoveSortOrder.msgText = qsTr("This will remove the ingredient \"" + modelSortOrders.name(lvSortOrders.currentIndex) + "\". Proceed?");
                    dlgRemoveSortOrder.open();
                }
            }
        }
    }

    Component {
        id: dragDelegate

        MouseArea {
            id: dragArea

            property bool held: false

            anchors { left: parent.left; right: parent.right }
            height: content.height

            drag.target: held ? content : undefined
            drag.axis: Drag.YAxis

            onPressed: {
                held = true
                lvCategories.currentIndex = -1
            }
            onReleased: held = false
            onClicked: lvCategories.currentIndex = index
            
            Rectangle {
                id: content

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                width: dragArea.width; 
                height: column.implicitHeight + 50

                color: dragArea.held || lvCategories.currentIndex == index ? "lightgray" : "transparent"
                
                Drag.active: dragArea.held
                Drag.source: dragArea
                Drag.hotSpot.x: width / 2
                Drag.hotSpot.y: height / 2

                states: State {
                    when: dragArea.held

                    ParentChange { target: content; parent: categoriesTab }
                    AnchorChanges {
                        target: content
                        anchors { horizontalCenter: undefined; verticalCenter: undefined }
                    }
                }

                Item {
                    id: column
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter

                    Image {
                        id: reorderImage
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        height: labelColumn.implicitHeight + 10
                        verticalAlignment: Image.AlignVCenter
                        anchors.margins: 15

                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/images/reorder.svg"
                    }

                    Label {
                        id: labelColumn
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: reorderImage.right

                        anchors.margins: 15

                        verticalAlignment: Text.AlignVCenter
                        text: modelData

                        
                    }
                }
            }

            DropArea {
                anchors { fill: parent; margins: 15 }

                onEntered: {
                    modelSortOrder.moveCategory(drag.source.DelegateModel.itemsIndex,
                                                dragArea.DelegateModel.itemsIndex);
                    categoriesDelegateModel.items.move(drag.source.DelegateModel.itemsIndex,
                                                       dragArea.DelegateModel.itemsIndex);
                }
            }
        }
    }

    Label {
        id: labelCurrentSortOrder
        
        anchors.left: scrollViewCategories.left
        anchors.top: parent.top
        anchors.topMargin: 48
        
        text: qsTr("Sort order \"" + modelSortOrders.name(lvSortOrders.currentIndex) + "\"")
        font.bold: true
    }

    ScrollView {
        id: scrollViewCategories
        anchors.left: scrollViewSortOrders.right
        anchors.top: labelCurrentSortOrder.bottom
        anchors.bottom: groupCategories.top
        anchors.topMargin: 48
        anchors.leftMargin: 96
        anchors.bottomMargin: 48
        width: 400

        ListView {
            id: lvCategories
            anchors.fill: parent
            topMargin: 5
            leftMargin: 5
            bottomMargin: 5
            rightMargin: 5
            boundsBehavior: Flickable.StopAtBounds

            spacing: 5
            model: DelegateModel {
                id: categoriesDelegateModel

                model: modelSortOrder
                delegate: dragDelegate
            }
        }
    }

    Pane {
        id: groupCategories
        anchors.left: scrollViewCategories.left
        anchors.right: scrollViewCategories.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 48

        RowLayout {
            anchors.fill: parent
            Button { 
                text: qsTr("Add") 
                onClicked: dlgAddCategory.open()
            }
            Button { 
                text: qsTr("Rename") 
                enabled: lvCategories.count > 0 && lvCategories.currentIndex != -1 && lvCategories.currentIndex >= 0
                onClicked: {
                    dlgRenameCategory.initialText = modelSortOrder.name(lvCategories.currentIndex);
                    dlgRenameCategory.open();
                }
            }
            Button { 
                text: qsTr("Remove") 
                enabled: lvCategories.count > 0 && lvCategories.currentIndex != -1 && modelSortOrder.canCategoryBeRemoved(lvCategories.currentIndex)
                onClicked: {
                    dlgRemoveCategory.msgText = qsTr("This will remove the category \"" + modelSortOrder.name(lvCategories.currentIndex) + "\" from all sort orders. Proceed?");
                    dlgRemoveCategory.open();
                }
            }
        }
    }
}
