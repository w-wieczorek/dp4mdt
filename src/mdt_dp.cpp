#include "mdt_dp.h"
#include "spdlog/spdlog.h"
#include <immer/set.hpp>
#include <vector>
#include <utility>
#include <unordered_map>

typedef struct _args {
    test t;
    immer::set<int> indices;
    bool operator==(const _args& rhs) const {
        return t == rhs.t && indices == rhs.indices;
    }
} args;

template<>
struct std::hash<immer::set<int>> {
    std::size_t operator()(const immer::set<int>& s) const noexcept {
        std::vector<int> vec(s.begin(), s.end());
        vec.sort();
        unsigned long long r = 11343719645984807545ull;
        unsigned long long h = 0;
        for (int i : vec) {
            r = 6364136223846793005ull * r + 1442695040888963407ull;
            h += r * static_cast<unsigned long long>(i);
        } 
        return h;
    }
};

template<>
struct hash<args> {
    std::size_t operator()(const args& a) const noexcept {
        std::size_t h1 = std::hash<int>()(a.t.a);
        std::size_t h2 = std::hash<int>()(a.t.v);
        std::size_t h3 = std::hash<immer::set<int>>()(a.indices);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

std::unordered_map<args, int> cache;

node* build_decision_tree(const Table& dtable, const immer::set<test>& selected_tests) {
    return nullptr;
}

void proba_cache() {
    args key{test{0, 0}, immer::set<int>{1, 2, 3}};
    cache[key] = 42;
    spdlog::info("Cache size: {0:d}\n", cache.size());
    for (const auto& [k, v] : cache) {
        spdlog::info("Key: (Attribute: {0:d}, Value: {1:d}), Indices size: {2:d} => Value: {3:d}\n",
                     k.t.a, k.t.v, k.indices.size(), v);
    }
    cache.clear();
}