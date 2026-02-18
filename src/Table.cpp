#include "Table.h"
#include "Constants.h"
bool compareValues(const Value& v, const string& op, const Value& cond)
{
    if(v.index() == 0) 
    {
        int a = get<int>(v);
        int b = get<int>(cond);

        if(op == "=")  return a == b;
        if(op == "!=") return a != b;
        if(op == "<")  return a < b;
    }
    else 
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
                error = MSG::ERR_REQUIRED_MISSING;
                return false;
            }
        }
    }

    if(isEnhanced){
        for (const Field& f : fields) {
            if (f.required){
                for (const Record& r : records) {
                    if (r.values.find(f.name) != r.values.end() &&
                        rec.values.at(f.name) == r.values.at(f.name)) {
                        error = MSG::ERR_DUPLICATE_REQUIRED;
                        return false;
                    }
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

bool Table::selectRecords(vector<string> requestedFields,
                          string searchField, string op, string valueStr,
                          string& msg)
{
    vector<Record> result;

    FieldType searchType = FieldType::STRING;
    for(const Field& f : fields)
        if(f.name == searchField)
            searchType = f.type;

    Value searchValue = (searchType == FieldType::INT)
                        ? Value(stoi(valueStr))
                        : Value(valueStr);

    for(const Record& rec : records)
    {
        if(rec.values.find(searchField) == rec.values.end())
            continue;

        if(compareValues(rec.values.at(searchField), op, searchValue))
            result.push_back(rec);
    }

    if(result.empty()){
        msg = MSG::ERR_NO_MATCHING_RECORD;
        return false;
    }

    if(isEnhanced)
    {
        sort(result.begin(), result.end(),
             [&](const Record& a, const Record& b){
                const Value& va = a.values.at(requiredFieldName);
                const Value& vb = b.values.at(requiredFieldName);

                if(va.index()==0)
                    return get<int>(va) < get<int>(vb);
                else
                    return get<string>(va) < get<string>(vb);
             });
    }

    int idx = 1;
    for(const Record& rec : result)
    {
        const Value& keyVal = rec.values.at(searchField);

        cout << idx++ << ". ";

        for(int i=0;i<requestedFields.size();i++)
        {
            const Value& v = rec.values.at(requestedFields[i]);

            if(v.index()==0) cout << get<int>(v);
            else cout << get<string>(v);

            if(i != requestedFields.size()-1)
                cout << " : ";
        }

        cout << KW::NEXT_LINE;
    }

    msg = "";
    return true;
}
