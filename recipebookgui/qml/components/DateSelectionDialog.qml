import QtQml 2.15
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

Dialog {
	id: dlgCalendar
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2
	
	property date selectedDate: new Date()

	DatePicker {
		id: dlgDateSelection

		implicitWidth: 300
		implicitHeight: 200

		onVisibleChanged: {
			if(visible)
			{	
				set(dlgCalendar.selectedDate)
			}
		}
		onClicked: dlgCalendar.selectedDate = date
	}

	footer: DialogButtonBox {
		id: buttons
		standardButtons: Dialog.Ok | Dialog.Cancel | Dialog.Reset
	}
}
