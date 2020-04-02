import QtQuick 2.14
import QtQuick.Controls 2.14

Dialog {
    modal: true

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: 300

    property string initialText: ""
    property alias outputText: dlgTextField.text
    property alias validator: dlgTextField.validator
    
    TextField { 
        id: dlgTextField
        anchors.centerIn: parent
        width: parent.width       
        // TODO: This doesn't work for umlauts. Maybe replace with a negative regex (not ...). Also: 
        validator: RegularExpressionValidator { regularExpression: /[0-9a-zA-Z������ ()._-]+$/ }

        onTextEdited: {
            buttons.standardButton(Dialog.Ok).enabled = text != "" && !modelIngredients.existsIngredient(text);
        }
    }

    footer: DialogButtonBox {
        id: buttons
        standardButtons: Dialog.Ok | Dialog.Cancel
    }

    onAboutToShow: {
        dlgTextField.text = initialText;
        buttons.standardButton(Dialog.Ok).enabled = false;
        dlgTextField.forceActiveFocus();
    }  
}