import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.1
import Qt.labs.platform 1.1

import "components"

Item {
	id: rbConfigPage

	property int currentConfig: -1
	
	visible: currentConfig != -1

	onVisibleChanged: {
		if(visible)
		{
			modelRBConfigItems.setCurrentConfig(currentConfig)
			btnAddRecipe.enabled = modelRBConfigItems.canRecipesBeAdded()
		}
	}

	TextInputDialog {
		id: dlgAddHeader
		title: qsTr("Add header")
		onAccepted: {
			lvItems.currentIndex = modelRBConfigItems.addHeader(outputText, lvItems.currentIndex)
			lvItems.positionViewAtIndex(lvItems.currentIndex, ListView.Center)
		}
	}

	EditItemsListDialog {
		id: dlgAddRecipes
		useScrollBars: true
		title: qsTr("Add recipe")
		onListChanged: {
			lvItems.currentIndex = -1;
			btnAddRecipe.enabled = modelRBConfigItems.canRecipesBeAdded();
		}
	}
    
	TextInputDialog {
		id: dlgRenameHeader
		title: qsTr("Rename header")
		onAccepted: modelRBConfigItems.renameHeader(lvItems.currentIndex, outputText)
	}

	TextMessageDialog {
		id: dlgRemoveItem
		title: qsTr("Remove item")
		onAccepted: {
			modelRBConfigItems.removeItem(lvItems.currentIndex)
			lvItems.currentIndex = -1
			btnAddRecipe.enabled = modelRBConfigItems.canRecipesBeAdded()
		}
	}

	Label {
		id: labelConfig
        
		anchors.left: parent.left
		anchors.top: parent.top
		anchors.topMargin: 24
		anchors.leftMargin: 48
        
		text: qsTr("Configuration \"%1\"").arg(modelRecipeBookConfigurations.name(currentConfig))
		font.bold: true
	}

	GridLayout {
		id: grid
		anchors.left: parent.left
		anchors.top: labelConfig.bottom
		anchors.topMargin: 24
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		anchors.rightMargin: 24
        
		width: 375

		columns: 2

		Label { 
			text: qsTr("Title") 
		}
		TextField { 
			Layout.fillWidth: true
			selectByMouse: true

			ToolTip.delay: 1000
			ToolTip.timeout: 3000
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Recipe book title")

			text: modelRecipeBookConfigurations.title(currentConfig)
			onEditingFinished: modelRecipeBookConfigurations.setTitle(currentConfig, text)
		}

		Label { 
			Layout.rightMargin: 50

			text: qsTr("Subtitle") 
		}
		TextField { 
			Layout.fillWidth: true
			selectByMouse: true

			ToolTip.delay: 1000
			ToolTip.timeout: 3000
			ToolTip.visible: hovered
			ToolTip.text: qsTr("Recipe book subtitle")

			text: modelRecipeBookConfigurations.subtitle(currentConfig)
			onEditingFinished: modelRecipeBookConfigurations.setSubtitle(currentConfig, text)
		}

		Label { 
			text: qsTr("Font size") 
		}
		SpinBox { 
			from: 10
			to: 14
			wheelEnabled: true
			
			value: modelRecipeBookConfigurations.fontSize(currentConfig)
			onValueModified: modelRecipeBookConfigurations.setFontSize(currentConfig, value)
		}

		Label { 
			text: qsTr("Language") 
		}
		ComboBox { 
			Layout.fillWidth: true
			model: recipeBookExporter.availableLanguages()

			currentIndex: recipeBookExporter.getIndexFromLanguageCode(modelRecipeBookConfigurations.languageCode(currentConfig))
			onActivated: modelRecipeBookConfigurations.setLanguageCode(currentConfig, recipeBookExporter.getCodeFromLanguageName(currentText))
		}
	}

	ToolSeparator {
		anchors.left: grid.right
		anchors.top: labelConfig.bottom
		anchors.bottom: parent.bottom
		anchors.topMargin: 24
		anchors.leftMargin: 24
		anchors.bottomMargin: 24
	}

	// Rearrange recipe items and headers

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
				if(!isHeader)
				{
					held = true
					modelRBConfigItems.beginMove(index);
					lvItems.currentIndex = -1
				}
			}
			onReleased: {
				if(!isHeader)
				{
					held = false
					modelRBConfigItems.applyMove();
				}
			}
			onClicked: lvItems.currentIndex = index
            
			Rectangle {
				id: content

				anchors.horizontalCenter: parent.horizontalCenter
				anchors.verticalCenter: parent.verticalCenter

				width: dragArea.width; 
				height: column.implicitHeight + 47

				color: dragArea.held || lvItems.currentIndex == index ? "lightgray" : "transparent"
                
				Drag.active: dragArea.held
				Drag.source: dragArea
				Drag.hotSpot.x: width / 2
				Drag.hotSpot.y: height / 2

				states: State {
					when: dragArea.held

					ParentChange { target: content; parent: cookbookTab }
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
						visible: !isHeader

						fillMode: Image.PreserveAspectFit
						source: "qrc:/images/reorder.svg"
					}

					// Item name
					Label {
						id: labelColumn
						anchors.verticalCenter: parent.verticalCenter
						anchors.left: !isHeader ? reorderImage.right : parent.left

						anchors.margins: 15

						verticalAlignment: Text.AlignVCenter
						font.bold: isHeader
						text: everythingSet ? name : name + " (*)"
					}

					SpinBox { 
						anchors.verticalCenter: parent.verticalCenter
						anchors.right: parent.right
						anchors.rightMargin: 24

						from: 1
						to: maxHeaderLevel+1
						visible: isHeader
			
						value: headerLevel+1
						onValueModified: headerLevel = value - 1
					}
				}
			}

			DropArea {
				anchors { fill: parent; margins: 15 }

				onEntered: {
					modelRBConfigItems.updateMoveTarget(dragArea.DelegateModel.itemsIndex);
					rearrangeCurrentItemDelegateModel.items.move(drag.source.DelegateModel.itemsIndex,
																 dragArea.DelegateModel.itemsIndex);
				}
			}
		}
	}

	// Rearrange recipe item list view
	ListView {
		id: lvItems
		anchors.left: grid.right
		anchors.top: labelConfig.bottom
		anchors.bottom: groupItems.top
		anchors.topMargin: 24
		anchors.leftMargin: 48
		anchors.bottomMargin: 48
		width: 400
		
		ScrollBar.vertical: ScrollBar { }
		boundsBehavior: Flickable.StopAtBounds

		spacing: 0
		model: DelegateModel {
			id: rearrangeCurrentItemDelegateModel

			model: modelRBConfigItems
			delegate: dragDelegate
		}
	}

	Pane {
		id: groupItems
		anchors.left: lvItems.left
		anchors.right: lvItems.right
		anchors.bottom: parent.bottom

		RowLayout {
			anchors.centerIn: parent
			spacing: 20

			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"
				icon.color: "blue"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add header")

				onClicked: dlgAddHeader.open()
			}
			RoundButton {
				id: btnAddRecipe
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/add-black.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Add recipe")

				enabled: modelRBConfigItems.canRecipesBeAdded()
				onClicked: {
					filterModelUnusedRecipes.setRecipeBookConfiguration(currentConfig)

					dlgAddRecipes.editListModel = modelRBConfigItems;
					dlgAddRecipes.allValuesFilterModel = filterModelUnusedRecipes;

					dlgAddRecipes.open()
				}
			}

			RoundButton {
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/edit.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Rename header")

				enabled: modelRBConfigItems.isHeader(lvItems.currentIndex)
				onClicked: {
					dlgRenameHeader.initialText = modelRBConfigItems.name(lvItems.currentIndex);
					dlgRenameHeader.open()
				}
			}
			
			RoundButton { 
				display: AbstractButton.IconOnly
				icon.source: "qrc:/images/remove.svg"

				ToolTip.delay: 1000
				ToolTip.timeout: 5000
				ToolTip.visible: hovered
				ToolTip.text: qsTr("Remove item")

				enabled: lvItems.count > 0
				onClicked: {
					dlgRemoveItem.msgText = qsTr("This will remove the item \"%1\". Proceed?").arg(modelRBConfigItems.name(lvItems.currentIndex));
					dlgRemoveItem.open();
				}
			}
		}
	}

	ToolSeparator {
		anchors.left: lvItems.right
		anchors.top: labelConfig.bottom
		anchors.bottom: parent.bottom
		anchors.topMargin: 24
		anchors.leftMargin: 12
		anchors.bottomMargin: 24
	}

	Rectangle {
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.left: lvItems.right
		anchors.right: parent.right
		anchors.leftMargin: 24

		color: "transparent"

		Button {
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.verticalCenter: parent.verticalCenter

			width: 300
			height: 60

			font.capitalization: Font.MixedCase

			enabled: recipeBookSettings.pdfLatexExporterAvailable()

			ToolTip.delay: 1000
			ToolTip.timeout: 3000
			ToolTip.visible: hovered && !enabled
			ToolTip.text: qsTr("PDF Latex path has to be set in settings first")

			text: qsTr("Generate PDF")

			onClicked: {
				fileDialogExport.folder = "file:///" + recipeBookSettings.lastUsedRecipeBookConfigurationExportFolder()
				fileDialogExport.open()
			}

			FileDialog {
				id: fileDialogExport

				title: qsTr("Generate recipe book pdf")
				modality: Qt.WindowModal
				nameFilters: recipeBookExporter.getDlgNameFilters()
				fileMode: FileDialog.SaveFile
				onAccepted: recipeBookExporter.exportRecipeBook(file, currentConfig)
			}
		}
	}
}
