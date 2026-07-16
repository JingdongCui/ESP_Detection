import QtQuick

QtObject {
    id: root

    property bool darkMode: true

    // Foundation
    readonly property color bg: darkMode ? "#07111f" : "#eef5ff"
    readonly property color bgTop: darkMode ? "#0d2037" : "#fbfdff"
    readonly property color bgBottom: darkMode ? "#030914" : "#dbe9f8"
    readonly property color panel: darkMode ? "#b0142235" : "#b8ffffff"
    readonly property color panel2: darkMode ? "#c01b2c43" : "#d8f7fbff"
    readonly property color surfaceGlass: darkMode ? "#8f182940" : "#a8ffffff"
    readonly property color surfaceRaised: darkMode ? "#d021324a" : "#eafafcff"
    readonly property color imageBg: darkMode ? "#050b14" : "#dce8f5"

    // Liquid glass surfaces
    readonly property color panelTop: darkMode ? "#d0223550" : "#e6ffffff"
    readonly property color panelBottom: darkMode ? "#aa0d1929" : "#bdebf3fb"
    readonly property color navTop: darkMode ? "#dc172940" : "#eaf9fcff"
    readonly property color navBottom: darkMode ? "#b3081220" : "#cbe4eef9"
    readonly property color navActive: darkMode ? "#a82c4c70" : "#d8e7f4ff"
    readonly property color glassOverlay: darkMode ? "#20ffffff" : "#52ffffff"
    readonly property color panelStroke: darkMode ? "#526d8da9" : "#9fb9cce0"
    readonly property color highlight: darkMode ? "#70ffffff" : "#f0ffffff"
    readonly property color highlightSoft: darkMode ? "#28ffffff" : "#80ffffff"
    readonly property color shadowStrong: darkMode ? "#b8000000" : "#35162c48"
    readonly property color glow: darkMode ? "#7049d8ff" : "#5558a9ff"

    // Brand and semantic colors
    readonly property color accent: darkMode ? "#49d8ff" : "#087fd1"
    readonly property color accent2: darkMode ? "#6f7dff" : "#4357d8"
    readonly property color purple: darkMode ? "#b889ff" : "#7555c9"
    readonly property color warn: darkMode ? "#ffc66d" : "#b66b00"
    readonly property color danger: darkMode ? "#ff718a" : "#d73a57"
    readonly property color success: darkMode ? "#54e7ae" : "#087f59"
    readonly property color onAccent: darkMode ? "#02131b" : "#ffffff"

    // Text
    readonly property color text: darkMode ? "#f5f9ff" : "#102033"
    readonly property color muted: darkMode ? "#afc0d4" : "#4d6278"
    readonly property color faint: darkMode ? "#75889e" : "#75879a"
    readonly property color navText: darkMode ? "#d9e8f6" : "#2e455d"

    // Controls and separators
    readonly property color line: darkMode ? "#455d768f" : "#91afc5da"
    readonly property color lineSoft: darkMode ? "#2e536b82" : "#70bfd0df"
    readonly property color track: darkMode ? "#7a101d2d" : "#a8d7e4ef"
    readonly property color thumb: darkMode ? "#eef9ff" : "#ffffff"

    // Washes
    readonly property color accentWash: darkMode ? "#3449d8ff" : "#24087fd1"
    readonly property color accentMist: darkMode ? "#1849d8ff" : "#12087fd1"
    readonly property color dangerWash: darkMode ? "#2cff718a" : "#1fd73a57"
    readonly property color warnWash: darkMode ? "#2affc66d" : "#20b66b00"
    readonly property color statusBg: darkMode ? "#2636c991" : "#dff6ef"
    readonly property color statusText: darkMode ? "#bffff0" : "#086947"
    readonly property color warnText: darkMode ? "#ffe6b8" : "#7a4900"
}
