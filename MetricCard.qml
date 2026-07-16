import QtQuick
import QtQuick.Layouts

PremiumPanel {
    id: root
    property string title: ""
    property string value: "--"
    property string note: ""
    property color accent: theme.accent
    radius: 14
    topColor: theme.panelTop
    bottomColor: theme.panelBottom
    overlayColor: theme.glassOverlay
    borderColor: theme.panelStroke
    accentColor: root.accent
    accented: true
    glassOpacity: 0.28
    implicitHeight: 108

    Item {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 12
        width: 42
        height: 42

        Rectangle {
            anchors.centerIn: parent
            width: 42
            height: 42
            radius: 15
            color: Qt.rgba(root.accent.r, root.accent.g, root.accent.b, 0.10)
        }

        Rectangle {
            anchors.centerIn: parent
            width: 36
            height: 36
            radius: 12
            color: Qt.rgba(root.accent.r, root.accent.g, root.accent.b, 0.20)
            border.width: 2
            border.color: root.accent

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.leftMargin: 8
                anchors.rightMargin: 8
                height: 1
                color: root.theme.highlight
            }

            Rectangle {
                anchors.centerIn: parent
                width: 11
                height: 11
                radius: 6
                color: root.accent

                Rectangle {
                    anchors.centerIn: parent
                    width: 4
                    height: 4
                    radius: 2
                    color: root.theme.thumb
                    opacity: 0.92
                }
            }
        }
    }
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 6
        Text {
            text: root.title
            color: theme.muted
            font.pixelSize: 12
            font.weight: Font.DemiBold
            elide: Text.ElideRight
            Layout.fillWidth: true
        }
        Text {
            text: root.value
            color: theme.text
            font.pixelSize: 24
            font.weight: Font.Black
            elide: Text.ElideRight
            Layout.fillWidth: true
            Behavior on opacity { NumberAnimation { duration: 180 } }
        }
        Text {
            text: root.note
            color: theme.faint
            font.pixelSize: 12
            elide: Text.ElideRight
            Layout.fillWidth: true
        }
    }
}
