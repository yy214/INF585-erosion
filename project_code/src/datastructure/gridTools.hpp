#pragma once

#include "cgp/cgp.hpp"
using namespace cgp;

int getIndex(int x, int y, int N);

int2 getCoord(int index, int N);

float& get_height(cgp::mesh& m, cgp::int2 pos, int N);
float get_height(cgp::mesh const& m, cgp::int2 pos, int N);

inline bool operator==(cgp::int2 const& a, cgp::int2 const& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(cgp::int2 const& a, cgp::int2 const& b) {
    return !(a == b);
}

inline bool lex_order(cgp::int2 const& a, cgp::int2 const& b) {
    if (a.x == b.x) return a.y < b.y;
    return a.x < b.x;
}

class lex_order_class {
public:
    inline bool operator()(cgp::int2 const& a, cgp::int2 const& b) const {
        return lex_order(a, b);
    }
};