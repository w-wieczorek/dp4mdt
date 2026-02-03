#include "read_data.h"
#include "mts_ilp.h"
#include "mdt_dp.h"
#include "spdlog/spdlog.h"
#include <CLI/CLI.hpp>

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
    spdlog::info("Selected tests:");
    for (const auto& t : selected_tests) {
        spdlog::info("Attribute: {0:d}, Value: {1:d}", t.a, t.v);
    }
    node* decision_tree = build_decision_tree(tre_data, selected_tests);
    return 0;
}