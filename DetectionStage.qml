import QtQuick

PremiumPanel {
    id: root
    property var host
    property string imageUrl: ""
    property var detections: []
    property string frameInfo: ""
    property string categoryLabel: "极兔"
    property int categoryConfidence: 0
    property bool overlayEnabled: true
    radius: 18
    topColor: theme.imageBg
    bottomColor: theme.bgBottom
    overlayColor: theme.glassOverlay
    borderColor: theme.panelStroke
    glassOpacity: 0.18
    clip: true

    Image {
        id: frameImage
        anchors.fill: parent
        anchors.margins: 18
        source: root.imageUrl
        fillMode: Image.PreserveAspectFit
        asynchronous: true
        cache: false
        smooth: true
        opacity: source.toString().length > 0 ? 1 : 0
        Behavior on opacity { NumberAnimation { duration: 260 } }
    }

    Canvas {
        id: previewCanvas
        anchors.fill: parent
        anchors.margins: 18
        visible: root.imageUrl.length === 0
        onVisibleChanged: requestPaint()
        Connections {
            target: root
            function onFrameInfoChanged() { previewCanvas.requestPaint() }
            function onThemeChanged() { previewCanvas.requestPaint() }
        }
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            var g = ctx.createLinearGradient(0, 0, width, height)
            g.addColorStop(0, root.theme.panelTop)
            g.addColorStop(0.55, root.theme.imageBg)
            g.addColorStop(1, root.theme.bgBottom)
            ctx.fillStyle = g
            ctx.fillRect(0, 0, width, height)

            function roundedRect(x, y, w, h, r) {
                ctx.beginPath()
                ctx.moveTo(x + r, y)
                ctx.lineTo(x + w - r, y)
                ctx.quadraticCurveTo(x + w, y, x + w, y + r)
                ctx.lineTo(x + w, y + h - r)
                ctx.quadraticCurveTo(x + w, y + h, x + w - r, y + h)
                ctx.lineTo(x + r, y + h)
                ctx.quadraticCurveTo(x, y + h, x, y + h - r)
                ctx.lineTo(x, y + r)
                ctx.quadraticCurveTo(x, y, x + r, y)
                ctx.closePath()
            }

            ctx.globalAlpha = 0.1
            ctx.strokeStyle = root.theme.highlight
            ctx.lineWidth = 1
            for (var x = 24; x < width; x += 72) {
                ctx.beginPath(); ctx.moveTo(x, 28); ctx.lineTo(x + 28, height - 28); ctx.stroke()
            }
            ctx.globalAlpha = 1

            var cardW = width * 0.56
            var cardH = height * 0.3
            var cardX = (width - cardW) / 2
            var cardY = height * 0.31
            roundedRect(cardX, cardY + 10, cardW, cardH, 26)
            ctx.fillStyle = "rgba(0, 0, 0, 0.26)"
            ctx.fill()
            roundedRect(cardX, cardY, cardW, cardH, 26)
            var panel = ctx.createLinearGradient(cardX, cardY, cardX + cardW, cardY + cardH)
            panel.addColorStop(0, root.theme.panelTop)
            panel.addColorStop(1, root.theme.panelBottom)
            ctx.fillStyle = panel
            ctx.fill()
            ctx.strokeStyle = root.theme.panelStroke
            ctx.lineWidth = 1
            ctx.stroke()

            ctx.fillStyle = root.theme.accent
            ctx.globalAlpha = 0.84
            roundedRect(cardX + 28, cardY + 30, 68, 12, 6)
            ctx.fill()
            ctx.fillStyle = root.theme.accent2
            ctx.globalAlpha = 0.52
            roundedRect(cardX + 28, cardY + 54, cardW - 56, 8, 4)
            ctx.fill()
            roundedRect(cardX + 28, cardY + 76, cardW * 0.58, 8, 4)
            ctx.fill()
            ctx.globalAlpha = 1

            ctx.fillStyle = root.theme.text
            ctx.font = "700 28px sans-serif"
            ctx.textAlign = "center"
            ctx.fillText(root.frameInfo.length > 0 ? root.frameInfo : "等待板端 JPEG 预览", width / 2, cardY + cardH - 42)
        }
    }

    Repeater {
        model: root.overlayEnabled ? root.detections : []
        delegate: Rectangle {
            id: detectionBox
            property real conf: Number(modelData.confidence || 0) * 100
            x: 18 + Number(modelData.x || 0) * (root.width - 36)
            y: 18 + Number(modelData.y || 0) * (root.height - 36)
            width: Number(modelData.w || 0.2) * (root.width - 36)
            height: Number(modelData.h || 0.2) * (root.height - 36)
            color: detectionBox.conf < host.dangerThreshold ? theme.dangerWash : theme.accentMist
            border.width: 1
            border.color: detectionBox.conf < host.dangerThreshold ? theme.danger : theme.accent
            radius: 4
            opacity: 0.96
            Behavior on x { NumberAnimation { duration: 260; easing.type: Easing.OutCubic } }
            Behavior on y { NumberAnimation { duration: 260; easing.type: Easing.OutCubic } }
            SequentialAnimation on opacity {
                running: detectionBox.conf < host.dangerThreshold
                loops: Animation.Infinite
                NumberAnimation { to: 0.55; duration: 520 }
                NumberAnimation { to: 1.0; duration: 520 }
            }
            Repeater {
                model: [
                    {x: 0, y: 0, w: 20, h: 3},
                    {x: 0, y: 0, w: 3, h: 20},
                    {x: parent.width - 20, y: 0, w: 20, h: 3},
                    {x: parent.width - 3, y: 0, w: 3, h: 20},
                    {x: 0, y: parent.height - 3, w: 20, h: 3},
                    {x: 0, y: parent.height - 20, w: 3, h: 20},
                    {x: parent.width - 20, y: parent.height - 3, w: 20, h: 3},
                    {x: parent.width - 3, y: parent.height - 20, w: 3, h: 20}
                ]
                delegate: Rectangle {
                    x: modelData.x
                    y: modelData.y
                    width: modelData.w
                    height: modelData.h
                    radius: 2
                    color: detectionBox.conf < host.dangerThreshold ? theme.danger : theme.accent
                }
            }
            Rectangle {
                x: -1
                y: -32
                height: 28
                radius: 10
                color: detectionBox.conf < host.dangerThreshold ? theme.dangerWash : theme.accentWash
                border.color: detectionBox.conf < host.dangerThreshold ? theme.danger : theme.accent
                width: labelText.width + 18
                Text {
                    id: labelText
                    anchors.centerIn: parent
                    text: (modelData.label || "快递标识") + "  " + Math.round(detectionBox.conf) + "%"
                    color: theme.text
                    font.pixelSize: 12
                    font.weight: Font.Black
                }
            }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 24
        width: categoryText.width + 28
        height: 42
        radius: 14
        color: theme.surfaceGlass
        border.color: theme.accent
        Text {
            id: categoryText
            anchors.centerIn: parent
            text: root.categoryLabel + "  " + Math.round(Number(root.categoryConfidence || 0)) + "%"
            color: theme.text
            font.pixelSize: 16
            font.weight: Font.Black
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 18
        height: 58
        radius: 18
        color: theme.surfaceGlass
        border.color: theme.panelStroke
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 24
            anchors.verticalCenter: parent.verticalCenter
            text: root.frameInfo.length > 0 ? root.frameInfo : "等待图像 socket 数据"
            color: theme.text
            font.pixelSize: 14
            font.weight: Font.DemiBold
        }
        Text {
            anchors.right: parent.right
            anchors.rightMargin: 24
            anchors.verticalCenter: parent.verticalCenter
            text: root.overlayEnabled ? "叠加框开启" : "叠加框关闭"
            color: root.overlayEnabled ? theme.accent : theme.faint
            font.pixelSize: 13
            font.weight: Font.Black
        }
    }
}
