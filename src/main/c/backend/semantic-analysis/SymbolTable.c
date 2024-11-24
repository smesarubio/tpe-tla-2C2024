// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <SymbolTable.h>
// // Define the maximum size for the symbol table


// // Hash function for the symbol table
// unsigned int hash(char *name) {
//     unsigned int hash_value = 0;
//     while (*name) {
//         hash_value = (hash_value << 5) + *name++;
//     }
//     return hash_value % SYMBOL_TABLE_SIZE;
// }

// // Create a new symbol table
// SymbolTable* createSymbolTable() {
//     SymbolTable *symbolTable = (SymbolTable *)malloc(sizeof(SymbolTable));
//     for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
//         symbolTable->table[i] = NULL;
//     }
//     return symbolTable;
// }

// // Add a symbol to the symbol table
// void addSymbol(SymbolTable *symbolTable, char *name, char *type, char *scope, int initialized) {
//     unsigned int index = hash(name);
//     Symbol *newSymbol = (Symbol *)malloc(sizeof(Symbol));
//     newSymbol->name = strdup(name);
//     newSymbol->type = strdup(type);
//     //newSymbol->scope = strdup(scope);
//     //newSymbol->initialized = initialized;
//     //newSymbol->next = symbolTable->table[index];
//     symbolTable->table[index] = newSymbol;
//     printf("Added symbol: %s, Type: %s, Scope: %s\\n", name, type, scope);
// }

// // Find a symbol in the symbol table
// Symbol* findSymbol(SymbolTable *symbolTable, char *name) {
//     unsigned int index = hash(name);
//     Symbol *current = symbolTable->table[index];
//     while (current) {
//         if (strcmp(current->name, name) == 0) {
//             return current;
//         }
//         current = current->next;
//     }
//     return NULL;
// }

// // Update a symbol in the symbol table
// void updateSymbol(SymbolTable *symbolTable, char *name, char *type, int initialized) {
//     Symbol *symbol = findSymbol(symbolTable, name);
//     if (symbol) {
//         if (type) {
//             free(symbol->type);
//             symbol->type = strdup(type);
//         }
//         symbol->initialized = initialized;
//         printf("Updated symbol: %s, New Type: %s, Initialized: %d\\n", name, symbol->type, initialized);
//     } else {
//         printf("Symbol not found: %s\\n", name);
//     }
// }

// // Print the symbol table (for debugging)
// void printSymbolTable(SymbolTable *symbolTable) {
//     for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
//         Symbol *current = symbolTable->table[i];
//         if (current) {
//             printf("Index %d:\\n", i);
//             while (current) {
//                 printf("  Name: %s, Type: %s, Scope: %s, Initialized: %d\\n", 
//                        current->name, current->type, current->scope, current->initialized);
//                 current = current->next;
//             }
//         }
//     }
// }

// // Clean up the symbol table
// void freeSymbolTable(SymbolTable *symbolTable) {
//     for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
//         Symbol *current = symbolTable->table[i];
//         while (current) {
//             Symbol *temp = current;
//             current = current->next;
//             free(temp->name);
//             free(temp->type);
//             free(temp->scope);
//             free(temp);
//         }
//     }
//     free(symbolTable);
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SymbolTable.h"

SymbolTable* createSymbolTable() {
    SymbolTable *symbolTable = (SymbolTable *)malloc(sizeof(SymbolTable));
    if (!symbolTable) {
        printf("Error: Failed to allocate memory for SymbolTable.\n");
        return NULL;
    }
    symbolTable->count = 0; 
    return symbolTable;
}

void addSymbol(SymbolTable *symbolTable, char *name, char *type, void *value) {
    if (symbolTable->count >= SYMBOL_TABLE_SIZE) {
        printf("Symbol table is full, cannot add more symbols.\n");
        return;
    }

    Symbol *newSymbol = &symbolTable->table[symbolTable->count++];
    newSymbol->name = strdup(name);
    newSymbol->type = strdup(type);
    newSymbol->value = value;

    printf("Added symbol: %s, Type: %s\n", name, type);
}

Symbol* findSymbol(SymbolTable *symbolTable, char *name) {
    for (int i = 0; i < symbolTable->count; i++) {
        if (strcmp(symbolTable->table[i].name, name) == 0) {
            return &symbolTable->table[i];
        }
    }
    return NULL;
}

void updateSymbol(SymbolTable *symbolTable, char *name, char *type, void *value) {
    Symbol *symbol = findSymbol(symbolTable, name);
    if (symbol) {
        free(symbol->type);
        symbol->type = strdup(type);
        symbol->value = value;

        printf("Updated symbol: %s, New Type: %s\n", name, type);
    } else {
        printf("Symbol not found: %s\n", name);
    }
}

void printSymbolTable(SymbolTable *symbolTable) {
    for (int i = 0; i < symbolTable->count; i++) {
        Symbol *symbol = &symbolTable->table[i];
        printf("Name: %s, Type: %s, Value: %p\n", 
               symbol->name, symbol->type, symbol->value);
    }
}

void freeSymbolTable(SymbolTable *symbolTable) {
    for (int i = 0; i < symbolTable->count; i++) {
        Symbol *symbol = &symbolTable->table[i];
        free(symbol->name);
        free(symbol->type);
    }
    free(symbolTable);
}
