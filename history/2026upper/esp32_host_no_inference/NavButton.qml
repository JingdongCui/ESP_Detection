import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    id: control
    property bool selected: false
    property var theme
    property string shortText: ""
    Layout.fillWidth: true
    implicitHeight: 52
    hoverEnabled: true
    padding: 0
    leftInset: 0
    rightInset: 0
    topInset: 0
    bottomInset: 0

    contentItem: Item {
        implicitWidth: 128
        implicitHeight: 52

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 14
            anchors.rightMargin: 12
            spacing: 10

            Rectangle {
                Layout.preferredWidth: 22
                Layout.preferredHeight: 22
                Layout.alignment: Qt.AlignVCenter
                radius: 11
                color: control.selected ? control.theme.accentWash : control.theme.highlightSoft
                border.color: control.selected ? control.theme.accent : control.theme.lineSoft
                opacity: control.selected ? 1 : (control.hovered ? 0.85 : 0.5)
                Rectangle {
                    anchors.centerIn: parent
                    width: control.selected ? 8 : 6
                    height: width
                    radius: width / 2
                    color: control.selected ? control.theme.accent : control.theme.faint
                    Behavior on width { NumberAnimation { duration: 160; easing.type: Easing.OutCubic } }
                    Behavior on color { ColorAnimation { duration: 180 } }
                }
                Behavior on opacity { NumberAnimation { duration: 160 } }
                Behavior on color { ColorAnimation { duration: 180 } }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                spacing: 0

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
                    color: control.selected ? control.theme.accent : control.theme.muted
                    font.pixelSize: 10
                    font.weight: Font.DemiBold
                    elide: Text.ElideRight
                    opacity: 0.9
                }
            }
        }
    }

    background: Rectangle {
        radius: 18
        anchors.fill: parent
        border.color: control.selected ? control.theme.panelStroke : (control.hovered ? control.theme.lineSoft : "transparent")
        border.width: 1
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
            anchors.fill: parent
            anchors.margins: 1
            radius: parent.radius - 1
            color: control.theme.accentMist
            opacity: control.selected ? 0.75 : (control.hovered ? 0.35 : 0)
            Behavior on opacity { NumberAnimation { duration: 170 } }
        }

        Rectangle {
            width: control.selected ? 44 : 0
            height: 2
            radius: 1
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            color: control.theme.accent
            opacity: control.selected ? 0.86 : 0
            Behavior on width { NumberAnimation { duration: 170; easing.type: Easing.OutCubic } }
            Behavior on opacity { NumberAnimation { duration: 170 } }
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            height: 1
            color: control.theme.highlight
            opacity: control.selected || control.hovered ? 0.45 : 0
            Behavior on opacity { NumberAnimation { duration: 170 } }
        }
    }
}
