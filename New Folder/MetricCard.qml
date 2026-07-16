import QtQuick
import QtQuick.Layouts

PremiumPanel {
    id: root
    property string title: ""
    property string value: "--"
    property string note: ""
    property color accent: theme.accent

    radius: 18
    topColor: theme.panelTop
    bottomColor: theme.panelBottom
    overlayColor: theme.glassOverlay
    borderColor: theme.panelStroke
    accentColor: root.accent
    accented: true
    glassOpacity: 0.34
    implicitHeight: 112

    Rectangle {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 14
        width: 38
        height: 38
        radius: 15
        color: Qt.rgba(root.accent.r, root.accent.g, root.accent.b, 0.14)
        border.color: Qt.rgba(root.accent.r, root.accent.g, root.accent.b, 0.38)

        Rectangle {
            anchors.centerIn: parent
            width: 10
            height: 10
            radius: 5
            color: root.accent
        }
        Rectangle {
            anchors.centerIn: parent
            width: 24
            height: 24
            radius: 12
            color: root.accent
            opacity: 0.10
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 6

        Text {
            Layout.fillWidth: true
            Layout.rightMargin: 44
            text: root.title
            color: theme.muted
            font.pixelSize: 12
            font.weight: Font.DemiBold
            elide: Text.ElideRight
        }
        Text {
            Layout.fillWidth: true
            text: root.value
            color: theme.text
            font.pixelSize: 25
            font.weight: Font.Black
            elide: Text.ElideRight
        }
        Text {
            Layout.fillWidth: true
            text: root.note
            color: theme.faint
            font.pixelSize: 11
            elide: Text.ElideRight
        }
    }
}
