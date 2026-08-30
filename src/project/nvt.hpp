#pragma once

#include <system_error>

#if defined(PROJECTLIB)
#  define PROJECT_API __declspec(dllexport)
#else
#  define PROJECT_API __declspec(dllimport)
#endif

#define res_dir "H:/Projects/cpp/NVT-widgets/res/"
#define src_dir "H:/Projects/cpp/NVT-widgets/src/"

typedef int epoch;
typedef int nid; // entity id
typedef int vid; // event id
typedef int tid; // timeline id
typedef int cid; // chain id
typedef int rid; // relation id
#define id_null 0

namespace nvt {
    class log;

    namespace core {
        struct parse_result;
        class story_entry;

        class property;
        class entity;

        class relation;
        struct linkage_element;
        class linkage;

        class event;
        class event_chain;

        class timeline;

        struct launch_details;
        struct less_launch_details;
        class global;

        class story;
    }

    template <typename T>
    inline T sgn(T r) {
        if (r > 0)       return 1;
        else if (r == 0) return 0;
        else             return -1;
    }
    
    template <typename T>
    inline T max(T a, T b) {
        if (a > b) return a;
        else       return b;
    }

    const class error_category : public std::error_category {
    public:
        enum errc {
            ok = 0,
            cyclic_event_chain,     // this operation will result in a cyclic event chain
            entry_exists,           // the story entry already exists
            ambiguous_link,         // the link is ambiguous
            max_log_level_exceeded, // max log level exceeded, did not write to log
        };

        virtual const char* name() const noexcept {
            return "nvt";
        }

        virtual std::string message(int ev) const {
            switch (ev) {
            case ok:
                return "OK";
            case cyclic_event_chain:
                return "this operation will result in a cyclic event chain";
            case entry_exists:
                return "the story entry already exists";
            case ambiguous_link:
                return "the link is ambiguous";
            case max_log_level_exceeded: 
                return "Max log level exceeded, did not write to log.";
            default:
                return "Unknown err";
            }
        }
    } nvt_error;
}
