#include "mts_ilp.h"
#include "spdlog/spdlog.h"
#include <set>
#include <map>
#include <scippp/model.hpp>
#include <scippp/parameters.hpp>

using namespace scippp;

struct TestComparator {
    bool operator()(const test& lhs, const test& rhs) const {
        if (lhs.a != rhs.a) return lhs.a < rhs.a;
        return lhs.v < rhs.v;
    }
};

void input_all_tests(const Table& dt, std::set<test, TestComparator>& all_tests) {
    int num_attributes = dt.data[0].size() - 1;
    for (int a = 0; a < num_attributes; ++a) {
        std::set<int> values;
        for (const auto& row : dt.data) {
            values.insert(row[a]);
        }
        for (const auto& v : values) {
            test t{a, v};
            all_tests.insert(t);
        }
    }
}

immer::set<test> build_and_solve_ilp_model(const Table& dt, bool debug_mode) {
    spdlog::debug("Building ILP model...");
    Model model("ilp4mts");
    model.setParam(params::LIMITS::MAXSOL, 1);
    if (debug_mode)
        model.setParam(params::DISPLAY::VERBLEVEL, 4);
    else
        model.setParam(params::DISPLAY::VERBLEVEL, 0);
    std::set<test, TestComparator> all_tests;
    input_all_tests(dt, all_tests);
    spdlog::info("Total number of tests: {0:d}", all_tests.size());
    std::map<test, Var, TestComparator> z;
    for (const auto& t : all_tests) {
        std::string name = "z_" + std::to_string(t.a) + "_" + std::to_string(t.v);
        z[t] = model.addVar(name, 1.0, VarType::BINARY);
    }
    spdlog::debug("Creating inequalities...");
    for (int u = 0; u < dt.data.size() - 1; ++u) {
        for (int w = u + 1; w < dt.data.size(); ++w) {
            if (dt.data[u].back() != dt.data[w].back()) {
                LinExpr lhs;
                for (int j = 0; j < dt.data[0].size() - 1; ++j) {
                    if (dt.data[u][j] != dt.data[w][j]) {
                        test tu{j, dt.data[u][j]};
                        lhs += z[tu];
                        test tw{j, dt.data[w][j]};
                        lhs += z[tw];
                    }
                }
                model.addConstr(lhs >= 1.0, "sep_" + std::to_string(u) + "_" + std::to_string(w));
            }
        }   
    }
    model.setObjsense(Sense::MINIMIZE);
    spdlog::debug("Optimizing ILP model...");
    model.solve();
    immer::set<test> result;
    if (model.getNSols() > 0 && model.getStatus() == SCIP_STATUS_OPTIMAL) {
        Solution sol { model.getBestSol() };
        for (const auto& t : all_tests) {
            if (z[t].getSolValAsInt(sol) == 1) {
                result = result.insert(t);
            }
        }
        spdlog::info("Optimal solution found with {0:d} tests.", result.size());
    }
    else {
        spdlog::error("No optimal solution found.");
    }
    return result;
}