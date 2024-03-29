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
			changeCurrentIngredient(filterModelIngredients.addIngredient(outputText));
			lvIngredients.positionViewAtIndex(lvIngredients.currentIndex, ListView.Center)
		}
	}

	TextInputDialog {
		id: dlgRenameIngredient
		title: qsTr("Rename ingredient")
		onCurrentTextChanged: currentTextAllowed = !filterModelIngredients.existsIngredient(outputText)
		onAccepted: {
			changeCurrentIngredient(filterModelIngredients.renameIngredient(lvIngredients.currentIndex, outputText));
			lvIngredients.positionViewAtIndex(lvIngredients.currentIndex, ListView.Center)
		}
	}
    
    TextMessageDialog {
        id: dlgCantRemoveIngredient
        title: qsTr("Ingredient can't be removed")
        okOnly: true
    }

    // Header Component

    onVisibleChanged: {
        if(visible)
        {
            headerSubpageSpace.sourceComponent = ingredientHeaderComponent;

			textFilterIngredients.text = "";
			filterModelIngredients.setFilterString("");
			changeCurrentIngredient(-1);
        }
    }

    Component {
        id: ingredientHeaderComponent

        ToolButton {
            anchors.right: parent.right
            anchors.rightMargin: 10

            display: AbstractButton.IconOnly
            icon.source: "qrc:/images/add-black.svg"

            onVisibleChanged: {
                if(visible)
                {
                    enabled = filterModelIngredients.canIngredientsBeAdded();
                }
            }
            onClicked: dlgAddIngredient.open()
        }
    }

    // Main page

    TextField {
		id: textFilterIngredients
		anchors.left: lvIngredients.left
		anchors.right: lvIngredients.right
        anchors.top: parent.top
        selectByMouse: true

        placeholderText: qsTr("Filter ingredients")

		onTextEdited: {
			filterModelIngredients.setFilterString(text);
			changeCurrentIngredient(-1)
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
					changeCurrentIngredient(-1);
					textFilterIngredients.forceActiveFocus()
				}
			}
		}
	}

	function changeCurrentIngredient(myIndex: int) {
		if(lvIngredients.currentIndex === myIndex)
		{
			if(myIndex !== -1)
			{
				lvIngredients.currentItem.switchExtendedInfo(false);
			}
			lvIngredients.currentIndex = -1;
		}
		else
		{
			if(lvIngredients.currentIndex != -1)
			{
				lvIngredients.currentItem.switchExtendedInfo(false);
			}

			lvIngredients.currentIndex = myIndex;
			if(myIndex !== -1)
			{
				lvIngredients.currentItem.switchExtendedInfo(true);
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
		ScrollIndicator.vertical: ScrollIndicator { }

		Component.onCompleted: changeCurrentIngredient(-1)
        Connections {
            target: modelIngredients
            function onModelReset() {
				changeCurrentIngredient(-1);
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

		spacing: 0
		model: filterModelIngredients
		delegate: SwipeDelegate {
            id: listIngredientsItem
            highlighted: ListView.isCurrentItem
            width: lvIngredients.width - lvIngredients.leftMargin - lvIngredients.rightMargin
			onClicked: changeCurrentIngredient(index)

			function switchExtendedInfo(enable: bool) {
				loaderExtendedInfo.sourceComponent = enable ? componentExtendedInfo : undefined;
			}

			implicitHeight: listIngredientsItemGroup.implicitHeight
			contentItem: Item {
				id: listIngredientsItemGroup
				implicitHeight: listItemDelegateName.height + 15 + (highlighted ? loaderExtendedInfo.height + 10 : 0)

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

					Dialog {
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
                            ScrollIndicator.vertical: ScrollIndicator { }

                            model: filterModelIngredients.listUsedInRecipes(lvIngredients.currentIndex)
                            delegate: Label {
                                text: modelData
                            }
                        }

						footer: DialogButtonBox {
							Button {
								text: qsTr("Close")
								DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
								flat: true
							}
						}
                    }
                }

				Loader {
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.top: listItemDelegateName.bottom

					id: loaderExtendedInfo
				}

				// Extended information for active ingredient
				Component {
					id: componentExtendedInfo

					GridLayout {
						id: listItemGridIngredient

						Component.onCompleted: {
							cbxCategory.currentIndex = cbxCategory.indexOfValue(filterModelIngredients.category(lvIngredients.currentIndex))
							cbxUnit.currentIndex = cbxUnit.indexOfValue(filterModelIngredients.defaultUnit(lvIngredients.currentIndex))
						}

						anchors.left: parent.left
						anchors.right: parent.right
						anchors.top: parent.top
						anchors.leftMargin: 10
						anchors.topMargin: 10

						columns: 2

						Label {
							text: qsTr("Category")
						}
						ComboBox {
							id: cbxCategory
							Layout.fillWidth: true
							model: modelCategories
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

							ScrollIndicator.horizontal: ScrollIndicator { }

							model: modelSortOrders
							delegate: CheckBox {
								id: cbxItemName
								text: name

								width: lvSortOrders.cellWidth

								checked: filterModelIngredients.provenanceAvailable(lvIngredients.currentIndex, name)
								onClicked: filterModelIngredients.setProvenanceAvailable(lvIngredients.currentIndex, name, checked)

							}
						}

						Label { text: qsTr("Unit") }
						ComboBox {
							id: cbxUnit
							Layout.fillWidth: true
							model: uiStrings.getAllUnitNames()
							onActivated: filterModelIngredients.setDefaultUnit(lvIngredients.currentIndex, currentText)
						}
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
						changeCurrentIngredient(-1)
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

					SwipeDelegate.onClicked: {
						undoTimer.stop()
                        listIngredientsItem.swipe.close()
					}

                    visible: listIngredientsItem.swipe.complete
				}
			}
        }
	}
}
