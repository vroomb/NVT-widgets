#pragma once

#include "project.hpp"
#include <unordered_map>

class PROJECT_API nvt::story {
public:
    static int open(fs::path location);
    static int close();
    static story* instance();

    bool has_errors();
    std::string error_message();

    std::string name();
    void setName(std::string name);

    fs::path location();

private:
    story(fs::path location);
    ~story();

    void add_error(std::string message);

    int setLocation(fs::path location);

    static story* m_instance;

    std::string m_name;
    fs::path m_location;
    fs::path m_config_dir;

    std::optional<std::string> err = std::nullopt;
};