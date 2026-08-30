#include "project/project.hpp"

namespace cr = std::chrono;
using time_point = cr::time_point<cr::steady_clock>;
using seconds = cr::seconds;

// nvt::abstract_story_entry::entry_type nvt::abstract_story_entry::discern_type(fs::path location) {
// 
// }

nvt::core::story_entry::story_entry(fs::path location) :
    m_location{location}
{
    if (fs::is_regular_file(m_location)) read();
    else if (fs::exists(m_location))
        throw std::runtime_error("story entry exists, but is not a regular file??");
    else write();
}

std::error_code nvt::core::story_entry::write(std::string file_contents) {
    return std::error_code{};
}

std::error_code nvt::core::story_entry::read() {
    return std::error_code{};
}
