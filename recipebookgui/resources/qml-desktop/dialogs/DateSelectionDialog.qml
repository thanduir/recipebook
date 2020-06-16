import QtQuick 2.14
import QtQuick.Controls 1.4
import QtQuick.Controls 2.14

Dialog {
	id: dlgCalendar
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2
	
	property alias selectedDate: cal.selectedDate

	property alias minimumDate: cal.minimumDate
	property alias maximumDate: cal.maximumDate

	Calendar {
		id: cal

		onDoubleClicked: {
			dlgCalendar.accept();
		}
	}

	footer: DialogButtonBox {
		id: buttons
		standardButtons: Dialog.Ok | Dialog.Cancel
	}
}