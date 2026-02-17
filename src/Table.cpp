#include "Table.h"

bool Table::insertRecord(const Record& rec, string& error) {
    records.push_back(rec);
    return true;
}
