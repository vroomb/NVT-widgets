add_subdirectory(project)
add_subdirectory(colors)
add_subdirectory(timeline)

qt_add_executable(NVT-widgets
    WIN32 MACOSX_BUNDLE
    explorer.cpp
    main_window.cpp
    log.cpp
    application.cpp
    editor.cpp
    main.cpp
)

target_include_directories(NVT-widgets
    PRIVATE
        .
        ../FakeVim
)

target_link_libraries(NVT-widgets
    PRIVATE
        fakevim
        colorlib
        Qt::Core
        Qt::Widgets
        projectlib
        timelinelib
)

set_target_properties(NVT-widgets PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 23
)
