pragma ComponentBehavior

import QtQuick
import QtQuick.Layouts
import Simbar

RowLayout {
    id: root
    property double widgetHeight: SimbarConfig.qmlDefaultBoxSize

    property double iconBoxWidth: SimbarConfig.qmlDefaultBoxSize
    property color iconBoxColor: SimbarConfig.themeBlue
    property string iconText: ""
    property color iconTextColor: SimbarConfig.themeBase

    property double contentPaddingLeft: SimbarConfig.qmlDefaultPadding
    property double contentPaddingRight: SimbarConfig.qmlDefaultPadding
    property color contentBoxColor: SimbarConfig.themeBase
    readonly property string contentText: content.text
    property color contentTextColor: iconBoxColor

    property bool clickable: false
    signal clicked

    spacing: 0

    FlexRectangle {
        id: iconBox
        Layout.preferredWidth: root.iconBoxWidth
        Layout.preferredHeight: root.widgetHeight
        radius: contentBox.visible ? [8, 0, 0, 8] : [8, 8, 8, 8]
        color: root.iconBoxColor

        BaseText {
            id: icon
            anchors.centerIn: parent
            font.pixelSize: SimbarConfig.qmlDefaultIconSize
            color: root.iconTextColor
            text: root.iconText
        }

        MouseArea {
            anchors.fill: parent
            enabled: root.clickable
            onClicked: {
                root.clicked();
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

        AnimatedText {
            id: content
            anchors.left: contentBox.left
            anchors.leftMargin: root.contentPaddingLeft
            anchors.verticalCenter: contentBox.verticalCenter

            font.pixelSize: SimbarConfig.qmlDefaultFontSize
            font.bold: true
            color: root.contentTextColor

            duration: 200
        }

        MouseArea {
            anchors.fill: parent
            enabled: root.clickable
            onClicked: {
                root.clicked();
            }
        }
    }

    function updateText(text) {
        content.updateText(text);
    }

    function instantUpdateText(text) {
        content.instantUpdateText(text);
    }
}
