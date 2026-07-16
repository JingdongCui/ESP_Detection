import QtQuick

Item {
    id: root
    property var theme

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: root.theme.bgTop }
            GradientStop { position: 0.52; color: root.theme.bg }
            GradientStop { position: 1.0; color: root.theme.bgBottom }
        }
    }

    // Iridescent liquid-light fields. They remain intentionally subtle so the
    // engineering data keeps visual priority.
    Rectangle {
        width: parent.width * 0.52
        height: width
        radius: width / 2
        x: -width * 0.28
        y: -height * 0.38
        opacity: root.theme.darkMode ? 0.22 : 0.32
        gradient: Gradient {
            GradientStop { position: 0.0; color: root.theme.accent2 }
            GradientStop { position: 0.48; color: root.theme.accent }
            GradientStop { position: 1.0; color: "#00000000" }
        }
    }

    Rectangle {
        width: parent.width * 0.46
        height: width
        radius: width / 2
        x: parent.width - width * 0.68
        y: parent.height - height * 0.55
        opacity: root.theme.darkMode ? 0.16 : 0.23
        gradient: Gradient {
            GradientStop { position: 0.0; color: root.theme.purple }
            GradientStop { position: 0.55; color: root.theme.accent2 }
            GradientStop { position: 1.0; color: "#00000000" }
        }
    }

    Rectangle {
        width: parent.width * 0.28
        height: width
        radius: width / 2
        x: parent.width * 0.47
        y: -height * 0.48
        opacity: root.theme.darkMode ? 0.12 : 0.20
        gradient: Gradient {
            GradientStop { position: 0.0; color: root.theme.success }
            GradientStop { position: 1.0; color: "#00000000" }
        }
    }

    // Precision grid: enough to feel like an enterprise control surface,
    // faint enough to avoid becoming decorative noise.
    Repeater {
        model: 12
        Rectangle {
            required property int index
            x: index * root.width / 11
            width: 1
            height: root.height
            color: root.theme.lineSoft
            opacity: root.theme.darkMode ? 0.10 : 0.14
        }
    }
    Repeater {
        model: 8
        Rectangle {
            required property int index
            y: index * root.height / 7
            height: 1
            width: root.width
            color: root.theme.lineSoft
            opacity: root.theme.darkMode ? 0.08 : 0.12
        }
    }

    Rectangle {
        anchors.fill: parent
        color: root.theme.darkMode ? "#18000000" : "#08ffffff"
    }
}
