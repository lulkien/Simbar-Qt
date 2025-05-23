import QtQuick
import QtQuick.Window
import SimbarQt

Item {
    id: root
    width: 3440
    height: 40

    KFrame {
        anchors.fill: parent
        color: "blue"
    }

    Component.onCompleted: {
        console.log(Bluetooth.Idle);
    }
}
