#pragma once

#include "dtree.h"
#include "read_data.h"
#include "mts_types.h"

node* build_decision_tree(const Table& dtable, const immer::set<test>& selected_tests);
void proba_cache();