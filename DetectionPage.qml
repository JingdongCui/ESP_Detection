import QtQuick
import QtQuick.Layouts

Item {
    id: root
    property var theme
    property var host
    property var selectedFrame: ({})
    readonly property bool hasSelectedFrame: !!(selectedFrame && selectedFrame.imageUrl)
    readonly property string selectedImageUrl: hasSelectedFrame ? selectedFrame.imageUrl : host.latestImageUrl
    readonly property var selectedDetections: hasSelectedFrame && selectedFrame.detections ? selectedFrame.detections : host.currentDetections
    readonly property string selectedCategoryLabel: hasSelectedFrame && selectedFrame.categoryLabel ? selectedFrame.categoryLabel : host.latestCategoryLabel
    readonly property int selectedCategoryConfidence: hasSelectedFrame && selectedFrame.categoryConfidence !== undefined ? selectedFrame.categoryConfidence : host.latestCategoryConfidence
    readonly property string selectedFrameInfo: hasSelectedFrame
        ? (selectedFrame.packageLabel || selectedFrame.title) + "  " + root.selectedCategoryLabel
        : host.latestFrameInfo
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
                detections: host.detectionEnabled ? root.selectedDetections : []
                frameInfo: root.selectedFrameInfo
                categoryLabel: root.selectedCategoryLabel
                categoryConfidence: root.selectedCategoryConfidence
                overlayEnabled: host.previewOverlayEnabled
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
                    MetricCard { Layout.fillWidth: true; Layout.fillHeight: true; theme: root.theme; title: "总包裹数"; value: host.imageCount; note: "已接收包裹"; accent: theme.accent }
                    MetricCard { Layout.fillWidth: true; Layout.fillHeight: true; theme: root.theme; title: qsTr("最新类别"); value: host.imageCount > 0 ? host.latestCategoryLabel : qsTr("无"); note: host.imageCount > 0 ? qsTr("%1% 置信度").arg(host.latestCategoryConfidence) : qsTr("暂无图像"); accent: theme.accent2 }
                    MetricCard { Layout.fillWidth: true; Layout.fillHeight: true; theme: root.theme; title: "置信度阈值"; value: host.dangerThreshold + "%"; note: "低置信度重点突出"; accent: theme.danger }
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
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Text {
                        text: "包裹图像记录"
                        color: theme.text
                        font.pixelSize: 20
                        font.weight: Font.Black
                    }
                    Item { Layout.fillWidth: true }
                    PillButton {
                        theme: root.theme
                        text: "清空"
                        accent: theme.danger
                        enabled: host.frameHistory.length > 0
                        opacity: enabled ? 1.0 : 0.4
                        onClicked: {
                            host.clearFrameHistory()
                            root.selectedFrame = ({})
                        }
                    }
                }
                Text {
                    text: "按接收时间倒序排列，点击可查看历史图像"
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
                                clip: true
                                border.color: theme.panelStroke
                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: theme.imageBg }
                                    GradientStop { position: 1.0; color: theme.bgBottom }
                                }
                                Image {
                                    anchors.fill: parent
                                    source: modelData.imageUrl || ""
                                    fillMode: Image.PreserveAspectCrop
                                    asynchronous: true
                                    cache: false
                                    sourceSize.width: 192
                                    sourceSize.height: 132
                                    visible: status === Image.Ready
                                }
                                Rectangle {
                                    anchors.centerIn: parent
                                    width: 48
                                    height: 34
                                    radius: 8
                                    color: modelData.danger ? theme.dangerWash : theme.accentMist
                                    border.color: modelData.danger ? theme.danger : theme.accent
                                    opacity: modelData.imageUrl ? 0.42 : 0.9
                                }
                                Text {
                                    anchors.centerIn: parent
                                    text: modelData.packageLabel || ("#" + modelData.seq)
                                    color: modelData.imageUrl ? theme.text : theme.muted
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
                                Text {
                                    text: (modelData.categoryLabel || "极兔") + "  " + Math.round(Number(modelData.categoryConfidence || 0)) + "%"
                                    color: theme.accent
                                    font.pixelSize: 12
                                    font.weight: Font.Black
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }
                                Text { text: modelData.processMs + " ms 处理"; color: theme.faint; font.pixelSize: 11 }
                                Text {
                                    text: "接收时间 " + modelData.time
                                    color: theme.faint
                                    font.pixelSize: 12
                                    font.weight: Font.DemiBold
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
