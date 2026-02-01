#pragma once

#include <functional>

using attribute = int;
using value = int;

typedef struct _test{
    attribute a;
    value v;
    bool operator==(const _test& rhs) const {
        return a == rhs.a && v == rhs.v;
    }
} test;

template<>
struct std::hash<test> {
    std::size_t operator()(const test& t) const noexcept {
        unsigned long long h = 7634518198605363719ull * static_cast<unsigned long long>(t.a);
        h += 459136724432927870ull * static_cast<unsigned long long>(t.v);
        return h;
    }
};
