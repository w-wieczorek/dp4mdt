#include "read_data.h"
#include <lazycsv.hpp>
#include <vector>  

Table::Table(std::string filename) {
    lazycsv::parser<
        lazycsv::mmap_source,           /* source type of csv data */
        lazycsv::has_header<false>,     /* first row is header or not */
        lazycsv::delimiter<','>,        /* column delimiter */
        lazycsv::quote_char<'"'>,       /* quote character */
        lazycsv::trim_chars<' ', '\t'>> /* trim characters of cells */
        parser{ filename };
    for (const auto row : parser) {
        std::vector<int> row_data;
        for (const auto cell : row) {
            row_data.push_back(std::stoi(std::string(cell.raw())));
        }        
        data.push_back(row_data);
    }
}

int Table::num_classes(const immer::set<int>& indices) const {
    immer::set<int> classes;
    for (const auto& index : indices) {
        int class_label = data[index].back();
        classes = classes.insert(class_label);
    }
    return classes.size();
}

immer::set<int> Table::select_eq(test t, immer::set<int>& indices) const {
    immer::set<int> selected;
    for (const auto& index : indices) {
        if (data[index][t.a] == t.v) {
            selected = selected.insert(index);
        }
    }
    return selected;
}
