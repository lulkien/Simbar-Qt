import QtQuick
import Simbar

Row {
    id: root
    spacing: 10

    TextBaseWidget {
        id: bluetooth
        iconText: "󰂲"
        iconBoxColor: SimbarConfig.themeRed
        contentTextColor: iconBoxColor
    }

    TextBaseWidget {
        id: wifi
        iconText: "󰖩"
        clickable: true
        iconBoxColor: contentText === "" ? SimbarConfig.themeRed : SimbarConfig.themeGreen
        onClicked: {
            wifi.updateText(contentText !== "" ? "" : "OpenWrt_Home");
        }

        Component.onCompleted: {
            wifi.instantUpdateText("OpenWrt_Home");
        }
    }

    TextBaseWidget {
        id: dateTime
        iconText: "󰸗"
        iconBoxColor: SimbarConfig.themeMauve
        contentPaddingRight: 10

        Timer {
            interval: 2000
            running: true
            repeat: true
            onTriggered: {
                dateTime.instantUpdateText(Qt.formatDateTime(new Date(), "ddd MMM dd | hh:mm AP"));
            }
        }

        Component.onCompleted: {
            dateTime.instantUpdateText(Qt.formatDateTime(new Date(), "ddd MMM dd | hh:mm AP"));
        }
    }
}
