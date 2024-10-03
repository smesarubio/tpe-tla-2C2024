

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void releaseExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case ADDITION:
			case DIVISION:
			case MULTIPLICATION:
			case SUBTRACTION:
				releaseExpression(expression->leftExpression);
				releaseExpression(expression->rightExpression);
				break;
			case FACTOR:
				releaseFactor(expression->factor);
				break;
		}
		free(expression);
	}
}

void releaseFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case CONSTANT:
				releaseConstant(factor->constant);
				break;
			case EXPRESSION:
				releaseExpression(factor->expression);
				break;
		}
		free(factor);
	}
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseExpression(program->expression);
		free(program);
	}
}
///////
#include "AbstractSyntaxTree.h"

/* ... existing code ... */

/* New types */
typedef enum {
    CREATE_ACTION,
    SELECT_ACTION,
    DELETE_ACTION,
    ADD_ACTION
} ActionType;

typedef struct {
    char* name;
    DataType type;
} Column;

typedef struct {
    char* tableName;
    Column* columns;
    int columnCount;
} CreateAction;

typedef struct {
    char** columns;
    int columnCount;
    char* tableName;
    Condition* where;
} SelectAction;

typedef struct {
    char* tableName;
    Condition* where;
} DeleteAction;

typedef struct {
    char* tableName;
    Value* values;
    int valueCount;
} AddAction;

typedef struct {
    ActionType type;
    union {
        CreateAction create;
        SelectAction select;
        DeleteAction delete;
        AddAction add;
    } action;
} Action;

typedef struct {
    Action* action;
} Program;

/* New release functions */
void releaseAction(Action* action) {
    if (action == NULL) return;
    
    switch (action->type) {
        case CREATE_ACTION:
            free(action->action.create.tableName);
            for (int i = 0; i < action->action.create.columnCount; i++) {
                free(action->action.create.columns[i].name);
            }
            free(action->action.create.columns);
            break;
        /* ... other cases ... */
    }
    free(action);
}

void releaseProgram(Program* program) {
    if (program == NULL) return;
    releaseAction(program->action);
    free(program);
}

/* ... other necessary functions ... */