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

    MetricStatusIndicator {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 12
        theme: root.theme
        accent: root.accent
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
