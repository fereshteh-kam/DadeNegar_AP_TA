#include "Table.h"

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
