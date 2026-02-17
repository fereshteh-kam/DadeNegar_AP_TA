#pragma once
#include "include.h"

using Value = variant<int, string>;

struct Record {
    unordered_map<string, Value> values;
};
