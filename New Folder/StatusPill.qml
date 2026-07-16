import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    property var theme
    property string text: ""
    property color fillColor: theme && theme.statusBg ? theme.statusBg : "#2636c991"
    property color strokeColor: theme && theme.lineSoft ? theme.lineSoft : "#455d768f"
    property color textColor: theme && theme.statusText ? theme.statusText : "#bffff0"
    property int textSize: 12
    property bool showDot: true

    implicitWidth: row.implicitWidth + 26
    implicitHeight: 36
    radius: height / 2
    color: fillColor
    border.width: 1
    border.color: strokeColor

    RowLayout {
        id: row
        anchors.centerIn: parent
        spacing: 8

        Rectangle {
            visible: root.showDot
            Layout.preferredWidth: 8
            Layout.preferredHeight: 8
            radius: 4
            color: root.textColor
            Rectangle {
                anchors.centerIn: parent
                width: 16
                height: 16
                radius: 8
                color: root.textColor
                opacity: 0.12
            }
        }
        Text {
            text: root.text
            color: root.textColor
            font.pixelSize: root.textSize
            font.weight: Font.Black
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        height: 1
        color: theme && theme.highlight ? theme.highlight : "#70ffffff"
        opacity: 0.58
    }
}
