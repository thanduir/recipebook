import QtQuick 2.14
import QtQuick.Controls 2.14

Item {
    // TODO: Define layout and implement everything! -> SortOrders instead of categories model

    ScrollView {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 48
        anchors.leftMargin: 48
        anchors.bottomMargin: 48
        width: 400

        ListView {
            id: listView
            anchors.fill: parent
            topMargin: 5
            leftMargin: 5
            bottomMargin: 5
            rightMargin: 5
            boundsBehavior: Flickable.StopAtBounds

            spacing: 5
            model: modelCategories
            delegate: SwipeDelegate {
                text: modelData
                width: listView.width - listView.leftMargin - listView.rightMargin
            }
        }
    }
}