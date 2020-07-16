import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "components"

Item {
    onVisibleChanged: {
        if(visible) {
            textUID.text = recipeBookSettings.getApplicationInstanceUID();
            cbxDefaultUnit.currentIndex = recipeBookSettings.getDefaultUnit();
            spinDefaultNrPersons.value = recipeBookSettings.getDefaultRecipeNrPersons();

            cbxDefaulCategory.currentIndex = cbxDefaulCategory.indexOfValue(recipeBookSettings.getDefaultCategory());
            cbxDefaultSortOrder.currentIndex = cbxDefaultSortOrder.indexOfValue(recipeBookSettings.getDefaultSortOrder());

            cbxLanguages.currentIndex = cbxLanguages.indexOfValue(RBLanguageManager.getCurrentLanguageName());
        }
    }

	TextMessageDialog {
		id: dlgLoadDefaultData
		title: qsTr("Load default data")
		msgText: qsTr("This will replace current data with the default data. Current data will be lost. Proceed?");
		onAccepted: recipeBookSettings.loadDefaultData()
	}

	TextMessageDialog {
		id: dlgResetData
		title: qsTr("Reset all data")
		msgText: qsTr("This will reset all data. Current data will be lost. Proceed?");
		onAccepted: recipeBookSettings.resetAllData()
    }

    Flickable {
		anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
		width: paneSettings.width

        contentWidth: paneSettings.width
        contentHeight: paneSettings.height
        boundsBehavior: Flickable.StopAtBounds

        Pane {
            id: paneSettings
            anchors.top: parent.top
			anchors.horizontalCenter: parent.horizontalCenter
			width: groupDefaultValues.width + 12
			height: groupSettings.height + groupDefaultValues.height + groupData.height + rowUID.height + 48 + 12 + 36

            GroupBox {
                id: groupSettings
				anchors.horizontalCenter: parent.horizontalCenter

                title: qsTr("Settings")

				ColumnLayout {
                    anchors.fill: parent

					Label {
                        text: qsTr("Current language")
                    }
                    ComboBox {
                        id: cbxLanguages
                        Layout.fillWidth: true
                        model: RBLanguageManager.availableLanguages()

                        onActivated: RBLanguageManager.setCurrentLanguageIndex(currentIndex)
                    }
                }
            }

            GroupBox {
                id: groupDefaultValues
                anchors.top: groupSettings.bottom
				anchors.topMargin: 24
				anchors.horizontalCenter: parent.horizontalCenter

				title: qsTr("Default values")

				ColumnLayout {
                    anchors.fill: parent

					Label {
                        text: qsTr("Default unit")
                    }
                    ComboBox {
                        id: cbxDefaultUnit
                        Layout.fillWidth: true
                        model: uiStrings.getAllUnitNames()

                        onActivated: recipeBookSettings.setDefaultUnit(currentIndex)
                    }

                    Label {
                        text: qsTr("Default number of persons")
                    }
                    SpinBox {
                        id: spinDefaultNrPersons
                        from: 1
                        to: 50
                        editable: true
                        wheelEnabled: true

                        onValueModified: recipeBookSettings.setDefaultRecipeNrPersons(value)
                    }

                    Label {
                        text: qsTr("Default category")
                    }
                    ComboBox {
                        id: cbxDefaulCategory
                        Layout.fillWidth: true
                        model: modelCategories

                        onActivated: recipeBookSettings.setDefaultCategory(currentText)
                    }

                    Label {
                        text: qsTr("Default sort order")
                    }
                    ComboBox {
                        id: cbxDefaultSortOrder
                        Layout.fillWidth: true
                        model: modelSortOrders

                        onActivated: recipeBookSettings.setDefaultSortOrder(currentText)
                    }
                }
            }

            GroupBox {
                id: groupData
                anchors.top: groupDefaultValues.bottom
				anchors.topMargin: 24
				anchors.horizontalCenter: parent.horizontalCenter

				title: qsTr("Data")

                ColumnLayout {
                    anchors.centerIn: parent

                    Button {
                        text: qsTr("Load default data")
                        font.capitalization: Font.MixedCase
                        Layout.alignment: Qt.AlignHCenter
                        onClicked: dlgLoadDefaultData.open()
                    }

                    Button {
                        text: qsTr("Reset all data")
                        font.capitalization: Font.MixedCase
                        Layout.alignment: Qt.AlignHCenter
                        onClicked: dlgResetData.open()
                    }
                }
            }

			ColumnLayout {
                id: rowUID
                anchors.top: groupData.bottom
				anchors.topMargin: 36
                anchors.horizontalCenter: parent.horizontalCenter

                Label {
                    text: qsTr("Application instance UID: ")
                }
                Label {
                    id: textUID
                }
            }
        }
    }
}
