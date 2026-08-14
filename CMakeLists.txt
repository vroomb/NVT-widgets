cmake_minimum_required(VERSION 3.19)
project(NVT-widgets VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(NVT-widgets
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.ui
)

target_link_libraries(NVT-widgets
    PRIVATE
        Qt::Core
        Qt::Widgets
)

set_target_properties(NVT-widgets PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 23
)

install(TARGETS NVT-widgets
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET NVT-widgets
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
