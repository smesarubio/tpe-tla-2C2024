// #ifndef SYMBOL_TABLE_H
// #define SYMBOL_TABLE_H

// #define SYMBOL_TABLE_SIZE 100

// // Symbol Table Entry Structure
// typedef struct Symbol {
//     char *name;            // Name of the identifier
//     char *type;            // Type (e.g., int, string, etc.)
//     //char *scope;           // Scope (e.g., global, local)
//     //int initialized;       // Flag to indicate if initialized
//     void *value;
//     //struct Symbol *next;   // Pointer for handling collisions (chaining)
// } Symbol;

// // Symbol Table Structure (Hash Table)
// typedef struct SymbolTable {
//     //Symbol *table[SYMBOL_TABLE_SIZE];
//     Symbol table[SYMBOL_TABLE_SIZE];
// } SymbolTable;

// unsigned int hash(char *name);
// SymbolTable* createSymbolTable();
// void addSymbol(SymbolTable *symbolTable, char *name, char *type, char *scope, int initialized);
// Symbol* findSymbol(SymbolTable *symbolTable, char *name);
// void updateSymbol(SymbolTable *symbolTable, char *name, char *type, int initialized);
// void printSymbolTable(SymbolTable *symbolTable);
// void freeSymbolTable(SymbolTable *symbolTable);

// #endif

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

// Define the maximum number of symbols
#define SYMBOL_TABLE_SIZE 100

// Symbol structure
typedef struct Symbol {
    char *name;            // Identifier name
    char *type;            // Data type (e.g., int, float)
    void *value;           // Value pointer
} Symbol;

// Symbol table structure
typedef struct SymbolTable {
    Symbol table[SYMBOL_TABLE_SIZE]; // Array of symbols
    int count;                       // Current count of symbols
} SymbolTable;

// Function prototypes
SymbolTable* createSymbolTable();
void addSymbol(SymbolTable *symbolTable, char *name, char *type, void *value);
Symbol* findSymbol(SymbolTable *symbolTable, char *name);
void updateSymbol(SymbolTable *symbolTable, char *name, char *type, void *value);
void printSymbolTable(SymbolTable *symbolTable);
void freeSymbolTable(SymbolTable *symbolTable);

#endif // SYMBOL_TABLE_H
