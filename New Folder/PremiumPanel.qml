import QtQuick

Item {
    id: root
    property var theme
    property real radius: 22
    property color topColor: theme && theme.panelTop ? theme.panelTop : "#cc20324a"
    property color bottomColor: theme && theme.panelBottom ? theme.panelBottom : "#aa0d1929"
    property color overlayColor: theme && theme.glassOverlay ? theme.glassOverlay : "transparent"
    property color borderColor: theme && theme.panelStroke ? theme.panelStroke : "#526d8da9"
    property color accentColor: theme && theme.accent ? theme.accent : "#49d8ff"
    property bool elevated: true
    property bool accented: false
    property bool hoverActive: false
    property real glassOpacity: 0.42
    default property alias content: contentLayer.data

    implicitWidth: 260
    implicitHeight: 150
    scale: hoverActive ? 1.006 : 1.0

    Behavior on scale { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }

    Rectangle {
        anchors.fill: parent
        anchors.margins: -10
        y: root.elevated ? 8 : 0
        radius: root.radius + 10
        color: root.theme && root.theme.shadowStrong ? root.theme.shadowStrong : "#99000000"
        opacity: root.elevated ? 0.30 : 0
    }

    Rectangle {
        anchors.fill: parent
        radius: root.radius
        border.width: 1
        border.color: root.borderColor
        gradient: Gradient {
            GradientStop { position: 0.0; color: root.topColor }
            GradientStop { position: 0.48; color: Qt.rgba(root.topColor.r, root.topColor.g, root.topColor.b, 0.78) }
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

    // Liquid specular sweep.
    Rectangle {
        width: parent.width * 0.58
        height: Math.max(50, parent.height * 0.48)
        radius: height / 2
        x: -width * 0.18
        y: -height * 0.56
        rotation: -8
        color: root.theme && root.theme.highlight ? root.theme.highlight : "#70ffffff"
        opacity: 0.12
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: root.radius
        anchors.rightMargin: root.radius
        height: 1
        color: root.theme && root.theme.highlight ? root.theme.highlight : "#70ffffff"
        opacity: 0.78
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: root.radius
        anchors.rightMargin: root.radius
        height: root.accented ? 2 : 1
        opacity: root.accented ? 0.82 : 0.24
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
