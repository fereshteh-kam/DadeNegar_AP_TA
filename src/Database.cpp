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
