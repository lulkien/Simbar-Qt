import QtQuick
import Simbar

Item {
    id: root
    width: SimbarConfig.qmlWidth
    height: SimbarConfig.qmlHeight

    Rectangle {
        anchors.fill: parent
        color: Global.Color.background
    }

    RightRegion {
        id: right_widgets
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: SimbarConfig.qmlDefaultPadding
    }
}
