pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import Simbar

RowLayout {
    id: root
    property double widgetHeight: Global.Size.defaultBoxSize

    property double iconBoxWidth: Global.Size.defaultBoxSize
    property string iconBoxColor: Global.Color.primary
    property string iconText: ""
    property string iconTextColor: Global.Theme.base

    property double contentPaddingLeft: Global.Size.defaultContentPadding
    property double contentPaddingRight: Global.Size.defaultContentPadding
    property string contentBoxColor: Global.Theme.base
    property string contentText: ""
    property string contentTextColor: Global.Theme.base

    property bool clickable: false
    signal clicked()

    spacing: 0

    Behavior on implicitWidth {
        NumberAnimation {
            duration: 500 // Duration of the animation in milliseconds
            easing.type: Easing.InOutQuad // Easing function for smooth transition
        }
    }

    FlexRectangle {
        id: iconBox
        Layout.preferredWidth: root.iconBoxWidth
        Layout.preferredHeight: root.widgetHeight
        radius: contentBox.visible ? [8, 0, 0, 8] : [8, 8, 8, 8]
        color: root.iconBoxColor

        BaseText {
            id: icon
            anchors.centerIn: parent
            font.pixelSize: Global.Size.defaultIconFontSize
            color: root.iconTextColor
            text: root.iconText
        }

        MouseArea {
            anchors.fill: parent
            enabled: root.clickable
            onClicked: {
                root.clicked()
            }
        }
    }

    FlexRectangle {
        id: contentBox
        Layout.fillWidth: true
        Layout.minimumWidth: root.contentPaddingLeft + content.implicitWidth + root.contentPaddingRight
        Layout.preferredHeight: root.widgetHeight
        radius: [0, 8, 8, 0]
        color: root.contentBoxColor
        visible: root.contentText !== ""


        BaseText {
            id: content
            anchors.left: contentBox.left
            anchors.leftMargin: root.contentPaddingLeft
            anchors.verticalCenter: contentBox.verticalCenter


            font.pixelSize: Global.Size.defaultContentFontSize
            font.bold: true
            color: root.contentTextColor
            text: root.contentText
        }

        MouseArea {
            anchors.fill: parent
            enabled: root.clickable
            onClicked: {
                root.clicked()
            }
        }
    }

}
