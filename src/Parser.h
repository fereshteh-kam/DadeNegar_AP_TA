#pragma once
#include "include.h"
#include "Database.h"

class Parser {
public:
    void execute(const string& line, Database& db);

private:
    void parseCreateTable(const string& line, Database& db, bool enhanced);
    void parseDropTable(const string& line, Database& db);
    void parseInsert(const string& line, Database& db);
    void parseSelect(const string& line, Database& db);
    void parseUpdate(const string& line, Database& db);
};
