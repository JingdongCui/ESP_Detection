cmake_minimum_required(VERSION 3.16)

project(esp32_host_no_inference VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Quick Network)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appesp32_host
    main.cpp
    hostcontroller.cpp
    hostcontroller.h
    hostnetworkworker.cpp
    hostnetworkworker.h
    packetprotocol.cpp
    packetprotocol.h
)

qt_add_qml_module(appesp32_host
    URI esp32_host
    QML_FILES
        Main.qml
        AppTheme.qml
        AmbientBackground.qml
        HeaderBar.qml
        PremiumPanel.qml
        StatusPill.qml
        NavButton.qml
        PillButton.qml
        MetricCard.qml
        CircularGauge.qml
        TrendChart.qml
        DashboardPage.qml
        DetectionStage.qml
        DetectionPage.qml
        ControlSlider.qml
        PremiumSwitch.qml
        ControlPage.qml
        ReservePage.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appesp32_host PROPERTIES
    OUTPUT_NAME esp32_host_no_inference
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
    MACOSX_BUNDLE_GUI_IDENTIFIER com.kazeform.esp32host
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(appesp32_host PRIVATE ESP32_HOST_NO_INFERENCE=1)

target_link_libraries(appesp32_host
    PRIVATE Qt6::Quick Qt6::Network
)

install(TARGETS appesp32_host
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(WIN32 OR APPLE)
    qt_generate_deploy_qml_app_script(
        TARGET appesp32_host
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${deploy_script})
endif()
