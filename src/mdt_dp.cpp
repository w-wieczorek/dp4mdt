#include "mdt_dp.h"
#include "spdlog/spdlog.h"
#include <immer/set.hpp>
#include <vector>
#include <utility>
#include <algorithm>
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
        std::sort(vec.begin(), vec.end());
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
struct std::hash<args> {
    std::size_t operator()(const args& a) const noexcept {
        std::size_t h1 = std::hash<int>()(a.t.a);
        std::size_t h2 = std::hash<int>()(a.t.v);
        std::size_t h3 = std::hash<immer::set<int>>()(a.indices);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

std::unordered_map<args, int> cache;

int m(test t, immer::set<int> indices, const Table& dtable, const immer::set<test>& selected_tests) {
    args a{t, indices};
    auto it = cache.find(a);
    if (it != cache.end()) {
        return it->second;
    }
    int result = INT32_MAX;
    if (dtable.num_classes(indices) == 1) {
        result = 1;
    } else {
        immer::set<int> right_indices = dtable.select_eq(t, indices);
        if (right_indices.size() == 0) {
            goto label;
        }
        immer::set<int> left_indices = indices;
        for (int idx : right_indices) {
            left_indices = left_indices.erase(idx);
        }
        if (left_indices.size() == 0) {
            goto label;
        }
        int best_left = INT32_MAX;
        for (const test& st : selected_tests) {
            if (st == t) {
                continue;
            }
            int score = m(st, left_indices, dtable, selected_tests);
            if (score < best_left) {
                best_left = score;
            }
        }
        int best_right = INT32_MAX;
        for (const test& st : selected_tests) {
            if (st == t) {
                continue;
            }
            int score = m(st, right_indices, dtable, selected_tests);
            if (score < best_right) {
                best_right = score;
            }
        }
        result = 1 + best_left + best_right;
    }
    label:
    cache[a] = result;
    return result;
}

node* reconstruct_tree(const args& a, const Table& dtable, const immer::set<test>& selected_tests) {
    if (cache[a] == 1) {
        int class_label = dtable.data[a.indices.begin()].back();
        return new node{class_label, nullptr, nullptr};
    }
    immer::set<int> right_indices = a.indices;
    for (int idx : a.indices) {
        if (a.t.v != dtable.data[idx][a.t.a]) {
            right_indices = right_indices.erase(idx);
        }
    }
    immer::set<int> left_indices = a.indices;
    for (int idx : right_indices) {
        left_indices = left_indices.erase(idx);
    }
    args best_left_args;
    int best_left_score = INT32_MAX;
    for (const test& st : selected_tests) {
        if (st == a.t) {
            continue;
        }
        int score = m(st, left_indices, dtable, selected_tests);
        if (score < best_left_score) {
            best_left_score = score;
            best_left_args = args{st, left_indices};
        }
    }
    args best_right_args;
    int best_right_score = INT32_MAX;
    for (const test& st : selected_tests) {
        if (st == a.t) {
            continue;
        }
        int score = m(st, right_indices, dtable, selected_tests);
        if (score < best_right_score) {
            best_right_score = score;
            best_right_args = args{st, right_indices};
        }
    }
    node* left_child = reconstruct_tree(best_left_args);
    node* right_child = reconstruct_tree(best_right_args);
    return new node{a.t, left_child, right_child};
}   

node* build_decision_tree(const Table& dtable, const immer::set<test>& selected_tests) {
    cache.clear();
    int best_score = INT32_MAX;
    args best_args;
    for (const test& st : selected_tests) {
        immer::set<int> all_indices;
        for (size_t i = 0; i < dtable.data.size(); ++i) {
            all_indices = all_indices.insert(static_cast<int>(i));
        }
        int score = m(st, all_indices, dtable, selected_tests);
        spdlog::debug("Test (a={}, v={}) has score {}", st.a, st.v, score);
        if (score < best_score) {
            best_score = score;
            best_args = args{st, all_indices};
        }
    }
    return reconstruct_tree(best_args, dtable, selected_tests);
}

