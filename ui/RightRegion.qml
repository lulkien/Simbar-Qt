import QtQuick
import Simbar

Row {
    id: root
    spacing: 10

    TextBaseWidget {
        id: bluetooth
        iconText: "󰂲"
        iconBoxColor: Global.Theme.red
        contentTextColor: iconBoxColor
    }

    TextBaseWidget {
        id: wifi
        iconText: "󰖩"
        iconBoxColor: Global.Theme.green
        contentTextColor: iconBoxColor
        contentText: "OpenWrt_Home"
        clickable: true
        onClicked: {
            contentText =  contentText !== "" ? "" : "OpenWrt_Home"
        }


    }

    TextBaseWidget {
        id: dateTime
        iconText: "󰸗"
        iconBoxColor: Global.Theme.mauve
        contentTextColor: iconBoxColor
        contentText: Qt.formatDateTime(new Date(), "ddd MMM dd | hh:mm AP")
        contentPaddingRight: 10

        Timer {
            interval: 2000
            running: true
            repeat: true
            onTriggered: {
                dateTime.contentText = Qt.formatDateTime(new Date(), "ddd MMM dd | hh:mm AP")
            }
        }
    }
}
