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



/**
 * Generates the output of the program.
 */
// static void _generateProgram(JsonQuery *program)
// {
// 	_generateSQL(program->json_query);
// }
static char* removeQuotes(const char* str) {
    if (str == NULL) return NULL;

    size_t len = strlen(str);
    if (len < 2) return strdup(str); // Si el string es demasiado corto, lo devuelve igual.

    // Comprueba si el string comienza y termina con comillas
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
	_output(0, "ALTER TABLE %s\n ADD (",removeQuotes(addAction->table_name));
	_generateColumnObject(addAction->column_object);
	_output(0, ");\n");

}

static void _generateValueList(ValueList * valueList) {
	if(valueList->value_list_union.second.value_list != NULL){
		_generateValueList(valueList->value_list_union.second.value_list);
		_output(0, ", ");
	}
	_generateValue(valueList->value_list_union.first.value);
}

static void _generateValue(Value * value) {
	if(value->values.string != NULL){
		_output(0, "%s", value->values.string);
	}
	else if(value->values.integer != 0){
		_output(0, "%d", value->values.integer);
	}
	else { 
		_output(0, "%f", value->values.float_value);
	}
}


static void _generateCreateAction(CreateAction * createAction) {
	logDebugging(_logger, "Generate action");
	_output(0, "CREATE TABLE %s (", removeQuotes(createAction->table_name));
	_generateColumnObject(createAction->column_object);
	_output(0, ");\n");
}

static void _generateColumnObject(ColumnObject *columnObject) {
    if (columnObject == NULL) return;
    _generateColumnList(columnObject->column_list);
}

static void _generateColumnList(ColumnList *columnList) {
    if (columnList == NULL) return;

    _generateColumnItem(columnList->columnListUnion.first.column_item);

    if (columnList->columnListUnion.second.column_list != NULL) {
        _output(0, ", ");
        _generateColumnList(columnList->columnListUnion.second.column_list);
    }
}

static void _generateColumnItem(ColumnItem *columnItem) {
    if (columnItem == NULL) return;
    _output(0, "%s %s", removeQuotes(columnItem->left), removeQuotes(columnItem->right));
}

static void _generateDeleteAction(DeleteAction * deleteAction) {
	if (deleteAction->where_object == NULL) {
		_output(0, "DELETE FROM %s", removeQuotes(deleteAction->table_name));
	}
	else {
		_output(0, "DELETE FROM %s WHERE ", removeQuotes(deleteAction->table_name));
		_generateWhereObject(deleteAction->where_object);
	}
	_output(0, ";\n");
}

static const char* _getLogOpString(LogOpType *logOpType) {
    if (logOpType == NULL) return "AND";

    switch (*logOpType) {
        case E_NOT:
            return "NOT";
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

    // Handle the second union case
    if (whereObject->where_object_union.second.where_object != NULL) {
        _generateCondition(whereObject->where_object_union.second.condition);

        // Print the logical operator
        if (whereObject->where_object_union.second.log_op != NULL) {
            _output(0, " %s ", _getLogOpString(whereObject->where_object_union.second.log_op->log_op_type));
        }

        _generateWhereObject(whereObject->where_object_union.second.where_object);
    }
    // Handle the third union case
    else if (whereObject->where_object_union.third.where_object != NULL) {
        // Print the logical operator
        if (whereObject->where_object_union.third.log_op != NULL) {
            _output(0, " %s ", _getLogOpString(whereObject->where_object_union.third.log_op->log_op_type));
        }

        _generateWhereObject(whereObject->where_object_union.third.where_object);
    }
    // Handle the first union case (base case)
    else {
        _generateCondition(whereObject->where_object_union.first.condition);
    }
}
static const char* _getOperatorString(OperatorType *operatorType) {
    if (operatorType == NULL) return "UNKNOWN";

    switch (*operatorType) {
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
    if (condition == NULL) return;

    if (condition->operator == NULL) {
		// logCritical(_logger, "gen cond %s", condition->string);

        // No operator, print only the string
        _output(0, "%s = %d", removeQuotes(condition->string), condition->value);
    } else {
        // Print the condition with operator
        _output(0, "%s %s ", removeQuotes(condition->string), _getOperatorString(condition->operator->operator_type));
        _generateValue(condition->value);
    }
}


static void _generateArray(Array *array) {
    if (array == NULL) return;

    // Recursively print each column name
    if (array->string_list_union.second.string_list != NULL) {
        _generateArray(array->string_list_union.second.string_list);
        _output(0, ", ");
    }

    // Print the current column
    _output(0, "%s", array->string_list_union.first.string);
}

static void _generateInsertList(InsertList *insertList) {
    if (insertList == NULL) return;
    // Print the opening parenthesis for the value list
    _output(0, "(");

    // Generate the values in the first ValueList
    _generateValueList(insertList->first.value_list);

    // Print the closing parenthesis for the value list
    _output(0, ")");

    // If there are more InsertList elements, print a comma and recurse
    if (insertList->second.list != NULL) {
        _output(0, ", ");
        _generateInsertList(insertList->second.list);
    }
}

static void _generateInsertAction(InsertAction *insertAction) {
    if (insertAction == NULL) {
        logError(_logger, "InsertAction is NULL");
        return;
    }

    // Print the basic INSERT INTO statement
    _output(0, "INSERT INTO %s ", removeQuotes(insertAction->table_name));

    // If columns are provided, generate the column list
    if (insertAction->columns != NULL) {
        _output(0, "(");
        _generateArray(insertAction->columns);
        _output(0, ") ");
    }

    // Print VALUES keyword
    _output(0, "VALUES ");

    // Generate the values list
    if (insertAction->value_list != NULL) {
        _generateInsertList(insertAction->value_list);
    } else {
        logError(_logger, "Value list is NULL");
    }

    // End the statement
    _output(0, ";\n");
}

static void _generateSelectAction(SelectAction *selectAction) {
    _output(0, "SELECT ");

    if (selectAction->table_column_list != NULL) {
        _generateArray(selectAction->table_column_list);
    } else {
        _output(0, "*");
    }

    _output(0, " FROM %s", removeQuotes(selectAction->table_name));

    if (selectAction->join != NULL) {
        _output(0, " JOIN %s ON %s = %s", removeQuotes(selectAction->join->table_name2),
                selectAction->join->cond1, selectAction->join->cond2);
    }

    if (selectAction->where_objects != NULL) {
        _output(0, " WHERE ");
        _generateWhereObject(selectAction->where_objects);
    }

    if (selectAction->group_by_column_list != NULL) {
        _output(0, " GROUP BY ");
        _generateArray(selectAction->group_by_column_list);
    }

    if (selectAction->having_object != NULL) {
        _output(0, " HAVING ");
        //_generateHavingObject(selectAction->having_object);
    }

    if (selectAction->order_by_column_list != NULL) {
        _output(0, " ORDER BY ");
        _generateArray(selectAction->order_by_column_list);
    }

    _output(0, ";\n");
}

static void _generateUpdateAction(UpdateAction *updateAction) {
    _output(0, "UPDATE %s SET ", removeQuotes(updateAction->table_name));

    UpdateItems *updateItems = updateAction->update_list->update_items;
    while (updateItems != NULL) {
        _output(0, "%s = ",removeQuotes( updateItems->update_items_union.first.string));
        _generateValue(updateItems->update_items_union.first.value);

        updateItems = updateItems->update_items_union.second.update_items;
        if (updateItems != NULL) {
            _output(0, ", ");
        }
    }

    if (updateAction->where_object != NULL) {
        _output(0, " WHERE ");
        _generateWhereObject(updateAction->where_object);
    }

    _output(0, ";\n");
}


/**
 * Generates the output of an expression.
 */
static void _generateSQL(JsonQuery * json_query) {
	switch (json_query->query.action->type) {
        case E_CREATE:
            logDebugging(_logger, "Generando CREATE...");
            _generateCreateAction(json_query->query.action->actions.create_action);
            break;

        case E_SELECT:
            logDebugging(_logger, "Generando SELECT...");
            _generateSelectAction(json_query->query.action->actions.select_action);
            break;

        case E_DELETE:
            logDebugging(_logger, "Generando DELETE...");
            _generateDeleteAction(json_query->query.action->actions.delete_action);
            break;

        case E_ADD:
            logDebugging(_logger, "Generando ADD...");
            _generateAddAction(json_query->query.action->actions.add_action);
            break;

        case E_UPDATE:
            logDebugging(_logger, "Generando UPDATE...");
            _generateUpdateAction(json_query->query.action->actions.update_action);
            break;

        case E_INSERT:
            logDebugging(_logger, "Generando INSERT...");
            _generateInsertAction(json_query->query.action->actions.insert_action);
            break;

        default:
            logError(_logger, "Tipo de acción desconocido: %d", json_query->query.action->type);
            break;
    }
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
static void _generatePrologue(void) {
	_output(0, "%s",
		"\\documentclass{standalone}\n\n"
		"\\usepackage[utf8]{inputenc}\n"
		"\\usepackage[T1]{fontenc}\n"
		"\\usepackage{amsmath}\n"
		"\\usepackage{forest}\n"
		"\\usepackage{microtype}\n\n"
		"\\begin{document}\n"
		"    \\centering\n"
		"    \\begin{forest}\n"
		"        [ \\text{$=$}, circle, draw, purple\n"
	);
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
