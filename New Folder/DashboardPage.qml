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
            CircularGauge { Layout.fillWidth: true; value: host.cpuUsage; title: "CPU 占用"; subtitle: "板端总负载"; accent: theme.accent; theme: root.theme }
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
                title: "内存与链路波动"
                fieldA: "heap"
                fieldB: "internal"
                colorA: theme.warn
                colorB: theme.purple
            }
        }

        GridLayout {
            Layout.fillWidth: true
            columns: 4
            columnSpacing: 14
            rowSpacing: 14
            Repeater {
                model: host.courierStatsCards
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
    }
}
