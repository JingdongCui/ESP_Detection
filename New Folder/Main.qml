import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 1440
    height: 900
    minimumWidth: 1180
    minimumHeight: 760
    visible: true
    title: qsTr("ESP32-P4 ???????")
    color: theme.bg

    property int pageIndex: 0
    property bool darkMode: true
    readonly property var theme: appTheme

    AppTheme {
        id: appTheme
        darkMode: root.darkMode
    }

    AmbientBackground {
        anchors.fill: parent
        theme: root.theme
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 18
        spacing: 16

        PremiumPanel {
            Layout.preferredWidth: 188
            Layout.fillHeight: true
            radius: 28
            theme: root.theme
            topColor: theme.navTop
            bottomColor: theme.navBottom
            overlayColor: theme.glassOverlay
            borderColor: theme.panelStroke
            glassOpacity: root.darkMode ? 0.34 : 0.54

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 12

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 82
                    radius: 22
                    border.width: 1
                    border.color: theme.panelStroke
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
                        opacity: 0.75
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 11

                        Rectangle {
                            Layout.preferredWidth: 42
                            Layout.preferredHeight: 42
                            radius: 15
                            border.color: theme.highlight
                            border.width: 1
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: theme.accent }
                                GradientStop { position: 0.52; color: theme.accent2 }
                                GradientStop { position: 1.0; color: theme.purple }
                            }
                            Text {
                                anchors.centerIn: parent
                                text: "P4"
                                color: theme.onAccent
                                font.pixelSize: 14
                                font.weight: Font.Black
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 1
                            Text {
                                Layout.fillWidth: true
                                text: "ESP SORT"
                                color: theme.text
                                font.pixelSize: 15
                                font.weight: Font.Black
                                elide: Text.ElideRight
                            }
                            Text {
                                Layout.fillWidth: true
                                text: "VISION CONTROL"
                                color: theme.muted
                                font.pixelSize: 9
                                font.letterSpacing: 0.6
                                elide: Text.ElideRight
                            }
                        }
                    }
                }

                Text {
                    Layout.leftMargin: 10
                    text: "???"
                    color: theme.faint
                    font.pixelSize: 10
                    font.weight: Font.DemiBold
                    font.letterSpacing: 1.2
                }

                NavButton { text: "????"; shortText: "SYSTEM OVERVIEW"; glyph: "?"; selected: root.pageIndex === 0; theme: root.theme; onClicked: root.pageIndex = 0 }
                NavButton { text: "????"; shortText: "VISION EVENTS"; glyph: "?"; selected: root.pageIndex === 1; theme: root.theme; onClicked: root.pageIndex = 1 }
                NavButton { text: "????"; shortText: "DEVICE CONTROL"; glyph: "?"; selected: root.pageIndex === 2; theme: root.theme; onClicked: root.pageIndex = 2 }
                NavButton { text: "????"; shortText: "MAINTENANCE"; glyph: "?"; selected: root.pageIndex === 3; theme: root.theme; onClicked: root.pageIndex = 3 }

                Item { Layout.fillHeight: true }

                PremiumPanel {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 92
                    theme: root.theme
                    radius: 20
                    elevated: false
                    accented: true
                    accentColor: theme.accent2
                    glassOpacity: 0.24

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8

                        RowLayout {
                            Layout.fillWidth: true
                            Text {
                                Layout.fillWidth: true
                                text: "????"
                                color: theme.text
                                font.pixelSize: 12
                                font.weight: Font.Black
                            }
                            Text {
                                text: root.darkMode ? "??" : "??"
                                color: theme.accent
                                font.pixelSize: 11
                                font.weight: Font.DemiBold
                            }
                        }

                        Button {
                            id: themeButton
                            Layout.fillWidth: true
                            Layout.preferredHeight: 34
                            hoverEnabled: true
                            padding: 0
                            onClicked: root.darkMode = !root.darkMode

                            contentItem: Text {
                                text: root.darkMode ? "?????" : "?????"
                                color: theme.text
                                font.pixelSize: 11
                                font.weight: Font.DemiBold
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            background: Rectangle {
                                radius: 17
                                color: theme.accentMist
                                border.color: themeButton.hovered ? theme.accent : theme.panelStroke
                                Rectangle {
                                    width: 24
                                    height: 24
                                    radius: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    x: root.darkMode ? parent.width - width - 5 : 5
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: root.darkMode ? theme.accent2 : theme.warn }
                                        GradientStop { position: 1.0; color: root.darkMode ? theme.purple : theme.accent }
                                    }
                                    Behavior on x { NumberAnimation { duration: 220; easing.type: Easing.OutCubic } }
                                }
                                Behavior on border.color { ColorAnimation { duration: 160 } }
                            }
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
                Layout.preferredHeight: 86
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
                ControlPage { theme: root.theme; host: hostController }
                ReservePage { theme: root.theme; host: hostController }
            }
        }
    }
}
