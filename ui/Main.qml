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
        id: flex
        anchors.centerIn: parent
        width: 100
        height: 32
    }
}
