import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

PremiumPanel {
    id: root
    property string title: ""
    property string note: ""
    property int value: 0
    property int from: 0
    property int to: 100
    signal moved(int value)
    radius: 18
    topColor: theme.panelTop
    bottomColor: theme.panelBottom
    overlayColor: theme.accentMist
    borderColor: theme.panelStroke
    accentColor: theme.accent
    glassOpacity: 0.26
    implicitHeight: 142

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 18
        spacing: 12
        RowLayout {
            Layout.fillWidth: true
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                Text {
                    text: root.title
                    color: theme.text
                    font.pixelSize: 16
                    font.weight: Font.Black
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
                Text {
                    text: root.note
                    color: theme.muted
                    font.pixelSize: 11
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
            }
            StatusPill {
                Layout.preferredWidth: 70
                Layout.preferredHeight: 38
                theme: root.theme
                text: root.value + "%"
                fillColor: theme.accentMist
                strokeColor: slider.pressed ? theme.accent : theme.panelStroke
                textColor: theme.accent
                textSize: 18
            }
        }
        Slider {
            id: slider
            Layout.fillWidth: true
            Layout.preferredHeight: 42
            from: root.from
            to: root.to
            value: root.value
            hoverEnabled: true
            onMoved: root.moved(Math.round(value))

            background: Item {
                x: slider.leftPadding
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: 220
                implicitHeight: 16
                width: slider.availableWidth
                height: 16

                Rectangle {
                    anchors.fill: parent
                    radius: 8
                    color: theme.track
                    border.color: theme.lineSoft
                }
                Rectangle {
                    width: Math.max(16, slider.visualPosition * parent.width)
                    height: parent.height
                    radius: 8
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.0; color: theme.accent2 }
                        GradientStop { position: 1.0; color: theme.accent }
                    }
                    Behavior on width { NumberAnimation { duration: slider.pressed ? 0 : 140; easing.type: Easing.OutCubic } }
                }
                Rectangle {
                    width: Math.max(0, slider.visualPosition * parent.width - 10)
                    height: 4
                    radius: 2
                    anchors.verticalCenter: parent.verticalCenter
                    color: theme.glow
                    opacity: slider.pressed ? 0.95 : 0.48
                    Behavior on opacity { NumberAnimation { duration: 160 } }
                }
            }

            handle: Rectangle {
                x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: slider.pressed ? 34 : 30
                implicitHeight: implicitWidth
                radius: width / 2
                color: theme.thumb
                border.color: theme.accent
                border.width: 2
                scale: slider.pressed ? 1.08 : (slider.hovered ? 1.03 : 1.0)

                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width - 12
                    height: width
                    radius: width / 2
                    color: theme.accent
                    opacity: slider.pressed ? 0.88 : 0.58
                    Behavior on opacity { NumberAnimation { duration: 160 } }
                }
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width + 14
                    height: width
                    radius: width / 2
                    color: theme.glow
                    opacity: slider.pressed ? 0.55 : 0.0
                    z: -1
                    Behavior on opacity { NumberAnimation { duration: 160 } }
                }

                Behavior on implicitWidth { NumberAnimation { duration: 130; easing.type: Easing.OutCubic } }
                Behavior on scale { NumberAnimation { duration: 130; easing.type: Easing.OutCubic } }
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: 20
            Rectangle {
                width: valueBubbleText.implicitWidth + 20
                height: valueBubbleText.implicitHeight + 8
                radius: 10
                x: Math.max(0, Math.min(parent.width - width, slider.leftPadding + slider.visualPosition * slider.availableWidth - width / 2))
                y: slider.pressed ? -6 : 2
                color: theme.panelTop
                border.color: theme.accent
                opacity: slider.pressed ? 1 : 0
                Text {
                    id: valueBubbleText
                    anchors.centerIn: parent
                    text: root.value + "%"
                    color: theme.text
                    font.pixelSize: 11
                    font.weight: Font.Black
                }
                Behavior on opacity { NumberAnimation { duration: 120 } }
                Behavior on y { NumberAnimation { duration: 140; easing.type: Easing.OutCubic } }
            }
        }
    }
}
