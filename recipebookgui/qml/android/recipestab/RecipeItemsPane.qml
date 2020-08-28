import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.14

Item {
	id: recipeItemsPage

	property int currentRecipe : -1

	// General rounding method
	function roundValue(value) {
		if(Math.abs(value - Math.round(value)) <= 0.0001)
		{
			return Number(value).toLocaleString(Qt.locale("en_US"), 'f', 0);
		}
		else if(Math.abs(value*10 - Math.round(value * 10)) <= 0.0001)
		{
			return Number(value).toLocaleString(Qt.locale("en_US"), 'f', 1);
		}
		else if(Math.abs(value*100 - Math.round(value * 100)) <= 0.0001)
		{
			return Number(value).toLocaleString(Qt.locale("en_US"), 'f', 2);
		}
		else
		{
			return Number(value).toLocaleString(Qt.locale("en_US"), 'f', 3);
		}
	}

	// Pane

	RowLayout {
		id: grid
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.leftMargin: 10
		anchors.rightMargin: 10

		visible: currentRecipe != -1

		Label {
			text: qsTr("Number of persons")
		}
		Item {
			Layout.fillWidth: true
		}
		SpinBox {
			from: 1
			to: 50
			editable: true
			wheelEnabled: true
			value: modelRecipes.numberOfPersons(currentRecipe)

			onValueModified: modelRecipes.setNumberOfPersons(currentRecipe, value)
		}
	}

	Switch {
		id: idRearrangeCurrentItems
		anchors.right: lvCurrentRecipe.right
		anchors.top: grid.bottom

		visible: currentRecipe != -1

		text: qsTr("Rearrange recipe items")
	}

	// Edit recipe item list view
	ListView {
		id: lvCurrentRecipe
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: idRearrangeCurrentItems.bottom
		anchors.bottom: parent.bottom
		anchors.topMargin: 24
		anchors.bottomMargin: 24
		visible: !idRearrangeCurrentItems.checked

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
		model: modelRecipeItems
		delegate: SwipeDelegate {
			id: listItemRecipeItem
			highlighted: ListView.isCurrentItem
			onClicked: lvCurrentRecipe.currentIndex == index ? lvCurrentRecipe.currentIndex = -1 : lvCurrentRecipe.currentIndex = index
			width: lvCurrentRecipe.width - lvCurrentRecipe.leftMargin - lvCurrentRecipe.rightMargin
			implicitHeight: listItemRecipeItemGroup.implicitHeight

			contentItem: Item {
				id: listItemRecipeItemGroup
				implicitHeight: listItemRecipeItemName.height + 15 + (highlighted ? listItemGridRecipeItem.height + 10 : 0)

				// Group bar
				Rectangle {
					id: groupBar
					anchors.left: parent.left
					anchors.top: parent.top
					anchors.bottom: parent.bottom
					anchors.topMargin: -8
					anchors.bottomMargin: -8

					visible: hasGroup
					color: groupColor
					width: 5
				}

				// Ingredient / group name
				Label {
					id: listItemRecipeItemName
					anchors.left: hasGroup ? groupBar.right : parent.left
					anchors.leftMargin: hasGroup ? 5 : 0

					font.bold: !optional
					font.italic: optional
					text: name
					verticalAlignment: Text.AlignVCenter
				}

				// Summary for inactive ingredients
				Label {
					function recipeItemSmallDesc(n){
						var text = "";
						if(amountUnit !== modelRecipeItems.indexUnitUnitless())
						{
							text += amountMin;
							if(amountIsRange)
							{
								text += "-" + amountMax;
							}
							text += " " + uiStrings.getUnitShortName(amountUnit);
						}
						if(sizeIndex != 1)
						{
							if(text !== "")
							{
								text = text + ", ";
							}
							text += uiStrings.getSizeName(sizeIndex, amountUnit)
						}
						if(additionalInfo != "")
						{
							if(text !== "")
							{
								text = text + ", ";
							}
							text += additionalInfo;
						}

						if(text !== "")
						{
							text = " (" + text + ")";
						}
						return text;
					}

					anchors.left: listItemRecipeItemName.right
					color: "gray"
					text: recipeItemSmallDesc(index)
					verticalAlignment: Text.AlignVCenter
					font.italic: optional
					width: parent.width - listItemRecipeItemName.width + 10
					wrapMode: Label.WordWrap
					visible: !listItemRecipeItem.highlighted
				}

				// Extended information for active ingredient elements
				GridLayout {
					id: listItemGridRecipeItem
					visible: listItemRecipeItem.highlighted

					anchors.left: parent.left
					anchors.right: parent.right
					anchors.top: listItemRecipeItemName.bottom
					anchors.leftMargin: 20
					anchors.rightMargin: 10
					anchors.topMargin: 10

					columns: 2
					columnSpacing: 10

					Label {
						text: qsTr("Amount")
					}
					RowLayout {
						Layout.fillWidth: true

						ComboBox {
							Layout.fillWidth: true
							model: uiStrings.getAllUnitNames()
							currentIndex: amountUnit
							onActivated: amountUnit = currentIndex
						}

						CheckBox {
							visible: amountUnit !== modelRecipeItems.indexUnitUnitless()

							text: qsTr("Range")
							checked: amountIsRange

							onClicked: amountIsRange = checked
						}
					}

					Label {
						visible: amountUnit !== modelRecipeItems.indexUnitUnitless()
						text: " "
					}
					GridLayout {
						visible: amountUnit !== modelRecipeItems.indexUnitUnitless()
						columns: 2
						columnSpacing: 10

						Label {
							visible: amountIsRange
							text: qsTr("Min.")
						}

						RowLayout {
							RoundButton {
								display: AbstractButton.IconOnly
								icon.source: "qrc:/images/remove-black.svg"
								flat: true

								onClicked: modelRecipeItems.decreaseAmountMin(lvCurrentRecipe.currentIndex)
							}

							TextField {
								Layout.preferredWidth: 50
								inputMethodHints: Qt.ImhFormattedNumbersOnly

								selectByMouse: true
								horizontalAlignment: TextInput.AlignHCenter
								onFocusChanged: {
									if(focus)
										selectAll()
								}

								text: roundValue(amountMin)
								validator: DoubleValidator { bottom: 0; top: 9999; decimals: 3; locale: "en_US" }
								onTextEdited: {
									if(text === "")
									{
										return;
									}

									if(amountIsRange && text > amountMax)
									{
										amountMax = text;
									}
									amountMin = text;
								}
							}

							RoundButton {
								display: AbstractButton.IconOnly
								icon.source: "qrc:/images/add-black.svg"
								flat: true

								onClicked: modelRecipeItems.increaseAmountMin(lvCurrentRecipe.currentIndex)
							}
						}

						Label {
							visible: amountUnit !== modelRecipeItems.indexUnitUnitless() && amountIsRange
							text: qsTr("Max.")
						}

						RowLayout {
							visible: amountUnit !== modelRecipeItems.indexUnitUnitless() && amountIsRange

							RoundButton {
								display: AbstractButton.IconOnly
								icon.source: "qrc:/images/remove-black.svg"
								flat: true

								onClicked: modelRecipeItems.decreaseAmountMax(lvCurrentRecipe.currentIndex)
							}

							TextField {
								Layout.preferredWidth: 50
								inputMethodHints: Qt.ImhFormattedNumbersOnly

								selectByMouse: true
								horizontalAlignment: TextInput.AlignHCenter
								onFocusChanged: {
									if(focus)
										selectAll()
								}

								validator: DoubleValidator { bottom: 0; top: 9999; decimals: 3; locale: "en_US" }
								text: roundValue(amountMax)
								onTextEdited: {
									if(text === "")
									{
										return;
									}

									if(amountIsRange && text < amountMin)
									{
										amountMin = text;
									}
									amountMax = text
								}
							}

							RoundButton {
								display: AbstractButton.IconOnly
								icon.source: "qrc:/images/add-black.svg"
								flat: true

								onClicked: modelRecipeItems.increaseAmountMax(lvCurrentRecipe.currentIndex)
							}
						}
					}

					Label {
						text: qsTr("Size")
					}
					SpinBox {
						from: 0
						to: uiStrings.getSizesCount() - 1

						textFromValue: function(value, locale) {
							return uiStrings.getSizeName(value, amountUnit);
						}
						valueFromText: function(text, locale) {
							return uiStrings.getSizeIndex(text);
						}

						value: sizeIndex
						onValueModified: sizeIndex = value
					}

					Label {
						text: qsTr("Add. Info")
					}
					TextField {
						Layout.fillWidth: true
						selectByMouse: true

						text: additionalInfo
						onTextEdited: additionalInfo = text
					}

					Label {
						text: " "
					}
					CheckBox {
						text: qsTr("Optional")
						checked: optional

						onClicked: optional = checked
					}

					Label { text: qsTr("Group") }
					ComboBox {
						Layout.fillWidth: true

						model: alternativesGroups
						textRole: "name"
						valueRole: "name"

						currentIndex: indexOfValue(group)
						onActivated: {
							lvCurrentRecipe.currentIndex = modelRecipeItems.setGroup(lvCurrentRecipe.currentIndex, currentText)
						}

						onVisibleChanged: {
							if(visible)
								currentIndex = indexOfValue(group)
						}
					}
				}
			}

			Timer {
				id: undoTimer
				interval: 2750
				onTriggered: {
					modelRecipeItems.removeItem(index)
				}
			}

			swipe.onCompleted: {
				undoTimer.start()
				if(index == lvCurrentRecipe.currentIndex)
				{
					lvCurrentRecipe.currentIndex = -1
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

					visible: !listItemRecipeItem.swipe.complete

					fillMode: Image.PreserveAspectFit
					source: "qrc:/images/remove.svg"
				}
				Label {
					anchors.left: parent.left
					anchors.leftMargin: 10
					anchors.verticalCenter: parent.verticalCenter
					text: qsTr("Ingredient \"%1\" removed").arg(name)

					visible: listItemRecipeItem.swipe.complete
				}
				RoundButton {
					text: qsTr("Undo")

					anchors.right: parent.right
					anchors.verticalCenter: parent.verticalCenter

					onClicked: {
						undoTimer.stop()
						listItemRecipeItem.swipe.close()
					}

					visible: listItemRecipeItem.swipe.complete
				}
			}
		}
	}

	// Rearrange recipe item list moveable delegate
	Component {
		id: dragDelegate

		MouseArea {
			id: dragArea

			property bool held: false

			anchors { left: parent != null ? parent.left : undefined; right: parent != null ? parent.right : undefined }
			height: content.height

			drag.target: held ? content : undefined
			drag.axis: Drag.YAxis

			onPressed: {
				held = true
				filterModelRecipeItems.beginMove(index);
				
			}
			onReleased: {
				held = false
				filterModelRecipeItems.applyMove();
			}
            
			Rectangle {
				id: content

				anchors.horizontalCenter: parent.horizontalCenter
				anchors.verticalCenter: parent.verticalCenter

				width: dragArea.width; 
				height: column.implicitHeight + 47

				color: dragArea.held ? "lightgray" : "transparent"
                
				Drag.active: dragArea.held
				Drag.source: dragArea
				Drag.hotSpot.x: width / 2
				Drag.hotSpot.y: height / 2

				states: State {
					when: dragArea.held

					ParentChange { target: content; parent: recipeItemsPage }
					AnchorChanges {
						target: content
						anchors { horizontalCenter: undefined; verticalCenter: undefined }
					}
				}

				Item {
					id: column
					anchors.left: parent.left
					anchors.right: parent.right
					anchors.verticalCenter: parent.verticalCenter

					// Move image
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

					Rectangle {
						id: reorderGroupBar
						anchors.left: reorderImage.right
						anchors.top: parent.top
						anchors.leftMargin: 10
						anchors.topMargin: -20
						anchors.bottomMargin: 20

						visible: hasGroup
						color: groupColor
						height: content.height
						width: 5
					}

					// Item name
					Label {
						id: labelColumn
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: hasGroup ? reorderGroupBar.right : reorderImage.right

						anchors.margins: 15

						verticalAlignment: Text.AlignVCenter
						font.bold: !optional
						font.italic: optional
						text: groupOrItemName
					}
				}
			}

			DropArea {
				anchors { fill: parent; margins: 15 }

				onEntered: {
					filterModelRecipeItems.updateMoveTarget(dragArea.DelegateModel.itemsIndex);
					rearrangeCurrentItemDelegateModel.items.move(drag.source.DelegateModel.itemsIndex,
																 dragArea.DelegateModel.itemsIndex);
				}
			}
		}
	}

	// Rearrange recipe item list view
	ListView {
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: idRearrangeCurrentItems.bottom
		anchors.bottom: parent.bottom
		anchors.topMargin: 24
		anchors.bottomMargin: 24
		visible:  idRearrangeCurrentItems.checked

		spacing: 0
		model: DelegateModel {
			id: rearrangeCurrentItemDelegateModel

			model: filterModelRecipeItems
			delegate: dragDelegate
		}
	}
}
