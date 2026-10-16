#include "project/story.hpp"
#include "log.hpp"

#include <lmdb.h>

#include <random>
#include <iostream>

#ifdef    PROJECTLIB
nvt::core::story* nvt::core::story::m_instance = nullptr;
#endif // PROJECTLIB


std::error_code nvt::core::story::open(fs::path location) {
    log log{};
    if (m_instance != nullptr) {
        std::cout << "don't instantiate story twice.\n";
        return std::make_error_code(std::errc::already_connected);
    }

    if (fs::directory_entry(location).exists() == false) {
        std::error_code ec{};
        if (fs::create_directory(location, ec) == false) {
            log("story directory not created");
            log(ec);
            return ec;
        }
    }

    m_instance = new story(location);
    return std::error_code{};
}

nvt::core::story::story(fs::path location) :
    m_location{ location },
    m_config_dir{ location / ".nvt" }
{
    m_name = location.filename().string();
}

nvt::core::story* nvt::core::story::instance() {
    return m_instance;
}

std::string nvt::core::story::name() {
    return m_name;
}

fs::path nvt::core::story::location() {
    return m_location;
}

int nvt::core::story::close() {
    if (m_instance == nullptr) {
        return -1;
    } else {
        delete m_instance;
        return 0;
    }
}

nvt::core::story::~story() {}
