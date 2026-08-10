import QtQuick
import QtQuick.Layouts

Item {
    id: root
    property var theme
    property var host
    property var selectedFrame: ({})
    readonly property string selectedImageUrl: selectedFrame && selectedFrame.imageUrl ? selectedFrame.imageUrl : ""
    readonly property var selectedDetections: selectedFrame && selectedFrame.detections ? selectedFrame.detections : []
    readonly property string selectedFrameInfo: selectedFrame && selectedFrame.resolution
        ? "回放帧  " + selectedFrame.resolution + "  #" + selectedFrame.seq
        : ""
    readonly property int selectedTargetCount: selectedFrame && selectedFrame.count ? selectedFrame.count : 0
    opacity: StackLayout.isCurrentItem ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 220 } }

    RowLayout {
        anchors.fill: parent
        spacing: 14

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 14

            DetectionStage {
                Layout.fillWidth: true
                Layout.fillHeight: true
                theme: root.theme
                host: root.host
                imageUrl: root.selectedImageUrl
                detections: root.selectedDetections
                frameInfo: root.selectedFrameInfo
                targetCount: root.selectedTargetCount
            }

            PremiumPanel {
                Layout.fillWidth: true
                Layout.preferredHeight: 136
                radius: 16
                theme: root.theme
                topColor: theme.panelTop
                bottomColor: theme.panelBottom
                overlayColor: theme.glassOverlay
                borderColor: theme.panelStroke
                glassOpacity: 0.22
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 14
                    spacing: 14
                    MetricCard { Layout.fillWidth: true; Layout.fillHeight: true; theme: root.theme; title: "累计检测"; value: host.detectionCount; note: "快递标识目标"; accent: theme.accent }
                    MetricCard { Layout.fillWidth: true; Layout.fillHeight: true; theme: root.theme; title: "最新帧"; value: host.latestFrameInfo; note: "图像/检测同步"; accent: theme.accent2 }
                    MetricCard { Layout.fillWidth: true; Layout.fillHeight: true; theme: root.theme; title: "危险阈值"; value: host.dangerThreshold + "%"; note: "低置信度重点突出"; accent: theme.danger }
                }
            }
        }

        PremiumPanel {
            Layout.preferredWidth: 360
            Layout.fillHeight: true
            radius: 18
            theme: root.theme
            topColor: theme.panelTop
            bottomColor: theme.panelBottom
            overlayColor: theme.glassOverlay
            borderColor: theme.panelStroke
            glassOpacity: 0.28

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 12
                Text {
                    text: "检测历史"
                    color: theme.text
                    font.pixelSize: 20
                    font.weight: Font.Black
                }
                Text {
                    text: "缩略记录按时间倒序排列，低置信度自动标红"
                    color: theme.muted
                    font.pixelSize: 12
                    Layout.fillWidth: true
                    wrapMode: Text.Wrap
                }
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    spacing: 10
                    model: host.frameHistory
                    delegate: PremiumPanel {
                        id: historyCard
                        property bool selected: root.selectedFrame && root.selectedFrame.seq === modelData.seq && root.selectedFrame.time === modelData.time
                        width: ListView.view.width
                        height: 122
                        theme: root.theme
                        radius: 14
                        topColor: selected ? theme.navActive : (modelData.danger ? theme.dangerWash : theme.panelTop)
                        bottomColor: theme.panelBottom
                        overlayColor: selected ? theme.accentMist : theme.glassOverlay
                        borderColor: selected ? theme.accent : (modelData.danger ? theme.danger : theme.panelStroke)
                        accentColor: selected ? theme.accent : (modelData.danger ? theme.danger : theme.accent2)
                        accented: selected || modelData.danger
                        hoverActive: cardMouse.containsMouse
                        opacity: 0
                        NumberAnimation on opacity { to: 1; duration: 260; easing.type: Easing.OutCubic }

                        MouseArea {
                            id: cardMouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: root.selectedFrame = modelData
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 10
                            Rectangle {
                                Layout.preferredWidth: 96
                                Layout.fillHeight: true
                                radius: 10
                                border.color: theme.panelStroke
                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: theme.imageBg }
                                    GradientStop { position: 1.0; color: theme.bgBottom }
                                }
                                Rectangle {
                                    anchors.centerIn: parent
                                    width: 48
                                    height: 34
                                    radius: 8
                                    color: modelData.danger ? theme.dangerWash : theme.accentMist
                                    border.color: modelData.danger ? theme.danger : theme.accent
                                    opacity: 0.9
                                }
                                Text {
                                    anchors.centerIn: parent
                                    text: "#" + modelData.seq
                                    color: theme.muted
                                    font.pixelSize: 16
                                    font.weight: Font.Black
                                }
                            }
                            ColumnLayout {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                spacing: 4
                                Text {
                                    text: modelData.title
                                    color: theme.text
                                    font.pixelSize: 15
                                    font.weight: Font.Black
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }
                                Text { text: modelData.model; color: theme.muted; font.pixelSize: 11; elide: Text.ElideRight; Layout.fillWidth: true }
                                Text { text: modelData.resolution + "  " + modelData.inferenceMs + " ms"; color: theme.faint; font.pixelSize: 11 }
                                Text {
                                    text: "目标 " + modelData.count + " 个  最高置信度 " + Math.round(Number(modelData.confidence) * 100) + "%"
                                    color: modelData.danger ? theme.danger : theme.accent
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                }
                                Text { text: modelData.time; color: theme.faint; font.pixelSize: 11 }
                            }
                        }
                    }
                }
            }
        }
    }
}
