#include "include.h"
#include "Database.h"
#include "Parser.h"

int main() {
    string line;
    Parser parser;
    Database db;

    while(getline(cin, line)) {
        parser.execute(line,db);
    }
}
