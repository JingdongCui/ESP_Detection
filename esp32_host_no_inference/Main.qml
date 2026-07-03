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
    title: qsTr("ESP32 快递 Logo 检测上位机")
    color: theme.bg

    property int pageIndex: 0
    property bool darkMode: true
    readonly property var theme: darkMode ? darkTheme : lightTheme
    readonly property var darkTheme: ({
        bg: "#07090d", bgTop: "#111825", bgBottom: "#05070b",
        panel: "#151a22", panel2: "#202734", surfaceGlass: "#b017202a",
        surfaceRaised: "#242c3a", navBg: "#111720", navActive: "#263548",
        panelTop: "#222b39", panelBottom: "#111720", navTop: "#17212d", navBottom: "#0d131c",
        panelStroke: "#3a4656", glassOverlay: "#18ffffff", highlight: "#32ffffff",
        highlightSoft: "#16ffffff", shadowStrong: "#aa000000",
        line: "#334050", lineSoft: "#26313e", track: "#293240", thumb: "#edf7ff",
        text: "#f3f8ff", muted: "#a1afbf", faint: "#6f7d8c", accent: "#20e0c2",
        accent2: "#62b9ff", warn: "#e7b75d", danger: "#ff5d77", purple: "#9b8cff",
        softAccent: "#2dffe044", accentWash: "#2420e0c2", accentMist: "#1020e0c2",
        dangerWash: "#26ff5d77", warnWash: "#24e7b75d",
        glow: "#6620e0c2", shadow: "#88000000",
        navText: "#dbe9f4", statusBg: "#14392f", statusText: "#b8ffea", warnText: "#ffe1a3",
        imageBg: "#090d13", onAccent: "#04110f"
    })
    readonly property var lightTheme: ({
        bg: "#f8f3ec", bgTop: "#fffaf3", bgBottom: "#efe7dc",
        panel: "#fffdf8", panel2: "#f3ece1", surfaceGlass: "#ccfffaf1",
        surfaceRaised: "#ffffff", navBg: "#f7efe5", navActive: "#fff5e7",
        panelTop: "#ffffff", panelBottom: "#f3ece1", navTop: "#fffaf3", navBottom: "#efe7dc",
        panelStroke: "#e6d7c5", glassOverlay: "#66ffffff", highlight: "#b8ffffff",
        highlightSoft: "#70ffffff", shadowStrong: "#260f0903",
        line: "#e4d7c7", lineSoft: "#eee4d8", track: "#e5d8c8", thumb: "#fffdf8",
        text: "#211d1b", muted: "#746b62", faint: "#a09386", accent: "#b9874d",
        accent2: "#6e9278", warn: "#c79539", danger: "#d85568", purple: "#8a76c8",
        softAccent: "#d9b06b44", accentWash: "#24b9874d", accentMist: "#12b9874d",
        dangerWash: "#24d85568", warnWash: "#28c79539",
        glow: "#55d7a85d", shadow: "#240f0903",
        navText: "#3c352f", statusBg: "#f6ead2", statusText: "#594018", warnText: "#5f4107",
        imageBg: "#17130f", onAccent: "#1b1208"
    })

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: theme.bgTop }
            GradientStop { position: 0.58; color: theme.bg }
            GradientStop { position: 1.0; color: theme.bgBottom }
        }
        Behavior on color { ColorAnimation { duration: 260 } }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 18
        spacing: 16

        PremiumPanel {
            Layout.preferredWidth: 164
            Layout.fillHeight: true
            radius: 26
            theme: root.theme
            topColor: theme.navTop
            bottomColor: theme.navBottom
            overlayColor: theme.glassOverlay
            borderColor: theme.panelStroke
            glassOpacity: root.darkMode ? 0.28 : 0.5

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 14

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 76
                    radius: 21
                    border.color: theme.panelStroke
                    border.width: 1
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: theme.panelTop }
                        GradientStop { position: 1.0; color: theme.panelBottom }
                    }
                    Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.leftMargin: 18
                        anchors.rightMargin: 18
                        height: 1
                        color: theme.highlight
                        opacity: 0.7
                    }
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 10
                        Rectangle {
                            Layout.preferredWidth: 38
                            Layout.preferredHeight: 38
                            radius: 14
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: theme.accent2 }
                                GradientStop { position: 1.0; color: theme.accent }
                            }
                            Rectangle {
                                anchors.centerIn: parent
                                width: parent.width - 10
                                height: width
                                radius: 10
                                color: theme.highlightSoft
                            }
                            Text {
                                anchors.centerIn: parent
                                text: "E"
                                color: theme.onAccent
                                font.pixelSize: 18
                                font.weight: Font.Black
                            }
                        }
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 1
                            Text {
                                text: "ESP32"
                                color: theme.text
                                font.pixelSize: 16
                                font.weight: Font.Black
                            }
                            Text {
                                text: "Logo Vision"
                                color: theme.muted
                                font.pixelSize: 10
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                        }
                    }
                }

                NavButton { text: "性能总览"; shortText: "性能"; selected: root.pageIndex === 0; theme: root.theme; onClicked: root.pageIndex = 0 }
                NavButton { text: "视觉检测"; shortText: "检测"; selected: root.pageIndex === 1; theme: root.theme; onClicked: root.pageIndex = 1 }
                NavButton { text: "模型工作台"; shortText: "模型"; selected: root.pageIndex === 2; theme: root.theme; onClicked: root.pageIndex = 2 }
                NavButton { text: "设备控制"; shortText: "控制"; selected: root.pageIndex === 3; theme: root.theme; onClicked: root.pageIndex = 3 }
                NavButton { text: "扩展预留"; shortText: "保留"; selected: root.pageIndex === 4; theme: root.theme; onClicked: root.pageIndex = 4 }

                Item { Layout.fillHeight: true }

                Button {
                    id: themeButton
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    hoverEnabled: true
                    onClicked: root.darkMode = !root.darkMode
                    contentItem: Text {
                        text: root.darkMode ? "暗色科技" : "浅色雅致"
                        color: theme.text
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: root.darkMode ? 28 : 0
                        rightPadding: root.darkMode ? 0 : 28
                        font.pixelSize: 12
                        font.weight: Font.Black
                    }
                    background: Rectangle {
                        radius: 19
                        border.color: theme.panelStroke
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: theme.panelTop }
                            GradientStop { position: 1.0; color: theme.panelBottom }
                        }
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 1
                            radius: parent.radius - 1
                            color: theme.accentMist
                            opacity: themeButton.hovered ? 1 : 0.45
                            Behavior on opacity { NumberAnimation { duration: 160 } }
                        }
                        Rectangle {
                            width: 28
                            height: 28
                            radius: 14
                            x: root.darkMode ? parent.width - width - 10 : 10
                            anchors.verticalCenter: parent.verticalCenter
                            color: root.darkMode ? theme.accent2 : theme.warn
                            border.color: theme.highlight
                            Behavior on x { NumberAnimation { duration: 220; easing.type: Easing.OutCubic } }
                        }
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
                Layout.preferredHeight: 82
                theme: root.theme
                host: hostController
            }

            StackLayout {
                id: pages
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: root.pageIndex

                DashboardPage { theme: root.theme; host: hostController }
                DetectionPage { theme: root.theme; host: hostController }
                ModelWorkspacePage { theme: root.theme; host: hostController }
                ControlPage { theme: root.theme; host: hostController }
                ReservePage { theme: root.theme; host: hostController }
            }
        }
    }
}
