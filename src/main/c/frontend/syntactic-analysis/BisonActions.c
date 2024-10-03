#include "BisonActions.h"

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
    _logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

static void _logSyntacticAnalyzerAction(const char * functionName) {
    logDebugging(_logger, "%s", functionName);
}

Action* CreateTableActionSemanticAction(char* tableName, Column* columns, int columnCount) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Action* action = calloc(1, sizeof(Action));
    action->type = CREATE_ACTION;
    action->action.create.tableName = strdup(tableName);
    action->action.create.columns = columns;
    action->action.create.columnCount = columnCount;
    return action;
}

Action* SelectActionSemanticAction(char** columns, int columnCount, char* tableName, Condition* where) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Action* action = calloc(1, sizeof(Action));
    action->type = SELECT_ACTION;
    action->action.select.columns = columns;
    action->action.select.columnCount = columnCount;
    action->action.select.tableName = strdup(tableName);
    action->action.select.where = where;
    return action;
}

Action* DeleteActionSemanticAction(char* tableName, Condition* where) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Action* action = calloc(1, sizeof(Action));
    action->type = DELETE_ACTION;
    action->action.delete.tableName = strdup(tableName);
    action->action.delete.where = where;
    return action;
}

Action* AddActionSemanticAction(char* tableName, Value* values, int valueCount) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Action* action = calloc(1, sizeof(Action));
    action->type = ADD_ACTION;
    action->action.add.tableName = strdup(tableName);
    action->action.add.values = values;
    action->action.add.valueCount = valueCount;
    return action;
}

Program* ProgramSemanticAction(CompilerState* compilerState, Action* action) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Program* program = calloc(1, sizeof(Program));
    program->action = action;
    compilerState->abstractSyntaxtTree = program;
    compilerState->succeed = true;
    return program;
}