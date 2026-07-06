import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

PremiumPanel {
    id: root
    property var host
    radius: 18
    topColor: theme.panelTop
    bottomColor: theme.panelBottom
    overlayColor: theme.glassOverlay
    borderColor: theme.panelStroke
    glassOpacity: 0.32

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 22
        anchors.rightMargin: 18
        spacing: 18

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4
            Text {
                text: "ESP32 板端视觉分拣控制台"
                color: theme.text
                font.pixelSize: 26
                font.weight: Font.Black
            }
            Text {
                text: host.statusText + "    保存目录  " + host.saveDir
                color: theme.muted
                font.pixelSize: 12
                elide: Text.ElideRight
                Layout.fillWidth: true
            }
        }

        StatusPill {
            Layout.preferredWidth: 122
            Layout.preferredHeight: 38
            theme: root.theme
            text: host.connected ? "设备在线" : "待连接"
            fillColor: host.connected ? theme.statusBg : theme.warnWash
            strokeColor: host.connected ? theme.accent : theme.warn
            textColor: host.connected ? theme.statusText : theme.warnText
            textSize: 13
        }

        Button {
            id: syncButton
            Layout.preferredWidth: 116
            Layout.preferredHeight: 42
            text: "同步时间"
            hoverEnabled: true
            onClicked: host.sendTimeSync()
            contentItem: Text {
                text: syncButton.text
                color: theme.text
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 13
                font.weight: Font.DemiBold
            }
            background: Rectangle {
                radius: 14
                border.color: syncButton.hovered ? theme.accent : theme.panelStroke
                gradient: Gradient {
                    GradientStop { position: 0.0; color: syncButton.hovered ? theme.navActive : theme.panelTop }
                    GradientStop { position: 1.0; color: theme.panelBottom }
                }
                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    height: 1
                    color: theme.highlight
                    opacity: 0.6
                }
                Behavior on border.color { ColorAnimation { duration: 160 } }
            }
        }
    }
}
