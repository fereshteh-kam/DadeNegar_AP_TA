#include "Table.h"

bool compareValues(const Value& v, const string& op, const Value& cond)
{
    if(v.index() == 0) // int
    {
        int a = get<int>(v);
        int b = get<int>(cond);

        if(op == "=")  return a == b;
        if(op == "!=") return a != b;
        if(op == "<")  return a < b;
    }
    else // string
    {
        string a = get<string>(v);
        string b = get<string>(cond);

        if(op == "=")  return a == b;
        if(op == "!=") return a != b;
        if(op == "<")  return a < b;
    }

    return false;
}

bool Table::insertRecord(const Record& rec, string& error) {
    for (const Field& f : fields) {
        if (f.required) {
            if (rec.values.find(f.name) == rec.values.end()) {
                error = "Error: Missing value for required field";
                return false;
            }
        }
    }

    if(isEnhanced){
        for (const Field& f : fields) {
            if (f.required) 
                for (const Record& r : records) {
                    if (r.values.find(f.name) != r.values.end() &&
                        rec.values.at(f.name) == r.values.at(f.name)) {
                        error = "Error: Duplicate value for required field";
                        return false;
                    }
                }
            }
        }

    records.push_back(rec);
    return true;
}

void Table::updateRecords(string searchField, string op, string valueStr,
                          string updateField, string newValueStr)
{
    FieldType searchType = FieldType::STRING;
    FieldType updateType = FieldType::STRING;

    for(const Field& f : fields){
        if(f.name == searchField) searchType = f.type;
        if(f.name == updateField) updateType = f.type;
    }

    Value searchValue = (searchType == FieldType::INT)
                        ? Value(stoi(valueStr))
                        : Value(valueStr);

    Value newValue = (updateType == FieldType::INT)
                     ? Value(stoi(newValueStr))
                     : Value(newValueStr);

    for(Record& rec : records)
    {
        if(rec.values.find(searchField) == rec.values.end())
            continue;

        if(compareValues(rec.values[searchField], op, searchValue))
        {
            rec.values[updateField] = newValue;
        }
    }
}

