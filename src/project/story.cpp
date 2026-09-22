#include "project/story.hpp"

#include <lmdb.h>

#include <random>
#include <iostream>

#ifdef    PROJECTLIB
nvt::core::story* nvt::core::story::m_instance = nullptr;
#endif // PROJECTLIB


int nvt::core::story::open(fs::path location) {
    if (m_instance != nullptr) {
        std::cout << "don't instantiate story twice.\n";
        return -1;
    }

    m_instance = new story(location);
    return 0;
}

nvt::core::story::story(fs::path location) :
    m_location{ location },
    m_config_dir{ location / ".nvt" }
{
    if (fs::directory_entry(m_config_dir).exists() == false) {
        std::error_code ec{};
        if (fs::create_directory(m_config_dir, ec) == false)
            std::cout << "story directory not created: " << ec << "\n";
    }

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
