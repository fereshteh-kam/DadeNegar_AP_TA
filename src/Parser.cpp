#include "Parser.h"
#include "Database.h"
#include "include.h"
#include "Constants.h"

using namespace std;

void Parser::execute(const string& line, Database& db)
{
    if(line.rfind(KW::CREATE_TABLE,0) == 0)
        parseCreateTable(line, db, false);

    else if(line.rfind(KW::CREATE_ENHANCED_TABLE,0) == 0)
        parseCreateTable(line, db, true);

    else if(line.rfind(KW::DROP_TABLE,0) == 0)
        parseDropTable(line, db);

    else if(line.rfind(KW::INSERT_INTO,0) == 0)
        parseInsert(line, db);

    else if(line.rfind(KW::SELECT,0) == 0)
        parseSelect(line, db);

    else if(line.rfind(KW::UPDATE,0) == 0)
        parseUpdate(line, db);
}

void Parser::parseCreateTable(const string& line, Database& db, bool enhanced) {
    istringstream ss(line);
    string token;
    ss >> token; 

    string tableName;
    ss >> tableName;

    string fieldsStr;
    getline(ss, fieldsStr);
    
    unordered_set<string> fieldNames;
    vector<Field> fields;

    size_t start = 0;
    while (start < fieldsStr.size()) {
        size_t end = fieldsStr.find(KW::FIELD_SEPARATOR, start);
        string fieldDef = fieldsStr.substr(start, end - start);

        fieldDef.erase(0, fieldDef.find_first_not_of(" \t"));
        fieldDef.erase(fieldDef.find_last_not_of(" \t") + 1);

        if (!fieldDef.empty()) {
            istringstream fss(fieldDef);
            Field f;
            string typeStr, requiredStr;

            fss >> f.name >> typeStr >> requiredStr;
                
            if (fieldNames.find(f.name) != fieldNames.end()) {
                cout << MSG::ERR_DUPLICATE_FIELD << KW::NEXT_LINE;
                return;
            }
            fieldNames.insert(f.name);

            transform(typeStr.begin(), typeStr.end(), typeStr.begin(), ::tolower);
            if (typeStr == KW::TYPE_INT) f.type = FieldType::INT;
            else if (typeStr == KW::TYPE_STRING) f.type = FieldType::STRING;
            else {
                return;
            }

            transform(requiredStr.begin(), requiredStr.end(), requiredStr.begin(), ::tolower);
            if (requiredStr == KW::REQUIRED) f.required = true;
            else if (requiredStr == KW::OPTIONAL) f.required = false;
            else {
                return;
            }

            fields.push_back(f);
        }

        if (end == string::npos) break;
        start = end + 1;
    }

    string msg;
    db.createTable(tableName, fields, enhanced, msg);
    cout << msg << KW::NEXT_LINE;
}

void Parser::parseDropTable(const string& line, Database& db){
    istringstream ss(line);
    string token;
    ss >> token;

    string tableName;
    ss >> tableName;

    string msg;
    db.dropTable(tableName,msg);
    cout << msg << KW::NEXT_LINE;
}

void Parser::parseInsert(const string& line, Database& db){
    istringstream ss(line);
    string token;
    ss >> token >> token;

    string tableName;
    ss >> tableName;
    auto it = db.tables.find(tableName);
    if (it == db.tables.end()) {
        cout<< MSG::ERR_TABLE_NOT_FOUND_1 + tableName + MSG::ERR_TABLE_NOT_FOUND_2 << KW::NEXT_LINE;
        return;
    }
    Table& table = it->second;

    string fieldsLine;
    getline(ss, fieldsLine);
    if (fieldsLine.empty() || fieldsLine.find(KW::KEY_VALUE_SEPARATOR) == string::npos) {
        getline(cin, fieldsLine);
    }

    Record record;
    size_t start = 0;
    while (start < fieldsLine.size()) {
        size_t end = fieldsLine.find(KW::FIELD_SEPARATOR, start);
        string pair = fieldsLine.substr(start, end - start);

        pair.erase(0, pair.find_first_not_of(" \t"));
        pair.erase(pair.find_last_not_of(" \t") + 1);

        if (!pair.empty()) {
            size_t colon = pair.find(KW::KEY_VALUE_SEPARATOR);
            if (colon == string::npos) {
                start = (end == std::string::npos ? fieldsLine.size() : end + 1);
                continue;
            }

            std::string fieldName = pair.substr(0, colon);
            std::string valueStr = pair.substr(colon + 1);

            fieldName.erase(0, fieldName.find_first_not_of(" \t"));
            fieldName.erase(fieldName.find_last_not_of(" \t") + 1);
            valueStr.erase(0, valueStr.find_first_not_of(" \t"));
            valueStr.erase(valueStr.find_last_not_of(" \t") + 1);
            record.values[fieldName] = valueStr;
            
            auto fIt = std::find_if(table.fields.begin(), table.fields.end(),
                                    [&](const Field& f){ return f.name == fieldName; });
            if (fIt == table.fields.end()) {
                start = (end == std::string::npos ? fieldsLine.size() : end + 1);
                continue; // skip unknown fields
            }

            if (fIt->type == FieldType::INT) {
                try {
                    record.values[fieldName] = std::stoi(valueStr);
                } catch (...) {
                    // invalid int => treat as missing
                }
            } else {
                record.values[fieldName] = valueStr;
            }
        }

        if (end == std::string::npos) break;
        start = end + 1;
    }

    string msg;
    db.insertInfo(tableName,record,msg);
    cout << msg << KW::NEXT_LINE;
}

void Parser::parseUpdate(const string& line, Database& db)
{
    istringstream ss(line);
    string token;
    ss >> token; 

    string tableName;
    ss >> tableName;

    ss >> token;

    string searchField, op, valueStr;
    ss >> searchField >> op >> valueStr;

    ss >> token;

    string updateField, newValueStr;
    ss >> updateField >> newValueStr;

    string msg;
    db.updateRecords(tableName, searchField, op, valueStr,
                     updateField, newValueStr, msg);

    cout << msg << KW::NEXT_LINE;
}

void Parser::parseSelect(const string& line, Database& db)
{
    size_t fromPos  = line.find(" from ");
    size_t wherePos = line.find(" where ");

    string requestedFieldsStr = line.substr(7, fromPos - 7);
    string tableName = line.substr(fromPos + 6, wherePos - (fromPos + 6));
    string condition = line.substr(wherePos + 7);

    vector<string> requestedFields;
    string temp;
    stringstream fs(requestedFieldsStr);
    while(getline(fs, temp, KW::FIELD_SEPARATOR)){
        if(!temp.empty())
            requestedFields.push_back(temp);
    }

    string searchField, op, valueStr;
    stringstream cs(condition);
    cs >> searchField >> op >> valueStr;

    string msg;
    db.selectRecords(tableName, requestedFields,
                     searchField, op, valueStr, msg);

    if(!msg.empty())
        cout << msg << KW::NEXT_LINE;
}

