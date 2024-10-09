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
/*
Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->value = value;
	return constant;
}*/

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

Action* SelectActionSemanticAction(ColumnList* table_column_list, String* table_name, WhereObject* where_object, ColumnList* groupby_column_list,HavingObject* having_object){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Action* newAction = calloc(1, sizeof(Action));
	SelectAction* newSelectAction = calloc(1, sizeof(SelectAction));

	newSelectAction->group_by_column_list = groupby_column_list;
	newSelectAction->having_object = having_object;
	newSelectAction->table_column_list = table_column_list;
	newSelectAction->table_name = table_name;
	newSelectAction->where_objects = where_object;
	newAction->actions.select_action = newSelectAction;
	return newAction;
}


ColumnList * ColumnListSemanticAction(String* left, String* right, ColumnList* column_list){
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ColumnList * newColumnList = calloc(1, sizeof(ColumnList));
	if (column_list == NULL)
	{
		newColumnList->u.first.left = left;
		newColumnList->u.first.right = right;
		return newColumnList;
	}else{
		newColumnList->u.second.left = left;
		newColumnList->u.second.right = right;
		newColumnList->u.second.column_list = column_list;
		return newColumnList;
	}
}


UpdateItems * UpdateItemSemanticAction(String * string, Value * value, UpdateItems * updateItems ){
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

WhereObject * WhereObjectSemanticAction(Condition * condition, ){

}


HavingObject * HavingObjectSemanticAction(AggFunc * func, String * string, ,Value * value){

}