import QtQml 2.15
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import Qt.labs.calendar 1.0

// TODO: Test on android tablet and phone
Dialog {
	id: dlgCalendar
	modal: true

	x: (parent.width - width) / 2
	y: (parent.height - height) / 2
	
	property alias selectedDate: listview.selectedDate

	property date minimumDate: new Date()
	property date maximumDate: addMonth(new Date())

	function addMonth(myDate: date)
	{
		var d = myDate.getDate();
		myDate.setMonth(myDate.getMonth() + 1);
		if (myDate.getDate() !== d) {
			myDate.setDate(0);
		}
		return myDate
	}

	// TODO: Improve layout!
	header: Label {
		height: 30

		font.bold: true
		verticalAlignment: Text.AlignVCenter
		horizontalAlignment: Text.AlignHCenter
		text: formatDate(listview.model.monthAt(listview.currentIndex), listview.model.yearAt(listview.currentIndex))

		function formatDate(month: int, year: int)
		{
			var d = new Date();
			d.setDate(0);
			d.setMonth(month);
			d.setYear(year);
			return d.toLocaleDateString(Qt.locale(), "MMMM yyyy")
		}
	}

	ListView {
		id: listview

		implicitWidth: 300
		implicitHeight: 200

		snapMode: ListView.SnapOneItem
		orientation: ListView.Horizontal
		highlightRangeMode: ListView.StrictlyEnforceRange

		property date selectedDate: new Date()

		model: CalendarModel {
			id: calendarModel
			from: minimumDate
			to: maximumDate
		}

		delegate: GridLayout {
			width: listview.width
			height: listview.height
			visible: index == listview.currentIndex
				
			columns: 2
				
			DayOfWeekRow {
				locale: grid.locale

				Layout.column: 1
				Layout.fillWidth: true
			}

			WeekNumberColumn {
				month: grid.month
				year: grid.year
				locale: grid.locale

				Layout.fillHeight: true
			}

			MonthGrid {
				id: grid
				
				month: model.month
				year: model.year
				locale: Qt.locale("en_US")

				Layout.fillWidth: true
				Layout.fillHeight: true

				delegate: Rectangle {
					height: dayText.height
					width: dayText.width
					radius: height * 0.5
					property bool highlighted: enabled && model.day === listview.selectedDate.getDate() && model.month === listview.selectedDate.getMonth() && model.year === listview.selectedDate.getFullYear()

					enabled: model.month === grid.month
					color: highlighted ? "lightgray" : "transparent"

					Text {
						id: dayText
						anchors.centerIn: parent
						text: model.day
						scale: highlighted ? 1.25 : 1
						Behavior on scale { NumberAnimation { duration: 150 } }
						color: parent.enabled ? "black" : "gray"
					}
					MouseArea {
						anchors.fill: parent

						onClicked: {
							listview.selectedDate = model.date;
						}
					}
				}
			}
		}

		ScrollIndicator.horizontal: ScrollIndicator { }
	}

	footer: DialogButtonBox {
		id: buttons
		standardButtons: Dialog.Ok | Dialog.Cancel
	}
}
