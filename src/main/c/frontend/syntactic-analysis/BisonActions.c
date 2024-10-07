#include "BisonActions.h"
/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->value = value;
	return constant;
}

Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	return expression;
}

Expression * FactorExpressionSemanticAction(Factor * factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR;
	return expression;
}

Factor * ConstantFactorSemanticAction(Constant * constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->constant = constant;
	factor->type = CONSTANT;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION;
	return factor;
}

Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->expression = expression;
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}


/*
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
}*/