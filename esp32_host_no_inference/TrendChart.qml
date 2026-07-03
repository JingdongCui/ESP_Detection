import QtQuick

PremiumPanel {
    id: root
    property var history: []
    property string title: ""
    property string fieldA: "cpu"
    property string fieldB: "psram"
    property color colorA: theme.accent
    property color colorB: theme.accent2
    radius: 16
    topColor: theme.panelTop
    bottomColor: theme.panelBottom
    overlayColor: theme.glassOverlay
    borderColor: theme.panelStroke
    glassOpacity: 0.27

    Text {
        x: 18
        y: 16
        text: root.title
        color: theme.text
        font.pixelSize: 16
        font.weight: Font.Black
    }

    Canvas {
        id: chart
        anchors.fill: parent
        anchors.margins: 18
        anchors.topMargin: 52
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            var w = width
            var h = height
            ctx.strokeStyle = root.theme.lineSoft
            ctx.lineWidth = 1
            ctx.globalAlpha = 0.42
            for (var g = 0; g < 4; ++g) {
                var gy = h * g / 3
                ctx.beginPath()
                ctx.moveTo(0, gy)
                ctx.lineTo(w, gy)
                ctx.stroke()
            }
            ctx.globalAlpha = 1
            drawArea(root.fieldA, root.colorA, 0.13)
            drawArea(root.fieldB, root.colorB, 0.1)
            drawLine(root.fieldA, root.colorA)
            drawLine(root.fieldB, root.colorB)

            function valueAt(item, field) {
                var v = item[field]
                if (v === undefined || v === null) return 0
                return Math.max(0, Math.min(100, Number(v)))
            }
            function drawArea(field, color, alpha) {
                var n = root.history.length
                if (n < 2) return
                var fill = ctx.createLinearGradient(0, 0, 0, h)
                fill.addColorStop(0, color)
                fill.addColorStop(1, "rgba(0, 0, 0, 0)")
                ctx.fillStyle = fill
                ctx.globalAlpha = alpha
                ctx.beginPath()
                for (var i = 0; i < n; ++i) {
                    var x = n === 1 ? 0 : w * i / (n - 1)
                    var y = h - h * valueAt(root.history[i], field) / 100
                    if (i === 0) ctx.moveTo(x, y)
                    else ctx.lineTo(x, y)
                }
                ctx.lineTo(w, h)
                ctx.lineTo(0, h)
                ctx.closePath()
                ctx.fill()
                ctx.globalAlpha = 1
            }
            function drawLine(field, color) {
                var n = root.history.length
                if (n < 2) return
                ctx.strokeStyle = color
                ctx.lineWidth = 3
                ctx.lineJoin = "round"
                ctx.lineCap = "round"
                ctx.beginPath()
                for (var i = 0; i < n; ++i) {
                    var x = n === 1 ? 0 : w * i / (n - 1)
                    var y = h - h * valueAt(root.history[i], field) / 100
                    if (i === 0) ctx.moveTo(x, y)
                    else ctx.lineTo(x, y)
                }
                ctx.stroke()
            }
        }
    }

    Row {
        anchors.right: parent.right
        anchors.rightMargin: 18
        y: 18
        spacing: 14
        Repeater {
            model: [{name: fieldA, color: colorA}, {name: fieldB, color: colorB}]
            Row {
                spacing: 6
                Rectangle { width: 8; height: 8; radius: 4; color: modelData.color; anchors.verticalCenter: parent.verticalCenter }
                Text { text: modelData.name.toUpperCase(); color: theme.muted; font.pixelSize: 11; font.weight: Font.DemiBold }
            }
        }
    }

    Connections {
        target: root
        function onHistoryChanged() { chart.requestPaint() }
    }
}
