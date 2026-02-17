#include "Database.h"

bool Database::createTable(string name, vector<Field> fields, bool enhanced, string& msg) {
    if(tables.count(name)) {
        msg = "Error: A table with this name already exists";
        return false;
    }

    Table t(name);
    t.fields = fields;
    t.isEnhanced = enhanced;

    if(enhanced) {
        for(auto &f : fields)
            if(f.required)
                t.requiredFieldName = f.name;
    }

    tables[name] = t;
    msg = "Success: Table " + name + " created successfully";
    return true;
}

bool Database::dropTable(string name, string& msg) {
    if(!tables.count(name)) {
        msg = "Error: Table " + name + " does not exist";
        return false;
    }

    tables.erase(name);
    msg = "Success: Table " + name + " deleted successfully";
    return true;
}

bool Database::insertInfo(string tableName, Record record, string& msg){
    auto it = tables.find(tableName);
    if (it == tables.end()) {
        msg =  "Error: Table " + tableName + " does not exist";
        return false;
    }

    Table& table = it->second;

    if(table.insertRecord(record,msg)){
        msg = "Success: Record inserted into table successfully";
        return true;
    }
    return false;
}

bool Database::updateRecords(string tableName,string searchField, string op, string valueStr,
                            string updateField, string newValueStr,string& msg)
{
    auto it = tables.find(tableName);
    if(it == tables.end()){
        msg = "Error: Table " + tableName + " does not exist";
        return false;
    }

    Table& table = it->second;
    table.updateRecords(searchField, op, valueStr, updateField, newValueStr);

    msg = "Success: Records updated in table";
    return true;
}

bool Database::selectRecords(string tableName,
                             vector<string> requestedFields,
                             string searchField, string op, string valueStr,
                             string& msg)
{
    auto it = tables.find(tableName);
    if(it == tables.end()){
        msg = "Error: Table " + tableName + " does not exist";
        return false;
    }

    Table& table = it->second;
    return table.selectRecords(requestedFields, searchField, op, valueStr, msg);
}
