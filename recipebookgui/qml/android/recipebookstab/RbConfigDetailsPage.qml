import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Item {
	id: rbConfigPage

	property int currentConfig: -1
	
	visible: currentConfig != -1

	onVisibleChanged: {
		if(visible)
		{
			modelRBConfigItems.setCurrentConfig(currentConfig)
		}
	}

	GridLayout {
		id: grid
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: parent.top
		anchors.leftMargin: 10
		anchors.rightMargin: 10

		columns: 2

		Label { 
			text: qsTr("Title") 
		}
		TextField { 
			Layout.fillWidth: true
			selectByMouse: true

			placeholderText: qsTr("Recipe book title")

			text: modelRecipeBookConfigurations.title(currentConfig)
			onEditingFinished: modelRecipeBookConfigurations.setTitle(currentConfig, text)
		}

		Label { 
			Layout.rightMargin: 30

			text: qsTr("Subtitle") 
		}
		TextField { 
			Layout.fillWidth: true
			selectByMouse: true

			placeholderText: qsTr("Recipe book subtitle")

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
	}	
}
