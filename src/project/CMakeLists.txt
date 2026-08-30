include("C:/Program Files/Microsoft Visual Studio/18/Community/VC/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(yaml-cpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-lmdb CONFIG REQUIRED)

add_library(projectlib SHARED
    "project.cpp"
    "global.cpp"
    "story.cpp"
    "log.cpp"
)

target_include_directories(projectlib PUBLIC ..)
target_compile_definitions(projectlib PRIVATE PROJECTLIB)
set_target_properties(projectlib PROPERTIES CXX_STANDARD 23)
target_link_libraries(projectlib
    PUBLIC
        unofficial::lmdb::lmdb
        yaml-cpp::yaml-cpp
        nlohmann_json::nlohmann_json
)
