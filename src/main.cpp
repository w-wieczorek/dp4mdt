#include "read_data.h"
#include <iostream>
#include <CLI/CLI.hpp>

int main(int argc, char** argv) {
    CLI::App app{"Generates decision tree based on a given decision table."};
    std::string tre_filename;
    std::string test_filename;
    app.add_option("-t,--tre", tre_filename, "Csv file for training data")
        ->required();
    app.add_option("-e,--test", test_filename, "Csv file for test data")
        ->required();
    CLI11_PARSE(app, argc, argv);
    Table tre_data(tre_filename);
    Table test_data(test_filename);
    std::cout << "Training data has " << tre_data.data.size() << " rows." << std::endl;
    for (const auto& row : tre_data.data) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Test data has " << test_data.data.size() << " rows." << std::endl;
    for (const auto& row : test_data.data) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}