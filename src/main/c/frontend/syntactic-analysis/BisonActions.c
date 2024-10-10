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

extern unsigned int flexCurrentContext
(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

// Constant * IntegerConstantSemanticAction(const int value) {
// 	_logSyntacticAnalyzerAction(__FUNCTION__);
// 	Constant * constant = calloc(1, sizeof(Constant));
// 	constant->value = value;
// 	return constant;
// }
// Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type) {
// 	_logSyntacticAnalyzerAction(__FUNCTION__);
// 	Expression * expression = calloc(1, sizeof(Expression));
// 	expression->leftExpression = leftExpression;
// 	expression->rightExpression = rightExpression;
// 	expression->type = type;
// 	return expression;
// }
// Factor * ExpressionFactorSemanticAction(Expression * expression) {
// 	_logSyntacticAnalyzerAction(__FUNCTION__);
// 	Factor * factor = calloc(1, sizeof(Factor));
// 	factor->expression = expression;
// 	factor->type = EXPRESSION;
// 	return factor;
// }
// Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Expression * expression) {
// 	_logSyntacticAnalyzerAction(__FUNCTION__);
// 	Program * program = calloc(1, sizeof(Program));
// 	program->expression = expression;
// 	compilerState->abstractSyntaxtTree = program;
// 	if (0 < flexCurrentContext()) {
// 		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
// 		compilerState->succeed = false;
// 	}
// 	else {
// 		compilerState->succeed = true;
// 	}
// 	return program;
// }




JsonQuery * JsonQuerySemanticAction(Action * action, JsonQuery * jsonQuery){
    _logSyntacticAnalyzerAction(__FUNCTION__);
    JsonQuery *newQuery = calloc(1, sizeof(JsonQuery));
    
	newQuery->query.node.action= action;
	newQuery->query.node.json_query = jsonQuery;
	return newQuery;

}



CreateAction * CreateActionSemanticAction(String table_name, ColumnObject* col_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	CreateAction *newCreateAction = calloc(1, sizeof(CreateAction));

	newCreateAction->table_name = table_name;
	newCreateAction->column_object = col_object;
	return newCreateAction;
}

UpdateAction * UpdateActionSemanticAction(String table_name, UpdateList* update_list, WhereObject* where_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Action *newAction = calloc(1, sizeof(Action));
	UpdateAction *newUpdateAction = calloc(1, sizeof(UpdateAction));

	newUpdateAction->table_name = table_name;
	newUpdateAction->update_list = update_list;
	newUpdateAction->where_object = where_object;
	return newUpdateAction;
}

AddAction * AddActionSemanticAction(String table_name, Array* array){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	AddAction *newAddAction = calloc(1, sizeof(AddAction));

	newAddAction->table_name = table_name;
	newAddAction->array = array;
	return newAddAction;
}


DeleteAction * DeleteActionSemanticAction(String table_name, WhereObject* where_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DeleteAction *newDeleteAction = calloc(1, sizeof(DeleteAction));

	newDeleteAction->table_name = table_name;
	newDeleteAction->where_object = where_object;
	return newDeleteAction;
}

SelectAction* SelectActionSemanticAction(ColumnList* table_column_list, String table_name, WhereObject* where_object, ColumnList* groupby_column_list,HavingObject* having_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SelectAction* newSelectAction = calloc(1, sizeof(SelectAction));

	newSelectAction->group_by_column_list = groupby_column_list;
	newSelectAction->having_object = having_object;
	newSelectAction->table_column_list = table_column_list;
	newSelectAction->table_name = table_name;
	newSelectAction->where_objects = where_object;
	return newSelectAction;
}


ColumnList * ColumnListSemanticAction(ColumnItem * column_item, ColumnList* column_list){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ColumnList * newColumnList = calloc(1, sizeof(ColumnList));
	if(column_list == NULL){
		newColumnList->columnListUnion.first.column_item = column_item;
		return newColumnList;
	}else{
		newColumnList->columnListUnion.second.column_item = column_item;
		newColumnList->columnListUnion.second.column_list = column_list;
		return newColumnList;
	}
}

ColumnItem * ColumnItemSemanticAction(String left, String right){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ColumnItem * newColumnItem = calloc(1, sizeof(ColumnItem));
	newColumnItem->left = left;
	newColumnItem->right = right;
	return newColumnItem;
}


UpdateItems * UpdateItemSemanticAction(String string, Value * value, UpdateItems * updateItems ){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	UpdateItems * newUpdateList = calloc(1, sizeof(UpdateItems));
	if(updateItems != NULL){
		newUpdateList->update_items_union.first.string = string;
		newUpdateList->update_items_union.first.value = value;
		return newUpdateList;
	}else{
		newUpdateList->update_items_union.second.string = string;
		newUpdateList->update_items_union.second.value = value;
		newUpdateList->update_items_union.second.update_items = updateItems;
		return newUpdateList;
	}
}

WhereObject * WhereObjectSemanticAction(Condition * condition, LogOp* logical_op, WhereObject* where_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	WhereObject* newWhereObject = calloc(1, sizeof(WhereObject));

	if(condition != NULL){
		newWhereObject->where_object_union.second.condition = condition;
		newWhereObject->where_object_union.second.log_op = logical_op;
		newWhereObject->where_object_union.second.where_object = where_object;
	}else{
		newWhereObject->where_object_union.third.log_op = logical_op;
		newWhereObject->where_object_union.third.where_object = where_object;
	}
	return newWhereObject;
}


HavingObject * HavingObjectSemanticAction(HavingCondition* having_condition, LogOp* logical_op, HavingObject* having_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	HavingObject * newHavingObject = calloc(1, sizeof(HavingObject));

	if(having_condition == NULL){
		newHavingObject->having_object_union.third.log_op = logical_op;
		newHavingObject->having_object_union.third.having_object = having_object;
		return newHavingObject;
	}else{
		newHavingObject->having_object_union.second.condition = having_condition;
		newHavingObject->having_object_union.second.having_object = having_object;
		newHavingObject->having_object_union.second.log_op = logical_op;
		return newHavingObject;
	}
}

Condition * ConditionSemanticAction(String string, Operator* operator, Value* value){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Condition * newCondition = calloc(1, sizeof(Condition));
	newCondition->string = string;
	newCondition->operator = operator;
	newCondition->value = value;
	return newCondition;
}

Value * StringValueSemanticAction(String string){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Value * newValue = calloc(1, sizeof(Value));
	newValue->values.string = string;
	return newValue;
}

Value * IntegerValueSemanticAction(Integer integer){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Value * newValue = calloc(1, sizeof(Value));
	newValue->values.integer = integer;
	return newValue;
}

Value * FloatValueSemanticAction(Float float_value){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Value * newValue = calloc(1, sizeof(Value));
	newValue->values.float_value = float_value;
	return newValue;
}

ValueList * ValueListSemanticAction(Value* value, ValueList* value_list){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ValueList * newValueList = calloc(1, sizeof(ValueList));
	if(value_list == NULL){
		newValueList->value_list_union.first.value = value;
		return newValueList;
	}else{
		newValueList->value_list_union.second.value = value;
		newValueList->value_list_union.second.value_list = value_list;
		return newValueList;
	}
}

HavingCondition* HavingConditionSemanticAction(AggFunc * agg_func, String string, Operator* operator, Value* value){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	HavingCondition* newHavingCondition = calloc(1, sizeof(HavingCondition));

	newHavingCondition->aggregate_func = agg_func;
	newHavingCondition->operator = operator;
	newHavingCondition->value = value;
	return newHavingCondition;
}

