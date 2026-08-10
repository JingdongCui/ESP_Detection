import QtQuick

Item {
    id: root
    property var theme
    property real radius: 18
    property color topColor: theme && theme.panelTop ? theme.panelTop : "#202734"
    property color bottomColor: theme && theme.panelBottom ? theme.panelBottom : "#151a22"
    property color overlayColor: theme && theme.glassOverlay ? theme.glassOverlay : "transparent"
    property color borderColor: theme && theme.panelStroke ? theme.panelStroke : "#334050"
    property color accentColor: theme && theme.accent ? theme.accent : "#20e0c2"
    property bool elevated: true
    property bool accented: false
    property bool hoverActive: false
    property real glassOpacity: 0.36
    default property alias content: contentLayer.data

    implicitWidth: 240
    implicitHeight: 140
    scale: hoverActive ? 1.006 : 1.0

    Behavior on scale { NumberAnimation { duration: 160; easing.type: Easing.OutCubic } }

    Rectangle {
        anchors.fill: parent
        anchors.margins: -8
        y: root.elevated ? 7 : 0
        radius: root.radius + 8
        color: root.theme && root.theme.shadowStrong ? root.theme.shadowStrong : "#66000000"
        opacity: root.elevated ? 0.42 : 0
    }

    Rectangle {
        anchors.fill: parent
        radius: root.radius
        border.width: 1
        border.color: root.borderColor
        gradient: Gradient {
            GradientStop { position: 0.0; color: root.topColor }
            GradientStop { position: 1.0; color: root.bottomColor }
        }
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: 1
        radius: Math.max(0, root.radius - 1)
        color: root.overlayColor
        opacity: root.glassOpacity
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: root.radius
        anchors.rightMargin: root.radius
        height: 1
        color: root.theme && root.theme.highlight ? root.theme.highlight : "#30ffffff"
        opacity: 0.72
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: root.radius
        anchors.rightMargin: root.radius
        height: root.accented ? 2 : 1
        opacity: root.accented ? 0.78 : 0.26
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop { position: 0.0; color: "#00000000" }
            GradientStop { position: 0.5; color: root.accentColor }
            GradientStop { position: 1.0; color: "#00000000" }
        }
    }

    Item {
        id: contentLayer
        anchors.fill: parent
    }
}
