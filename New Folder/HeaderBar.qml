import QtQuick
import QtQuick.Layouts

PremiumPanel {
    id: root
    property var host

    radius: 22
    topColor: theme.panelTop
    bottomColor: theme.panelBottom
    overlayColor: theme.glassOverlay
    borderColor: theme.panelStroke
    glassOpacity: 0.36

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 24
        anchors.rightMargin: 18
        spacing: 14

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 3

            Text {
                text: "ESP32-P4 ???????"
                color: theme.text
                font.pixelSize: 24
                font.weight: Font.Black
            }
            Text {
                Layout.fillWidth: true
                text: host.statusText + "   ?   ???? " + host.saveDir
                color: theme.muted
                font.pixelSize: 11
                elide: Text.ElideRight
            }
        }

        ColumnLayout {
            Layout.preferredWidth: 92
            spacing: 2
            Text { text: "????"; color: theme.faint; font.pixelSize: 10; Layout.alignment: Qt.AlignRight }
            Text { text: host.uptimeText; color: theme.text; font.pixelSize: 14; font.weight: Font.Black; Layout.alignment: Qt.AlignRight }
        }

        ColumnLayout {
            Layout.preferredWidth: 78
            spacing: 2
            Text { text: "????"; color: theme.faint; font.pixelSize: 10; Layout.alignment: Qt.AlignRight }
            Text { text: host.lastTelemetryTime; color: theme.text; font.pixelSize: 14; font.weight: Font.Black; Layout.alignment: Qt.AlignRight }
        }

        StatusPill {
            Layout.preferredWidth: 120
            Layout.preferredHeight: 38
            theme: root.theme
            text: host.connected ? "????" : "????"
            fillColor: host.connected ? theme.statusBg : theme.warnWash
            strokeColor: host.connected ? theme.success : theme.warn
            textColor: host.connected ? theme.statusText : theme.warnText
            textSize: 12
        }

        PillButton {
            Layout.preferredWidth: 104
            Layout.preferredHeight: 38
            theme: root.theme
            accent: theme.accent
            text: "????"
            onClicked: host.sendTimeSync()
        }
    }
}
