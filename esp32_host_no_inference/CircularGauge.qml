import QtQuick

PremiumPanel {
    id: root
    property real value: 0
    property string title: ""
    property string subtitle: ""
    property color accent: theme.accent
    property real animatedValue: value
    property real sweepAngle: 0
    readonly property string paintKey: [
        theme.line, theme.lineSoft, theme.panel, theme.panel2, theme.thumb,
        theme.text, theme.faint, accent
    ].join("|")
    implicitWidth: 190
    implicitHeight: 218
    radius: 17
    topColor: theme.panelTop
    bottomColor: theme.panelBottom
    overlayColor: theme.glassOverlay
    borderColor: theme.panelStroke
    accentColor: root.accent
    accented: true
    glassOpacity: 0.26

    Behavior on animatedValue { NumberAnimation { duration: 520; easing.type: Easing.OutCubic } }
    onValueChanged: animatedValue = value
    onPaintKeyChanged: canvas.requestPaint()
    onAccentChanged: canvas.requestPaint()

    SequentialAnimation on sweepAngle {
        loops: Animation.Infinite
        NumberAnimation { from: 0; to: 360; duration: 6200; easing.type: Easing.InOutSine }
    }

    Canvas {
        id: canvas
        anchors.horizontalCenter: parent.horizontalCenter
        y: 18
        width: 148
        height: 148
        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            var cx = width / 2
            var cy = height / 2
            var r = 54
            var start = Math.PI * 0.72
            var span = Math.PI * 1.56

            ctx.lineWidth = 18
            ctx.lineCap = "round"
            ctx.strokeStyle = root.theme.line
            ctx.globalAlpha = 0.72
            ctx.beginPath()
            ctx.arc(cx, cy, r, start, start + span, false)
            ctx.stroke()

            ctx.lineWidth = 2
            ctx.strokeStyle = root.theme.lineSoft
            ctx.globalAlpha = 0.52
            for (var i = 0; i <= 16; ++i) {
                var a = start + span * i / 16
                var x1 = cx + Math.cos(a) * 40
                var y1 = cy + Math.sin(a) * 40
                var x2 = cx + Math.cos(a) * 44
                var y2 = cy + Math.sin(a) * 44
                ctx.beginPath()
                ctx.moveTo(x1, y1)
                ctx.lineTo(x2, y2)
                ctx.stroke()
            }

            ctx.lineWidth = 18
            ctx.strokeStyle = root.accent
            ctx.globalAlpha = 0.96
            ctx.beginPath()
            ctx.arc(cx, cy, r, start, start + span * Math.max(0, Math.min(100, root.animatedValue)) / 100, false)
            ctx.stroke()

            var sweep = (root.sweepAngle % 360) * Math.PI / 180
            var marker = start + (span * Math.max(0, Math.min(100, root.animatedValue)) / 100) + Math.sin(sweep) * 0.08
            ctx.lineWidth = 4
            ctx.globalAlpha = 0.88
            ctx.strokeStyle = root.theme.thumb
            ctx.beginPath()
            ctx.arc(cx, cy, r, marker - 0.045, marker + 0.045, false)
            ctx.stroke()
        }
        Connections {
            target: root
            function onAnimatedValueChanged() { canvas.requestPaint() }
            function onSweepAngleChanged() { canvas.requestPaint() }
        }
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        y: 62
        text: Math.round(root.animatedValue) + "%"
        color: theme.text
        font.pixelSize: 28
        font.weight: Font.Black
    }
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        y: 166
        text: root.title
        color: theme.text
        font.pixelSize: 15
        font.weight: Font.DemiBold
    }
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        y: 188
        text: root.subtitle
        color: theme.faint
        font.pixelSize: 11
    }
}
