#pragma once

#include <nlohmann/json.hpp>

#include <set>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <variant>
#include <system_error>
#include <expected>
#include <lmdb.h>

namespace fs = std::filesystem;

#if defined(PROJECTLIB)
#  define PROJECT_API __declspec(dllexport)
#else
#  define PROJECT_API __declspec(dllimport)
#endif

namespace nvt {
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

    void log(std::string);

    enum errc {
        ok = 0,
        cyclic_event_chain, // this operation will result in a cyclic event chain
        entry_exists,       // the story entry already exists
        ambiguous_link,     // the link is ambiguous
    };

    class error_category : public std::error_category {
    public:
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
            default:
                return "Unknown err";
            }
        }
    } nvt_error;
}

namespace mdb {
    class error_category : public std::error_category {
    public:
        virtual const char* name() const noexcept {
            return "mdb";
        }

        virtual std::string message(int ev) const {
            return mdb_strerror(ev);
        }
    } mdb_error;
}

typedef int epoch;
typedef int nid; // entity id
typedef int vid; // event id
typedef int tid; // timeline id
typedef int cid; // chain id
typedef int rid; // relation id
#define id_null 0

struct nvt::parse_result {
    enum entry_type {
        unknown = 0,
        entity_entry,
        event_entry,
        timeline_entry,
        chain_entry,
        relation_entry
    };

    entry_type type();
    epoch when();
    std::map<std::string, std::string> properties();
    std::set<cid> chains();
    std::list<vid> events();
    nid chain_entity();
};

class nvt::story_entry {
public:
    virtual std::string name() {
        auto r = m_location.stem().string();
        return r;
    }

    const fs::path& location();
    std::error_code move_to(fs::path new_location);

    story_entry(fs::path location);

    virtual std::error_code read() {}

    virtual std::string gen_metadata() { return ""; }

    // take the file, take the metadata, merge the two.
    // if any conflicts arise in the existing metadata
    // and new metadata, replace the existing metadata
    // with new metadata
    // 
    // we will use the undefined properties as generics
    // 
    std::error_code write(std::string file_contents = "");

    //static entry_type PROJECT_API discern_type(fs::path location);

    static parse_result parse();

private:
    fs::path m_location;
};
