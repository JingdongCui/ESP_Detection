import QtQuick
import QtQuick.Layouts

Item {
    id: root
    property var theme
    property var host
    opacity: StackLayout.isCurrentItem ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 220 } }

    ColumnLayout {
        anchors.fill: parent
        spacing: 14

        GridLayout {
            Layout.fillWidth: true
            columns: 4
            columnSpacing: 14
            rowSpacing: 14
            Repeater {
                model: host.dashboardCards
                MetricCard {
                    Layout.fillWidth: true
                    theme: root.theme
                    title: modelData.title
                    value: modelData.value
                    note: modelData.note
                    accent: modelData.accent
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 218
            spacing: 14
            CircularGauge { Layout.fillWidth: true; value: host.cpuUsage; title: "CPU 占用"; subtitle: "实时计算负载"; accent: theme.accent; theme: root.theme }
            CircularGauge { Layout.fillWidth: true; value: host.psramUsage; title: "PSRAM"; subtitle: host.freePsramMb.toFixed(2) + " / " + host.totalPsramMb.toFixed(2) + " MB"; accent: theme.accent2; theme: root.theme }
            CircularGauge { Layout.fillWidth: true; value: host.heapUsage; title: "堆内存"; subtitle: host.freeHeapMb.toFixed(2) + " MB 空闲"; accent: theme.warn; theme: root.theme }
            CircularGauge { Layout.fillWidth: true; value: host.internalUsage; title: "内部内存"; subtitle: host.freeInternalMb.toFixed(2) + " MB 空闲"; accent: theme.purple; theme: root.theme }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 14
            TrendChart {
                Layout.fillWidth: true
                Layout.fillHeight: true
                theme: root.theme
                history: host.metricHistory
                title: "短时性能趋势"
                fieldA: "cpu"
                fieldB: "psram"
                colorA: theme.accent
                colorB: theme.accent2
            }
            TrendChart {
                Layout.fillWidth: true
                Layout.fillHeight: true
                theme: root.theme
                history: host.metricHistory
                title: "内存与处理波动"
                fieldA: "heap"
                fieldB: "internal"
                colorA: theme.warn
                colorB: theme.purple
            }
        }

        PremiumPanel {
            Layout.fillWidth: true
            Layout.preferredHeight: 132
            radius: 16
            theme: root.theme
            topColor: theme.panelTop
            bottomColor: theme.panelBottom
            overlayColor: theme.glassOverlay
            borderColor: theme.panelStroke
            glassOpacity: 0.25
            RowLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 18
                ColumnLayout {
                    Layout.preferredWidth: 200
                    Text { text: "实时事件流"; color: theme.text; font.pixelSize: 16; font.weight: Font.Black }
                    Text { text: "性能、图像与控制链路"; color: theme.muted; font.pixelSize: 12 }
                }
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    orientation: ListView.Horizontal
                    spacing: 10
                    clip: true
                    model: host.logLines
                    delegate: Rectangle {
                        width: 260
                        height: ListView.view.height
                        radius: 12
                        border.color: index === 0 ? theme.accent : theme.panelStroke
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: index === 0 ? theme.navActive : theme.panelTop }
                            GradientStop { position: 1.0; color: theme.panelBottom }
                        }
                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.leftMargin: 14
                            anchors.rightMargin: 14
                            height: 1
                            color: theme.highlight
                            opacity: 0.55
                        }
                        Text {
                            anchors.fill: parent
                            anchors.margins: 12
                            text: modelData
                            color: index === 0 ? theme.text : theme.muted
                            font.pixelSize: 12
                            wrapMode: Text.Wrap
                            maximumLineCount: 4
                            elide: Text.ElideRight
                        }
                    }
                }
            }
        }
    }
}
