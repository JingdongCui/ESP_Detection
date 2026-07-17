pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: root

    property var theme
    property var host
    readonly property bool compact: height < 700 || width < 1050

    opacity: StackLayout.isCurrentItem ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 180 } }

    component CompactSlider: Item {
        id: compactSlider

        property string title: ""
        property int value: 0
        property int from: 0
        property int to: 100
        property int stepSize: 1
        property string suffix: "%"
        property int displayValue: value
        signal moved(int value)
        signal committed(int value)

        Layout.fillWidth: true
        Layout.minimumWidth: 0
        Layout.preferredHeight: root.compact ? 34 : 38
        opacity: enabled ? 1 : 0.42

        onValueChanged: {
            if (!slider.pressed)
                displayValue = value
        }

        RowLayout {
            anchors.fill: parent
            spacing: root.compact ? 6 : 8

            Text {
                Layout.preferredWidth: root.compact ? 88 : 104
                Layout.minimumWidth: 72
                text: compactSlider.title
                color: root.theme.text
                font.pixelSize: root.compact ? 11 : 12
                font.weight: Font.DemiBold
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }

            Slider {
                id: slider

                Layout.fillWidth: true
                Layout.minimumWidth: 50
                Layout.preferredHeight: 28
                from: compactSlider.from
                to: compactSlider.to
                stepSize: compactSlider.stepSize
                value: compactSlider.displayValue
                enabled: compactSlider.enabled
                activeFocusOnTab: true
                hoverEnabled: true

                onMoved: {
                    compactSlider.displayValue = Math.round(value)
                    compactSlider.moved(compactSlider.displayValue)
                }
                onPressedChanged: {
                    if (!pressed) {
                        compactSlider.displayValue = Math.round(value)
                        compactSlider.committed(compactSlider.displayValue)
                    }
                }

                background: Rectangle {
                    x: slider.leftPadding
                    y: slider.topPadding + slider.availableHeight / 2 - height / 2
                    width: slider.availableWidth
                    height: root.compact ? 7 : 8
                    radius: height / 2
                    color: root.theme.track
                    border.width: 1
                    border.color: root.theme.lineSoft

                    Rectangle {
                        width: Math.max(parent.height, slider.visualPosition * parent.width)
                        height: parent.height
                        radius: height / 2
                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop { position: 0.0; color: root.theme.accent2 }
                            GradientStop { position: 1.0; color: root.theme.accent }
                        }
                    }
                }

                handle: Rectangle {
                    x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                    y: slider.topPadding + slider.availableHeight / 2 - height / 2
                    implicitWidth: slider.pressed ? 20 : 17
                    implicitHeight: implicitWidth
                    radius: width / 2
                    color: root.theme.thumb
                    border.width: 2
                    border.color: root.theme.accent

                    Rectangle {
                        anchors.centerIn: parent
                        width: 7
                        height: 7
                        radius: 4
                        color: root.theme.accent
                    }

                    Behavior on implicitWidth { NumberAnimation { duration: 120; easing.type: Easing.OutCubic } }
                }
            }

            Rectangle {
                Layout.preferredWidth: root.compact ? 42 : 48
                Layout.preferredHeight: 24
                radius: 8
                color: root.theme.accentMist
                border.width: 1
                border.color: slider.pressed ? root.theme.accent : root.theme.lineSoft

                Text {
                    anchors.centerIn: parent
                    text: compactSlider.displayValue + compactSlider.suffix
                    color: root.theme.accent2
                    font.pixelSize: root.compact ? 10 : 11
                    font.weight: Font.Black
                }
            }
        }
    }

    component CompactToggle: Button {
        id: compactToggle

        property color accent: root.theme.accent

        Layout.fillWidth: true
        Layout.minimumWidth: 0
        Layout.preferredHeight: root.compact ? 32 : 36
        checkable: false
        hoverEnabled: true
        activeFocusOnTab: true
        opacity: enabled ? 1 : 0.42

        contentItem: RowLayout {
            spacing: 6

            Text {
                Layout.fillWidth: true
                Layout.minimumWidth: 0
                text: compactToggle.text
                color: root.theme.text
                font.pixelSize: root.compact ? 10 : 11
                font.weight: Font.DemiBold
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                text: compactToggle.checked ? qsTr("开启") : qsTr("关闭")
                color: compactToggle.checked ? compactToggle.accent : root.theme.faint
                font.pixelSize: 9
                font.weight: Font.Bold
            }

            Rectangle {
                Layout.preferredWidth: root.compact ? 32 : 36
                Layout.preferredHeight: root.compact ? 18 : 20
                radius: height / 2
                color: compactToggle.checked ? root.theme.navActive : root.theme.track
                border.width: 1
                border.color: compactToggle.checked ? compactToggle.accent : root.theme.line

                Rectangle {
                    width: parent.height - 6
                    height: width
                    radius: width / 2
                    x: compactToggle.checked ? parent.width - width - 3 : 3
                    anchors.verticalCenter: parent.verticalCenter
                    color: compactToggle.checked ? compactToggle.accent : root.theme.faint
                    Behavior on x { NumberAnimation { duration: 160; easing.type: Easing.OutCubic } }
                }
            }
        }

        background: Rectangle {
            radius: 9
            color: compactToggle.checked
                ? root.theme.accentMist
                : (compactToggle.hovered ? root.theme.highlightSoft : "transparent")
            border.width: 1
            border.color: compactToggle.activeFocus
                ? compactToggle.accent
                : (compactToggle.checked ? root.theme.softAccent : root.theme.lineSoft)
        }
    }

    component InfoTile: Rectangle {
        id: infoTile

        property string title: ""
        property string value: "--"
        property color accent: root.theme.accent

        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.minimumWidth: 0
        Layout.minimumHeight: 0
        radius: 11
        color: root.theme.highlightSoft
        border.width: 1
        border.color: root.theme.lineSoft

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: root.compact ? 9 : 11
            spacing: 3

            Text {
                Layout.fillWidth: true
                text: infoTile.title
                color: root.theme.muted
                font.pixelSize: root.compact ? 10 : 11
                font.weight: Font.DemiBold
                elide: Text.ElideRight
            }

            Text {
                Layout.fillWidth: true
                text: infoTile.value
                color: infoTile.accent
                font.pixelSize: root.compact ? 14 : 16
                font.weight: Font.Black
                fontSizeMode: Text.Fit
                minimumPixelSize: 10
                elide: Text.ElideRight
            }
        }
    }

    component SectionTitle: RowLayout {
        id: sectionTitle

        property string title: ""
        property string badge: ""
        property color accent: root.theme.accent

        Layout.fillWidth: true
        Layout.minimumWidth: 0
        Layout.preferredHeight: root.compact ? 22 : 26
        spacing: 6

        Rectangle {
            Layout.preferredWidth: 4
            Layout.preferredHeight: 16
            radius: 2
            color: sectionTitle.accent
        }

        Text {
            Layout.fillWidth: true
            Layout.minimumWidth: 0
            text: sectionTitle.title
            color: root.theme.text
            font.pixelSize: root.compact ? 14 : 16
            font.weight: Font.Black
            elide: Text.ElideRight
        }

        Text {
            visible: sectionTitle.badge.length > 0
            text: sectionTitle.badge
            color: root.theme.faint
            font.pixelSize: 9
            elide: Text.ElideRight
        }
    }

    component ActionButton: Button {
        id: actionButton

        property color accent: root.theme.accent
        property bool dangerous: false

        Layout.fillWidth: true
        Layout.preferredHeight: root.compact ? 38 : 42
        hoverEnabled: true
        activeFocusOnTab: true
        opacity: enabled ? 1 : 0.42

        contentItem: Text {
            text: actionButton.text
            color: actionButton.dangerous ? root.theme.danger : root.theme.text
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: root.compact ? 11 : 12
            font.weight: Font.Black
        }

        background: Rectangle {
            radius: 10
            color: actionButton.down
                ? Qt.rgba(actionButton.accent.r, actionButton.accent.g, actionButton.accent.b, 0.24)
                : (actionButton.hovered
                    ? Qt.rgba(actionButton.accent.r, actionButton.accent.g, actionButton.accent.b, 0.14)
                    : (actionButton.dangerous ? root.theme.dangerWash : root.theme.surfaceRaised))
            border.width: 1
            border.color: actionButton.activeFocus || actionButton.hovered
                ? actionButton.accent
                : (actionButton.dangerous ? root.theme.danger : root.theme.panelStroke)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: root.compact ? 8 : 10

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: root.compact ? 40 : 44
            radius: 12
            color: root.theme.highlightSoft
            border.width: 1
            border.color: root.theme.lineSoft

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 10
                spacing: 8

                Rectangle {
                    Layout.preferredWidth: 8
                    Layout.preferredHeight: 8
                    radius: 4
                    color: root.host.connected ? root.theme.accent : root.theme.warn
                }

                Text {
                    text: root.host.connected ? qsTr("设备在线") : qsTr("设备离线")
                    color: root.theme.text
                    font.pixelSize: 12
                    font.weight: Font.Black
                }

                Rectangle {
                    Layout.preferredWidth: 1
                    Layout.preferredHeight: 20
                    color: root.theme.lineSoft
                }

                Text {
                    Layout.fillWidth: true
                    Layout.minimumWidth: 0
                    text: root.host.controlStatusText
                    color: root.theme.muted
                    font.pixelSize: 10
                    elide: Text.ElideRight
                }

                StatusPill {
                    Layout.preferredHeight: 26
                    theme: root.theme
                    text: root.host.detectionEnabled ? qsTr("检测已启用") : qsTr("检测已关闭")
                    fillColor: root.host.detectionEnabled ? root.theme.statusBg : root.theme.track
                    strokeColor: root.host.detectionEnabled ? root.theme.accent : root.theme.lineSoft
                    textColor: root.host.detectionEnabled ? root.theme.statusText : root.theme.faint
                    textSize: 9
                }
            }
        }

        GridLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            columns: 3
            rows: 2
            columnSpacing: root.compact ? 8 : 10
            rowSpacing: root.compact ? 8 : 10
            uniformCellWidths: true
            uniformCellHeights: true

            PremiumPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 0
                Layout.minimumHeight: 0
                theme: root.theme
                radius: 16
                topColor: root.theme.panelTop
                bottomColor: root.theme.panelBottom
                overlayColor: root.theme.glassOverlay
                borderColor: root.theme.panelStroke
                glassOpacity: 0.24

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: root.compact ? 10 : 12
                    spacing: root.compact ? 3 : 4

                    SectionTitle { title: qsTr("显示与色彩"); badge: qsTr("LCD / ISP"); accent: root.theme.accent }
                    CompactSlider {
                        title: qsTr("屏幕亮度")
                        value: root.host.screenBrightness
                        enabled: root.host.connected && root.host.controlStateReady
                        onMoved: root.host.setScreenBrightness(value)
                        onCommitted: {
                            root.host.setScreenBrightness(value)
                            root.host.commitPendingControls()
                        }
                    }
                    CompactSlider {
                        title: qsTr("画面亮度")
                        value: root.host.cameraBrightness
                        from: root.host.cameraBrightnessMin
                        to: root.host.cameraBrightnessMax
                        stepSize: root.host.cameraBrightnessStep
                        suffix: ""
                        enabled: root.host.connected && root.host.controlStateReady && root.host.cameraBrightnessSupported
                        onMoved: root.host.setCameraBrightness(value)
                        onCommitted: {
                            root.host.setCameraBrightness(value)
                            root.host.commitPendingControls()
                        }
                    }
                    CompactSlider {
                        title: qsTr("对比度")
                        value: root.host.cameraContrast
                        from: root.host.cameraContrastMin
                        to: root.host.cameraContrastMax
                        stepSize: root.host.cameraContrastStep
                        suffix: ""
                        enabled: root.host.connected && root.host.controlStateReady && root.host.cameraContrastSupported && !root.host.cameraContrastAuto
                        onMoved: root.host.setCameraContrast(value)
                        onCommitted: {
                            root.host.setCameraContrast(value)
                            root.host.commitPendingControls()
                        }
                    }
                    CompactSlider {
                        title: qsTr("饱和度")
                        value: root.host.cameraSaturation
                        from: root.host.cameraSaturationMin
                        to: root.host.cameraSaturationMax
                        stepSize: root.host.cameraSaturationStep
                        suffix: ""
                        enabled: root.host.connected && root.host.controlStateReady && root.host.cameraSaturationSupported && !root.host.cameraSaturationAuto
                        onMoved: root.host.setCameraSaturation(value)
                        onCommitted: {
                            root.host.setCameraSaturation(value)
                            root.host.commitPendingControls()
                        }
                    }
                    CompactSlider {
                        title: qsTr("色相")
                        value: root.host.cameraHue
                        from: root.host.cameraHueMin
                        to: root.host.cameraHueMax
                        stepSize: root.host.cameraHueStep
                        suffix: ""
                        enabled: root.host.connected && root.host.controlStateReady && root.host.cameraHueSupported
                        onMoved: root.host.setCameraHue(value)
                        onCommitted: {
                            root.host.setCameraHue(value)
                            root.host.commitPendingControls()
                        }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 6
                        CompactToggle {
                            text: qsTr("自动对比")
                            checked: root.host.cameraContrastAuto
                            enabled: root.host.connected && root.host.controlStateReady && root.host.cameraContrastSupported
                            onClicked: root.host.setCameraContrastAuto(!checked)
                        }
                        CompactToggle {
                            text: qsTr("自动饱和")
                            checked: root.host.cameraSaturationAuto
                            enabled: root.host.connected && root.host.controlStateReady && root.host.cameraSaturationSupported
                            onClicked: root.host.setCameraSaturationAuto(!checked)
                        }
                    }
                }
            }

            PremiumPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 0
                Layout.minimumHeight: 0
                theme: root.theme
                radius: 16
                topColor: root.theme.panelTop
                bottomColor: root.theme.panelBottom
                overlayColor: root.theme.glassOverlay
                borderColor: root.theme.panelStroke
                glassOpacity: 0.24

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: root.compact ? 10 : 12
                    spacing: root.compact ? 5 : 7

                    SectionTitle { title: qsTr("检测设置"); badge: qsTr("VISION"); accent: root.theme.accent2 }
                    CompactToggle {
                        text: qsTr("视觉检测")
                        checked: root.host.detectionEnabled
                        enabled: root.host.connected && root.host.controlStateReady
                        onClicked: root.host.setDetectionEnabled(!checked)
                    }
                    CompactToggle {
                        text: qsTr("预览叠加框")
                        checked: root.host.previewOverlayEnabled
                        enabled: root.host.connected && root.host.controlStateReady
                        onClicked: root.host.setPreviewOverlayEnabled(!checked)
                    }
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 1
                        color: root.theme.lineSoft
                    }
                    CompactSlider {
                        title: qsTr("面单置信度")
                        value: root.host.waybillThreshold
                        enabled: root.host.connected && root.host.controlStateReady
                        onMoved: root.host.setWaybillThreshold(value)
                        onCommitted: {
                            root.host.setWaybillThreshold(value)
                            root.host.commitPendingControls()
                        }
                    }
                    CompactSlider {
                        title: qsTr("Logo 置信度")
                        value: root.host.logoThreshold
                        enabled: root.host.connected && root.host.controlStateReady
                        onMoved: root.host.setLogoThreshold(value)
                        onCommitted: {
                            root.host.setLogoThreshold(value)
                            root.host.commitPendingControls()
                        }
                    }
                    Item { Layout.fillHeight: true }
                    Text {
                        Layout.fillWidth: true
                        text: qsTr("阈值越高，识别结果越严格")
                        color: root.theme.faint
                        font.pixelSize: 9
                        elide: Text.ElideRight
                    }
                }
            }

            PremiumPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 0
                Layout.minimumHeight: 0
                theme: root.theme
                radius: 16
                topColor: root.theme.panelTop
                bottomColor: root.theme.panelBottom
                overlayColor: root.theme.glassOverlay
                borderColor: root.theme.panelStroke
                glassOpacity: 0.24

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: root.compact ? 10 : 12
                    spacing: root.compact ? 7 : 9

                    SectionTitle { title: qsTr("分拣电机"); badge: qsTr("OPEN LOOP"); accent: root.theme.warn }
                    CompactToggle {
                        text: qsTr("开启电机输出")
                        checked: root.host.motorOutputEnabled
                        enabled: root.host.connected && root.host.controlStateReady
                        onClicked: root.host.setMotorOutputEnabled(!checked)
                    }
                    CompactSlider {
                        title: qsTr("A 路 / 入口")
                        value: root.host.motorASpeed
                        enabled: root.host.connected && root.host.controlStateReady
                        onMoved: root.host.setMotorASpeed(value)
                        onCommitted: {
                            root.host.setMotorASpeed(value)
                            root.host.commitPendingControls()
                        }
                    }
                    CompactSlider {
                        title: qsTr("B 路 / 中段")
                        value: root.host.motorBSpeed
                        enabled: root.host.connected && root.host.controlStateReady
                        onMoved: root.host.setMotorBSpeed(value)
                        onCommitted: {
                            root.host.setMotorBSpeed(value)
                            root.host.commitPendingControls()
                        }
                    }
                    CompactSlider {
                        title: qsTr("C 路 / 末段")
                        value: root.host.motorCSpeed
                        enabled: root.host.connected && root.host.controlStateReady
                        onMoved: root.host.setMotorCSpeed(value)
                        onCommitted: {
                            root.host.setMotorCSpeed(value)
                            root.host.commitPendingControls()
                        }
                    }
                    Item { Layout.fillHeight: true }
                }
            }

            PremiumPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 0
                Layout.minimumHeight: 0
                theme: root.theme
                radius: 16
                topColor: root.theme.panelTop
                bottomColor: root.theme.panelBottom
                overlayColor: root.theme.glassOverlay
                borderColor: root.theme.panelStroke
                glassOpacity: 0.24

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: root.compact ? 10 : 12
                    spacing: root.compact ? 6 : 8

                    SectionTitle { title: qsTr("成像与模型"); badge: qsTr("STATUS"); accent: root.theme.purple }
                    GridLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        columns: 2
                        rows: 2
                        columnSpacing: 7
                        rowSpacing: 7
                        uniformCellWidths: true
                        uniformCellHeights: true

                        InfoTile { title: qsTr("当前曝光"); value: root.host.exposureText; accent: root.theme.accent2 }
                        InfoTile { title: qsTr("当前增益"); value: root.host.gainText; accent: root.theme.warn }
                        InfoTile { title: qsTr("当前白平衡"); value: root.host.whiteBalanceText; accent: root.theme.accent }
                        InfoTile { title: qsTr("模型信息"); value: root.host.modelInfo; accent: root.theme.purple }
                    }
                }
            }

            PremiumPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 0
                Layout.minimumHeight: 0
                theme: root.theme
                radius: 16
                topColor: root.theme.panelTop
                bottomColor: root.theme.panelBottom
                overlayColor: root.theme.glassOverlay
                borderColor: root.theme.panelStroke
                glassOpacity: 0.24

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: root.compact ? 10 : 12
                    spacing: root.compact ? 6 : 8

                    SectionTitle { title: qsTr("网络与上报"); badge: qsTr("REPORT"); accent: root.theme.accent }
                    InfoTile {
                        Layout.preferredHeight: root.compact ? 54 : 62
                        Layout.fillHeight: false
                        title: qsTr("板端 IP")
                        value: root.host.localIp
                        accent: root.theme.accent2
                    }
                    InfoTile {
                        Layout.preferredHeight: root.compact ? 54 : 62
                        Layout.fillHeight: false
                        title: qsTr("上位机 IP")
                        value: root.host.hostIp
                        accent: root.theme.accent
                    }
                    Item { Layout.fillHeight: true }
                    CompactToggle {
                        text: qsTr("上报识别图像")
                        checked: root.host.reportImageEnabled
                        enabled: root.host.connected && root.host.controlStateReady
                        onClicked: root.host.setReportImageEnabled(!checked)
                    }
                    CompactToggle {
                        text: qsTr("上报性能指标")
                        checked: root.host.reportMetricsEnabled
                        enabled: root.host.connected && root.host.controlStateReady
                        onClicked: root.host.setReportMetricsEnabled(!checked)
                    }
                }
            }

            PremiumPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 0
                Layout.minimumHeight: 0
                theme: root.theme
                radius: 16
                topColor: root.theme.panelTop
                bottomColor: root.theme.panelBottom
                overlayColor: root.theme.glassOverlay
                borderColor: root.theme.panelStroke
                glassOpacity: 0.24

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: root.compact ? 10 : 12
                    spacing: root.compact ? 6 : 8

                    SectionTitle { title: qsTr("设备操作"); badge: qsTr("DEVICE"); accent: root.theme.danger }
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: root.compact ? 42 : 48
                        radius: 10
                        color: root.theme.highlightSoft
                        border.width: 1
                        border.color: root.theme.lineSoft

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10
                            spacing: 1

                            Text {
                                Layout.fillWidth: true
                                text: qsTr("控制通道")
                                color: root.theme.faint
                                font.pixelSize: 9
                            }
                            Text {
                                Layout.fillWidth: true
                                text: root.host.controlStatusText
                                color: root.theme.accent2
                                font.pixelSize: root.compact ? 10 : 11
                                font.weight: Font.Bold
                                elide: Text.ElideRight
                            }
                        }
                    }
                    Item { Layout.fillHeight: true }
                    ActionButton {
                        text: qsTr("刷新设备状态")
                        enabled: root.host.connected
                        accent: root.theme.accent
                        onClicked: root.host.requestDeviceState()
                    }
                    ActionButton {
                        text: qsTr("同步设备时间")
                        enabled: root.host.connected
                        accent: root.theme.accent2
                        onClicked: root.host.sendTimeSync()
                    }
                    ActionButton {
                        text: qsTr("重启设备")
                        enabled: root.host.connected && root.host.controlStateReady
                        accent: root.theme.danger
                        dangerous: true
                        onClicked: root.host.restartDevice()
                    }
                }
            }
        }
    }
}
