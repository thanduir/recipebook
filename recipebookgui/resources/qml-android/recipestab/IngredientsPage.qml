import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "components"

Item {
    
	TextInputDialog {
		id: dlgAddIngredient
		title: qsTr("Add ingredient")
		onCurrentTextChanged: currentTextAllowed = !filterModelIngredients.existsIngredient(outputText)
		onAccepted: {
			lvIngredients.currentIndex = filterModelIngredients.addIngredient(outputText)
			lvIngredients.positionViewAtIndex(lvIngredients.currentIndex, ListView.Center)
		}
	}

	TextInputDialog {
		id: dlgRenameIngredient
		title: qsTr("Rename ingredient")
		onCurrentTextChanged: currentTextAllowed = !filterModelIngredients.existsIngredient(outputText)
		onAccepted: {
			lvIngredients.currentIndex = filterModelIngredients.renameIngredient(lvIngredients.currentIndex, outputText)
			lvIngredients.positionViewAtIndex(lvIngredients.currentIndex, ListView.Center)
		}
	}
    
    TextMessageDialog {
        id: dlgCantRemoveIngredient
        title: qsTr("Ingredient can't be removed")
        okOnly: true
    }

    // TODO: Textfield handling doesn't work as expected on android tablet for some reason
    TextField {
		id: textFilterIngredients
		anchors.left: lvIngredients.left
		anchors.right: lvIngredients.right
        anchors.top: parent.top
        selectByMouse: true

        placeholderText: qsTr("Filter ingredients")

		onTextEdited: {
			filterModelIngredients.setFilterString(text);
			lvIngredients.currentIndex = -1
			forceActiveFocus();
		}

		Image {
			anchors { top: parent.top; right: parent.right }
			id: clearText
			fillMode: Image.PreserveAspectFit
			visible: textFilterIngredients.text
			source: "qrc:/images/backspace.svg"
			height: parent.height - 5

			MouseArea {
				id: clear
				anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
				height: textFilterIngredients.height; width: textFilterIngredients.height
				onClicked: {
					textFilterIngredients.text = ""
					filterModelIngredients.setFilterString(textFilterIngredients.text);
					lvIngredients.currentIndex = -1
					textFilterIngredients.forceActiveFocus()
				}
			}
		}
	}

	ListView {
		id: lvIngredients
		anchors.left: parent.left
        anchors.right: parent.right
		anchors.top: textFilterIngredients.bottom 
        anchors.bottom: parent.bottom
        anchors.topMargin: 24
        anchors.bottomMargin: 24

		boundsBehavior: Flickable.StopAtBounds
		ScrollBar.vertical: ScrollBar { }

        Component.onCompleted: lvIngredients.currentIndex = -1
        Connections {
            target: modelIngredients
            function onModelReset() {
                lvIngredients.currentIndex = -1
            }
        }

        remove: Transition {
            SequentialAnimation {
                PauseAnimation { duration: 125 }
                NumberAnimation { property: "height"; to: 0; easing.type: Easing.InOutQuad }
            }
        }

        displaced: Transition {
            SequentialAnimation {
                PauseAnimation { duration: 125 }
                NumberAnimation { property: "y"; easing.type: Easing.InOutQuad }
            }
        }

        // TODO: Binding loop for height... Why?
        spacing: 0
		model: filterModelIngredients
        delegate: SwipeDelegate {
            id: listIngredientsItem
            highlighted: ListView.isCurrentItem
            width: lvIngredients.width - lvIngredients.leftMargin - lvIngredients.rightMargin
            onClicked: lvIngredients.currentIndex == index ? lvIngredients.currentIndex = -1 : lvIngredients.currentIndex = index
            height: listIngredientsItemGroup.height
                
            contentItem: Item {
                id: listIngredientsItemGroup
                height: listItemDelegateName.height + 20 + (highlighted ? listItemGridIngredient.height + 10 : 0)

                // Ingredient name
				Label {
					id: listItemDelegateName
                    anchors.left: parent.left
                    text: name

                    MouseArea {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        onPressAndHold: {
                            if(listIngredientsItem.highlighted)
                            {
                                dlgRenameIngredient.initialText = filterModelIngredients.name(lvIngredients.currentIndex);
                                dlgRenameIngredient.open();
                            }
                            else
                            {
                                mouse.accepted = false
                            }
                        }
                        onClicked: listIngredientsItem.clicked()

                        onPressed: if(!listIngredientsItem.highlighted) { mouse.accepted = false; }
                        onReleased: if(!listIngredientsItem.highlighted) { mouse.accepted = false; }
                        onDoubleClicked: if(!listIngredientsItem.highlighted) { mouse.accepted = false; }
                        onPositionChanged: if(!listIngredientsItem.highlighted) { mouse.accepted = false; }
                    }
				}

                // Summary for inactive ingredients
				Label {
					anchors.left: listItemDelegateName.right
					color: "gray"
					text: " (" + category + ", " + defaultUnit + ")"

                    width: parent.width - listItemDelegateName.width - 10
					wrapMode: Label.WordWrap
                    visible: !listIngredientsItem.highlighted
				}

                RoundButton {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: -10
                    anchors.rightMargin: -10

                    flat: true
                    display: AbstractButton.IconOnly
                    icon.source: "qrc:/images/info.svg"

                    visible: listIngredientsItem.highlighted && gridUsedInRecipes.count > 0
                    onClicked: popupUsedIn.open()

                    Popup {
                        id: popupUsedIn

                        anchors.centerIn: Overlay.overlay
                        modal: true

                        implicitWidth: gridUsedInRecipes.width
                        implicitHeight: lblUsedIn.height + gridUsedInRecipes.height + 60

                        Label {
                            id: lblUsedIn
                            anchors.left: parent.left
                            anchors.right: parent.right
                            font.bold: true
                            wrapMode: Text.Wrap
                            text: qsTr("%1 is used in the following recipes:").arg(name)
                        }
                        GridView {
                            id: gridUsedInRecipes
                            anchors.top: lblUsedIn.bottom
                            anchors.topMargin: 30
                            anchors.left: parent.left
                            anchors.leftMargin: 10

                            width: listIngredientsItem.width - 40

                            visible: count > 0

                            height: 210

                            cellWidth: 275
                            cellHeight: 30
                            flow: GridView.FlowLeftToRight
                            ScrollBar.vertical: ScrollBar { }

                            model: filterModelIngredients.listUsedInRecipes(lvIngredients.currentIndex)
                            delegate: Label {
                                text: modelData
                            }
                        }
                    }
                }

                // Extended information for active ingredient
                GridLayout {
                    id: listItemGridIngredient
                    visible: listIngredientsItem.highlighted

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: listItemDelegateName.bottom
                    anchors.leftMargin: 10
                    anchors.topMargin: 10

                    columns: 2

                    Label {
                        text: qsTr("Category")
                    }
                    ComboBox {
                        Layout.fillWidth: true
                        model: modelCategories
                        currentIndex: indexOfValue(filterModelIngredients.category(lvIngredients.currentIndex))
                        onActivated: filterModelIngredients.setCategory(lvIngredients.currentIndex, currentText)
                    }

                    Label {
                        Layout.rightMargin: 25
                        Layout.topMargin: 10
                        Layout.alignment: Qt.AlignTop
                        text: qsTr("Provenance")
                    }
                    GridView {
                        id: lvSortOrders
                        Layout.fillWidth: true
                        height: 80
                        cellWidth: 155
                        cellHeight: 40
                        flow: GridView.FlowTopToBottom

                        ScrollBar.horizontal: ScrollBar { }

                        model: modelSortOrders
                        delegate: CheckBox {
                            id: cbxItemName
                            text: name

                            width: lvSortOrders.cellWidth

                            checked: modelIngredients.provenanceAvailable(lvIngredients.currentIndex, name)
                            onClicked: modelIngredients.setProvenanceAvailable(lvIngredients.currentIndex, name, checked)

                        }
                    }

                    Label { text: qsTr("Unit") }
                    ComboBox {
                        id: cbxUnit
                        Layout.fillWidth: true
                        model: uiStrings.getAllUnitNames()
                        currentIndex: indexOfValue(filterModelIngredients.defaultUnit(lvIngredients.currentIndex))
                        onActivated: filterModelIngredients.setDefaultUnit(lvIngredients.currentIndex, currentText)
                    }
                }
			}

            Timer {
                id: undoTimer
                interval: 2750
                onTriggered: {
                    filterModelIngredients.removeIngredient(index)
                }
            }

            swipe.onCompleted: {
                if(!filterModelIngredients.canIngredientBeRemoved(index))
                {
                    listIngredientsItem.swipe.close()
                    dlgCantRemoveIngredient.msgText = qsTr("Ingredient \"%1\" is still in use and can't be removed.").arg(name);
                    dlgCantRemoveIngredient.open();
                }
                else
                {
                    undoTimer.start()
                    if(index == lvIngredients.currentIndex)
                    {
                        lvIngredients.currentIndex = -1
                    }
                }
            }

            swipe.left: Rectangle {
                width: parent.width
                height: parent.height

                clip: true
                color: "darkgray"

                Image {
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter

                    visible: !listIngredientsItem.swipe.complete

                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/images/remove.svg"
                }
                Label {
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Ingredient \"%1\" removed").arg(name)

                    visible: listIngredientsItem.swipe.complete
                }
                RoundButton {
                    text: qsTr("Undo")

                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    onClicked: {
                        undoTimer.stop()
                        listIngredientsItem.swipe.close()
                    }

                    visible: listIngredientsItem.swipe.complete
                }
            }
        }
	}

    /*
    // TODO: Where to put this? Button in header row? Floating action button?
    RoundButton {
        id: buttonAdd

        display: AbstractButton.IconOnly
        icon.source: "qrc:/images/add-black.svg"

        ToolTip.delay: 1000
        ToolTip.timeout: 5000
        ToolTip.visible: hovered
        ToolTip.text: qsTr("Add ingredient")

        onVisibleChanged: {
            if(visible)
            {
                enabled = filterModelIngredients.canIngredientsBeAdded();
            }
        }
        onClicked: dlgAddIngredient.open()
    }*/
}
