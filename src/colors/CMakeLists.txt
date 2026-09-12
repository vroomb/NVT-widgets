qt_add_library(colorlib STATIC
    nvt_colors.cpp
)

target_include_directories(colorlib PUBLIC ..)
# target_compile_definitions(colorlib PRIVATE COLORLIB)

set_target_properties(colorlib
    PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 23
)
target_link_libraries(colorlib
    PRIVATE
        Qt::Gui
)
