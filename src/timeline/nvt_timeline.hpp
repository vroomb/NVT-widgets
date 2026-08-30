#pragma once

#if defined(TIMELINELIB)
#  define TIMELINE_API __declspec(dllexport)
#else
#  define TIMELINE_API __declspec(dllimport)
#endif

#include <project/nvt.hpp>

namespace nvt {
    namespace timeline {
        class node;
        class chain;

        class graph;
    }
}