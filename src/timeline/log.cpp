#include <timeline/log.hpp>
#include <fstream>
#include <format>
#include <string>
#include <iostream>

thread_local int nvt::timeline::log::log_level = -1;

nvt::timeline::log::log() {
    init_log_level = log_level;
    log_level++;
}

nvt::timeline::log::~log() {
    log_level = init_log_level;
}

std::error_code nvt::timeline::log::operator()(
    const char * str,
    const int max_log_level,
    const std::source_location location
) {
    std::error_code ec{};
    std::string s(((log_level > 0) ? (log_level * 4) : 0), ' ');

    if (log_level < max_log_level || max_log_level == 0) {
        s = std::format(
            "{:35}: {:90}: {}\n",
            std::format("{}({:3}:{:3})",
                std::string{ location.file_name() }.substr(sizeof(src_dir) / sizeof(char) - 1),
                location.line(), location.column()
            ),
            s + location.function_name(), str
        );
        std::ofstream o{ "log.txt", std::ios::out | std::ios::app };

        if (o) o << s;
        else return std::make_error_code(std::errc(errno));
        std::cout << s;
    }

    return ec;
}

int nvt::timeline::log::operator++() {
    return log_level++;
}

int nvt::timeline::log::operator--() {
    return log_level--;
}
