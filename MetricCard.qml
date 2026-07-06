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

    Rectangle {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 14
        width: 34
        height: 34
        radius: 13
        color: root.accent
        opacity: 0.16
        border.color: root.accent
        Rectangle {
            anchors.centerIn: parent
            width: 9
            height: 9
            radius: 5
            color: Qt.lighter(root.accent, 1.6)
            opacity: 1.0
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
