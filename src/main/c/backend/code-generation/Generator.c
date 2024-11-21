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

static void _generateArray(Array *array) {
    if (array->string_list_union.second.string_list != NULL) {
        _generateArray(array->string_list_union.second.string_list);
        _output(0, ", ");
    }
    _output(0, "%s", array->string_list_union.first.string);
}


static void _generateAddAction(AddAction * addAction) {
	_output(0, "INSERT INTO %s VALUES (", addAction->table_name);
	_generateValueList(addAction->array);
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
	else if(value->values.integer != NULL){
		_output(0, "%d", value->values.integer);
	}
	else if((value->values.float_value) != NULL){
		_output(0, "%f", value->values.float_value);
	}
}


static void _generateCreateAction(CreateAction * createAction) {
	_output(0, "CREATE TABLE %s (", createAction->table_name);
	_generateColumnObject(createAction->column_object);
	_output(0, ");\n");
}

static void _generateColumnObject(ColumnObject * columnObject) {
	_generateColumnList(columnObject->column_list);
}

static void _generateColumnList(ColumnList * columnList) {
	if(columnList->columnListUnion.second.column_list != NULL){
		_generateColumnItem(columnList->columnListUnion.second.column_item);
		_output(0, ", ");
		_generateColumnList(columnList->columnListUnion.second.column_list);
	}else{
		_generateColumnItem(columnList->columnListUnion.first.column_item);
	}
}

static void _generateColumnItem(ColumnItem * columnItem) {
	_output(0, "%s %s", columnItem->left, columnItem->right);
}

static void _generateDeleteAction(DeleteAction * deleteAction) {
	if (deleteAction->where_object == NULL) {
		_output(0, "DELETE FROM %s", deleteAction->table_name);
	}
	else {
		_output(0, "DELETE FROM %s WHERE ", deleteAction->table_name);
		_generateWhereObject(deleteAction->where_object);
	}
	_output(0, ";\n");
}

static void _generateWhereObject(WhereObject * whereObject) {
	if (whereObject->where_object_union.second.where_object != NULL) {
		_generateCondition(whereObject->where_object_union.second.condition);
		_output(0, " %s ", whereObject->where_object_union.second.log_op);
		_generateWhereObject(whereObject->where_object_union.second.where_object);
	} else if (whereObject->where_object_union.third.where_object != NULL) {
		_output(0, " %s ", whereObject->where_object_union.third.log_op);
		_generateWhereObject(whereObject->where_object_union.third.where_object);
	} else {
		_generateCondition(whereObject->where_object_union.first.condition);
	}
}

static void _generateCondition(Condition * condition) {
	if (condition->operator == NULL) {
		_output(0, "%s", condition->string);
	} else {
		_output(0, "%s %s ", condition->string, condition->operator->operator_type);
		_generateValue(condition->value);
	}
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue(const int value) {
	_output(0, "%s%d%s",
		"            [ $", value, "$, circle, draw, blue ]\n"
		"        ]\n"
		"    \\end{forest}\n"
		"\\end{document}\n\n"
	);
}

static void _generateSelectAction(SelectAction *selectAction) {
    _output(0, "SELECT ");

    if (selectAction->table_column_list != NULL) {
        _generateArray(selectAction->table_column_list);
    } else {
        _output(0, "*");
    }

    _output(0, " FROM %s", selectAction->table_name);

    if (selectAction->join != NULL) {
        _output(0, " JOIN %s ON %s = %s", selectAction->join->table_name2,
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
        _generateHavingObject(selectAction->having_object);
    }

    if (selectAction->order_by_column_list != NULL) {
        _output(0, " ORDER BY ");
        _generateArray(selectAction->order_by_column_list);
    }

    _output(0, ";\n");
}

static void _generateUpdateAction(UpdateAction *updateAction) {
    _output(0, "UPDATE %s SET ", updateAction->table_name);

    UpdateItems *updateItems = updateAction->update_list->update_items;
    while (updateItems != NULL) {
        _output(0, "%s = ", updateItems->update_items_union.first.string);
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
	// _output(indentationLevel, "%s", "[ $E$, circle, draw, black!20\n");
	if (json_query->query.action->delete_action != NULL){
		_generateDeleteAction(json_query->query.action->delete_action);
	}
	else if (json_query->query.action->create_action != NULL) {
		_generateCreateAction(json_query->query.action->create_action);
	}
	else if (json_query->query.action->select_action != NULL) {
		_generateSelectAction(json_query->query.action->select_action);
	}
	else if (json_query->query.action->add_action != NULL) {
		_generateAddAction(json_query->query.action->add_action);
	}
	else if (json_query->query.action->update_action != NULL) {
		_generateUpdateAction(json_query->query.action->update_action);
	}
	else {
		logError(_logger, "ni idea loco");
	}
	// _output(indentationLevel, "%s", "]\n");
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
