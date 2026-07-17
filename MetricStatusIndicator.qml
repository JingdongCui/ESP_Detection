import QtQuick

Item {
    id: root
    property var theme
    property color accent: theme.accent

    implicitWidth: 42
    implicitHeight: 42

    Rectangle {
        anchors.centerIn: parent
        width: 42
        height: 42
        radius: 15
        color: Qt.rgba(root.accent.r, root.accent.g, root.accent.b, 0.10)
    }

    Rectangle {
        anchors.centerIn: parent
        width: 36
        height: 36
        radius: 12
        color: Qt.rgba(root.accent.r, root.accent.g, root.accent.b, 0.20)
        border.width: 2
        border.color: root.accent

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            height: 1
            color: root.theme.highlight
        }

        Rectangle {
            anchors.centerIn: parent
            width: 11
            height: 11
            radius: 6
            color: root.accent

            Rectangle {
                anchors.centerIn: parent
                width: 4
                height: 4
                radius: 2
                color: root.theme.thumb
                opacity: 0.92
            }
        }
    }
}
