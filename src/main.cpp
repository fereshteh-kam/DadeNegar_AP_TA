#include "include.h"


int main() {
    string line;

    while(getline(cin, line)) {
        cout << "Command received: " << line << endl;
    }
}
