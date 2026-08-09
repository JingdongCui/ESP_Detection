import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

Item {
    id: root
    property var theme
    property var host
    readonly property bool compactLayout: width < 1020
    readonly property bool cloudReplyVisible: StackLayout.isCurrentItem
    readonly property string cloudReply: qsTr("本次分拣任务共处理包裹 50 件，综合识别正确率为 96%，当前分拣速度达到每分钟 18 件，系统整体运行稳定。\n\n从分类结果来看，韵达包裹的识别准确率相对较低，异常主要可能与面单遮挡、包裹摆放角度及现场光照变化有关。建议适当补充复杂场景下的训练样本，并重点检查低置信度包裹图像。\n\n当前设备未发现明显运行异常。建议保持现有运行模式，并将传送带速度提高 5% 进行试运行；若异常件比例上升或识别置信度持续下降，应恢复原速度并检查摄像头与包裹位置。")
    property int displayedCharacterCount: 0
    property string cloudReplyPhase: "loading"
    opacity: StackLayout.isCurrentItem ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 220 } }

    function startCloudReply() {
        displayedCharacterCount = 0
        cloudReplyPhase = "loading"
        typingTimer.stop()
        restartTimer.stop()
        loadingTimer.restart()
    }

    function stopCloudReply() {
        loadingTimer.stop()
        typingTimer.stop()
        restartTimer.stop()
        displayedCharacterCount = 0
        cloudReplyPhase = "loading"
    }

    onCloudReplyVisibleChanged: {
        if (cloudReplyVisible)
            startCloudReply()
        else
            stopCloudReply()
    }

    Component.onCompleted: {
        if (cloudReplyVisible)
            startCloudReply()
    }

    Timer {
        id: loadingTimer
        interval: 2000
        repeat: false
        onTriggered: {
            root.cloudReplyPhase = "typing"
            typingTimer.start()
        }
    }

    Timer {
        id: typingTimer
        interval: 50
        repeat: true
        onTriggered: {
            if (root.displayedCharacterCount >= root.cloudReply.length) {
                stop()
                root.cloudReplyPhase = "complete"
                restartTimer.start()
                return
            }
            ++root.displayedCharacterCount
        }
    }

    Timer {
        id: restartTimer
        interval: 10000
        repeat: false
        onTriggered: root.startCloudReply()
    }

    FolderDialog {
        id: saveFolderDialog
        title: qsTr("选择保存目录")
        currentFolder: root.host.saveDirUrl
        onAccepted: root.host.setSaveDirectory(selectedFolder)
    }

    component PortCard: PremiumPanel {
        id: portCard
        property string title: ""
        property string value: ""
        property string note: ""
        property color accent: root.theme.accent

        Layout.fillWidth: true
        Layout.minimumWidth: 0
        Layout.preferredHeight: 82
        radius: 14
        theme: root.theme
        topColor: theme.panelTop
        bottomColor: theme.panelBottom
        overlayColor: theme.glassOverlay
        borderColor: theme.panelStroke
        accentColor: portCard.accent
        accented: true
        elevated: false
        glassOpacity: 0.22

        RowLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 12

            Rectangle {
                Layout.preferredWidth: 6
                Layout.fillHeight: true
                radius: 3
                color: portCard.accent
                opacity: 0.8
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.minimumWidth: 0
                spacing: 2

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
                    fontSizeMode: Text.Fit
                    minimumPixelSize: 14
                    elide: Text.ElideRight
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

    ScrollView {
        id: maintenanceScroll
        anchors.fill: parent
        clip: true
        contentWidth: availableWidth
        contentHeight: pageGrid.height
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        GridLayout {
            id: pageGrid
            width: maintenanceScroll.availableWidth
            height: Math.max(implicitHeight, maintenanceScroll.availableHeight)
            columns: root.compactLayout ? 1 : 2
            columnSpacing: 14
            rowSpacing: root.compactLayout ? 14 : 0

            PremiumPanel {
                Layout.row: 0
                Layout.column: 0
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 0
                Layout.minimumHeight: 696
                Layout.preferredWidth: root.compactLayout ? pageGrid.width : (pageGrid.width - pageGrid.columnSpacing) * 0.57
                radius: 18
                theme: root.theme
                topColor: theme.panelTop
                bottomColor: theme.panelBottom
                overlayColor: theme.glassOverlay
                borderColor: theme.panelStroke
                glassOpacity: 0.3

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 14

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.minimumWidth: 0
                            spacing: 3

                            Text {
                                Layout.fillWidth: true
                                text: "运行概览"
                                color: theme.text
                                font.pixelSize: 26
                                font.weight: Font.Black
                                elide: Text.ElideRight
                            }

                            Text {
                                Layout.fillWidth: true
                                text: "监听状态、数据接收与本地存储"
                                color: theme.muted
                                font.pixelSize: 12
                                elide: Text.ElideRight
                            }
                        }

                        StatusPill {
                            Layout.preferredWidth: 92
                            Layout.preferredHeight: 30
                            theme: root.theme
                            text: host.displayConnected ? "设备在线" : "待连接"
                            fillColor: host.displayConnected ? theme.statusBg : theme.warnWash
                            strokeColor: host.displayConnected ? theme.accent : theme.warn
                            textColor: host.displayConnected ? theme.statusText : theme.warnText
                            textSize: 11
                        }
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        columns: 2
                        columnSpacing: 12
                        rowSpacing: 12

                        MetricCard {
                            Layout.fillWidth: true
                            Layout.minimumWidth: 0
                            theme: root.theme
                            title: "监听状态"
                            value: host.listening ? "已监听" : "未监听"
                            note: host.statusText
                            accent: host.listening ? theme.accent : theme.warn
                        }
                        MetricCard {
                            Layout.fillWidth: true
                            Layout.minimumWidth: 0
                            theme: root.theme
                            title: "最后遥测"
                            value: host.lastTelemetryTime
                            note: "最近一次状态更新时间"
                            accent: theme.accent2
                        }
                        MetricCard {
                            Layout.fillWidth: true
                            Layout.minimumWidth: 0
                            theme: root.theme
                            title: "累计接收"
                            value: (host.bytesReceived / 1048576).toFixed(2) + " MB"
                            note: "控制与图像链路流量"
                            accent: theme.purple
                        }
                        MetricCard {
                            Layout.fillWidth: true
                            Layout.minimumWidth: 0
                            theme: root.theme
                            title: "保存目录"
                            value: "ESP32Host"
                            note: host.saveDir
                            accent: theme.warn
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        PillButton {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 44
                            theme: root.theme
                            text: "重新监听"
                            accent: theme.accent
                            onClicked: host.startServer()
                        }

                        PillButton {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 44
                            theme: root.theme
                            text: qsTr("选择保存目录")
                            accent: theme.warn
                            onClicked: saveFolderDialog.open()
                        }

                        PillButton {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 44
                            theme: root.theme
                            text: "同步时间"
                            accent: theme.accent2
                            onClicked: host.sendTimeSync()
                        }
                    }

                    PremiumPanel {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumHeight: 196
                        radius: 14
                        theme: root.theme
                        topColor: theme.panelTop
                        bottomColor: theme.panelBottom
                        overlayColor: theme.glassOverlay
                        borderColor: theme.panelStroke
                        elevated: false
                        glassOpacity: 0.18

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 14
                            spacing: 10

                            RowLayout {
                                Layout.fillWidth: true

                                Text {
                                    Layout.fillWidth: true
                                    text: "运行日志"
                                    color: theme.text
                                    font.pixelSize: 17
                                    font.weight: Font.Black
                                }

                                Text {
                                    text: host.logLines.length + " 条"
                                    color: theme.faint
                                    font.pixelSize: 11
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 1
                                color: theme.lineSoft
                                opacity: 0.7
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Layout.minimumHeight: 136

                                ListView {
                                    anchors.fill: parent
                                    clip: true
                                    spacing: 6
                                    model: host.logLines
                                    boundsBehavior: Flickable.StopAtBounds
                                    ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                                    delegate: Text {
                                        required property int index
                                        required property string modelData
                                        width: ListView.view.width
                                        height: 22
                                        text: modelData
                                        color: index === 0 ? theme.accent : theme.muted
                                        font.pixelSize: 12
                                        elide: Text.ElideRight
                                    }
                                }

                                Text {
                                    anchors.centerIn: parent
                                    visible: host.logLines.length === 0
                                    text: "暂无运行日志"
                                    color: theme.faint
                                    font.pixelSize: 12
                                }
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.row: root.compactLayout ? 1 : 0
                Layout.column: root.compactLayout ? 0 : 1
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 0
                Layout.minimumHeight: 696
                Layout.preferredWidth: root.compactLayout ? pageGrid.width : (pageGrid.width - pageGrid.columnSpacing) * 0.43
                spacing: 14

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 338
                    Layout.minimumHeight: 338
                    radius: 18
                    theme: root.theme
                    topColor: theme.panelTop
                    bottomColor: theme.panelBottom
                    overlayColor: theme.glassOverlay
                    borderColor: theme.panelStroke
                    accentColor: theme.purple
                    accented: true
                    glassOpacity: 0.3

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 18
                        spacing: 10

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 10

                            Item {
                                Layout.preferredWidth: 28
                                Layout.preferredHeight: 28

                                Rectangle {
                                    width: 22
                                    height: 22
                                    radius: 11
                                    anchors.centerIn: parent
                                    color: "transparent"
                                    border.width: 3
                                    border.color: theme.purple
                                    opacity: root.cloudReplyPhase === "loading" ? 1.0 : 0.46
                                }

                                Rectangle {
                                    width: 7
                                    height: 7
                                    radius: 4
                                    x: (parent.width - width) / 2
                                    y: 1
                                    color: theme.accent2
                                }

                                RotationAnimator on rotation {
                                    running: root.cloudReplyVisible && root.cloudReplyPhase === "loading"
                                    from: 0
                                    to: 360
                                    duration: 850
                                    loops: Animation.Infinite
                                }
                            }

                            ColumnLayout {
                                Layout.fillWidth: true
                                Layout.minimumWidth: 0
                                spacing: 1

                                Text {
                                    Layout.fillWidth: true
                                    text: qsTr("云端大模型回复")
                                    color: theme.text
                                    font.pixelSize: 21
                                    font.weight: Font.Black
                                    elide: Text.ElideRight
                                }

                            }

                            StatusPill {
                                theme: root.theme
                                text: root.cloudReplyPhase === "loading" ? qsTr("思考中")
                                      : root.cloudReplyPhase === "typing" ? qsTr("生成中") : qsTr("回复完成")
                                fillColor: root.cloudReplyPhase === "complete" ? theme.statusBg : theme.accentWash
                                strokeColor: root.cloudReplyPhase === "complete" ? theme.accent : theme.purple
                                textColor: root.cloudReplyPhase === "complete" ? theme.statusText : theme.text
                                textSize: 10
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 1
                            color: theme.lineSoft
                        }

                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            contentWidth: availableWidth
                            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                            ScrollBar.vertical.policy: ScrollBar.AsNeeded

                            Text {
                                width: parent.width
                                text: root.cloudReply.slice(0, root.displayedCharacterCount)
                                color: theme.text
                                font.pixelSize: 13
                                lineHeight: 1.45
                                wrapMode: Text.Wrap
                            }
                        }
                    }
                }

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.minimumHeight: 268
                    Layout.preferredHeight: 268
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
                        spacing: 12

                        RowLayout {
                            Layout.fillWidth: true

                            ColumnLayout {
                                Layout.fillWidth: true
                                Layout.minimumWidth: 0
                                spacing: 2

                                Text {
                                    Layout.fillWidth: true
                                    text: "链路端口"
                                    color: theme.text
                                    font.pixelSize: 21
                                    font.weight: Font.Black
                                    elide: Text.ElideRight
                                }

                                Text {
                                    Layout.fillWidth: true
                                    text: "固定网络配置与服务入口"
                                    color: theme.muted
                                    font.pixelSize: 11
                                    elide: Text.ElideRight
                                }
                            }

                            StatusPill {
                                theme: root.theme
                                text: host.listening ? "监听中" : "未监听"
                                fillColor: host.listening ? theme.statusBg : theme.warnWash
                                strokeColor: host.listening ? theme.accent : theme.warn
                                textColor: host.listening ? theme.statusText : theme.warnText
                                textSize: 10
                            }
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            columns: 2
                            columnSpacing: 10
                            rowSpacing: 10

                            PortCard { title: "控制链路"; value: "5000"; note: "遥测 / 控制"; accent: theme.accent }
                            PortCard { title: "图像链路"; value: "5001"; note: "JPEG 预览"; accent: theme.accent2 }
                            PortCard { title: "板端地址"; value: "192.168.10.2"; note: "ESP32 Ethernet"; accent: theme.warn }
                            PortCard { title: "上位机地址"; value: "192.168.10.1"; note: "PC Ethernet"; accent: theme.purple }
                        }
                    }
                }

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 414
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
                        spacing: 12

                        RowLayout {
                            Layout.fillWidth: true

                            ColumnLayout {
                                Layout.fillWidth: true
                                Layout.minimumWidth: 0
                                spacing: 2

                                Text {
                                    Layout.fillWidth: true
                                    text: "图片链路健康"
                                    color: theme.text
                                    font.pixelSize: 21
                                    font.weight: Font.Black
                                    elide: Text.ElideRight
                                }

                                Text {
                                    Layout.fillWidth: true
                                    text: "编码、发送与队列状态"
                                    color: theme.muted
                                    font.pixelSize: 11
                                    elide: Text.ElideRight
                                }
                            }

                            StatusPill {
                                theme: root.theme
                                text: host.displayConnected ? "链路正常" : "等待数据"
                                fillColor: host.displayConnected ? theme.statusBg : theme.warnWash
                                strokeColor: host.displayConnected ? theme.accent : theme.warn
                                textColor: host.displayConnected ? theme.statusText : theme.warnText
                                textSize: 10
                            }
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            columns: 2
                            columnSpacing: 10
                            rowSpacing: 10

                            Repeater {
                                model: host.imageHealthCards

                                MetricCard {
                                    required property int index
                                    required property var modelData
                                    Layout.columnSpan: index === 0 ? 2 : 1
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    Layout.minimumWidth: 0
                                    Layout.minimumHeight: 92
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
    }
}
