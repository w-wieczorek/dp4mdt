#pragma once

#include "mts_types.h"
#include "read_data.h"

immer::set<test> build_and_solve_ilp_model(const Table& dt, bool debug_mode);
