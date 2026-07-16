import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    id: control
    property bool selected: false
    property var theme
    property string shortText: ""
    property string glyph: "?"

    Layout.fillWidth: true
    implicitHeight: 58
    hoverEnabled: true
    padding: 0
    leftInset: 0
    rightInset: 0
    topInset: 0
    bottomInset: 0

    contentItem: RowLayout {
        spacing: 12

        Rectangle {
            Layout.leftMargin: 12
            Layout.preferredWidth: 32
            Layout.preferredHeight: 32
            Layout.alignment: Qt.AlignVCenter
            radius: 12
            color: control.selected ? control.theme.accentWash : control.theme.highlightSoft
            border.color: control.selected ? control.theme.accent : control.theme.lineSoft

            Text {
                anchors.centerIn: parent
                text: control.glyph
                color: control.selected ? control.theme.accent : control.theme.muted
                font.pixelSize: 13
                font.weight: Font.Black
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.rightMargin: 10
            Layout.alignment: Qt.AlignVCenter
            spacing: 1

            Text {
                Layout.fillWidth: true
                text: control.text
                color: control.selected ? control.theme.text : control.theme.navText
                font.pixelSize: 14
                font.weight: Font.Black
                elide: Text.ElideRight
            }
            Text {
                Layout.fillWidth: true
                text: control.shortText
                color: control.selected ? control.theme.accent : control.theme.faint
                font.pixelSize: 10
                font.weight: Font.Medium
                elide: Text.ElideRight
            }
        }
    }

    background: Rectangle {
        radius: 19
        border.width: 1
        border.color: control.selected
            ? Qt.rgba(control.theme.accent.r, control.theme.accent.g, control.theme.accent.b, 0.46)
            : (control.hovered ? control.theme.panelStroke : "transparent")
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: control.selected ? control.theme.navActive : (control.hovered ? control.theme.panelTop : "#00000000")
            }
            GradientStop {
                position: 1.0
                color: control.selected ? control.theme.panelBottom : (control.hovered ? control.theme.panelBottom : "#00000000")
            }
        }

        Rectangle {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: control.selected ? 3 : 0
            height: control.selected ? 28 : 10
            radius: 2
            color: control.theme.accent
            opacity: control.selected ? 1 : 0
            Behavior on width { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
            Behavior on height { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
            Behavior on opacity { NumberAnimation { duration: 160 } }
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            height: 1
            color: control.theme.highlight
            opacity: control.selected || control.hovered ? 0.42 : 0
            Behavior on opacity { NumberAnimation { duration: 160 } }
        }
    }
}
