// Kevin Burgos
// CS 280 – Programming Assignment 1
// Lexical Analyzer

#include <cctype>
#include <algorithm>
#include "lex.h"

using std::istream;
using std::string;

static map<Token, string> tokenPrint = {
    {PROGRAM, "PROGRAM"}, {WRITE, "WRITE"},   {INT, "INT"},     {END, "END"},
    {IF, "IF"},          {FLOAT, "FLOAT"},   {STRING, "STRING"}, {REPEAT, "REPEAT"},
    {BEGIN, "BEGIN"},    {IDENT, "IDENT"},   {ICONST, "ICONST"}, {RCONST, "RCONST"},
    {SCONST, "SCONST"},  {PLUS, "PLUS"},     {MINUS, "MINUS"},   {MULT, "MULT"},
    {DIV, "DIV"},        {REM, "REM"},       {ASSOP, "ASSOP"},   {LPAREN, "LPAREN"},
    {RPAREN, "RPAREN"},  {COMMA, "COMMA"},   {EQUAL, "EQUAL"},   {GTHAN, "GTHAN"},
    {SEMICOL, "SEMICOL"},{ERR, "ERR"},       {DONE, "DONE"}
};

// keywords are lowercase per spec
static map<string, Token> kwmap = {
    {"program", PROGRAM}, {"end", END},       {"begin", BEGIN}, {"write", WRITE},
    {"if", IF},          {"int", INT},       {"float", FLOAT}, {"string", STRING},
    {"repeat", REPEAT}
};

static inline bool isOp(char c) {
    return c=='+'||c=='-'||c=='*'||c=='/'||c=='%'||c=='='||c=='>';
}

LexItem id_or_kw(const string& lexeme, int lineNum) {
    string low = lexeme;
    std::transform(low.begin(), low.end(), low.begin(), [](unsigned char ch){return std::tolower(ch);} );
    auto it = kwmap.find(low);
    if (it != kwmap.end()) return LexItem(it->second, lexeme, lineNum);
    return LexItem(IDENT, lexeme, lineNum);
}

ostream& operator<<(ostream& out, const LexItem& tok) {
    Token t = tok.GetToken();
    if (t==IDENT || t==ICONST || t==RCONST || t==SCONST || t==ERR) {
        out << tokenPrint[t] << " (" << tok.GetLexeme() << ")";
    } else {
        out << tokenPrint[t];
    }
    return out;
}

LexItem getNextToken(istream& in, int& linenum) {
    char ch;

    // helper to finish single-char tokens
    auto single = [&](Token t, const string& lx) { return LexItem(t, lx, linenum); };

    // Skip whitespace and comments
    while (true) {
        int c = in.peek();
        if (c == EOF) return LexItem(DONE, "", linenum);
        if (c == '\n') { in.get(); ++linenum; continue; }
        if (std::isspace(c)) { in.get(); continue; }
        if (c == '#') { // comment to end of line
            while (in.get(ch)) { if (ch=='\n') { ++linenum; break; } }
            continue;
        }
        break; // non-space, non-comment
    }

    // Start tokenization
    in.get(ch);

    // Punctuation & operators
    switch (ch) {
        case '+': return single(PLUS, "+");
        case '-': return single(MINUS, "-");
        case '*': return single(MULT, "*");
        case '/': return single(DIV, "/");
        case '%': return single(REM, "%");
        case '(': return single(LPAREN, "(");
        case ')': return single(RPAREN, ")");
        case ',': return single(COMMA, ",");
        case ';': return single(SEMICOL, ";");
        case '>': return single(GTHAN, ">");
        case '=': {
            if (in.peek() == '=') { in.get(); return single(EQUAL, "=="); }
            return single(ASSOP, "=");
        }
        default: break; // fallthrough
    }

    // String literal: must start with double quote, end on same line
    if (ch == '"') {
        string s;
        while (true) {
            int c = in.get();
            if (c == EOF || c == '\n') {
                return LexItem(ERR, s, linenum); // unterminated string
            }
            if (c == '"') break; // end quote
            s.push_back(static_cast<char>(c));
        }
        return LexItem(SCONST, s, linenum);
    }

    // Identifier: Letter (Letter|Digit|_)*
    if (std::isalpha(static_cast<unsigned char>(ch))) {
        string lx(1, ch);
        while (true) {
            int c = in.peek();
            if (std::isalnum(c) || c == '_') { lx.push_back(static_cast<char>(in.get())); }
            else break;
        }
        return id_or_kw(lx, linenum);
    }

    // Number: integer or real
    if (std::isdigit(static_cast<unsigned char>(ch))) {
        string lx(1, ch);
        while (std::isdigit(in.peek())) lx.push_back(static_cast<char>(in.get()));
        if (in.peek() == '.') {
            // lookahead for at least one digit after '.'
            in.get(); // consume '.'
            if (!std::isdigit(in.peek())) {
                lx.push_back('.');
                return LexItem(ERR, lx, linenum); // e.g., 2.
            }
            lx.push_back('.');
            while (std::isdigit(in.peek())) lx.push_back(static_cast<char>(in.get()));
            return LexItem(RCONST, lx, linenum);
        }
        return LexItem(ICONST, lx, linenum);
    }

    // Real that starts with '.' (e.g., .25)
    if (ch == '.') {
        if (!std::isdigit(in.peek())) {
            string e(1, '.');
            return LexItem(ERR, e, linenum);
        }
        string lx = ".";
        while (std::isdigit(in.peek())) lx.push_back(static_cast<char>(in.get()));
        return LexItem(RCONST, lx, linenum);
    }

    // Any other single character not recognized is an error
    string bad(1, ch);
    return LexItem(ERR, bad, linenum);
}