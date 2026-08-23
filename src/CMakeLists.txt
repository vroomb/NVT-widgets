qt_add_executable(NVT-widgets
    WIN32 MACOSX_BUNDLE
    nvt.hpp
    log.hpp
    log.cpp
    explorer.hpp
    explorer.cpp
    mainwindow.hpp
    mainwindow.cpp
    application.hpp
    application.cpp
    editor.hpp
    editor.cpp
    main.cpp
)

target_include_directories(NVT-widgets
    PRIVATE
        ../FakeVim
)

target_link_libraries(NVT-widgets
    PRIVATE
        fakevim
        Qt::Core
        Qt::Widgets
)

set_target_properties(NVT-widgets PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 23
)
