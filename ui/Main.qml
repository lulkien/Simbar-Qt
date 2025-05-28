import QtQuick
import Simbar

Item {
    id: root
    width: Global.Size.barWidth
    height: Global.Size.barHeight

    Rectangle {
        anchors.fill: parent
        color: Global.Color.background
    }

    RightRegion {
        id: right_widgets
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10

    }
}
