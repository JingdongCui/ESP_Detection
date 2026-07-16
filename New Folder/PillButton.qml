import QtQuick
import QtQuick.Controls

Button {
    id: control
    property var theme
    property color accent: theme && theme.accent ? theme.accent : "#49d8ff"

    hoverEnabled: true
    implicitWidth: Math.max(82, label.implicitWidth + 34)
    implicitHeight: 38
    padding: 0
    leftInset: 0
    rightInset: 0
    topInset: 0
    bottomInset: 0

    contentItem: Text {
        id: label
        text: control.text
        color: control.down ? control.theme.onAccent : control.theme.text
        font.pixelSize: 12
        font.weight: Font.Black
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        radius: height / 2
        border.width: 1
        border.color: control.hovered || control.down
            ? control.accent
            : Qt.rgba(control.accent.r, control.accent.g, control.accent.b, 0.46)
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: control.down
                    ? control.accent
                    : Qt.rgba(control.accent.r, control.accent.g, control.accent.b, control.hovered ? 0.24 : 0.12)
            }
            GradientStop {
                position: 1.0
                color: control.down
                    ? Qt.darker(control.accent, 1.12)
                    : Qt.rgba(control.theme.panelBottom.r, control.theme.panelBottom.g, control.theme.panelBottom.b, 0.66)
            }
        }
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.leftMargin: 14
            anchors.rightMargin: 14
            height: 1
            color: control.theme.highlight
            opacity: 0.68
        }
        Behavior on border.color { ColorAnimation { duration: 160 } }
    }
}
