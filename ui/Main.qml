import QtQuick
import QtQuick.Window
import SimbarQt

Item {
    id: root
    width: 3440
    height: 40

    Rectangle {
        anchors.fill: parent
        color: "green"
    }

    Text {
        anchors.centerIn: parent
        text: BluetoothEnums.Error
    }
}
