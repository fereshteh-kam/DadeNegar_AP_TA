#include "Parser.h"
#include "Database.h"
#include "include.h"

using namespace std;

void Parser::execute(const string& line, Database& db)
{
    if(line.rfind("create_table",0) == 0)
        parseCreateTable(line, db, false);

    else if(line.rfind("create_enhanced_table",0) == 0)
        parseCreateTable(line, db, true);

    else if(line.rfind("drop_table",0) == 0)
        parseDropTable(line, db);

    else if(line.rfind("insert into",0) == 0)
        parseInsert(line, db);

    // else if(line.rfind("select",0) == 0)
    //     parseSelect(line, db);

    // else if(line.rfind("update",0) == 0)
    //     parseUpdate(line, db);
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
        size_t end = fieldsStr.find(';', start);
        string fieldDef = fieldsStr.substr(start, end - start);

        fieldDef.erase(0, fieldDef.find_first_not_of(" \t"));
        fieldDef.erase(fieldDef.find_last_not_of(" \t") + 1);

        if (!fieldDef.empty()) {
            istringstream fss(fieldDef);
            Field f;
            string typeStr, requiredStr;

            if (!(fss >> f.name >> typeStr >> requiredStr)) {
                cout << "Error: Invalid field definition: " << fieldDef << "\n";
                return;
            }

            if (fieldNames.find(f.name) != fieldNames.end()) {
                cout << "Error: Duplicate field name in table definition\n";
                return;
            }
            fieldNames.insert(f.name);

            transform(typeStr.begin(), typeStr.end(), typeStr.begin(), ::tolower);
            if (typeStr == "int") f.type = FieldType::INT;
            else if (typeStr == "string") f.type = FieldType::STRING;
            else {
                cout << "Error: Unknown field type for field " << f.name << "\n";
                return;
            }

            transform(requiredStr.begin(), requiredStr.end(), requiredStr.begin(), ::tolower);
            if (requiredStr == "required") f.required = true;
            else if (requiredStr == "optional") f.required = false;
            else {
                cout << "Error: required/optional must be specified for field " << f.name << "\n";
                return;
            }

            fields.push_back(f);
        }

        if (end == string::npos) break;
        start = end + 1;
    }

    string msg;
    db.createTable(tableName, fields, enhanced, msg);
    cout << msg << "\n";
}

void Parser::parseDropTable(const string& line, Database& db){
    istringstream ss(line);
    string token;
    ss >> token;

    string tableName;
    ss >> tableName;

    string msg;
    db.dropTable(tableName,msg);
    cout << msg << "\n";
}

void Parser::parseInsert(const string& line, Database& db){
    istringstream ss(line);
    string token;
    ss >> token >> token;

    string tableName;
    ss >> tableName;
    auto it = db.tables.find(tableName);
    if (it == db.tables.end()) {
        cout<<  "Error: Table " << tableName << " does not exist\n";
        return;
    }
    Table& table = it->second;

    string fieldsLine;
    getline(ss, fieldsLine);
    if (fieldsLine.empty() || fieldsLine.find(':') == string::npos) {
        getline(cin, fieldsLine);
    }

    Record record;
    size_t start = 0;
    while (start < fieldsLine.size()) {
        size_t end = fieldsLine.find(';', start);
        string pair = fieldsLine.substr(start, end - start);

        pair.erase(0, pair.find_first_not_of(" \t"));
        pair.erase(pair.find_last_not_of(" \t") + 1);

        if (!pair.empty()) {
            size_t colon = pair.find(':');
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
    cout << msg << "\n";
}


