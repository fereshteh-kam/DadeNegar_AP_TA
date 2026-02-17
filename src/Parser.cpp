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

    // else if(line.rfind("drop_table",0) == 0)
    //     parseDropTable(line, db);

    // else if(line.rfind("insert into",0) == 0)
    //     parseInsert(line, db);

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