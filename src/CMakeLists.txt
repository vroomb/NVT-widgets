include("C:/Program Files/Microsoft Visual Studio/18/Community/VC/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(maddy CONFIG REQUIRED)

add_subdirectory(project)
add_subdirectory(colors)
add_subdirectory(timeline)

qt_add_executable(NVT-widgets
    WIN32 MACOSX_BUNDLE
    log.cpp
    main.cpp
    editor.cpp
    explorer.cpp
    main_window.cpp
    application.cpp
    yaml_display.cpp
    launch_window.cpp
)

target_include_directories(NVT-widgets
    PRIVATE
        .
        ../FakeVim
)

target_link_libraries(NVT-widgets
    PRIVATE
        maddy::maddy
        fakevim
        colorlib
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
        projectlib
        timelinelib
)

set_target_properties(NVT-widgets PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 23
)
