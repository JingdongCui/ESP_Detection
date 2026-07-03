import QtQuick

Item {
    id: root
    property var theme
    property bool checked: false
    property bool hovered: switchMouse.containsMouse
    property real pulse: 0
    signal toggled(bool checked)

    implicitWidth: 74
    implicitHeight: 36

    SequentialAnimation {
        id: clickPulse
        NumberAnimation { target: root; property: "pulse"; from: 0; to: 1; duration: 120; easing.type: Easing.OutCubic }
        NumberAnimation { target: root; property: "pulse"; from: 1; to: 0; duration: 260; easing.type: Easing.OutCubic }
    }

    Rectangle {
        id: glow
        anchors.fill: track
        anchors.margins: -5
        radius: height / 2
        color: theme.glow
        opacity: root.checked ? (root.hovered ? 0.42 : 0.28) : 0
        Behavior on opacity { NumberAnimation { duration: 180 } }
    }

    Rectangle {
        id: track
        anchors.fill: parent
        radius: height / 2
        scale: 1 + root.pulse * 0.035
        color: root.checked ? theme.navActive : theme.track
        border.color: root.checked ? theme.accent : theme.line
        border.width: 1
        Behavior on scale { NumberAnimation { duration: 120; easing.type: Easing.OutCubic } }

        Rectangle {
            anchors.centerIn: parent
            width: parent.width + 18 + root.pulse * 18
            height: parent.height + 10 + root.pulse * 10
            radius: height / 2
            color: root.checked ? theme.accent : theme.accent2
            opacity: root.pulse * 0.18
            z: -1
        }

        Rectangle {
            id: offLayer
            anchors.fill: parent
            anchors.margins: 2
            radius: height / 2
            color: theme.surfaceRaised
            opacity: root.checked ? 0 : 0.58
            Behavior on opacity { NumberAnimation { duration: 180 } }
        }

        Rectangle {
            anchors.fill: parent
            anchors.margins: 2
            radius: height / 2
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: theme.accent2 }
                GradientStop { position: 1.0; color: theme.accent }
            }
            opacity: root.checked ? 0.82 : 0
            Behavior on opacity { NumberAnimation { duration: 230; easing.type: Easing.OutCubic } }
        }

        Rectangle {
            id: knob
            width: (root.hovered ? 30 : 28) + root.pulse * 4
            height: width
            radius: width / 2
            x: root.checked ? parent.width - width - 4 : 4
            anchors.verticalCenter: parent.verticalCenter
            color: theme.thumb
            border.color: root.checked ? theme.accent : theme.lineSoft
            border.width: 1

            Rectangle {
                anchors.centerIn: parent
                width: parent.width - 12
                height: width
                radius: width / 2
                color: root.checked ? theme.accent : theme.faint
                opacity: root.checked ? 0.82 : 0.36
                Behavior on color { ColorAnimation { duration: 230 } }
                Behavior on opacity { NumberAnimation { duration: 180 } }
            }

            Behavior on x { NumberAnimation { duration: 220; easing.type: Easing.OutCubic } }
            Behavior on width { NumberAnimation { duration: 140; easing.type: Easing.OutCubic } }
            Behavior on border.color { ColorAnimation { duration: 220 } }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 12
            text: "ON"
            color: theme.onAccent
            font.pixelSize: 9
            font.weight: Font.Black
            opacity: root.checked ? 0.9 : 0
            Behavior on opacity { NumberAnimation { duration: 140 } }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
            text: "OFF"
            color: theme.faint
            font.pixelSize: 9
            font.weight: Font.Black
            opacity: root.checked ? 0 : 0.78
            Behavior on opacity { NumberAnimation { duration: 140 } }
        }

        Behavior on color { ColorAnimation { duration: 180 } }
        Behavior on border.color { ColorAnimation { duration: 220 } }
    }

    MouseArea {
        id: switchMouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            clickPulse.restart()
            root.checked = !root.checked
            root.toggled(root.checked)
        }
    }
}
