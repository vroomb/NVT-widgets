qt_add_library(timelinelib
    graph.cpp
    node.cpp
    chain.cpp
    log.cpp
)

target_include_directories(timelinelib PUBLIC ..)
target_compile_definitions(timelinelib PRIVATE TIMELINELIB)

set_target_properties(timelinelib
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 23
)
target_link_libraries(timelinelib
    PRIVATE
        colorlib
        Qt::Core
        Qt::Widgets
)