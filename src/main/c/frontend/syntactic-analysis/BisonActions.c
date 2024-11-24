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

JsonQuery *JsonQuerySemanticAction(CompilerState *compilerState, Action *action, JsonQuery *jsonQuery) {
    _logSyntacticAnalyzerAction(__FUNCTION__);

    JsonQuery *newQuery = calloc(1, sizeof(JsonQuery));
    if (newQuery == NULL) {
        logError(_logger, "Failed to allocate memory for JsonQuery");
        compilerState->succeed = false;
        return NULL;
    }

    newQuery->action = action;
    newQuery->next = jsonQuery;

    if (flexCurrentContext() != 0) {
        logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
        compilerState->succeed = false;
    } else {
        compilerState->succeed = true;
    }
    compilerState->abstractSyntaxtTree = newQuery;

    return newQuery;
}

InsertAction * InsertActionSemanticAction(String table_name, Array* columns, InsertList* value_list){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	InsertAction *newInsertAction = calloc(1, sizeof(InsertAction));
	newInsertAction->table_name = table_name;
	newInsertAction->columns = columns;
	newInsertAction->value_list = value_list;
	return newInsertAction;
}

CreateAction * CreateActionSemanticAction(String table_name, ColumnObject* col_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	CreateAction *newCreateAction = calloc(1, sizeof(CreateAction));
	newCreateAction->table_name = table_name;
	newCreateAction->column_object = col_object;

	return newCreateAction;
}

UpdateAction * UpdateActionSemanticAction(String table_name, UpdateObject* update_object, WhereObject* where_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	UpdateAction *newUpdateAction = calloc(1, sizeof(UpdateAction));
	newUpdateAction->table_name = table_name;
	newUpdateAction->update_object = update_object;
	newUpdateAction->where_object = where_object;
	return newUpdateAction;
}

AddAction * AddActionSemanticAction(String table_name, ColumnObject* col_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	AddAction *newAddAction = calloc(1, sizeof(AddAction));
	newAddAction->table_name = table_name;
	newAddAction->column_object = col_object;
	return newAddAction;
}


DeleteAction * DeleteActionSemanticAction(String table_name, WhereObject* where_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DeleteAction *newDeleteAction = calloc(1, sizeof(DeleteAction));
	newDeleteAction->table_name = table_name;
	newDeleteAction->where_object = where_object;
	return newDeleteAction;
}

SelectAction* SelectAllActionSemanticAction(String table_name){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SelectAction *newSelectAction = calloc(1, sizeof(SelectAction));
	newSelectAction->table_name = table_name;
	newSelectAction->table_column_list = NULL;
	return newSelectAction;
}

SelectAction* SelectActionSemanticAction(Array* table_column_list, String table_name, WhereObject* where_object, Array* groupby_column_list,Array* order_by_column_list, HavingObject* having_object, Join* join){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	SelectAction* newSelectAction = calloc(1, sizeof(SelectAction));
	newSelectAction->group_by_column_list = groupby_column_list;
	newSelectAction->order_by_column_list = order_by_column_list;
	newSelectAction->table_column_list = table_column_list;
	newSelectAction->table_name = table_name;
	newSelectAction->where_objects = where_object;
	newSelectAction->having_object = having_object;
	newSelectAction->join = join;
	return newSelectAction;
}

ColumnObject *ColumnObjectSemanticAction(String column_name, String column_type, ColumnObject *next) {
    _logSyntacticAnalyzerAction(__FUNCTION__);

    ColumnItem *newItem = calloc(1, sizeof(ColumnItem));
    if (newItem == NULL) {
        logError(_logger, "Failed to allocate memory for ColumnItem");
        return NULL;
    }
    newItem->left = column_name;
    newItem->right = column_type;

    ColumnList *newList = calloc(1, sizeof(ColumnList));
    if (newList == NULL) {
        logError(_logger, "Failed to allocate memory for ColumnList");
        free(newItem);
        return NULL;
    }
    newList->columnListUnion.first.column_item = newItem;

    if (next != NULL) {
        newList->columnListUnion.second.column_list = next->column_list;
    }

    ColumnObject *newColumnObject = calloc(1, sizeof(ColumnObject));
    if (newColumnObject == NULL) {
        logError(_logger, "Failed to allocate memory for ColumnObject");
        free(newItem);
        free(newList);
        return NULL;
    }
    newColumnObject->column_list = newList;
    return newColumnObject;
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

UpdateObject *UpdateObjectSemanticAction(Condition *condition, UpdateObject *update_object) {
	UpdateObject *newUpdateObject = calloc(1, sizeof(UpdateObject));
	if (newUpdateObject == NULL) {
		logError(_logger, "Failed to allocate memory for UpdateObject");
		return NULL;
	}
	newUpdateObject->condition = condition;
	newUpdateObject->condition->operator = E_EQUALS;
	newUpdateObject->next = update_object;
	return newUpdateObject;
}


WhereObject *WhereObjectSemanticAction(Condition *condition, LogOpType logical_op, WhereObject *where_object) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    WhereObject *newWhereObject = calloc(1, sizeof(WhereObject));
    if (newWhereObject == NULL) {
        logError(_logger, "Failed to allocate memory for WhereObject");
        return NULL;
    }
    if (condition != NULL && where_object == NULL) {
        newWhereObject->where_object_union.first.condition = condition;
    }
    else if (condition != NULL && where_object != NULL) {
        newWhereObject->where_object_union.second.condition = condition;
        newWhereObject->where_object_union.second.log_op = logical_op;
        newWhereObject->where_object_union.second.where_object = where_object;
    } else {
        logError(_logger, "Invalid parameters for WhereObjectSemanticAction");
        free(newWhereObject);
        return NULL;
    }
    return newWhereObject;
}




HavingObject *HavingObjectSemanticAction(HavingCondition *having_condition, LogOpType logical_op, HavingObject *having_object) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	HavingObject *newHavingObject = calloc(1, sizeof(HavingObject));
	if (newHavingObject == NULL) {
		logError(_logger, "Failed to allocate memory for HavingObject");
		return NULL;
	}

	if (having_condition != NULL && having_object == NULL) {
		newHavingObject->having_object_union.first.condition = having_condition;
	}
	else if (having_condition != NULL && having_object != NULL) {
		newHavingObject->having_object_union.second.condition = having_condition;
		newHavingObject->having_object_union.second.log_op = logical_op;
		newHavingObject->having_object_union.second.having_object = having_object;
	} else {
		logError(_logger, "Invalid parameters for HavingObjectSemanticAction");
		free(newHavingObject);
		return NULL;
	}

	return newHavingObject;
}

Condition * ConditionSemanticAction(String string, OperatorType operator, Value* value){
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
	newValue->type =  VALUE_TYPE_STRING;
	return newValue;
}

Value * IntegerValueSemanticAction(int integer){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Value * newValue = calloc(1, sizeof(Value));
	newValue->type =  VALUE_TYPE_INTEGER;
	newValue->values.integer = integer;
	return newValue;
}

Value * FloatValueSemanticAction(float float_value){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Value * newValue = calloc(1, sizeof(Value));
	newValue->values.float_value = float_value;
	newValue->type =  VALUE_TYPE_FLOAT;
	return newValue;
}


InsertList * SimpleInsertListSemanticAction(ValueList* value_list){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	InsertList * newInsertList = calloc(1, sizeof(InsertList));
	newInsertList->first.value_list = value_list;
	newInsertList->second.list = NULL; 
	return newInsertList;
}

InsertList * MultipleInsertListSemanticAction(ValueList* value_list, InsertList* list){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	InsertList * newInsertList = calloc(1, sizeof(InsertList));
	newInsertList->second.value_list = value_list;
	newInsertList->second.list = list;
	return newInsertList;
	
}

Array * ArraySemanticAction(String str, Array* string_list){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Array * newStringList = calloc(1, sizeof(Array));
	if(string_list == NULL){
		newStringList->string_list_union.first.string = str;
		return newStringList;
	}else{
		newStringList->string_list_union.second.string = str;
		newStringList->string_list_union.second.string_list = string_list;
		return newStringList;
	}
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

HavingCondition* HavingConditionSemanticAction(AggFuncType agg_func, String string, OperatorType operator, Value* value){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	HavingCondition* newHavingCondition = calloc(1, sizeof(HavingCondition));
	newHavingCondition->string = string;
	newHavingCondition->aggregate_func = agg_func;
	newHavingCondition->operator = operator;
	newHavingCondition->value = value;
	return newHavingCondition;
}


Clause * ClauseSemanticAction(WhereObject* where_object, Array* group_by_column_list, Array* order_by_column_list, HavingObject* having_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Clause* newClause = calloc(1, sizeof(Clause));
	newClause->where_object = where_object;
	newClause->having_object = having_object;
	newClause->group_by_column_list = group_by_column_list;
	newClause->order_by_column_list = order_by_column_list;
	return newClause;
}

Join * JoinSemanticAction(String table_name1, String table_name2, String cond1, String cond2){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Join * newJoin = calloc(1, sizeof(Join));
	newJoin->table_name1 = table_name1;
	newJoin->table_name2 = table_name2;
	newJoin->cond1 = cond1;
	newJoin->cond2 = cond2;
	return newJoin;
}