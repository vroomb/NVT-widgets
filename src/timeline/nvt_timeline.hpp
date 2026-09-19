#pragma once

#if defined(TIMELINELIB)
#  define TIMELINE_API __declspec(dllexport)
#else
#  define TIMELINE_API __declspec(dllimport)
#endif

#include <project/nvt.hpp>

namespace nvt {
    namespace timeline {
        class log;
        class circle;

        class node;
        class chain;

        class graph;
    }
}

//  round(pos.x() - ox, tile->width()) + ox
//  b * ((int)((a - (a2 - (int)((a2 / b2) + 1) * b)) / b) + 0.5)

inline double modulo(double a, double b) {
    return a - (int)((a / b) + 1) * b;
}

inline double round(double a, double b) {
    return b * ((int)(a / b) + 0.5);
}
