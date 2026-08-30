#pragma once

#include "nvt.hpp"
#include <lmdb.h>

#include <set>
#include <map>
#include <list>
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;

namespace mdb {
    const class error_category : public std::error_category {
    public:
        virtual const char* name() const noexcept {
            return "mdb";
        }

        virtual std::string message(int ev) const {
            return mdb_strerror(ev);
        }
    } mdb_error;
}

struct nvt::core::parse_result {
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

class nvt::core::story_entry {
public:
    virtual std::string name() {
        auto r = m_location.stem().string();
        return r;
    }

    const fs::path& location();
    std::error_code move_to(fs::path new_location);

    story_entry(fs::path location);

    virtual std::error_code read();

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
