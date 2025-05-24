import QtQuick

Text {
    id: root

    property string icon: ""
    property string content: ""

    text: "[" + root.icon + "|" + root.content + "]"
    font.family: "CodeNewRoman Nerd Font Mono"
    font.pixelSize: 20
    color: "#cdd6f4"
}
