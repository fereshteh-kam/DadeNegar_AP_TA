#pragma once
#include "Field.h"
#include "Record.h"
#include "include.h"

class Table {

public:
    string name;
    vector<Field> fields;
    vector<Record> records;

    bool isEnhanced = false;
    string requiredFieldName;

    unordered_map<int,int> uniqueIntIndex;
    unordered_map<string,int> uniqueStringIndex;

    Table() = default;
    Table(string n) : name(n) {}

    bool insertRecord(const Record& rec, string& error);
    void updateRecords(string searchField, string op, string valueStr, string updateField, string newValueStr);

};
