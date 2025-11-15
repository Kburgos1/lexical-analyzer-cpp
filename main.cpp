// Kevin Burgos
// CS 280 – Programming Assignment 1
// Driver / tester

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <set>
#include "lex.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Flag parsing
    if (argc == 1) {
        cerr << "NO SPECIFIED INPUT FILE NAME FOUND" << endl;
        return 1;
    }

    bool vFlag=false, iFlag=false, fFlag=false, sFlag=false, idFlag=false;
    string filename;
    bool sawFile=false;

    for (int i=1; i<argc; ++i) {
        string arg(argv[i]);
        if (!arg.empty() && arg[0] != '-') {
            if (sawFile) { cerr << "ONLY ONE FILE NAME ALLOWED" << endl; return 1; }
            filename = arg; sawFile = true; continue;
        }
        if (arg == "-v") vFlag = true;
        else if (arg == "-iconst") iFlag = true;
        else if (arg == "-fconst") fFlag = true;
        else if (arg == "-strconst") sFlag = true;
        else if (arg == "-ident") idFlag = true;
        else { cerr << "UNRECOGNIZED FLAG " << arg << endl; return 1; }
    }

    if (!sawFile) { cerr << "NO SPECIFIED INPUT FILE NAME FOUND" << endl; return 1; }

    ifstream fin(filename);
    if (!fin) { cerr << "CANNOT OPEN THE FILE " << filename << endl; return 1; }

    int linenum = 1;
    int tokenCount = 0;

    // unique collections
    set<string> idSet;        // identifiers (alpha order)
    set<string> strSet;       // strings (alpha order)
    set<long long> iSet;      // integers (numeric order)
    set<double> fSet;         // reals (numeric order)

    while (true) {
        LexItem tok = getNextToken(fin, linenum);
        if (tok == DONE) break;
        if (tok == ERR) {
            cerr << "Error in line " << tok.GetLinenum() << " (" << tok.GetLexeme() << ")" << endl;
            return 1;
        }
        ++tokenCount;

        if (vFlag) {
            cout << tok << "\n";  // operator<< prints with or without (lexeme)
        }

        switch (tok.GetToken()) {
            case IDENT:   idSet.insert(tok.GetLexeme()); break;
            case SCONST:  strSet.insert(tok.GetLexeme()); break;           // no quotes stored
            case ICONST:  try { iSet.insert(stoll(tok.GetLexeme())); } catch(...){} break;
            case RCONST:  try { fSet.insert(stod(tok.GetLexeme())); } catch(...){} break;
            default: break;
        }
    }

    // If file was totally empty, linenum counted first line; adjust per spec
    // (Spec prints only Lines: L; if L == 0, no further output.)
    // Our lexer keeps linenum on last seen line; to get line count, it's fine to print linenum.

    cout << "Lines: " << linenum << "\n";
    if (linenum > 0) cout << "Tokens: " << tokenCount << "\n";

    if (sFlag && !strSet.empty()) {
        cout << "STRINGS:" << "\n";
        for (const auto& s : strSet) cout << s << "\n";  // already alphabetical
    }

    if (iFlag && !iSet.empty()) {
        cout << "INTEGERS:" << "\n";
        for (auto v : iSet) cout << v << "\n";
    }

    if (fFlag && !fSet.empty()) {
        cout << "FLOATS:" << "\n";
        for (auto v : fSet) cout << v << "\n";
    }

    if (idFlag && !idSet.empty()) {
        cout << "IDENTIFIERS: ";
        for (auto it = idSet.begin(); it != idSet.end(); ) {
            cout << *it;
            if (++it != idSet.end()) cout << ", ";
        }
        cout << "\n";
    }

    return 0;
}