#pragma once

#include <nlohmann/json.hpp>
#include "project.hpp"

struct nvt::core::launch_details {
    std::string name{};
    fs::path location{};
    long long int last_updated;
};

const struct nvt::core::less_launch_details {
    bool operator()(const launch_details& lhs, const launch_details& rhs) const {
        return lhs.last_updated > rhs.last_updated;
    }
};

class PROJECT_API nvt::core::global {
public:
    static int open(fs::path location);
    static int close();
    static global* instance();

    std::set<launch_details, less_launch_details>* launches();
    int add_launch(launch_details ld, bool override = true);
    int rem_launch(std::string location);

    bool has_errors();
    std::string error_message();

private:
    global(fs::path location);
    ~global();

    static global* m_instance;
    fs::path m_location;

    std::set<launch_details, less_launch_details> m_launches{};

    nlohmann::json config_json;

    std::optional<std::string> err = std::nullopt;
};
