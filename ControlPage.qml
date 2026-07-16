pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Basic
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
        Layout.preferredHeight: 70
        radius: 14
        color: checked ? root.theme.accentMist : root.theme.highlightSoft
        border.width: 1
        border.color: checked ? root.theme.accent : root.theme.lineSoft

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 14
            anchors.rightMargin: 12
            spacing: 12

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
                    font.pixelSize: 11
                    elide: Text.ElideRight
                }
            }

            StatusPill {
                Layout.preferredWidth: 56
                Layout.preferredHeight: 28
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
                enabled: toggleRow.enabled
                activeFocusOnTab: true
                onToggled: function(value) { toggleRow.toggled(value) }
            }
        }
    }

    component InfoTile: Rectangle {
        id: infoTile

        property string title: ""
        property string value: "--"
        property color accent: root.theme.accent

        Layout.fillWidth: true
        Layout.preferredHeight: 72
        radius: 13
        color: root.theme.highlightSoft
        border.width: 1
        border.color: root.theme.lineSoft

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 3

            Text {
                Layout.fillWidth: true
                text: infoTile.title
                color: root.theme.muted
                font.pixelSize: 11
                elide: Text.ElideRight
            }
            Text {
                Layout.fillWidth: true
                text: infoTile.value
                color: infoTile.accent
                font.pixelSize: 16
                font.weight: Font.Black
                elide: Text.ElideRight
            }
        }
    }

    ScrollView {
        id: controlPageScroll
        anchors.fill: parent
        contentWidth: availableWidth
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        ColumnLayout {
            width: controlPageScroll.availableWidth - 12
            spacing: 14

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 62
                spacing: 12

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 3

                    Text {
                        Layout.fillWidth: true
                        text: qsTr("设备控制")
                        color: root.theme.text
                        font.pixelSize: 24
                        font.weight: Font.Black
                    }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("显示、视觉、分拣执行与网络上报参数")
                        color: root.theme.muted
                        font.pixelSize: 12
                    }
                }

                StatusPill {
                    Layout.preferredWidth: 112
                    Layout.preferredHeight: 34
                    theme: root.theme
                    text: root.host.connected ? qsTr("设备在线") : qsTr("设备离线")
                    fillColor: root.host.connected ? root.theme.statusBg : root.theme.warnWash
                    strokeColor: root.host.connected ? root.theme.accent : root.theme.warn
                    textColor: root.host.connected ? root.theme.statusText : root.theme.warnText
                    textSize: 11
                }

                StatusPill {
                    Layout.preferredWidth: 170
                    Layout.preferredHeight: 34
                    theme: root.theme
                    text: root.host.controlStatusText
                    fillColor: root.theme.highlightSoft
                    strokeColor: root.theme.lineSoft
                    textColor: root.theme.muted
                    textSize: 10
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 620
                spacing: 14

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    theme: root.theme
                    radius: 18
                    topColor: root.theme.panelTop
                    bottomColor: root.theme.panelBottom
                    overlayColor: root.theme.glassOverlay
                    borderColor: root.theme.panelStroke
                    glassOpacity: 0.26

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 10

                        Text {
                            Layout.fillWidth: true
                            text: qsTr("显示与画面")
                            color: root.theme.text
                            font.pixelSize: 20
                            font.weight: Font.Black
                        }
                        Text {
                            Layout.fillWidth: true
                            text: qsTr("屏幕背光与摄像头 ISP 实时参数")
                            color: root.theme.muted
                            font.pixelSize: 11
                        }

                        ControlSlider {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 128
                            theme: root.theme
                            title: qsTr("屏幕亮度")
                            note: qsTr("调整板端 LCD 背光")
                            value: root.host.screenBrightness
                            enabled: root.host.connected
                            onMoved: root.host.setScreenBrightness(value)
                        }

                        ControlSlider {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 128
                            theme: root.theme
                            title: qsTr("画面亮度")
                            note: root.host.cameraBrightnessSupported ? qsTr("摄像头 ISP 亮度") : qsTr("当前摄像头不支持")
                            value: root.host.cameraBrightness
                            from: root.host.cameraBrightnessMin
                            to: root.host.cameraBrightnessMax
                            stepSize: root.host.cameraBrightnessStep
                            suffix: ""
                            enabled: root.host.connected && root.host.cameraBrightnessSupported
                            onMoved: root.host.setCameraBrightness(value)
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 3
                            columnSpacing: 10
                            rowSpacing: 10
                            InfoTile { title: qsTr("当前曝光"); value: root.host.exposureText; accent: root.theme.accent2 }
                            InfoTile { title: qsTr("当前增益"); value: root.host.gainText; accent: root.theme.warn }
                            InfoTile { title: qsTr("当前白平衡"); value: root.host.whiteBalanceText; accent: root.theme.accent }
                        }
                    }
                }

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    theme: root.theme
                    radius: 18
                    topColor: root.theme.panelTop
                    bottomColor: root.theme.panelBottom
                    overlayColor: root.theme.glassOverlay
                    borderColor: root.theme.panelStroke
                    glassOpacity: 0.26

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 9

                        Text {
                            Layout.fillWidth: true
                            text: qsTr("ISP 色彩")
                            color: root.theme.text
                            font.pixelSize: 20
                            font.weight: Font.Black
                        }

                        ToggleRow {
                            title: qsTr("自动对比度")
                            note: qsTr("开启时禁用手动对比度")
                            checked: root.host.cameraContrastAuto
                            enabled: root.host.connected && root.host.cameraContrastSupported
                            onToggled: function(value) { root.host.setCameraContrastAuto(value) }
                        }
                        ControlSlider {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 122
                            theme: root.theme
                            title: qsTr("对比度")
                            note: qsTr("摄像头画面对比度")
                            value: root.host.cameraContrast
                            from: root.host.cameraContrastMin
                            to: root.host.cameraContrastMax
                            stepSize: root.host.cameraContrastStep
                            suffix: ""
                            enabled: root.host.connected && root.host.cameraContrastSupported && !root.host.cameraContrastAuto
                            onMoved: root.host.setCameraContrast(value)
                        }
                        ToggleRow {
                            title: qsTr("自动饱和度")
                            note: qsTr("开启时禁用手动饱和度")
                            checked: root.host.cameraSaturationAuto
                            enabled: root.host.connected && root.host.cameraSaturationSupported
                            onToggled: function(value) { root.host.setCameraSaturationAuto(value) }
                        }
                        ControlSlider {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 122
                            theme: root.theme
                            title: qsTr("饱和度")
                            note: qsTr("摄像头画面色彩浓度")
                            value: root.host.cameraSaturation
                            from: root.host.cameraSaturationMin
                            to: root.host.cameraSaturationMax
                            stepSize: root.host.cameraSaturationStep
                            suffix: ""
                            enabled: root.host.connected && root.host.cameraSaturationSupported && !root.host.cameraSaturationAuto
                            onMoved: root.host.setCameraSaturation(value)
                        }
                        ControlSlider {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 122
                            theme: root.theme
                            title: qsTr("色相")
                            note: qsTr("摄像头画面色相偏移")
                            value: root.host.cameraHue
                            from: root.host.cameraHueMin
                            to: root.host.cameraHueMax
                            stepSize: root.host.cameraHueStep
                            suffix: ""
                            enabled: root.host.connected && root.host.cameraHueSupported
                            onMoved: root.host.setCameraHue(value)
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 560
                spacing: 14

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    theme: root.theme
                    radius: 18
                    topColor: root.theme.panelTop
                    bottomColor: root.theme.panelBottom
                    overlayColor: root.theme.glassOverlay
                    borderColor: root.theme.panelStroke
                    glassOpacity: 0.26

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 10

                        Text {
                            Layout.fillWidth: true
                            text: qsTr("检测设置")
                            color: root.theme.text
                            font.pixelSize: 20
                            font.weight: Font.Black
                        }
                        ToggleRow {
                            title: qsTr("检测")
                            note: qsTr("启用板端视觉推理")
                            checked: root.host.detectionEnabled
                            enabled: root.host.connected
                            onToggled: function(value) { root.host.setDetectionEnabled(value) }
                        }
                        ToggleRow {
                            title: qsTr("预览叠加框")
                            note: qsTr("显示目标框、类别与置信度")
                            checked: root.host.previewOverlayEnabled
                            enabled: root.host.connected
                            onToggled: function(value) { root.host.setPreviewOverlayEnabled(value) }
                        }
                        ControlSlider {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 132
                            theme: root.theme
                            title: qsTr("面单置信度阈值")
                            note: qsTr("面单模型最低有效分数")
                            value: root.host.waybillThreshold
                            enabled: root.host.connected
                            onMoved: root.host.setWaybillThreshold(value)
                        }
                        ControlSlider {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 132
                            theme: root.theme
                            title: qsTr("Logo 置信度阈值")
                            note: qsTr("Logo 模型最低有效分数")
                            value: root.host.logoThreshold
                            enabled: root.host.connected
                            onMoved: root.host.setLogoThreshold(value)
                        }
                    }
                }

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    theme: root.theme
                    radius: 18
                    topColor: root.theme.panelTop
                    bottomColor: root.theme.panelBottom
                    overlayColor: root.theme.glassOverlay
                    borderColor: root.theme.panelStroke
                    glassOpacity: 0.26

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 10

                        Text {
                            Layout.fillWidth: true
                            text: qsTr("分拣开环速度")
                            color: root.theme.text
                            font.pixelSize: 20
                            font.weight: Font.Black
                        }
                        Text {
                            Layout.fillWidth: true
                            text: qsTr("A、B、C 三路独立设置；不包含 PID 闭环控制")
                            color: root.theme.muted
                            font.pixelSize: 11
                        }
                        ControlSlider {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 142
                            theme: root.theme
                            title: qsTr("A 路电机")
                            note: qsTr("入口输送带速度")
                            value: root.host.motorASpeed
                            enabled: root.host.connected
                            onMoved: root.host.setMotorASpeed(value)
                        }
                        ControlSlider {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 142
                            theme: root.theme
                            title: qsTr("B 路电机")
                            note: qsTr("中段输送带速度")
                            value: root.host.motorBSpeed
                            enabled: root.host.connected
                            onMoved: root.host.setMotorBSpeed(value)
                        }
                        ControlSlider {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 142
                            theme: root.theme
                            title: qsTr("C 路电机")
                            note: qsTr("末段输送带速度")
                            value: root.host.motorCSpeed
                            enabled: root.host.connected
                            onMoved: root.host.setMotorCSpeed(value)
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 330
                Layout.bottomMargin: 8
                spacing: 14

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    theme: root.theme
                    radius: 18
                    topColor: root.theme.panelTop
                    bottomColor: root.theme.panelBottom
                    overlayColor: root.theme.glassOverlay
                    borderColor: root.theme.panelStroke
                    glassOpacity: 0.26

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 10

                        Text {
                            Layout.fillWidth: true
                            text: qsTr("网络上报")
                            color: root.theme.text
                            font.pixelSize: 20
                            font.weight: Font.Black
                        }
                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            columnSpacing: 10
                            InfoTile { title: qsTr("板端 IP"); value: root.host.localIp; accent: root.theme.accent2 }
                            InfoTile { title: qsTr("上位机 IP"); value: root.host.hostIp; accent: root.theme.accent }
                        }
                        ToggleRow {
                            title: qsTr("上报图像")
                            note: qsTr("允许板端发送识别快照")
                            checked: root.host.reportImageEnabled
                            enabled: root.host.connected
                            onToggled: function(value) { root.host.setReportImageEnabled(value) }
                        }
                        ToggleRow {
                            title: qsTr("上报指标")
                            note: qsTr("允许板端发送性能遥测")
                            checked: root.host.reportMetricsEnabled
                            enabled: root.host.connected
                            onToggled: function(value) { root.host.setReportMetricsEnabled(value) }
                        }
                    }
                }

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    theme: root.theme
                    radius: 18
                    topColor: root.theme.panelTop
                    bottomColor: root.theme.panelBottom
                    overlayColor: root.theme.glassOverlay
                    borderColor: root.theme.panelStroke
                    glassOpacity: 0.26

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        Text {
                            Layout.fillWidth: true
                            text: qsTr("设备信息与操作")
                            color: root.theme.text
                            font.pixelSize: 20
                            font.weight: Font.Black
                        }
                        InfoTile {
                            title: qsTr("模型信息")
                            value: root.host.modelInfo
                            accent: root.theme.accent2
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            spacing: 10

                            Button {
                                id: refreshButton
                                Layout.fillWidth: true
                                Layout.preferredHeight: 52
                                text: qsTr("刷新设备状态")
                                enabled: root.host.connected
                                activeFocusOnTab: true
                                onClicked: root.host.requestDeviceState()
                                contentItem: Text {
                                    text: refreshButton.text
                                    color: root.theme.text
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.pixelSize: 13
                                    font.weight: Font.Bold
                                }
                                background: Rectangle {
                                    radius: 14
                                    color: root.theme.surfaceRaised
                                    border.width: 1
                                    border.color: refreshButton.activeFocus ? root.theme.accent : root.theme.panelStroke
                                }
                            }

                            Button {
                                id: syncButton
                                Layout.fillWidth: true
                                Layout.preferredHeight: 52
                                text: qsTr("同步时间")
                                enabled: root.host.connected
                                activeFocusOnTab: true
                                onClicked: root.host.sendTimeSync()
                                contentItem: Text {
                                    text: syncButton.text
                                    color: root.theme.text
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.pixelSize: 13
                                    font.weight: Font.Bold
                                }
                                background: Rectangle {
                                    radius: 14
                                    color: root.theme.surfaceRaised
                                    border.width: 1
                                    border.color: syncButton.activeFocus ? root.theme.accent : root.theme.panelStroke
                                }
                            }

                            Button {
                                id: restartButton
                                Layout.fillWidth: true
                                Layout.preferredHeight: 52
                                text: qsTr("重启设备")
                                enabled: root.host.connected
                                activeFocusOnTab: true
                                onClicked: root.host.restartDevice()
                                contentItem: Text {
                                    text: restartButton.text
                                    color: root.theme.warnText
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    font.pixelSize: 13
                                    font.weight: Font.Black
                                }
                                background: Rectangle {
                                    radius: 14
                                    color: root.theme.warnWash
                                    border.width: 1
                                    border.color: restartButton.activeFocus ? root.theme.danger : root.theme.warn
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
