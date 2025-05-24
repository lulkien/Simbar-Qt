import QtQuick
import QtQuick.Controls
import Simbar

Item {
    id: root
    width: 3440
    height: 40

    Rectangle {
        anchors.fill: parent
        color: "#18181f"
    }

    Widget {
        id: test
        anchors.centerIn: parent
        icon: "󰖩"
        content: "Hehe"
    }
}
