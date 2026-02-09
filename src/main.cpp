#include "read_data.h"
#include "mts_ilp.h"
#include "mdt_dp.h"
#include "spdlog/spdlog.h"
#include <CLI/CLI.hpp>
#include <iostream>
#include <vector>

void print_classification_results(node* decision_tree, const Table& test_data) {
    int n_observations = static_cast<int>(test_data.data.size());
    std::vector<int> targets(n_observations);
    std::vector<int> outputs(n_observations);
    for (int i = 0; i < n_observations; ++i) {
        int predicted = classify(decision_tree, test_data, i);
        int actual = test_data.data[i].back();
        targets[i] = actual;
        outputs[i] = predicted;
    }
    spdlog::info("Targets:");
    std::cout << "[ ";
    for (int c : targets) {
        std::cout << c << ", ";
    }
    std::cout << "]\n";
    spdlog::info("Outputs:");
    std::cout << "[ ";
    for (int c : outputs) {
        std::cout << c << ", ";
    }
    std::cout << "]\n";
}

int main(int argc, char** argv) {
    CLI::App app{"Generates decision tree based on a given decision table."};
    std::string tre_filename;
    std::string test_filename;
    std::string verbosity;
    app.add_option("-t,--tre", tre_filename, "Csv file for training data")
        ->required();
    app.add_option("-e,--test", test_filename, "Csv file for test data")
        ->required();
    app.add_option("-v,--verbosity", verbosity, "Logging verbosity level (debug, info)")
        ->default_val("info");
    CLI11_PARSE(app, argc, argv);
    bool debug_mode = (verbosity == "debug");
    if (debug_mode)
        spdlog::set_level(spdlog::level::debug);
    else
        spdlog::set_level(spdlog::level::info);
    Table tre_data(tre_filename);
    Table test_data(test_filename);
    spdlog::info("Training data has {0:d} rows", tre_data.data.size());
    spdlog::info("Test data has {0:d} rows", test_data.data.size());
    immer::set<test> selected_tests = build_and_solve_ilp_model(tre_data, debug_mode);
    node* decision_tree = build_decision_tree(tre_data, selected_tests);
    if (debug_mode) print_tree(decision_tree, 0);
    spdlog::info("Decision tree height: {0:d}", height(decision_tree));
    spdlog::info("Decision tree size: {0:d}", tree_size(decision_tree));
    print_classification_results(decision_tree, test_data);
    free_tree(decision_tree);
    return 0;
}