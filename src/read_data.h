#pragma once

#include <string>
#include <vector>
#include <immer/set.hpp>

using attribute = int;
using value = int;

class Table {
public:
    Table(std::string filename);
    ~Table() = default;
    int num_classes(const immer::set<int>& indices) const;
    immer::set<int> select_eq(attribute a, value v, immer::set<int>& indices) const;
    std::vector<std::vector<int>> data;
};
