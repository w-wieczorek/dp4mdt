#include "read_data.h"
#include "mts_ilp.h"
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
    if (verbosity == "debug")
        spdlog::set_level(spdlog::level::debug);
    else
        spdlog::set_level(spdlog::level::info);
    Table tre_data(tre_filename);
    Table test_data(test_filename);
    spdlog::info("Training data has {0:d} rows\n", tre_data.data.size());
    for (const auto& row : tre_data.data) {
        for (const auto& cell : row) {
            spdlog::info("{0:d}", cell);
        }
        spdlog::info("\n");
    }
    spdlog::info("Test data has {0:d} rows\n", test_data.data.size());
    for (const auto& row : test_data.data) {
        for (const auto& cell : row) {
            spdlog::info("{0:d}", cell);
        }
        spdlog::info("\n");
    }
    immer::set<test> selected_tests = build_and_solve_ilp_model(tre_data, true);
    spdlog::info("Selected tests:\n");
    for (const auto& t : selected_tests) {
        spdlog::info("Attribute: {0:d}, Value: {1:d}\n", t.a, t.v);
    }
    return 0;
}