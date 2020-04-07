import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {  
    /* TODO: 
            * move categories in sort order using:
                - https://doc.qt.io/qt-5/qtquick-tutorials-dynamicview-dynamicview2-example.html
                - https://doc.qt.io/qt-5/qtquick-tutorials-dynamicview-dynamicview3-example.html
                - https://doc.qt.io/qt-5/qtquick-tutorials-dynamicview-dynamicview4-example.html
            * show somewhere "used in" for categories and sortOrders!
            * can i add something (effect) so that it's obvious that the sort order determines the categories list?
            * do i need to add headers above the listviews?
    */  

    TextInputDialog {
        id: dlgAddSortOrder
        title: qsTr("Add sort order")
        onAccepted: {
            lvSortOrders.currentIndex = modelSortOrders.addSortOrder(outputText)
            lvSortOrders.positionViewAtIndex(lvSortOrders.currentIndex, ListView.Center)
            modelSortOrder.setSortOrder(lvSortOrders.currentIndex)
        }
    }

    TextInputDialog {
        id: dlgRenameSortOrder
        title: qsTr("Rename sort order")
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
        onAccepted: {
            lvCategories.currentIndex = modelSortOrder.addCategory(outputText)
            lvCategories.positionViewAtIndex(lvCategories.currentIndex, ListView.Center)
        }
    }

    TextInputDialog {
        id: dlgRenameCategory
        title: qsTr("Rename category")
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

    ScrollView {
        id: scrollViewSortOrders
        anchors.left: parent.left
        anchors.top: parent.top
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

    ScrollView {
        id: scrollViewCategories
        anchors.left: scrollViewSortOrders.right
        anchors.top: parent.top
        anchors.bottom: groupCategories.top
        anchors.topMargin: 48
        anchors.leftMargin: 48
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
            model: modelSortOrder
            delegate: ItemDelegate {                
                highlighted: ListView.isCurrentItem
                onClicked: lvCategories.currentIndex = index
                width: lvCategories.width - lvCategories.leftMargin - lvCategories.rightMargin

                text: modelData
                //DragHandler { }
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
                enabled: lvCategories.count > 0 && lvCategories.currentIndex >= 0
                onClicked: {
                    dlgRenameCategory.initialText = modelSortOrder.name(lvCategories.currentIndex);
                    dlgRenameCategory.open();
                }
            }
            Button { 
                text: qsTr("Remove") 
                enabled: lvCategories.count > 0 && modelSortOrder.canCategoryBeRemoved(lvCategories.currentIndex)
                onClicked: {
                    dlgRemoveCategory.msgText = qsTr("This will remove the category \"" + modelSortOrder.name(lvCategories.currentIndex) + "\" from all sort orders. Proceed?");
                    dlgRemoveCategory.open();
                }
            }
        }
    }
}