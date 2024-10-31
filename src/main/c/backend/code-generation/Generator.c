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
static void _generateProgram(Program *program)
{
	_generateExpression(3, program->expression);
}

static void _generateJsonQuery(JsonQuery * jsonQuery) {
	 if(jsonQuery->query.node.json_query == NULL){
		_generateAction(jsonQuery->query.node.action);
	 }
	 else{
		 _generateJsonQuery(jsonQuery->query.node.json_query);
		 _generateAction(jsonQuery->query.node.action);
	 }
}

static void _generateAction(Action * action) {
	if (action->actions.create_action != NULL){
		_generateCreateAction(action->actions.create_action);
	}
	else if (action->actions.delete_action != NULL){
		_generateDeleteAction(action->actions.delete_action);
	}
	else if (action->actions.select_action != NULL){
		_generateSelectAction(action->actions.select_action);
	}
	else if (action->actions.add_action != NULL){
		_generateAddAction(action->actions.add_action);
	}
	else if (action->actions.update_action != NULL){
		_generateUpdateAction(action->actions.update_action);
	}
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
	_output(0, "DELETE FROM %s WHERE ", deleteAction->table_name);
	_generateWhereObject(deleteAction->where_object);
	_output(0, ";\n");
}

static void _generateWhereObject(WhereObject * whereObject) {
	if(whereObject->where_object_union.second.condition != NULL){

	}else{
		
	}
}

static void _generateCondition(Condition * condition) {
	if(condition->operator == NULL){
		_output(0, "%s %s ", condition->string, condition->operator);
		_generateValue(condition->value);
	}else{

	}

}



static const char _expressionTypeToCharacter(const ExpressionType type) {
	switch (type) {
		case ADDITION: return '+';
		case DIVISION: return '/';
		case MULTIPLICATION: return '*';
		case SUBTRACTION: return '-';
		default:
			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
			return '\0';
	}
}

/**
 * Generates the output of a constant.
 */
static void _generateConstant(const unsigned int indentationLevel, Constant * constant) {
	_output(indentationLevel, "%s", "[ $C$, circle, draw, black!20\n");
	_output(1 + indentationLevel, "%s%d%s", "[ $", constant->value, "$, circle, draw ]\n");
	_output(indentationLevel, "%s", "]\n");
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

/**
 * Generates the output of an expression.
 */
static void _generateExpression(const unsigned int indentationLevel, Expression * expression) {
	_output(indentationLevel, "%s", "[ $E$, circle, draw, black!20\n");
	switch (expression->type) {
		case ADDITION:
		case DIVISION:
		case MULTIPLICATION:
		case SUBTRACTION:
			_generateExpression(1 + indentationLevel, expression->leftExpression);
			_output(1 + indentationLevel, "%s%c%s", "[ $", _expressionTypeToCharacter(expression->type), "$, circle, draw, purple ]\n");
			_generateExpression(1 + indentationLevel, expression->rightExpression);
			break;
		case FACTOR:
			_generateFactor(1 + indentationLevel, expression->factor);
			break;
		default:
			logError(_logger, "The specified expression type is unknown: %d", expression->type);
			break;
	}
	_output(indentationLevel, "%s", "]\n");
}

/**
 * Generates the output of a factor.
 */
static void _generateFactor(const unsigned int indentationLevel, Factor * factor) {
	_output(indentationLevel, "%s", "[ $F$, circle, draw, black!20\n");
	switch (factor->type) {
		case CONSTANT:
			_generateConstant(1 + indentationLevel, factor->constant);
			break;
		case EXPRESSION:
			_output(1 + indentationLevel, "%s", "[ $($, circle, draw, purple ]\n");
			_generateExpression(1 + indentationLevel, factor->expression);
			_output(1 + indentationLevel, "%s", "[ $)$, circle, draw, purple ]\n");
			break;
		default:
			logError(_logger, "The specified factor type is unknown: %d", factor->type);
			break;
	}
	_output(indentationLevel, "%s", "]\n");
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
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue(compilerState->value);
	logDebugging(_logger, "Generation is done.");
}
