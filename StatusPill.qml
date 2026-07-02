import QtQuick

Rectangle {
    id: root
    property var theme
    property string text: ""
    property color fillColor: theme && theme.statusBg ? theme.statusBg : "#14392f"
    property color strokeColor: theme && theme.lineSoft ? theme.lineSoft : "#26313e"
    property color textColor: theme && theme.statusText ? theme.statusText : "#b8ffea"
    property int textSize: 12

    implicitWidth: label.implicitWidth + 28
    implicitHeight: 34
    radius: height / 2
    color: fillColor
    border.width: 1
    border.color: strokeColor

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        height: 1
        color: theme && theme.highlight ? theme.highlight : "#30ffffff"
        opacity: 0.6
    }

    Text {
        id: label
        anchors.centerIn: parent
        text: root.text
        color: root.textColor
        font.pixelSize: root.textSize
        font.weight: Font.Black
    }
}
