import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

PremiumPanel {
    id: root

    property var host
    property string pageTitle: ""
    property string pageDescription: ""
    property string pageNumber: "01"

    radius: 20
    topColor: theme.panelTop
    bottomColor: theme.panelBottom
    overlayColor: theme.glassOverlay
    borderColor: theme.panelStroke
    glassOpacity: 0.28
    elevated: false

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 24
        anchors.rightMargin: 18
        spacing: 18

        Rectangle {
            Layout.preferredWidth: 44
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignVCenter
            radius: 14
            color: theme.accentWash
            border.width: 1
            border.color: theme.accent

            Text {
                anchors.centerIn: parent
                text: root.pageNumber
                color: theme.accent
                font.pixelSize: 12
                font.weight: Font.Black
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            spacing: 3

            Text {
                Layout.fillWidth: true
                text: root.pageTitle
                color: theme.text
                font.pixelSize: 22
                font.weight: Font.Black
                elide: Text.ElideRight
            }

            Text {
                Layout.fillWidth: true
                text: root.pageDescription
                color: theme.muted
                font.pixelSize: 12
                elide: Text.ElideRight
            }
        }

        Rectangle {
            Layout.preferredWidth: 1
            Layout.preferredHeight: 36
            color: theme.lineSoft
        }

        ColumnLayout {
            Layout.preferredWidth: 154
            Layout.alignment: Qt.AlignVCenter
            spacing: 3

            Text {
                text: qsTr("设备状态")
                color: theme.faint
                font.pixelSize: 10
                font.weight: Font.DemiBold
            }

            RowLayout {
                spacing: 7
                Rectangle {
                    Layout.preferredWidth: 8
                    Layout.preferredHeight: 8
                    radius: 4
                    color: host.displayConnected ? theme.accent : theme.warn
                }
                Text {
                    text: host.displayConnected ? qsTr("在线运行") : qsTr("等待连接")
                    color: theme.text
                    font.pixelSize: 13
                    font.weight: Font.Bold
                }
            }
        }

        Button {
            id: syncButton
            Layout.preferredWidth: 112
            Layout.preferredHeight: 42
            text: qsTr("同步时间")
            hoverEnabled: true
            onClicked: host.sendTimeSync()

            contentItem: Text {
                text: syncButton.text
                color: syncButton.hovered ? theme.onAccent : theme.text
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 13
                font.weight: Font.Bold
            }

            background: Rectangle {
                radius: 13
                color: syncButton.hovered ? theme.accent : theme.surfaceRaised
                border.width: 1
                border.color: syncButton.hovered ? theme.accent : theme.panelStroke
                Behavior on color { ColorAnimation { duration: 150 } }
                Behavior on border.color { ColorAnimation { duration: 150 } }
            }
        }
    }
}
