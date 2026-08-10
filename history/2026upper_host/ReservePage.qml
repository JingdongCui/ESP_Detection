import QtQuick
import QtQuick.Layouts

Item {
    id: root
    property var theme
    property var host
    opacity: StackLayout.isCurrentItem ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 220 } }

    PremiumPanel {
        anchors.fill: parent
        radius: 18
        theme: root.theme
        topColor: theme.panelTop
        bottomColor: theme.panelBottom
        overlayColor: theme.glassOverlay
        borderColor: theme.panelStroke
        glassOpacity: 0.28

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 28
            spacing: 18
            Text {
                text: "扩展能力中心"
                color: theme.text
                font.pixelSize: 30
                font.weight: Font.Black
            }
            Text {
                text: "为后续标定、模型管理、设备升级、批量导出和现场脚本预留入口。当前版本保持轻量展示，不影响主链路操作。"
                color: theme.muted
                font.pixelSize: 15
                wrapMode: Text.Wrap
                Layout.fillWidth: true
            }
            GridLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 260
                columns: 3
                columnSpacing: 14
                rowSpacing: 14
                Repeater {
                    model: [
                        {title: "视觉标定", note: "相机位置、检测区域和补光参数", accent: theme.accent},
                        {title: "模型管理", note: "模型版本、阈值模板和回滚策略", accent: theme.accent2},
                        {title: "设备维护", note: "固件升级、健康检查和现场脚本", accent: theme.warn}
                    ]
                    delegate: PremiumPanel {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        theme: root.theme
                        radius: 18
                        topColor: theme.panelTop
                        bottomColor: theme.panelBottom
                        overlayColor: theme.glassOverlay
                        borderColor: theme.panelStroke
                        accentColor: modelData.accent
                        accented: true
                        glassOpacity: 0.24
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 20
                            spacing: 10
                            Rectangle {
                                Layout.preferredWidth: 42
                                Layout.preferredHeight: 42
                                radius: 15
                                color: modelData.accent
                                opacity: 0.18
                                border.color: modelData.accent
                                Rectangle {
                                    anchors.centerIn: parent
                                    width: 10
                                    height: 10
                                    radius: 5
                                    color: modelData.accent
                                }
                            }
                            Text {
                                text: modelData.title
                                color: theme.text
                                font.pixelSize: 18
                                font.weight: Font.Black
                            }
                            Text {
                                Layout.fillWidth: true
                                text: modelData.note
                                color: theme.muted
                                font.pixelSize: 13
                                wrapMode: Text.Wrap
                            }
                            Item { Layout.fillHeight: true }
                            StatusPill {
                                theme: root.theme
                                text: "预留接口"
                                fillColor: theme.accentMist
                                strokeColor: modelData.accent
                                textColor: theme.text
                                textSize: 11
                            }
                        }
                    }
                }
            }
            RowLayout {
                Layout.fillWidth: true
                spacing: 14
                MetricCard { Layout.fillWidth: true; theme: root.theme; title: "兼容目标"; value: "Windows MinGW / Linux"; note: "Qt 6.5+ Desktop"; accent: theme.accent }
                MetricCard { Layout.fillWidth: true; theme: root.theme; title: "当前状态"; value: host.demoMode ? "演示数据" : "真实设备"; note: host.statusText; accent: theme.accent2 }
                MetricCard { Layout.fillWidth: true; theme: root.theme; title: "保存目录"; value: "ESP32Host"; note: host.saveDir; accent: theme.warn }
            }
            Item { Layout.fillHeight: true }
        }
    }
}
