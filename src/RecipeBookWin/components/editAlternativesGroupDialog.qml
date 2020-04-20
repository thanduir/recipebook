import QtQuick.Dialogs 1.3
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14
import QtQuick.Layouts 1.14

Dialog {
    modal: true

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    TextInputDialog {
        id: dlgAddType
        title: qsTr("Add alternatives group")
        onCurrentTextChanged: currentTextAllowed = !alternativesTypes.existsType(outputText)
        onAccepted: {
            lvValues.currentIndex = alternativesTypes.addType(outputText)
            lvValues.positionViewAtIndex(lvValues.currentIndex, ListView.Center)
        }
    }

    TextInputDialog {
        id: dlgRenameType
        title: qsTr("Rename alternatives group")
        onCurrentTextChanged: currentTextAllowed = !alternativesTypes.existsType(outputText)
        onAccepted: {
            lvValues.currentIndex = alternativesTypes.renameType(lvValues.currentIndex, outputText)
            lvValues.positionViewAtIndex(lvValues.currentIndex, ListView.Center)
        }
    }
    
    TextMessageDialog {
        id: dlgRemoveRecipe
        title: qsTr("Remove alternatives group")
        onAccepted: {
            alternativesTypes.removeType(lvValues.currentIndex)
            lvValues.incrementCurrentIndex()
            lvValues.decrementCurrentIndex()
        }
    }

    ColorDialog {
        id: colorDialog
        title: "Choose alternatives group color"
        modality: Qt.WindowModal
        property int currentIndex: -1
        onAccepted: {
            alternativesTypes.setColor(currentIndex, colorDialog.color)
            
        }
    }

    ScrollView {
        id: scrollViewValues
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 24
        anchors.bottomMargin: 24

        implicitWidth: 300
        implicitHeight: 475

        ListView {
            id: lvValues
            anchors.fill: parent
            topMargin: 5
            leftMargin: 5
            bottomMargin: 5
            rightMargin: 5
            boundsBehavior: Flickable.StopAtBounds

            spacing: 5
            model: alternativesTypes

            delegate: ItemDelegate {
                id: listItemRecipeItem
                highlighted: ListView.isCurrentItem
                onPressed: lvValues.currentIndex = index
                width: lvValues.width - lvValues.leftMargin - lvValues.rightMargin
                height: listItem.height

                Item {
                    id: listItem
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 15

                    height: labelGroupName.height + 30

                    // group name
                    Label {
                        id: labelGroupName
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        
                        text: name
                        verticalAlignment: Text.AlignVCenter
                    }

                    // group symbol
                    Rectangle {
                        id: rowGroupHeaderButtons
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.topMargin: 0
                        anchors.rightMargin: 10
                        width: 20
                        height: width
                        radius: 0.5 * width

                        color: typeColor

                        MouseArea {
                            anchors.fill: parent
                            onClicked: { 
                                colorDialog.currentColor = typeColor
                                colorDialog.currentIndex = index
                                colorDialog.open();
                            }
                        }
                    }
                }
            }

            footerPositioning: ListView.OverlayFooter
            footer: Rectangle {
                z: 2
                color: "white"

                height: buttonAdd.height
                width: lvValues.width - lvValues.leftMargin - lvValues.rightMargin

                RowLayout {
                    spacing: 20
                    anchors.horizontalCenter: parent.horizontalCenter
                    
                    Button {
                        id: buttonAdd
                        text: qsTr("Add")
                        onClicked: dlgAddType.open()
                    }

                    Button {
                        text: qsTr("Rename")
                        onClicked: {
                            dlgRenameType.initialText = alternativesTypes.name(lvValues.currentIndex);
                            dlgRenameType.open();
                        }
                    }

                    Button {
                        text: qsTr("Remove")
                        enabled: lvValues.count > 0 && alternativesTypes.canTypeBeRemoved(lvValues.currentIndex)
                        onClicked: {
                            dlgRemoveRecipe.msgText = qsTr("This will remove the alternatives group \"" + alternativesTypes.name(lvValues.currentIndex) + "\". Proceed?");
                            dlgRemoveRecipe.open();
                        }
                    }
                }
            }
        }
    }

    footer: DialogButtonBox {
        Button {
            text: qsTr("Close")
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
            flat: true
        }
    }
}
