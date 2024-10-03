#include "SyntacticAnalyzer.h"
#include "../lexical-analysis/LexicalAnalyzerContext.h"

/* MODULE INTERNAL STATE */

static CompilerState * _currentCompilerState = NULL;
static Logger * _logger = NULL;

void initializeSyntacticAnalyzerModule() {
    _logger = createLogger("SyntacticAnalyzer");
}

void shutdownSyntacticAnalyzerModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

/** IMPORTED FUNCTIONS */

extern LexicalAnalyzerContext * createLexicalAnalyzerContext();

extern int yyparse(void);

void yyerror(const char * string) {
    LexicalAnalyzerContext * lexicalAnalyzerContext = createLexicalAnalyzerContext();
    logError(_logger, "Syntax error in JSON-SQL query (line %d): %s", lexicalAnalyzerContext->line, string);
}

/* PUBLIC FUNCTIONS */

CompilerState * currentCompilerState() {
    return _currentCompilerState;
}

SyntacticAnalysisStatus parse(CompilerState * compilerState) {
    logDebugging(_logger, "Parsing JSON-SQL query...");
    _currentCompilerState = compilerState;
    const int code = yyparse();
    _currentCompilerState = NULL;
    SyntacticAnalysisStatus syntacticAnalysisStatus;
    logDebugging(_logger, "Parsing is done.");
    switch (code) {
        case 0:
            if (compilerState->succeed == true) {
                return ACCEPT;
            }
            else {
                syntacticAnalysisStatus = REJECT;
            }
        case 1:
            syntacticAnalysisStatus = REJECT;
            break;
        case 2:
            logError(_logger, "Parser ran out of memory.");
            syntacticAnalysisStatus = OUT_OF_MEMORY;
            break;
        default:
            logError(_logger, "Unknown error inside parser (code = %d).", code);
            syntacticAnalysisStatus = UNKNOWN_ERROR;
    }
    compilerState->succeed = false;
    return syntacticAnalysisStatus;
}