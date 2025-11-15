# Lexical Analyzer (C++17)

This project is a **Lexical Analyzer** written in **C++17**, created as part of NJIT's CS280 (Programming Language Concepts) coursework. It scans an input source program and breaks it into tokens such as identifiers, keywords, constants, operators, and punctuation symbols.

This repository includes:

* A complete lexer implementation (`lex.cpp`)
* The driver program (`main.cpp`)
* Provided header (`lex.h`)
* A large set of **official test cases** used to validate the lexer
* A `.gitignore` and MIT license

---

## Purpose of the Project

A lexical analyzer is the first phase of a compiler/interpreter. Its main task is to:

* Read an input program character-by-character
* Classify sequences of characters into **tokens**
* Track lexical information (line numbers, lexemes, types)
* Report lexical errors (invalid numbers, unterminated strings, bad symbols)

This project demonstrates:

* Finite-state token scanning
* Error handling
* File I/O in C++
* Command-line flag parsing
* Using sets/maps for tracking unique identifiers and constants
* Clean code modularization with separate `.h` and `.cpp` files

---

## Project Structure

```
lexical-analyzer-cpp/
│
├── lex.cpp                # Lexical analyzer implementation
├── lex.h                  # Token types & LexItem class
├── main.cpp               # Driver with flag parsing and summary reporting
│
├── PA1_Test_Cases/        # Official provided test inputs
│     ├── prog1
│     ├── ints
│     ├── reals
│     ├── strings
│     ├── idents
│     └── ... (more tests)
│
├── .gitignore             # C++ ignore rules
├── LICENSE                # MIT license
└── README.md              # (This file)
```

---

## ⚙️ Compilation

Make sure you have **g++** installed. Then compile using:

```bash
g++ -std=c++17 lex.cpp main.cpp -o pa1
```

This produces an executable named `pa1`.

---

## Running the Analyzer

Basic usage:

```bash
./pa1 <inputfile>
```

To enable token printing:

```bash
./pa1 -v PA1_Test_Cases/prog1
```

### Supported Flags

| Flag        | Meaning                                     |
| ----------- | ------------------------------------------- |
| `-v`        | Verbose: print each token as it is read     |
| `-iconst`   | Print all integer constants (sorted)        |
| `-fconst`   | Print all floating‑point constants (sorted) |
| `-strconst` | Print all string constants (sorted)         |
| `-ident`    | Print all identifiers (sorted)              |

### Example

```bash
./pa1 -v -ident PA1_Test_Cases/prog1
```

---

## Testing With Provided Test Cases

The `PA1_Test_Cases` folder includes 20+ official tests.
Run them like:

```bash
./pa1 -v PA1_Test_Cases/prog1
./pa1 -iconst PA1_Test_Cases/ints
./pa1 -fconst PA1_Test_Cases/reals
./pa1 -strconst PA1_Test_Cases/strings
```

Error tests:

```bash
./pa1 PA1_Test_Cases/realerr
./pa1 PA1_Test_Cases/invsymbol
./pa1 PA1_Test_Cases/nofile
```

---

## Features Implemented

* Identification of:

  * Keywords
  * Identifiers
  * Integers, floats
  * String constants
  * Operators and delimiters
* Proper lexeme preservation
* Line number tracking
* Error detection for:

  * Invalid numbers (`2.`)
  * Unterminated strings
  * Invalid characters
* Sorted output sets for summary flags
* Exact output formatting matching assignment specification

---

## Skills Demonstrated

* C++17 Programming
* String parsing & tokenization
* Finite-state lexical logic
* Map/set usage
* Error handling
* File input/output operations
* Git version control
* WSL/Linux development
