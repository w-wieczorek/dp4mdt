#pragma once

#include "mts_types.h"
#include <string>
#include <vector>
#include <immer/set.hpp>

class Table {
public:
    Table(std::string filename);
    ~Table() = default;
    int num_classes(const immer::set<int>& indices) const;
    immer::set<int> select_eq(test t, immer::set<int>& indices) const;
    std::vector<std::vector<int>> data;
};
