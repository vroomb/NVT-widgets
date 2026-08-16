qt_add_executable(NVT-widgets
    WIN32 MACOSX_BUNDLE
    nvt.hpp
    log.hpp
    log.cpp
    mainwindow.hpp
    mainwindow.cpp
    application.hpp
    application.cpp
    main.cpp
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
