import QtQuick 2.14
import QtQuick.Controls 2.14

Dialog {
	modal: true
	id: dlgSpinner

	x: (parent.width - width) / 2
	y: Qt.inputMethod.keyboardRectangle.y === 0 ? (parent.height - height) / 2 : 50
	width: 300

	property int from: 1
	property int stepSize: 1
	property int to: 50

	property int currentValue: 1

	property int decimals: 2
	signal selectedValueChanged

	SpinBox {
		id: spinbox
		from: dlgSpinner.from * scalingFactor
		to: dlgSpinner.to * scalingFactor
		stepSize: dlgSpinner.stepSize * scalingFactor
		editable: true
		wheelEnabled: true

		property real scalingFactor: Math.pow(10, dlgSpinner.decimals)
		property real realValue: value / scalingFactor

		value: currentValue * scalingFactor
		onValueModified: {
			currentValue = value / scalingFactor
			selectedValueChanged()
		}

		validator: DoubleValidator {
			bottom: Math.min(spinbox.from, spinbox.to)
			top:  Math.max(spinbox.from, spinbox.to)
			locale: "en_US"
		}

		textFromValue: function(value, locale) {
			return Number(value / 100).toLocaleString(Qt.locale("en_US"), 'f', dlgSpinner.decimals)
		}

		valueFromText: function(text, locale) {
			return Number.fromLocaleString(Qt.locale("en_US"), text) * 100
		}
	}

	footer: DialogButtonBox {
		Button {
			text: qsTr("Close")
			DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
			flat: true
		}
	}

	onAboutToShow: {
		spinbox.forceActiveFocus();
	}

	onAccepted: {
		selectedValueChanged()
	}
}
