import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    property var theme
    property var host
    opacity: StackLayout.isCurrentItem ? 1 : 0
    Behavior on opacity { NumberAnimation { duration: 220 } }

    ColumnLayout {
        anchors.fill: parent
        spacing: 14

        PremiumPanel {
            Layout.fillWidth: true
            Layout.preferredHeight: 178
            radius: 18
            theme: root.theme
            topColor: theme.panelTop
            bottomColor: theme.panelBottom
            overlayColor: theme.glassOverlay
            borderColor: theme.panelStroke
            glassOpacity: 0.24

            RowLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 14

                MetricCard {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    theme: root.theme
                    title: "推理状态"
                    value: host.inferenceBusy ? "运行中" : "待机"
                    note: host.inferenceStatus
                    accent: host.inferenceBusy ? theme.warn : theme.accent
                }
                MetricCard {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    theme: root.theme
                    title: "模型目录"
                    value: "YOLO26 s/m"
                    note: host.modelDir
                    accent: theme.accent2
                }
                MetricCard {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    theme: root.theme
                    title: "数据集"
                    value: "jt / zt / yd"
                    note: host.datasetDir
                    accent: theme.purple
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 14

            PremiumPanel {
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 18
                theme: root.theme
                topColor: theme.panelTop
                bottomColor: theme.panelBottom
                overlayColor: theme.glassOverlay
                borderColor: theme.panelStroke
                glassOpacity: 0.22

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 18
                    spacing: 12

                    Text {
                        text: "推理服务"
                        color: theme.text
                        font.pixelSize: 21
                        font.weight: Font.Black
                    }

                    TextField {
                        id: serviceUrl
                        Layout.fillWidth: true
                        text: host.inferenceServiceUrl
                        color: theme.text
                        selectByMouse: true
                        placeholderText: "http://127.0.0.1:8765"
                        onEditingFinished: host.inferenceServiceUrl = text
                        background: Rectangle {
                            radius: 10
                            color: theme.surfaceGlass
                            border.color: theme.panelStroke
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        Button {
                            Layout.preferredWidth: 150
                            Layout.preferredHeight: 44
                            text: "检查服务"
                            onClicked: host.pingInferenceService()
                        }
                        CheckBox {
                            id: inferenceToggle
                            checked: host.inferenceEnabled
                            text: "启用实时推理"
                            onToggled: host.inferenceEnabled = checked
                            contentItem: Text {
                                text: inferenceToggle.text
                                color: theme.text
                                leftPadding: inferenceToggle.indicator.width + inferenceToggle.spacing
                                verticalAlignment: Text.AlignVCenter
                                font.pixelSize: 13
                                font.weight: Font.DemiBold
                            }
                        }
                    }

                    Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: theme.lineSoft }

                    Text {
                        text: "启动命令"
                        color: theme.text
                        font.pixelSize: 17
                        font.weight: Font.Black
                    }
                    Text {
                        Layout.fillWidth: true
                        text: "cd /home/kazeform/2026upper && source .venv/bin/activate && python ml/logo_inference_service.py --model models/yolo26s.pt"
                        color: theme.muted
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: 13
                    }

                    Text {
                        text: "训练命令"
                        color: theme.text
                        font.pixelSize: 17
                        font.weight: Font.Black
                    }
                    Text {
                        Layout.fillWidth: true
                        text: "python ml/train_logo_yolo.py --model models/yolo26s.pt --data datasets/logo_yolo/data.yaml --imgsz 1024 --epochs 120"
                        color: theme.muted
                        wrapMode: Text.WrapAnywhere
                        font.pixelSize: 13
                    }

                    Item { Layout.fillHeight: true }
                }
            }

            PremiumPanel {
                Layout.preferredWidth: 430
                Layout.fillHeight: true
                radius: 18
                theme: root.theme
                topColor: theme.panelTop
                bottomColor: theme.panelBottom
                overlayColor: theme.glassOverlay
                borderColor: theme.panelStroke
                glassOpacity: 0.22

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 18
                    spacing: 12

                    Text {
                        text: "标注规则"
                        color: theme.text
                        font.pixelSize: 21
                        font.weight: Font.Black
                    }

                    Repeater {
                        model: [
                            "只框快递品牌 Logo 或品牌字样，框要贴紧。",
                            "不要框整个面单、条码、二维码、包裹或手。",
                            "同一张图有多个同品牌标识时全部框出。",
                            "目录 000/006 标为 jt，001/002/003 标为 zt，004/005 标为 yd。",
                            "导出 YOLO 后先抽查 20 张标签，再开始训练。"
                        ]
                        delegate: Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: Math.max(44, ruleText.implicitHeight + 18)
                            radius: 12
                            color: theme.surfaceGlass
                            border.color: theme.panelStroke
                            Text {
                                id: ruleText
                                anchors.fill: parent
                                anchors.margins: 11
                                text: modelData
                                color: theme.text
                                wrapMode: Text.Wrap
                                font.pixelSize: 13
                                font.weight: Font.DemiBold
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }
        }
    }
}
