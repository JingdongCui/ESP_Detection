import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    id: control

    property bool selected: false
    property var theme
    property string badgeText: "01"
    property string description: ""

    Layout.fillWidth: true
    implicitHeight: 66
    hoverEnabled: true
    padding: 0
    leftInset: 0
    rightInset: 0
    topInset: 0
    bottomInset: 0

    contentItem: RowLayout {
        spacing: 12

        Rectangle {
            Layout.preferredWidth: 36
            Layout.preferredHeight: 36
            Layout.leftMargin: 12
            Layout.alignment: Qt.AlignVCenter
            radius: 11
            color: control.selected ? control.theme.accent : control.theme.highlightSoft
            border.width: 1
            border.color: control.selected ? control.theme.accent : control.theme.line

            Text {
                anchors.centerIn: parent
                text: control.badgeText
                color: control.selected ? control.theme.onAccent : control.theme.muted
                font.pixelSize: 11
                font.weight: Font.Black
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            spacing: 2

            Text {
                Layout.fillWidth: true
                text: control.text
                color: control.selected ? control.theme.text : control.theme.navText
                font.pixelSize: 14
                font.weight: control.selected ? Font.Bold : Font.DemiBold
                elide: Text.ElideRight
            }

            Text {
                Layout.fillWidth: true
                text: control.description
                color: control.selected ? control.theme.muted : control.theme.faint
                font.pixelSize: 10
                elide: Text.ElideRight
            }
        }

        Rectangle {
            Layout.preferredWidth: 3
            Layout.preferredHeight: 28
            Layout.rightMargin: 1
            radius: 2
            color: control.theme.accent
            opacity: control.selected ? 1 : 0
            Behavior on opacity { NumberAnimation { duration: 150 } }
        }
    }

    background: Rectangle {
        radius: 16
        border.width: 1
        border.color: control.selected ? control.theme.panelStroke
                                       : (control.hovered ? control.theme.lineSoft : "transparent")
        color: control.selected ? control.theme.navActive
                                : (control.hovered ? control.theme.highlightSoft : "transparent")

        Rectangle {
            anchors.fill: parent
            anchors.margins: 1
            radius: parent.radius - 1
            color: control.theme.accentMist
            opacity: control.selected ? 0.72 : 0
        }

        Behavior on color { ColorAnimation { duration: 150 } }
        Behavior on border.color { ColorAnimation { duration: 150 } }
    }
}
