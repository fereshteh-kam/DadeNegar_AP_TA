#pragma once
#include "include.h"

enum class FieldType { INT, STRING };

struct Field {
    string name;
    FieldType type;
    bool required;
};
