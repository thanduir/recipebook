import QtQuick 2.14
import QtQuick.Controls 2.14

Dialog {
    id: dlgRemoveIngredient
    modal: true

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    property alias msgText: dlgAskLabel.text
    
    Label { 
        id: dlgAskLabel
        anchors.centerIn: parent
        width: parent.width
    }

    footer: DialogButtonBox {
        id: buttons
        standardButtons: Dialog.Yes | Dialog.No
    }
}