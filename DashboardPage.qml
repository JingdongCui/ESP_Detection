pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property var theme
    property var host

    opacity: StackLayout.isCurrentItem ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 180 } }

    component SectionTitle: RowLayout {
        property string title: ""
        property string subtitle: ""
        property color accentColor: root.theme.accent

        spacing: 9

        Rectangle {
            Layout.preferredWidth: 4
            Layout.preferredHeight: 18
            radius: 2
            color: parent.accentColor
        }

        Text {
            text: parent.title
            color: root.theme.text
            font.pixelSize: 14
            font.weight: Font.Bold
        }

        Text {
            Layout.fillWidth: true
            text: parent.subtitle
            color: root.theme.faint
            font.pixelSize: 10
            elide: Text.ElideRight
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        SectionTitle {
            Layout.fillWidth: true
            title: qsTr("核心状态")
            subtitle: qsTr("设备运行关键指标")
            accentColor: root.theme.accent
        }

        GridLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 220
            Layout.minimumHeight: 220
            columns: 4
            rows: 2
            columnSpacing: 12
            rowSpacing: 12

            Repeater {
                model: root.host.dashboardCards
                delegate: MetricCard {
                    required property var modelData
                    Layout.fillWidth: true
                    Layout.fillHeight: true
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
            Layout.fillHeight: true
            spacing: 14

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 5
                spacing: 9

                SectionTitle {
                    Layout.fillWidth: true
                    title: qsTr("资源占用")
                    subtitle: qsTr("处理器与存储资源")
                    accentColor: root.theme.accent2
                }

                GridLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    columns: 2
                    rows: 2
                    columnSpacing: 12
                    rowSpacing: 12

                    CircularGauge {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        value: root.host.cpuUsage
                        title: qsTr("CPU 占用")
                        subtitle: qsTr("板端总负载")
                        accent: root.theme.accent
                        theme: root.theme
                    }
                    CircularGauge {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        value: root.host.psramUsage
                        title: qsTr("PSRAM")
                        subtitle: qsTr("%1 / %2 MB").arg(root.host.freePsramMb.toFixed(2)).arg(root.host.totalPsramMb.toFixed(2))
                        accent: root.theme.accent2
                        theme: root.theme
                    }
                    CircularGauge {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        value: root.host.heapUsage
                        title: qsTr("堆内存")
                        subtitle: qsTr("%1 MB 空闲").arg(root.host.freeHeapMb.toFixed(2))
                        accent: root.theme.warn
                        theme: root.theme
                    }
                    CircularGauge {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        value: root.host.internalUsage
                        title: qsTr("内部内存")
                        subtitle: qsTr("%1 MB 空闲").arg(root.host.freeInternalMb.toFixed(2))
                        accent: root.theme.purple
                        theme: root.theme
                    }
                }
            }

            Rectangle {
                Layout.preferredWidth: 1
                Layout.fillHeight: true
                Layout.topMargin: 28
                color: root.theme.lineSoft
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 7
                spacing: 9

                SectionTitle {
                    Layout.fillWidth: true
                    title: qsTr("性能趋势")
                    subtitle: qsTr("短时波动与链路状态")
                    accentColor: root.theme.warn
                }

                TrendChart {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    theme: root.theme
                    history: root.host.metricHistory
                    title: qsTr("处理器与 PSRAM")
                    fieldA: "cpu"
                    fieldB: "psram"
                    colorA: root.theme.accent
                    colorB: root.theme.accent2
                }

                TrendChart {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    theme: root.theme
                    history: root.host.metricHistory
                    title: qsTr("堆内存与内部内存")
                    fieldA: "heap"
                    fieldB: "internal"
                    colorA: root.theme.warn
                    colorB: root.theme.purple
                }
            }
        }

        SectionTitle {
            Layout.fillWidth: true
            title: qsTr("分拣统计")
            subtitle: qsTr("业务吞吐与识别结果")
            accentColor: root.theme.purple
        }

        GridLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 96
            Layout.minimumHeight: 96
            columns: 4
            rows: 1
            columnSpacing: 12
            rowSpacing: 12

            Repeater {
                model: root.host.courierStatsCards
                delegate: MetricCard {
                    required property var modelData
                    Layout.fillWidth: true
                    Layout.fillHeight: true
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
