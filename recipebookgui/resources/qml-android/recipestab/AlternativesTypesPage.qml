import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQml.Models 2.1

import "components"

// TODO: ANPASSEN!
Item {
    id: alternativesTypesTab

    Label {
		id: labelAlternativesGroups
        
		anchors.left: paneAlternativesGroups.left
		anchors.top: parent.top
		anchors.topMargin: 24
        
		text: qsTr("Alternatives types")
		font.bold: true
	}

	EditAlternativesGroupPane {
		id: paneAlternativesGroups
        anchors.left: parent.left
		anchors.top: labelAlternativesGroups.bottom
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		anchors.topMargin: 48
		anchors.leftMargin: 48
		anchors.bottomMargin: 0
		anchors.rightMargin: 48
	}
}
