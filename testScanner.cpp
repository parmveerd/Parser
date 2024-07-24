
#include "Scanner.h"

#include <iostream>
using namespace std;

main () {
    Scanner S(cin);
    Token t;
    t = S.getnext();
    while (t.tt != eof) {
        cout << t.line << ": " << t.tt << " " << t.text << endl;
        t = S.getnext();
    }
}

