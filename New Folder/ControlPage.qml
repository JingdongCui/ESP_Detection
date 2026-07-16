import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property var theme
    property var host
    opacity: StackLayout.isCurrentItem ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 220 } }

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
                anchors.margins: 18
                spacing: 14

                Text {
                    text: "板端普通设置"
                    color: theme.text
                    font.pixelSize: 22
                    font.weight: Font.Black
                }

                ControlSlider {
                    Layout.fillWidth: true
                    theme: root.theme
                    title: "屏幕亮度"
                    note: "对齐板端亮度滑块；当前作为上位机本地显示状态"
                    value: host.brightness
                    onMoved: host.setBrightness(value)
                }

                ControlSlider {
                    Layout.fillWidth: true
                    theme: root.theme
                    title: "置信度阈值"
                    note: "低于阈值的检测框和历史记录使用警告色"
                    value: host.dangerThreshold
                    from: 0
                    to: 100
                    onMoved: host.setDangerThreshold(value)
                }

                ControlSlider {
                    Layout.fillWidth: true
                    theme: root.theme
                    title: "电机转速"
                    note: "发送 CONFIG a_speed/b_speed/c_speed 到板端"
                    value: host.motorSpeed
                    onMoved: host.setMotorSpeed(value)
                }

                Item { Layout.fillHeight: true }
            }
        }

        PremiumPanel {
            Layout.fillWidth: true
            Layout.preferredHeight: 270
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
                Text { text: "检测显示"; color: theme.text; font.pixelSize: 22; font.weight: Font.Black }

                Repeater {
                    model: [
                        {title: "检测", note: "仅影响上位机显示，不改变板端视觉任务", checked: host.detectionEnabled, command: "detect"},
                        {title: "预览叠加框", note: "对齐板端预览叠加框开关", checked: host.previewOverlayEnabled, command: "overlay"}
                    ]
                    delegate: PremiumPanel {
                        id: moduleCard
                        property bool moduleChecked: modelData.checked
                        Layout.fillWidth: true
                        Layout.preferredHeight: 72
                        theme: root.theme
                        radius: 14
                        topColor: moduleChecked ? theme.navActive : theme.panelTop
                        bottomColor: theme.panelBottom
                        overlayColor: moduleChecked ? theme.accentMist : theme.glassOverlay
                        borderColor: moduleChecked ? theme.accent : theme.panelStroke
                        accentColor: theme.accent
                        accented: moduleChecked
                        glassOpacity: moduleChecked ? 0.36 : 0.22

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2
                                Text { text: modelData.title; color: theme.text; font.pixelSize: 14; font.weight: Font.Black }
                                Text { text: modelData.note; color: theme.muted; font.pixelSize: 11; elide: Text.ElideRight; Layout.fillWidth: true }
                            }
                            StatusPill {
                                Layout.preferredWidth: 58
                                Layout.preferredHeight: 24
                                theme: root.theme
                                text: moduleCard.moduleChecked ? "开启" : "关闭"
                                fillColor: moduleCard.moduleChecked ? theme.statusBg : theme.track
                                strokeColor: moduleCard.moduleChecked ? theme.accent : theme.lineSoft
                                textColor: moduleCard.moduleChecked ? theme.statusText : theme.faint
                                textSize: 10
                            }
                            PremiumSwitch {
                                Layout.preferredWidth: 74
                                Layout.preferredHeight: 36
                                theme: root.theme
                                checked: moduleCard.moduleChecked
                                onToggled: function(checked) {
                                    moduleCard.moduleChecked = checked
                                    if (modelData.command === "detect") host.setDetectionEnabled(checked)
                                    else host.setPreviewOverlayEnabled(checked)
                                }
                            }
                        }
                    }
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
                Text { text: "链路日志"; color: theme.text; font.pixelSize: 22; font.weight: Font.Black }
                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    columnSpacing: 12
                    rowSpacing: 12
                    MetricCard { Layout.fillWidth: true; theme: root.theme; title: "图片端口"; value: "5001"; note: "JPEG 预览"; accent: theme.accent2 }
                    MetricCard { Layout.fillWidth: true; theme: root.theme; title: "控制端口"; value: "5000"; note: "遥测 / 控制命令"; accent: theme.accent }
                    MetricCard { Layout.fillWidth: true; theme: root.theme; title: "阈值"; value: host.dangerThreshold + "%"; note: "本地显示"; accent: theme.danger }
                    MetricCard { Layout.fillWidth: true; theme: root.theme; title: "电机速度"; value: host.motorSpeed + "%"; note: "下发板端"; accent: theme.warn }
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
                        border.color: syncButton.hovered ? theme.accent : theme.panelStroke
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: syncButton.hovered ? theme.navActive : theme.panelTop }
                            GradientStop { position: 1.0; color: theme.panelBottom }
                        }
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 1
                            radius: parent.radius - 1
                            color: theme.accentMist
                            opacity: syncButton.hovered ? 0.85 : 0.35
                            Behavior on opacity { NumberAnimation { duration: 170 } }
                        }
                        Behavior on border.color { ColorAnimation { duration: 170 } }
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
                    ListView {
                        anchors.fill: parent
                        anchors.margins: 12
                        clip: true
                        model: host.logLines
                        delegate: Text {
                            width: ListView.view.width
                            height: 24
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
}
