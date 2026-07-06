import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property var theme
    property var host
    opacity: StackLayout.isCurrentItem ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 220 } }

    component PortCard: PremiumPanel {
        id: portCard
        property string title: ""
        property string value: ""
        property string note: ""
        property color accent: root.theme.accent

        Layout.fillWidth: true
        Layout.preferredHeight: 78
        Layout.minimumWidth: 0
        radius: 14
        theme: root.theme
        topColor: theme.panelTop
        bottomColor: theme.panelBottom
        overlayColor: theme.glassOverlay
        borderColor: theme.panelStroke
        accentColor: portCard.accent
        accented: true
        glassOpacity: 0.22

        RowLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 12

            Rectangle {
                Layout.preferredWidth: 8
                Layout.fillHeight: true
                radius: 4
                color: portCard.accent
                opacity: 0.75
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.minimumWidth: 0
                spacing: 3

                Text {
                    Layout.fillWidth: true
                    text: portCard.title
                    color: theme.muted
                    font.pixelSize: 11
                    font.weight: Font.DemiBold
                    elide: Text.ElideRight
                }

                Text {
                    Layout.fillWidth: true
                    text: portCard.value
                    color: theme.text
                    font.pixelSize: portCard.value.length > 8 ? 18 : 24
                    font.weight: Font.Black
                    elide: Text.ElideRight
                    minimumPixelSize: 14
                    fontSizeMode: Text.Fit
                }

                Text {
                    Layout.fillWidth: true
                    text: portCard.note
                    color: theme.faint
                    font.pixelSize: 11
                    elide: Text.ElideRight
                }
            }
        }
    }

    GridLayout {
        anchors.fill: parent
        columns: 2
        columnSpacing: 14
        rowSpacing: 14

        PremiumPanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.rowSpan: 2
            radius: 18
            theme: root.theme
            topColor: theme.panelTop
            bottomColor: theme.panelBottom
            overlayColor: theme.glassOverlay
            borderColor: theme.panelStroke
            glassOpacity: 0.3

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 22
                spacing: 16

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4
                        Text {
                            text: "系统维护"
                            color: theme.text
                            font.pixelSize: 28
                            font.weight: Font.Black
                        }
                        Text {
                            text: "监听、链路、保存目录与运行日志"
                            color: theme.muted
                            font.pixelSize: 13
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }
                    }
                    StatusPill {
                        Layout.preferredWidth: 92
                        Layout.preferredHeight: 30
                        theme: root.theme
                        text: host.connected ? "在线" : "待连接"
                        fillColor: host.connected ? theme.statusBg : theme.warnWash
                        strokeColor: host.connected ? theme.accent : theme.warn
                        textColor: host.connected ? theme.statusText : theme.warnText
                        textSize: 11
                    }
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    columnSpacing: 12
                    rowSpacing: 12
                    MetricCard { Layout.fillWidth: true; theme: root.theme; title: "监听状态"; value: host.listening ? "已监听" : "未监听"; note: host.statusText; accent: host.listening ? theme.accent : theme.warn }
                    MetricCard { Layout.fillWidth: true; theme: root.theme; title: "最后遥测"; value: host.lastTelemetryTime; note: "遥测更新时间"; accent: theme.accent2 }
                    MetricCard { Layout.fillWidth: true; theme: root.theme; title: "累计接收"; value: (host.bytesReceived / 1048576).toFixed(2) + " MB"; note: "网络负载"; accent: theme.purple }
                    MetricCard { Layout.fillWidth: true; theme: root.theme; title: "保存目录"; value: "ESP32Host"; note: host.saveDir; accent: theme.warn }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12
                    Button {
                        id: listenButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 48
                        text: "重新监听"
                        hoverEnabled: true
                        onClicked: host.startServer()
                        contentItem: Text {
                            text: listenButton.text
                            color: theme.text
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 14
                            font.weight: Font.Black
                        }
                        background: Rectangle {
                            radius: 14
                            border.color: listenButton.hovered ? theme.accent : theme.panelStroke
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: listenButton.hovered ? theme.navActive : theme.panelTop }
                                GradientStop { position: 1.0; color: theme.panelBottom }
                            }
                        }
                    }
                    Button {
                        id: syncButton
                        Layout.fillWidth: true
                        Layout.preferredHeight: 48
                        text: "同步时间"
                        hoverEnabled: true
                        onClicked: host.sendTimeSync()
                        contentItem: Text {
                            text: syncButton.text
                            color: theme.text
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 14
                            font.weight: Font.Black
                        }
                        background: Rectangle {
                            radius: 14
                            border.color: syncButton.hovered ? theme.accent2 : theme.panelStroke
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: syncButton.hovered ? theme.navActive : theme.panelTop }
                                GradientStop { position: 1.0; color: theme.panelBottom }
                            }
                        }
                    }
                }

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 14
                    theme: root.theme
                    topColor: theme.panelTop
                    bottomColor: theme.panelBottom
                    overlayColor: theme.glassOverlay
                    borderColor: theme.panelStroke
                    glassOpacity: 0.2

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 14
                        spacing: 10
                        Text {
                            text: "运行日志"
                            color: theme.text
                            font.pixelSize: 17
                            font.weight: Font.Black
                        }
                        ListView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            spacing: 6
                            model: host.logLines
                            delegate: Text {
                                width: ListView.view.width
                                height: 22
                                text: modelData
                                color: index === 0 ? theme.accent : theme.muted
                                font.pixelSize: 12
                                elide: Text.ElideRight
                            }
                        }
                    }
                }
            }
        }

        PremiumPanel {
            Layout.fillWidth: true
            Layout.preferredHeight: 328
            radius: 18
            theme: root.theme
            topColor: theme.panelTop
            bottomColor: theme.panelBottom
            overlayColor: theme.glassOverlay
            borderColor: theme.panelStroke
            glassOpacity: 0.3

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 18
                spacing: 14
                Text { text: "链路端口"; color: theme.text; font.pixelSize: 22; font.weight: Font.Black }
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12
                    PortCard { title: "控制链路"; value: "5000"; note: "遥测 / 控制"; accent: theme.accent }
                    PortCard { title: "图像链路"; value: "5001"; note: "JPEG 预览"; accent: theme.accent2 }
                }
                PortCard {
                    Layout.fillWidth: true
                    title: "板端地址"
                    value: "192.168.10.2"
                    note: "ESP32 Ethernet"
                    accent: theme.warn
                }
                PortCard {
                    Layout.fillWidth: true
                    title: "上位机地址"
                    value: "192.168.10.1"
                    note: "PC Ethernet"
                    accent: theme.purple
                }
            }
        }

        PremiumPanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 18
            theme: root.theme
            topColor: theme.panelTop
            bottomColor: theme.panelBottom
            overlayColor: theme.glassOverlay
            borderColor: theme.panelStroke
            glassOpacity: 0.3

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 18
                spacing: 14
                Text { text: "图片链路健康"; color: theme.text; font.pixelSize: 22; font.weight: Font.Black }
                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    columnSpacing: 12
                    rowSpacing: 12
                    Repeater {
                        model: host.imageHealthCards
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
    }
}
