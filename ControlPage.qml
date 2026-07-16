pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property var theme
    property var host

    opacity: StackLayout.isCurrentItem ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 180 } }

    component ToggleRow: Rectangle {
        id: toggleRow

        property string title: ""
        property string note: ""
        property bool checked: false
        signal toggled(bool checked)

        Layout.fillWidth: true
        Layout.preferredHeight: 78
        radius: 14
        color: checked ? root.theme.accentMist : root.theme.highlightSoft
        border.width: 1
        border.color: checked ? root.theme.accent : root.theme.lineSoft

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 14
            anchors.rightMargin: 12
            spacing: 12

            Rectangle {
                Layout.preferredWidth: 34
                Layout.preferredHeight: 34
                radius: 11
                color: toggleRow.checked ? root.theme.accentWash : root.theme.track
                border.width: 1
                border.color: toggleRow.checked ? root.theme.accent : root.theme.line

                Rectangle {
                    anchors.centerIn: parent
                    width: 8
                    height: 8
                    radius: 4
                    color: toggleRow.checked ? root.theme.accent : root.theme.faint
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 3

                Text {
                    Layout.fillWidth: true
                    text: toggleRow.title
                    color: root.theme.text
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    elide: Text.ElideRight
                }

                Text {
                    Layout.fillWidth: true
                    text: toggleRow.note
                    color: root.theme.muted
                    font.pixelSize: 10
                    elide: Text.ElideRight
                }
            }

            StatusPill {
                Layout.preferredWidth: 56
                Layout.preferredHeight: 26
                theme: root.theme
                text: toggleRow.checked ? qsTr("开启") : qsTr("关闭")
                fillColor: toggleRow.checked ? root.theme.statusBg : root.theme.track
                strokeColor: toggleRow.checked ? root.theme.accent : root.theme.lineSoft
                textColor: toggleRow.checked ? root.theme.statusText : root.theme.faint
                textSize: 10
            }

            PremiumSwitch {
                Layout.preferredWidth: 68
                Layout.preferredHeight: 34
                theme: root.theme
                checked: toggleRow.checked
                onToggled: function(value) { toggleRow.toggled(value) }
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 14

        PremiumPanel {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 7
            radius: 18
            theme: root.theme
            topColor: theme.panelTop
            bottomColor: theme.panelBottom
            overlayColor: theme.glassOverlay
            borderColor: theme.panelStroke
            glassOpacity: 0.26

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 18
                spacing: 12

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 3

                        Text {
                            Layout.fillWidth: true
                            text: qsTr("参数控制")
                            color: root.theme.text
                            font.pixelSize: 22
                            font.weight: Font.Black
                            elide: Text.ElideRight
                        }

                        Text {
                            Layout.fillWidth: true
                            text: qsTr("调整显示、识别与执行机构参数")
                            color: root.theme.muted
                            font.pixelSize: 11
                            elide: Text.ElideRight
                        }
                    }

                    StatusPill {
                        Layout.preferredWidth: 88
                        Layout.preferredHeight: 30
                        theme: root.theme
                        text: root.host.connected ? qsTr("设备在线") : qsTr("本地配置")
                        fillColor: root.host.connected ? root.theme.statusBg : root.theme.warnWash
                        strokeColor: root.host.connected ? root.theme.accent : root.theme.warn
                        textColor: root.host.connected ? root.theme.statusText : root.theme.warnText
                        textSize: 10
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    color: root.theme.lineSoft
                }

                ControlSlider {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 142
                    theme: root.theme
                    title: qsTr("屏幕亮度")
                    note: qsTr("调整上位机显示亮度状态")
                    value: root.host.brightness
                    onMoved: root.host.setBrightness(value)
                }

                ControlSlider {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 142
                    theme: root.theme
                    title: qsTr("置信度阈值")
                    note: qsTr("低于阈值的检测结果使用警告色突出")
                    value: root.host.dangerThreshold
                    from: 0
                    to: 100
                    onMoved: root.host.setDangerThreshold(value)
                }

                ControlSlider {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 142
                    theme: root.theme
                    title: qsTr("电机转速")
                    note: qsTr("向板端下发三路电机速度参数")
                    value: root.host.motorSpeed
                    onMoved: root.host.setMotorSpeed(value)
                }

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 108
                    radius: 15
                    theme: root.theme
                    topColor: root.theme.navTop
                    bottomColor: root.theme.navBottom
                    overlayColor: root.theme.accentMist
                    borderColor: root.theme.lineSoft
                    elevated: false
                    glassOpacity: 0.22

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 14
                        spacing: 12

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4
                            Text {
                                text: qsTr("当前控制摘要")
                                color: root.theme.text
                                font.pixelSize: 13
                                font.weight: Font.Bold
                            }
                            Text {
                                Layout.fillWidth: true
                                text: root.host.connected
                                      ? qsTr("参数调整将通过控制链路发送至 ESP32")
                                      : qsTr("设备未连接，当前调整保留为本地状态")
                                color: root.theme.muted
                                font.pixelSize: 10
                                wrapMode: Text.WordWrap
                            }
                        }

                        Rectangle {
                            Layout.preferredWidth: 1
                            Layout.preferredHeight: 48
                            color: root.theme.lineSoft
                        }

                        ColumnLayout {
                            spacing: 2
                            Text { text: qsTr("亮度"); color: root.theme.faint; font.pixelSize: 9 }
                            Text { text: root.host.brightness + "%"; color: root.theme.text; font.pixelSize: 17; font.weight: Font.Black }
                        }
                        ColumnLayout {
                            spacing: 2
                            Text { text: qsTr("阈值"); color: root.theme.faint; font.pixelSize: 9 }
                            Text { text: root.host.dangerThreshold + "%"; color: root.theme.text; font.pixelSize: 17; font.weight: Font.Black }
                        }
                        ColumnLayout {
                            spacing: 2
                            Text { text: qsTr("转速"); color: root.theme.faint; font.pixelSize: 9 }
                            Text { text: root.host.motorSpeed + "%"; color: root.theme.text; font.pixelSize: 17; font.weight: Font.Black }
                        }
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 5
            spacing: 14

            PremiumPanel {
                Layout.fillWidth: true
                Layout.preferredHeight: 244
                radius: 18
                theme: root.theme
                topColor: theme.panelTop
                bottomColor: theme.panelBottom
                overlayColor: theme.glassOverlay
                borderColor: theme.panelStroke
                glassOpacity: 0.26

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 10

                    Text {
                        Layout.fillWidth: true
                        text: qsTr("检测显示")
                        color: root.theme.text
                        font.pixelSize: 19
                        font.weight: Font.Black
                    }

                    Text {
                        Layout.fillWidth: true
                        text: qsTr("控制上位机预览中的检测信息")
                        color: root.theme.muted
                        font.pixelSize: 10
                        elide: Text.ElideRight
                    }

                    ToggleRow {
                        title: qsTr("检测结果")
                        note: qsTr("在预览画面中显示识别结果")
                        checked: root.host.detectionEnabled
                        onToggled: function(value) { root.host.setDetectionEnabled(value) }
                    }

                    ToggleRow {
                        title: qsTr("预览叠加框")
                        note: qsTr("显示目标框、类别与置信度")
                        checked: root.host.previewOverlayEnabled
                        onToggled: function(value) { root.host.setPreviewOverlayEnabled(value) }
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
                glassOpacity: 0.26

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 10

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2
                            Text {
                                Layout.fillWidth: true
                                text: qsTr("运行与链路")
                                color: root.theme.text
                                font.pixelSize: 19
                                font.weight: Font.Black
                            }
                            Text {
                                Layout.fillWidth: true
                                text: qsTr("端口状态、当前参数和最近日志")
                                color: root.theme.muted
                                font.pixelSize: 10
                                elide: Text.ElideRight
                            }
                        }

                        Button {
                            id: syncButton
                            Layout.preferredWidth: 94
                            Layout.preferredHeight: 38
                            text: qsTr("同步时间")
                            hoverEnabled: true
                            onClicked: root.host.sendTimeSync()

                            contentItem: Text {
                                text: syncButton.text
                                color: syncButton.hovered ? root.theme.onAccent : root.theme.text
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 11
                                font.weight: Font.Bold
                            }

                            background: Rectangle {
                                radius: 12
                                color: syncButton.hovered ? root.theme.accent : root.theme.surfaceRaised
                                border.width: 1
                                border.color: syncButton.hovered ? root.theme.accent : root.theme.panelStroke
                                Behavior on color { ColorAnimation { duration: 140 } }
                            }
                        }
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 194
                        columns: 2
                        rows: 2
                        columnSpacing: 10
                        rowSpacing: 10

                        MetricCard {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            theme: root.theme
                            title: qsTr("图片端口")
                            value: "5001"
                            note: qsTr("JPEG 预览")
                            accent: root.theme.accent2
                        }
                        MetricCard {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            theme: root.theme
                            title: qsTr("控制端口")
                            value: "5000"
                            note: qsTr("遥测与控制")
                            accent: root.theme.accent
                        }
                        MetricCard {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            theme: root.theme
                            title: qsTr("当前阈值")
                            value: root.host.dangerThreshold + "%"
                            note: qsTr("检测显示")
                            accent: root.theme.danger
                        }
                        MetricCard {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            theme: root.theme
                            title: qsTr("电机速度")
                            value: root.host.motorSpeed + "%"
                            note: qsTr("板端参数")
                            accent: root.theme.warn
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8
                        Text {
                            text: qsTr("最近日志")
                            color: root.theme.text
                            font.pixelSize: 12
                            font.weight: Font.Bold
                        }
                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 1
                            color: root.theme.lineSoft
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        radius: 12
                        color: root.theme.highlightSoft
                        border.width: 1
                        border.color: root.theme.lineSoft

                        ListView {
                            anchors.fill: parent
                            anchors.margins: 10
                            clip: true
                            spacing: 4
                            model: root.host.logLines

                            delegate: Text {
                                required property int index
                                required property var modelData
                                width: ListView.view.width
                                height: 22
                                text: modelData
                                color: index === 0 ? root.theme.accent : root.theme.muted
                                font.pixelSize: 10
                                elide: Text.ElideRight
                            }
                        }
                    }
                }
            }
        }
    }
}
