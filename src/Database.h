#pragma once
#include "Table.h"
#include "include.h"

class Database {
public:
    unordered_map<string, Table> tables;

    bool createTable(string name, vector<Field> fields, bool enhanced, string& msg);
    bool dropTable(string name, string& msg);
    bool insertInfo(string tableName, Record record, string& msg);
};
