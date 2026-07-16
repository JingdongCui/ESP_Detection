import QtQuick
import QtQuick.Layouts

PremiumPanel {
    id: root

    property real value: 0
    property string title: ""
    property string subtitle: ""
    property color accent: theme.accent
    property real animatedValue: value
    property real sweepAngle: 0
    readonly property real gaugeSize: Math.max(86, Math.min(116, width - 28, height - 58))
    readonly property var paintKey: [
        theme.line, theme.lineSoft, theme.panel, theme.panel2, theme.thumb,
        theme.text, theme.faint, accent, gaugeSize
    ].join("|")

    implicitWidth: 170
    implicitHeight: 170
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
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        width: root.gaugeSize
        height: root.gaugeSize

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            var cx = width / 2
            var cy = height / 2
            var r = width * 0.34
            var trackWidth = Math.max(11, width * 0.12)
            var start = Math.PI * 0.72
            var span = Math.PI * 1.56

            ctx.lineWidth = trackWidth
            ctx.lineCap = "round"
            ctx.strokeStyle = root.theme.line
            ctx.globalAlpha = 0.72
            ctx.beginPath()
            ctx.arc(cx, cy, r, start, start + span, false)
            ctx.stroke()

            ctx.lineWidth = Math.max(1.5, width * 0.014)
            ctx.strokeStyle = root.theme.lineSoft
            ctx.globalAlpha = 0.52
            for (var i = 0; i <= 16; ++i) {
                var a = start + span * i / 16
                var innerRadius = r - trackWidth * 0.72
                var outerRadius = r - trackWidth * 0.45
                var x1 = cx + Math.cos(a) * innerRadius
                var y1 = cy + Math.sin(a) * innerRadius
                var x2 = cx + Math.cos(a) * outerRadius
                var y2 = cy + Math.sin(a) * outerRadius
                ctx.beginPath()
                ctx.moveTo(x1, y1)
                ctx.lineTo(x2, y2)
                ctx.stroke()
            }

            ctx.lineWidth = trackWidth
            ctx.strokeStyle = root.accent
            ctx.globalAlpha = 0.96
            ctx.beginPath()
            ctx.arc(cx, cy, r, start,
                    start + span * Math.max(0, Math.min(100, root.animatedValue)) / 100,
                    false)
            ctx.stroke()

            var sweep = (root.sweepAngle % 360) * Math.PI / 180
            var marker = start
                    + span * Math.max(0, Math.min(100, root.animatedValue)) / 100
                    + Math.sin(sweep) * 0.08
            ctx.lineWidth = Math.max(3, width * 0.028)
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
        anchors.centerIn: canvas
        anchors.verticalCenterOffset: -2
        text: Math.round(root.animatedValue) + "%"
        color: root.theme.text
        font.pixelSize: Math.max(20, Math.min(25, root.gaugeSize * 0.21))
        font.weight: Font.Black
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 9
        spacing: 1

        Text {
            Layout.fillWidth: true
            text: root.title
            color: root.theme.text
            font.pixelSize: 13
            font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
        }

        Text {
            Layout.fillWidth: true
            text: root.subtitle
            color: root.theme.faint
            font.pixelSize: 10
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
        }
    }
}
