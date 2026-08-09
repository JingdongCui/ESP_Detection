import QtQuick
import QtQuick.Controls.Basic
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

    function categoryColor(label) {
        if (label === "极兔")
            return "#ff4d5a"
        if (label === "韵达")
            return "#ffe600"
        return "#38a3ff"
    }

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
                imageAlreadyAnnotated: imageUrl.length > 0
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
                    MetricCard { Layout.fillWidth: true; Layout.fillHeight: true; theme: root.theme; title: qsTr("最新类别"); value: host.imageCount > 0 ? host.latestCategoryLabel : qsTr("无"); note: host.imageCount > 0 ? qsTr("%1% 置信度").arg(host.latestCategoryConfidence) : qsTr("暂无图像"); accent: theme.accent }
                    PremiumPanel {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        theme: root.theme
                        radius: 14
                        topColor: theme.panelTop
                        bottomColor: theme.panelBottom
                        overlayColor: theme.glassOverlay
                        borderColor: theme.panelStroke
                        accentColor: theme.danger
                        accented: true

                        MetricStatusIndicator {
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.margins: 12
                            theme: root.theme
                            accent: theme.accent
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 16
                            spacing: 4

                            Text {
                                Layout.fillWidth: true
                                text: qsTr("置信度阈值")
                                color: theme.muted
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                            }

                            Rectangle {
                                Layout.preferredWidth: 112
                                Layout.preferredHeight: 42
                                radius: 10
                                color: theme.surfaceGlass
                                border.width: thresholdInput.activeFocus ? 2 : 1
                                border.color: thresholdInput.activeFocus ? theme.danger : theme.panelStroke

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 12
                                    anchors.rightMargin: 12
                                    spacing: 2

                                    TextInput {
                                        id: thresholdInput
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        z: 2
                                        color: theme.text
                                        font.pixelSize: 22
                                        font.weight: Font.Black
                                        horizontalAlignment: TextInput.AlignHCenter
                                        verticalAlignment: TextInput.AlignVCenter
                                        inputMethodHints: Qt.ImhDigitsOnly
                                        activeFocusOnTab: true
                                        selectByMouse: true
                                        maximumLength: 3
                                        validator: IntValidator { bottom: 0; top: 100 }

                                        function commitValue() {
                                            var next = Math.max(0, Math.min(100, parseInt(text) || 0))
                                            root.host.setDangerThreshold(next)
                                            text = next.toString()
                                        }

                                        onActiveFocusChanged: {
                                            if (activeFocus)
                                                selectAll()
                                            else
                                                commitValue()
                                        }
                                        onAccepted: {
                                            commitValue()
                                            focus = false
                                        }

                                        Binding {
                                            target: thresholdInput
                                            property: "text"
                                            value: root.host.dangerThreshold.toString()
                                            when: !thresholdInput.activeFocus
                                        }
                                    }

                                    Text {
                                        text: "%"
                                        color: theme.danger
                                        font.pixelSize: 18
                                        font.weight: Font.Black
                                    }
                                }
                            }

                            Text {
                                Layout.fillWidth: true
                                text: qsTr("低于该值时列表红色高亮")
                                color: theme.faint
                                font.pixelSize: 12
                                elide: Text.ElideRight
                            }
                        }
                    }
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
                        readonly property bool danger: Number(modelData.logoConfidence || 0) < root.host.dangerThreshold
                        width: ListView.view.width
                        height: 122
                        theme: root.theme
                        radius: 14
                        topColor: selected ? theme.navActive : (historyCard.danger ? theme.dangerWash : theme.panelTop)
                        bottomColor: theme.panelBottom
                        overlayColor: selected ? theme.accentMist : theme.glassOverlay
                        borderColor: selected ? theme.accent : (historyCard.danger ? theme.danger : theme.panelStroke)
                        accentColor: selected ? theme.accent : (historyCard.danger ? theme.danger : theme.accent2)
                        accented: selected || historyCard.danger
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
                                    color: historyCard.danger ? theme.dangerWash : theme.accentMist
                                    border.color: historyCard.danger ? theme.danger : theme.accent
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
                                    color: root.categoryColor(modelData.categoryLabel)
                                    font.pixelSize: 15
                                    font.weight: Font.Black
                                    elide: Text.ElideRight
                                    Layout.fillWidth: true
                                }
                                Text { text: modelData.model; color: theme.muted; font.pixelSize: 11; elide: Text.ElideRight; Layout.fillWidth: true }
                                Text {
                                    text: (modelData.categoryLabel || "极兔") + " Logo  " + Math.round(Number(modelData.logoConfidence || 0)) + "%"
                                    color: root.categoryColor(modelData.categoryLabel)
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
