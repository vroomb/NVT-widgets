#include "project/global.hpp"
#include <iostream>
#include <fstream>

#ifdef    PROJECTLIB
nvt::core::global* nvt::core::global::m_instance = nullptr;
#endif // PROJECTLIB

int nvt::core::global::open(fs::path location) {
    if (m_instance != nullptr) {
        std::cout << "don't instantiate global twice.\n";
        return -1;
    }

    m_instance = new global(location);
    return 0;
}

nvt::core::global::global(fs::path location) :
    m_location{ location }
{
    auto stream = std::ifstream(location.string() + "config.json");
    config_json = nlohmann::json::parse(stream);

    if (stream.is_open() == true) {

        auto j = config_json["projects"];

        if (j.is_array() == true) for (auto i : j) {
            if ((i["name"].is_string() == true) &&
                (i["location"].is_string() == true)) {
                if (i["last updated"].is_number_unsigned() == true)
                    m_launches.insert(nvt::core::launch_details{
                        i["name"],
                        i["location"],
                        i["last updated"]
                    });
                else
                    m_launches.insert(nvt::core::launch_details{
                        i["name"],
                        i["location"],
                        0
                    });
            }
        }
    }
}

nvt::core::global* nvt::core::global::instance() {
    return m_instance;
}

std::set<nvt::core::launch_details, nvt::core::less_launch_details>* nvt::core::global::launches() {
    return &m_launches;
}

[[nodiscard]]
int nvt::core::global::add_launch(launch_details ld, bool override) {
    rem_launch(ld.location.string());

    config_json["projects"] += {
        { "name",         ld.name         },
        { "location",     ld.location     },
        { "last updated", ld.last_updated }
    };

    m_launches.insert(ld);

    if (fs::directory_entry(ld.location).exists() == false) {
        std::error_code ec{};
        if (fs::create_directory(ld.location, ec) == false)
            std::cout << "story directory not created: " << ec << "\n";
    }

    std::ofstream(m_location / "config.json",
        std::ios::out | std::ios::trunc) << config_json.dump(2);

    return 0;
}

[[nodiscard]]
int nvt::core::global::rem_launch(std::string location) {
    for (auto& i : config_json["projects"]) if (i["location"] == location)
            i = nullptr;

    std::ofstream(m_location / "config.json",
        std::ios::out | std::ios::trunc) << config_json.dump(2);
    return 0;
}

int nvt::core::global::close() {
    if (m_instance == nullptr) {
        return -1;
    } else {
        delete m_instance;
        return 0;
    }
}

nvt::core::global::~global() {}

