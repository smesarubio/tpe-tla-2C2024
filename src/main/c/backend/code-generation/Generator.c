#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

ComputationResult result = { .succeed = true, .sql = NULL };


/** PRIVATE FUNCTIONS */

static const char _expressionTypeToCharacter(const ExpressionType type);
static void _generateConstant(const unsigned int indentationLevel, Constant * constant);
static void _generateEpilogue(const int value);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
static void _generateProgram(Program *program);
static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

/* NUESTROS */


/**
 * Converts and expression type to the proper character of the operation
 * involved, or returns '\0' if that's not possible.
 */

static char* removeQuotes(const char* str) {
    if (str == NULL) return NULL;

    size_t len = strlen(str);
    if (len < 2) return strdup(str); 

    if (str[0] == '"' && str[len - 1] == '"') {
        // Crea un nuevo string sin las comillas
        char* result = (char*)malloc(len - 1); // Tamaño: len - 2 (contenido) + 1 (\0)
        if (result == NULL) return NULL; // Manejo de errores de asignación

        strncpy(result, str + 1, len - 2); // Copia el contenido interno
        result[len - 2] = '\0'; // Agrega el terminador nulo
        return result;
    }

    // Si no tiene comillas, lo devuelve igual.
    return strdup(str);
}


static void _generateAddAction(AddAction * addAction) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "ALTER TABLE %s\nADD (", removeQuotes(addAction->table_name));
    result.sql = strcat(result.sql, buffer);
    _generateColumnObject(addAction->column_object);
    result.sql = strcat(result.sql, ");\n");
}

static void _generateValueList(ValueList * valueList) {
    if (valueList->value_list_union.second.value_list != NULL) {
        _generateValueList(valueList->value_list_union.second.value_list);
        result.sql = strcat(result.sql, ", ");
    }
    _generateValue(valueList->value_list_union.first.value);
}

static void _generateValue(Value * value) {
    char buffer[1024];
    switch (value->type) {
        case VALUE_TYPE_STRING:
            if (value->values.string != NULL) {
                snprintf(buffer, sizeof(buffer), "%s", removeQuotes(value->values.string));
                result.sql = strcat(result.sql, buffer);
            }
            break;
        case VALUE_TYPE_INTEGER:
            snprintf(buffer, sizeof(buffer), "%d", value->values.integer);
            result.sql = strcat(result.sql, buffer);
            break;
        case VALUE_TYPE_FLOAT:
            snprintf(buffer, sizeof(buffer), "%f", value->values.float_value);
            result.sql = strcat(result.sql, buffer);
            break;
        default:
            logError(_logger, "Unknown value type");
            break;
    }
}


static void _generateCreateAction(CreateAction * createAction) {
    logDebugging(_logger, "Generate action");
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "CREATE TABLE %s (", removeQuotes(createAction->table_name));
    result.sql = strcat(result.sql, buffer);
    _generateColumnObject(createAction->column_object);
    result.sql = strcat(result.sql, ");\n");
}

static void _generateColumnObject(ColumnObject *columnObject) {
    if (columnObject == NULL) return;
    _generateColumnList(columnObject->column_list);
}

static void _generateColumnList(ColumnList *columnList) {
    if (columnList == NULL) return;

    _generateColumnItem(columnList->columnListUnion.first.column_item);

    if (columnList->columnListUnion.second.column_list != NULL) {
        strcat(result.sql, ", ");
        _generateColumnList(columnList->columnListUnion.second.column_list);
    }
}

static void _generateColumnItem(ColumnItem *columnItem) {
    if (columnItem == NULL) return;
    const char *typeStr = removeQuotes(columnItem->right);
    char buffer[256];
    if (strcmp(typeStr, "STRING") == 0) {
        snprintf(buffer, sizeof(buffer), "%s VARCHAR(40)", removeQuotes(columnItem->left));
    } else if (strcmp(typeStr, "INTEGER") == 0) {
        snprintf(buffer, sizeof(buffer), "%s int", removeQuotes(columnItem->left));
    } else {
        snprintf(buffer, sizeof(buffer), "%s %s", removeQuotes(columnItem->left), typeStr);
    }
    result.sql = strcat(result.sql, buffer);
}

static void _generateDeleteAction(DeleteAction * deleteAction) {
    char buffer[1024];
    if (deleteAction->where_object == NULL) {
        snprintf(buffer, sizeof(buffer), "DELETE FROM %s", removeQuotes(deleteAction->table_name));
        result.sql = strcat(result.sql, buffer);
    } else {
        snprintf(buffer, sizeof(buffer), "DELETE FROM %s \nWHERE ", removeQuotes(deleteAction->table_name));
        result.sql = strcat(result.sql, buffer);
        _generateWhereObject(deleteAction->where_object);
    }
    result.sql = strcat(result.sql, ";\n");
}

static const char* _getLogOpString(LogOpType logOpType) {
    switch (logOpType) {
        case E_AND:
            return "AND";
        case E_OR:
            return "OR";
        default:
            return "AND";
    }
}
static void _generateWhereObject(WhereObject *whereObject) {
    if (whereObject == NULL) return;
    char buffer[1024];
    if (whereObject->where_object_union.second.where_object != NULL) {
        strcat(result.sql, "(");
        _generateCondition(whereObject->where_object_union.second.condition);
        snprintf(buffer, sizeof(buffer), " %s ", _getLogOpString(whereObject->where_object_union.second.log_op));
        strcat(result.sql, buffer);
        _generateWhereObject(whereObject->where_object_union.second.where_object);
        strcat(result.sql, ")");
    }
    else if (whereObject->where_object_union.first.condition != NULL) {
        _generateCondition(whereObject->where_object_union.first.condition);
    }
}

static const char* _getOperatorString(OperatorType operatorType) {

    switch (operatorType) {
        case E_EQUALS:
            return "=";
        case E_GREATER_THAN:
            return ">";
        case E_LESS_THAN:
            return "<";
        default:
            return "=";
    }
}

static void _generateCondition(Condition *condition) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "%s %s ", removeQuotes(condition->string), _getOperatorString(condition->operator));
    result.sql = strcat(result.sql, buffer);
    _generateValue(condition->value);
}


static void _generateArray(Array *array) {
    if (array == NULL) return;

    if (array->string_list_union.second.string_list != NULL) {
        _generateArray(array->string_list_union.second.string_list);
        result.sql = strcat(result.sql, ", ");
    }

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s", removeQuotes(array->string_list_union.first.string));
    result.sql = strcat(result.sql, buffer);
}

static void _generateInsertList(InsertList *insertList) {
    if (insertList == NULL) return;
    result.sql = strcat(result.sql, "(");

    _generateValueList(insertList->first.value_list);

    result.sql = strcat(result.sql, ")");

    if (insertList->second.list != NULL) {
        result.sql = strcat(result.sql, ", ");
        _generateInsertList(insertList->second.list);
    }
}

static void _generateInsertAction(InsertAction *insertAction) {
    if (insertAction == NULL) {
        logError(_logger, "InsertAction is NULL");
        return;
    }

    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "INSERT INTO %s ", removeQuotes(insertAction->table_name));
    result.sql = strcat(result.sql, buffer);

    if (insertAction->columns != NULL) {
        result.sql = strcat(result.sql, "(");
        _generateArray(insertAction->columns);
        result.sql = strcat(result.sql, ") ");
    }

    result.sql = strcat(result.sql, "\nVALUES ");

    if (insertAction->value_list != NULL) {
        _generateInsertList(insertAction->value_list);
    } else {
        logError(_logger, "Value list is NULL");
    }

    result.sql = strcat(result.sql, ";\n");
}
static void _generateHavingCondition(HavingCondition *havingCondition) {
    if (havingCondition == NULL) return;

    const char *aggFuncStr;
    switch (havingCondition->aggregate_func) {
        case E_COUNT:
            aggFuncStr = "COUNT";
            break;
        case E_SUM:
            aggFuncStr = "SUM";
            break;
        case E_AVG:
            aggFuncStr = "AVG";
            break;
        case E_MAX:
            aggFuncStr = "MAX";
            break;
        case E_MIN:
            aggFuncStr = "MIN";
            break;
        default:
            aggFuncStr = "";
            break;
    }

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s(%s)", aggFuncStr, removeQuotes(havingCondition->string));
    result.sql = strcat(result.sql, buffer);

    snprintf(buffer, sizeof(buffer), " %s ", _getOperatorString(havingCondition->operator));
    result.sql = strcat(result.sql, buffer);

    _generateValue(havingCondition->value);
}

static void _generateHavingObject(HavingObject * HavingObject){
    if (HavingObject->having_object_union.first.condition != NULL) {
        _generateHavingCondition(HavingObject->having_object_union.first.condition);
    } else {
        result.sql = strcat(result.sql, "(");
        _generateHavingCondition(HavingObject->having_object_union.second.condition);
        char buffer[256];
        snprintf(buffer, sizeof(buffer), " %s ", _getLogOpString(HavingObject->having_object_union.second.log_op));
        result.sql = strcat(result.sql, buffer);
        _generateHavingObject(HavingObject->having_object_union.second.having_object);
        result.sql = strcat(result.sql, ")");
    }
}

static void _generateSelectAction(SelectAction *selectAction) {
    char buffer[1024];
    strcat(result.sql, "SELECT ");

    if (selectAction->table_column_list != NULL) {
        _generateArray(selectAction->table_column_list);
    } else {
        strcat(result.sql, "*");
    }

    snprintf(buffer, sizeof(buffer), " FROM %s", removeQuotes(selectAction->table_name));
    strcat(result.sql, buffer);

    if (selectAction->join != NULL) {
        snprintf(buffer, sizeof(buffer), "\nJOIN %s ON %s = %s", removeQuotes(selectAction->join->table_name2),
                removeQuotes(selectAction->join->cond1), removeQuotes(selectAction->join->cond2));
        strcat(result.sql, buffer);
    }

    if (selectAction->where_objects != NULL) {
        strcat(result.sql, "\nWHERE ");
        _generateWhereObject(selectAction->where_objects);
    }

    if (selectAction->group_by_column_list != NULL) {
        strcat(result.sql, "\nGROUP BY ");
        _generateArray(selectAction->group_by_column_list);
    }

    if (selectAction->having_object != NULL) {
        strcat(result.sql, "\nHAVING ");
        _generateHavingObject(selectAction->having_object);
    }

    if (selectAction->order_by_column_list != NULL) {
        strcat(result.sql, "\nORDER BY ");
        _generateArray(selectAction->order_by_column_list);
    }

    strcat(result.sql, ";\n");
}
static void _generateUpdateAction(UpdateAction *updateAction) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "UPDATE %s \nSET ", removeQuotes(updateAction->table_name));
    result.sql = strcat(result.sql, buffer);

    UpdateObject *updateItems = updateAction->update_object;
    while (updateItems != NULL) {
        snprintf(buffer, sizeof(buffer), "%s = ", removeQuotes(updateItems->condition->string));
        result.sql = strcat(result.sql, buffer);
        _generateValue(updateItems->condition->value);

        updateItems = updateItems->next;
        if (updateItems != NULL) {
            result.sql = strcat(result.sql, ", ");
        }
    }

    if (updateAction->where_object != NULL) {
        result.sql = strcat(result.sql, "\nWHERE ");
        _generateWhereObject(updateAction->where_object);
    }

    result.sql = strcat(result.sql, ";\n");
}

static void _generateAction(Action * action){
    switch (action->type) {
        case E_CREATE:
            logDebugging(_logger, "Generando CREATE...");
            _generateCreateAction(action->actions.create_action);
            break;

        case E_SELECT:
            logDebugging(_logger, "Generando SELECT...");
            _generateSelectAction(action->actions.select_action);
            break;

        case E_DELETE:
            logDebugging(_logger, "Generando DELETE...");
            _generateDeleteAction(action->actions.delete_action);
            break;

        case E_ADD:
            logDebugging(_logger, "Generando ADD...");
            _generateAddAction(action->actions.add_action);
            break;

        case E_UPDATE:
            logDebugging(_logger, "Generando UPDATE...");
            _generateUpdateAction(action->actions.update_action);
            break;

        case E_INSERT:
            logDebugging(_logger, "Generando INSERT...");
            _generateInsertAction(action->actions.insert_action);
            break;

        default:
            logError(_logger, "Tipo de acción desconocido: %d", action->type);
            break;
        }
}


/**
 * Generates the output of an expression.
 */
static void _generateSQL(JsonQuery * json_query) {
    while (json_query != NULL) {
        _generateAction(json_query->action);
        json_query = json_query->next;
    }
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	// _generatePrologue();
	_generateSQL(compilerState->abstractSyntaxtTree);
	// _generateEpilogue(compilerState->sql);
	logDebugging(_logger, "Generation is done.");
}

ComputationResult computeJson(JsonQuery * json_query) {
    logDebugging(_logger, "Computing JSON...");
    result.sql = (char*)malloc(1024);
    _generateSQL(json_query);
    return result;
}