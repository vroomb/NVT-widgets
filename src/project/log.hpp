#pragma once

#include "project/nvt.hpp"
#include <source_location>

class nvt::log {
public:
    log();
    ~log();

    std::error_code operator()(
        const char * str,
        const int max_log_level = 10,
        const std::source_location location = std::source_location::current()
    );

    inline std::error_code operator()(
        const std::string& str,
        const int max_log_level = 10,
        const std::source_location location = std::source_location::current()
    ) { return operator()(str.c_str(), max_log_level, location); }

    inline std::error_code operator()(
        const std::error_code ev,
        const int max_log_level = 10,
        const std::source_location location = std::source_location::current()
    ) { return operator()(ev.message().c_str(), max_log_level, location); }

    int operator++();
    int operator--();

private:
    static thread_local int log_level;

    int init_log_level;
};