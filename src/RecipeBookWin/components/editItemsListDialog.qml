import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14

Dialog {
    modal: true

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    property var allValues: []
    property var editListModel: []
    property int initialyHighlightedIndex: -1
    
    signal listChanged

    ScrollView {
        id: scrollViewValues
        anchors.left: parent.left
        anchors.top: parent.top
        //anchors.bottom: paneRecipes.top
        anchors.topMargin: 24
        anchors.bottomMargin: 24
        //anchors.leftMargin: 48

        implicitWidth: 300
        implicitHeight: 500

        ListView {
            id: lvValues
            anchors.fill: parent

            model: allValues
            delegate: CheckDelegate {
                checkState: editListModel.itemSelected(name) ? Qt.Checked : Qt.Unchecked
                onClicked: editListModel.changeState(name, checked)
                width: lvValues.width - lvValues.leftMargin - lvValues.rightMargin
                
                text: name
            }
        }
    }

    footer: DialogButtonBox {
        id: buttons
        standardButtons: Dialog.Ok | Dialog.Cancel
    }

    onAboutToShow: {
        editListModel.beginEditList();
        //dlgTextField.forceActiveFocus();

        if(initialyHighlightedIndex != -1)
        {
            lvValues.positionViewAtIndex(initialyHighlightedIndex, ListView.Center)
        }
    }

    onAccepted: {
        if(editListModel.applyEditList())
        {
            listChanged();
        }
    }
    onRejected: editListModel.cancelEditList()
}
