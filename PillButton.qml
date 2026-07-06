import QtQuick
import QtQuick.Controls

Button {
    id: control
    property var theme
    property color accent: theme && theme.accent ? theme.accent : "#20e0c2"

    hoverEnabled: true
    implicitHeight: 32
    padding: 0
    leftInset: 0
    rightInset: 0
    topInset: 0
    bottomInset: 0

    contentItem: Text {
        text: control.text
        color: control.accent
        font.pixelSize: 13
        font.weight: Font.Black
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        leftPadding: 16
        rightPadding: 16
    }

    background: Rectangle {
        implicitWidth: 64
        radius: height / 2
        color: control.down
            ? Qt.rgba(control.accent.r, control.accent.g, control.accent.b, 0.28)
            : (control.hovered ? Qt.rgba(control.accent.r, control.accent.g, control.accent.b, 0.16) : "transparent")
        border.width: 1
        border.color: Qt.rgba(control.accent.r, control.accent.g, control.accent.b, control.hovered ? 0.9 : 0.55)
        Behavior on color { ColorAnimation { duration: 150 } }
        Behavior on border.color { ColorAnimation { duration: 150 } }
    }
}
