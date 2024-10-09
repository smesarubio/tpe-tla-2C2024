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



JsonQuery * JsonQuerySemanticAction(Action * action, JsonQuery * jsonQuery){
    _logSyntacticAnalyzerAction(__FUNCTION__);
    JsonQuery *newQuery = calloc(1, sizeof(JsonQuery));
    
	newQuery->query.node.action= action;
	newQuery->query.node.json_query = jsonQuery;
	return newQuery;

}


Action * CreateActionSemanticAction(String* table_name, ColumnObject* col_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Action *newAction = calloc(1, sizeof(Action));
	CreateAction *newCreateAction = calloc(1, sizeof(CreateAction));

	newCreateAction->table_name = table_name;
	newCreateAction->column_object = col_object;
	newAction->actions.create_action = newCreateAction;
	return newAction;
}

Action * UpdateActionSemanticAction(String* table_name, UpdateList* update_list, WhereObject* where_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Action *newAction = calloc(1, sizeof(Action));
	UpdateAction *newUpdateAction = calloc(1, sizeof(UpdateAction));

	newUpdateAction->table_name = table_name;
	newUpdateAction->update_list = update_list;
	newUpdateAction->where_object = where_object;
	newAction->actions.update_action = newUpdateAction;
	return newAction;
}

Action * AddActionSemanticAction(String* table_name, Array* array){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Action *newAction = calloc(1, sizeof(Action));
	AddAction *newAddAction = calloc(1, sizeof(AddAction));

	newAddAction->table_name = table_name;
	newAddAction->array = array;
	newAction->actions.add_action = newAddAction;
	return newAction;
}

Action * DeleteActionSemanticAction(String* table_name, WhereObject* where_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Action *newAction  = calloc(1, sizeof(Action));
	DeleteAction *newDeleteAction = calloc(1, sizeof(DeleteAction));

	newDeleteAction->table_name = table_name;
	newDeleteAction->where_object = where_object;
	newAction->actions.delete_action = newDeleteAction;
	return newAction;
}

