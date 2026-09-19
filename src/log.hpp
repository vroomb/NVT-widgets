#pragma once

#include "nvt_widgets.hpp"
#include <QString>
#include <source_location>
#include <chrono>

namespace chr = std::chrono;

class nvt_widgets::log {
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

    inline std::error_code operator()(
        const QString& str,
        const int max_log_level = 10,
        const std::source_location location = std::source_location::current()
    ) { return operator()(str.toLocal8Bit().constData(), max_log_level, location); }

    inline std::error_code operator()(
        const QByteArray& str,
        const int max_log_level = 10,
        const std::source_location location = std::source_location::current()
    ) { return operator()(str.constData(), max_log_level, location); }

    int operator++();
    int operator--();

private:
    static thread_local int log_level;

    int init_log_level;
    const chr::time_point<chr::high_resolution_clock> tp;
};
