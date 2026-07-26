import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    width: 1440
    height: 900
    minimumWidth: 1180
    minimumHeight: 760
    visible: true
    title: qsTr("ESP32 板端视觉分拣上位机")
    color: theme.bg

    property int pageIndex: 0
    property bool darkMode: true
    readonly property var pageTitles: [qsTr("性能总览"), qsTr("视觉检测"), qsTr("设备控制"), qsTr("系统维护")]
    readonly property var pageDescriptions: [
        qsTr("板端负载、内存、链路与业务吞吐一屏掌握"),
        qsTr("查看实时画面、检测阶段与识别结果"),
        qsTr("集中调整电机、灯光和检测运行参数"),
        qsTr("管理服务、数据目录、日志与系统状态")
    ]
    readonly property var theme: darkMode ? darkTheme : lightTheme
    readonly property var darkTheme: ({
        bg: "#080b10", bgTop: "#101722", bgBottom: "#06080c",
        panel: "#151a22", panel2: "#202734", surfaceGlass: "#b017202a",
        surfaceRaised: "#222a36", navBg: "#10161f", navActive: "#1c2b3d",
        panelTop: "#1b222d", panelBottom: "#111720", navTop: "#141c26", navBottom: "#0d131b",
        panelStroke: "#34485f", glassOverlay: "#12ffffff", highlight: "#2cffffff",
        highlightSoft: "#12ffffff", shadowStrong: "#aa000000",
        line: "#34465a", lineSoft: "#27384a", track: "#293747", thumb: "#edf7ff",
        text: "#f3f8ff", muted: "#9eacbb", faint: "#687787", accent: "#4c9dff",
        accent2: "#79b8ff", warn: "#e7b75d", danger: "#ff657d", purple: "#9b8cff",
        softAccent: "#304c9dff", accentWash: "#2c4c9dff", accentMist: "#184c9dff",
        dangerWash: "#26ff5d77", warnWash: "#24e7b75d",
        glow: "#665aa8ff", shadow: "#88000000",
        navText: "#dbe9f4", statusBg: "#172d46", statusText: "#c9e3ff", warnText: "#ffe1a3",
        imageBg: "#090d13", onAccent: "#06111f"
    })
    readonly property var lightTheme: ({
        bg: "#d8d9dd", bgTop: "#e2e3e6", bgBottom: "#cfd1d5",
        panel: "#f1f1f3", panel2: "#e3e4e7", surfaceGlass: "#b8f7f7f8",
        surfaceRaised: "#fafafa", navBg: "#dddfe3", navActive: "#cbd9e8",
        panelTop: "#f7f7f8", panelBottom: "#e8e9ec", navTop: "#e9eaed", navBottom: "#d8dade",
        panelStroke: "#8f949e", glassOverlay: "#12ffffff", highlight: "#78ffffff",
        highlightSoft: "#29262b35", shadowStrong: "#4a121720",
        line: "#a5a9b1", lineSoft: "#c0c3c9", track: "#b7bac1", thumb: "#ffffff",
        text: "#191b20", muted: "#484b53", faint: "#60646d", accent: "#356d9b",
        accent2: "#547da6", warn: "#825313", danger: "#9d394d", purple: "#625196",
        softAccent: "#40356d9b", accentWash: "#44356d9b", accentMist: "#30356d9b",
        dangerWash: "#409d394d", warnWash: "#40825313",
        glow: "#50356d9b", shadow: "#42121720",
        navText: "#282b31", statusBg: "#c9dbea", statusText: "#1f4b70", warnText: "#503204",
        imageBg: "#101218", onAccent: "#ffffff"
    })

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: theme.bgTop }
            GradientStop { position: 0.64; color: theme.bg }
            GradientStop { position: 1.0; color: theme.bgBottom }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 14

        PremiumPanel {
            Layout.preferredWidth: 226
            Layout.fillHeight: true
            radius: 22
            theme: root.theme
            topColor: theme.navTop
            bottomColor: theme.navBottom
            overlayColor: theme.glassOverlay
            borderColor: theme.panelStroke
            glassOpacity: root.darkMode ? 0.22 : 0.18
            elevated: false

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 14
                spacing: 10

                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 62
                    spacing: 11

                    Rectangle {
                        Layout.preferredWidth: 42
                        Layout.preferredHeight: 42
                        radius: 13
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: theme.accent2 }
                            GradientStop { position: 1.0; color: theme.accent }
                        }
                        Text {
                            anchors.centerIn: parent
                            text: "E32"
                            color: theme.onAccent
                            font.pixelSize: 11
                            font.weight: Font.Black
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 1
                        Text {
                            Layout.fillWidth: true
                            text: qsTr("Vision Host")
                            color: theme.text
                            font.pixelSize: 16
                            font.weight: Font.Black
                            elide: Text.ElideRight
                        }
                        Text {
                            Layout.fillWidth: true
                            text: qsTr("ESP32 CONTROL DESK")
                            color: theme.faint
                            font.pixelSize: 9
                            font.letterSpacing: 0.8
                            elide: Text.ElideRight
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 1
                    color: theme.lineSoft
                }

                Text {
                    Layout.fillWidth: true
                    Layout.topMargin: 6
                    Layout.leftMargin: 10
                    text: qsTr("工作台")
                    color: theme.faint
                    font.pixelSize: 10
                    font.weight: Font.Bold
                    font.letterSpacing: 1.2
                }

                NavButton {
                    text: qsTr("性能总览")
                    description: qsTr("状态与遥测")
                    badgeText: "01"
                    selected: root.pageIndex === 0
                    theme: root.theme
                    onClicked: root.pageIndex = 0
                }
                NavButton {
                    text: qsTr("视觉检测")
                    description: qsTr("画面与结果")
                    badgeText: "02"
                    selected: root.pageIndex === 1
                    theme: root.theme
                    onClicked: root.pageIndex = 1
                }
                NavButton {
                    text: qsTr("设备控制")
                    description: qsTr("执行器参数")
                    badgeText: "03"
                    selected: root.pageIndex === 2
                    theme: root.theme
                    onClicked: root.pageIndex = 2
                }
                NavButton {
                    text: qsTr("系统维护")
                    description: qsTr("服务与日志")
                    badgeText: "04"
                    selected: root.pageIndex === 3
                    theme: root.theme
                    onClicked: root.pageIndex = 3
                }

                Item { Layout.fillHeight: true }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 92
                    radius: 16
                    color: theme.highlightSoft
                    border.width: 1
                    border.color: theme.lineSoft

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 13
                        spacing: 6

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 7
                            Rectangle {
                                Layout.preferredWidth: 8
                                Layout.preferredHeight: 8
                                radius: 4
                                color: hostController.displayConnected ? theme.accent : theme.warn
                            }
                            Text {
                                Layout.fillWidth: true
                                text: hostController.displayConnected ? qsTr("设备已连接") : qsTr("等待设备连接")
                                color: theme.text
                                font.pixelSize: 12
                                font.weight: Font.Bold
                                elide: Text.ElideRight
                            }
                        }

                        Text {
                            Layout.fillWidth: true
                            text: hostController.statusText
                            color: theme.muted
                            font.pixelSize: 10
                            elide: Text.ElideRight
                        }

                        Text {
                            Layout.fillWidth: true
                            text: qsTr("接收 %1").arg(hostController.bytesReceived)
                            color: theme.faint
                            font.pixelSize: 10
                            elide: Text.ElideRight
                        }
                    }
                }

                Button {
                    id: themeButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: 44
                    hoverEnabled: true
                    onClicked: root.darkMode = !root.darkMode

                    contentItem: RowLayout {
                        spacing: 8
                        Text {
                            Layout.fillWidth: true
                            Layout.leftMargin: 12
                            text: root.darkMode ? qsTr("深色模式") : qsTr("浅色模式")
                            color: theme.text
                            font.pixelSize: 12
                            font.weight: Font.Bold
                        }
                        Rectangle {
                            Layout.preferredWidth: 30
                            Layout.preferredHeight: 18
                            Layout.rightMargin: 10
                            radius: 9
                            color: root.darkMode ? theme.accentWash : theme.warnWash
                            border.width: 1
                            border.color: root.darkMode ? theme.accent : theme.warn
                            Rectangle {
                                width: 12
                                height: 12
                                radius: 6
                                anchors.verticalCenter: parent.verticalCenter
                                x: root.darkMode ? parent.width - width - 3 : 3
                                color: root.darkMode ? theme.accent : theme.warn
                                Behavior on x { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }
                            }
                        }
                    }

                    background: Rectangle {
                        radius: 14
                        color: themeButton.hovered ? theme.highlightSoft : "transparent"
                        border.width: 1
                        border.color: themeButton.hovered ? theme.line : theme.lineSoft
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 14

            HeaderBar {
                Layout.fillWidth: true
                Layout.preferredHeight: 78
                theme: root.theme
                host: hostController
                pageTitle: root.pageTitles[root.pageIndex]
                pageDescription: root.pageDescriptions[root.pageIndex]
                pageNumber: "0" + (root.pageIndex + 1)
            }

            StackLayout {
                id: pages
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: root.pageIndex

                DashboardPage { theme: root.theme; host: hostController }
                DetectionPage { theme: root.theme; host: hostController }
                ControlPage { theme: root.theme; host: hostController }
                ReservePage { theme: root.theme; host: hostController }
            }
        }
    }
}
