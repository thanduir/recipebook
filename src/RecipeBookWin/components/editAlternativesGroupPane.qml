import QtQuick.Dialogs 1.3
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {
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

	ListView {
		id: lvValues
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: groupButtons.top
		topMargin: 5
		leftMargin: 5
		rightMargin: 5
		anchors.bottomMargin: 48

		ScrollBar.vertical: ScrollBar { }
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
				anchors.left: listItemRecipeItem.left
				anchors.right: listItemRecipeItem.right
				anchors.top: listItemRecipeItem.top
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
					anchors.right: listItem.right
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
	}

	Pane {
		id: groupButtons
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom

		RowLayout {
			anchors.fill: parent
			//spacing: 20
                    
			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add alternatives group")

				onClicked: dlgAddType.open()
			}

			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/edit.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Rename alternatives group")

				onClicked: {
					dlgRenameType.initialText = alternativesTypes.name(lvValues.currentIndex);
					dlgRenameType.open();
				}
			}

			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/remove.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Remove alternatives group")

				enabled: lvValues.count > 0 && alternativesTypes.canTypeBeRemoved(lvValues.currentIndex)
				onClicked: {
					dlgRemoveRecipe.msgText = qsTr("This will remove the alternatives group \"" + alternativesTypes.name(lvValues.currentIndex) + "\". Proceed?");
					dlgRemoveRecipe.open();
				}
			}
		}
	}
}
