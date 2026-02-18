#include "Database.h"
#include "Constants.h"

bool Database::createTable(string name, vector<Field> fields, bool enhanced, string& msg) {
    if(tables.count(name)) {
        msg = MSG::ERR_TABLE_EXISTS;
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
    msg = MSG::TABLE_CREATED_1 + name + MSG::TABLE_CREATED_2;
    return true;
}

bool Database::dropTable(string name, string& msg) {
    if(!tables.count(name)) {
        msg = MSG::ERR_TABLE_NOT_FOUND_1 + name + MSG::ERR_TABLE_NOT_FOUND_2;
        return false;
    }

    tables.erase(name);
    msg = MSG::TABLE_DELETED_1 + name + MSG::TABLE_DELETED_2;
    return true;
}

bool Database::insertInfo(string tableName, Record record, string& msg){
    auto it = tables.find(tableName);
    if (it == tables.end()) {
        msg = MSG::ERR_TABLE_NOT_FOUND_1 + tableName + MSG::ERR_TABLE_NOT_FOUND_2;
        return false;
    }

    Table& table = it->second;

    if(table.insertRecord(record,msg)){
        msg = MSG::RECORD_INSERTED;
        return true;
    }
    return false;
}

bool Database::updateRecords(string tableName,string searchField, string op, string valueStr,
                            string updateField, string newValueStr,string& msg)
{
    auto it = tables.find(tableName);
    if(it == tables.end()){
        msg = MSG::ERR_TABLE_NOT_FOUND_1 + tableName + MSG::ERR_TABLE_NOT_FOUND_2;
        return false;
    }

    Table& table = it->second;
    table.updateRecords(searchField, op, valueStr, updateField, newValueStr);

    msg = MSG::RECORD_UPDATED;
    return true;
}

bool Database::selectRecords(string tableName,
                             vector<string> requestedFields,
                             string searchField, string op, string valueStr,
                             string& msg)
{
    auto it = tables.find(tableName);
    if(it == tables.end()){
        msg = MSG::ERR_TABLE_NOT_FOUND_1 + tableName + MSG::ERR_TABLE_NOT_FOUND_2;
        return false;
    }

    Table& table = it->second;
    return table.selectRecords(requestedFields, searchField, op, valueStr, msg);
}
