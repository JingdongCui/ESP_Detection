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
                    text: "核心执行控制"
                    color: theme.text
                    font.pixelSize: 22
                    font.weight: Font.Black
                }
                ControlSlider {
                    Layout.fillWidth: true
                    theme: root.theme
                    title: "相机/补光亮度"
                    note: "用于快递面单和标识区域的成像亮度测试"
                    value: host.brightness
                    onMoved: host.setBrightness(value)
                }
                ControlSlider {
                    Layout.fillWidth: true
                    theme: root.theme
                    title: "电机转速"
                    note: "控制传送结构的测试转速，后续可映射 PWM 或闭环速度"
                    value: host.motorSpeed
                    onMoved: host.setMotorSpeed(value)
                }
                ControlSlider {
                    Layout.fillWidth: true
                    theme: root.theme
                    title: "危险置信度阈值"
                    note: "检测置信度低于该值时，记录和检测框会使用强调色"
                    value: host.dangerThreshold
                    from: 1
                    to: 99
                    onMoved: host.setDangerThreshold(value)
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
                Text { text: "运行模块"; color: theme.text; font.pixelSize: 22; font.weight: Font.Black }
                Repeater {
                    model: [
                        {title: "补光灯", note: "提高低照度场景下 Logo 可见度", checked: host.lightEnabled, command: "fill_light"},
                        {title: "传送带", note: "控制快递件通过检测区域", checked: host.conveyorEnabled, command: "conveyor"},
                        {title: "自动运行", note: "检测、保存、告警链路自动执行", checked: host.autoRunEnabled, command: "auto_run"}
                    ]
                    delegate: PremiumPanel {
                        id: moduleCard
                        property bool moduleChecked: modelData.checked
                        Layout.fillWidth: true
                        Layout.preferredHeight: 60
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
                                text: moduleCard.moduleChecked ? "已启用" : "待机"
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
                                    if (modelData.command === "fill_light") host.setLightEnabled(checked)
                                    else if (modelData.command === "conveyor") host.setConveyorEnabled(checked)
                                    else host.setAutoRunEnabled(checked)
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
                Text { text: "高级测试模块"; color: theme.text; font.pixelSize: 22; font.weight: Font.Black }
                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    columnSpacing: 12
                    rowSpacing: 12
                    Repeater {
                        model: [
                            {title: "保存策略", value: "低置信度优先", accent: theme.danger},
                            {title: "模型模式", value: "YOLO INT8", accent: theme.accent},
                            {title: "链路策略", value: "TCP 实时流", accent: theme.accent2},
                            {title: "调试级别", value: "详细日志", accent: theme.warn}
                        ]
                        MetricCard {
                            Layout.fillWidth: true
                            theme: root.theme
                            title: modelData.title
                            value: modelData.value
                            note: "测试版占位控制"
                            accent: modelData.accent
                        }
                    }
                }
                Button {
                    id: syncButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: 48
                    text: "立即同步时间并刷新状态"
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
