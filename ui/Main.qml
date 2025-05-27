import QtQuick
import Simbar as Simbar

Item {
    id: root
    width: 3440
    height: 40

    Rectangle {
        anchors.fill: parent
        color: "#18181f"
    }

    Simbar.FlexRectangle {
        id: flex1
        anchors.centerIn: parent
        width: 32
        height: 32
        radius: [8, 0, 0, 8]
        segments: 8
        color: "pink"
    }

    Simbar.FlexRectangle {
        id: flex2
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: flex1.right
        width: 50
        height: 32
        radius: [0, 8, 8, 0]
        segments: 8
        color: "#4c4c4c"
    }
}
