#include "Parser.h"
#include "Database.h"
#include "include.h"

using namespace std;

void Parser::execute(const string& line, Database& db)
{
    cout << line;
    if(line.rfind("create_table",0) == 0)
        parseCreateTable(line, db, false);

    else if(line.rfind("create_enhanced_table",0) == 0)
        parseCreateTable(line, db, true);

    else if(line.rfind("drop_table",0) == 0)
        parseDropTable(line, db);

    else if(line.rfind("insert into",0) == 0)
        parseInsert(line, db);

    else if(line.rfind("select",0) == 0)
        parseSelect(line, db);

    else if(line.rfind("update",0) == 0)
        parseUpdate(line, db);
}
