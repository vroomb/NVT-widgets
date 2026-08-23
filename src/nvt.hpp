#pragma once

#include <system_error>

#define res_dir "H:/Projects/cpp/NVT-widgets/res/"
#define src_dir "H:/Projects/cpp/NVT-widgets/src/"

namespace nvt {
    class log;
    class application;
    class editor;
    class editor_status_bar;
    class proxy;
    class explorer;

    enum errc {
        ok = 0,
        cyclic_event_chain,
        max_log_level_exceeded,
    };

    const class error_category : public std::error_category {
    public:
        virtual const char* name() const noexcept {
            return "nvt";
        }

        virtual std::string message(int ev) const {
            switch (ev) {
            case ok:
                return "OK.";
            case cyclic_event_chain: 
                return "This operation will result in a cyclic event chain.";
            case max_log_level_exceeded: 
                return "Max log level exceeded, did not write to log.";
            default:
                return "Unknown error.";
            }
        }
    } nvt_error;
}
