#include "AbstractSyntaxTree.h"

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

void releaseProgram(JsonQuery *program) {
    if (program == NULL) {
        logDebugging(_logger, "releaseProgram: Program is NULL");
        return;
    }

    logDebugging(_logger, "Releasing program actions");
    while(program->next != NULL) {
        JsonQuery *next = program->next;
        releaseAction(program->action);
        free(program);
        program = next;
    }
    logDebugging(_logger, "Releasing program memory");
    releaseAction(program->action);
    free(program);

    logDebugging(_logger, "Program successfully released");
}


void releaseAction(Action *action) {
    if (action == NULL) return;
    switch (action->type) {
        case E_CREATE:
            if (action->actions.create_action != NULL) {
                releaseCreateAction(action->actions.create_action);
            }
            break;

        case E_SELECT:
            if (action->actions.select_action != NULL) {
                releaseSelectAction(action->actions.select_action);
            }
            break;

        case E_DELETE:
            if (action->actions.delete_action != NULL) {
                releaseDeleteAction(action->actions.delete_action);
            }
            break;

        case E_ADD:
            if (action->actions.add_action != NULL) {
                releaseAddAction(action->actions.add_action);
            }
            break;

        case E_UPDATE:
            if (action->actions.update_action != NULL) {
                releaseUpdateAction(action->actions.update_action);
            }
            break;

        case E_INSERT:
            if (action->actions.insert_action != NULL) {
                releaseInsertAction(action->actions.insert_action);
            }
            break;

        default:
            logError(_logger, "Unknown action type: %d", action->type);
            break;
    }
    free(action);
}


void releaseCreateAction(CreateAction *create_action) {
    if (create_action == NULL) return;

    free(create_action->table_name);
    if (create_action->column_object != NULL) {
        releaseColumnObject(create_action->column_object);
    }

    free(create_action);
}

void releaseDeleteAction(DeleteAction *delete_action) {
    if (delete_action == NULL) return;

    free(delete_action->table_name);
    if (delete_action->where_object != NULL) {
        releaseWhereObject(delete_action->where_object);
    }

    free(delete_action);
}

void releaseAddAction(AddAction *add_action) {
    if (add_action == NULL) return;

    if (add_action->table_name != NULL) {
        free(add_action->table_name);
        add_action->table_name = NULL;
    }

    if (add_action->column_object != NULL) {
        releaseColumnObject(add_action->column_object);
        add_action->column_object = NULL; 
    }

    free(add_action);
    add_action = NULL; 
}

void releaseUpdateObject(UpdateObject* updateObject){
    if (updateObject == NULL) return;

    if (updateObject->condition != NULL) {
        releaseCondition(updateObject->condition);
    }

    if (updateObject->next != NULL) {
        releaseUpdateObject(updateObject->next);
    }

    free(updateObject);
}

void releaseUpdateAction(UpdateAction *update_action) {
    if (update_action == NULL) return;
    free(update_action->table_name);
    if (update_action->update_object != NULL) {
        releaseUpdateObject(update_action->update_object);
    }
    if (update_action->where_object != NULL) {
        releaseWhereObject(update_action->where_object);
    }

    free(update_action);
}

void releaseColumnObject(ColumnObject *column_object) {
    if (column_object == NULL) return;

    if (column_object->column_list != NULL) {
        releaseColumnList(column_object->column_list);
        column_object->column_list = NULL; 
    }

    free(column_object);
    column_object = NULL; 
}



void releaseColumnList(ColumnList *column_list) {
    if (column_list == NULL) return;

    if (column_list->columnListUnion.first.column_item != NULL) {
        releaseColumnItem(column_list->columnListUnion.first.column_item);
        column_list->columnListUnion.first.column_item = NULL; 
    }

    if (column_list->columnListUnion.second.column_list != NULL) {
        releaseColumnList(column_list->columnListUnion.second.column_list);
        column_list->columnListUnion.second.column_list = NULL;
    }

    free(column_list);
    column_list = NULL; 
}


void releaseColumnItem(ColumnItem *column_item) {
    if (column_item == NULL) return;

    if (column_item->left != NULL) {
        free(column_item->left);
        column_item->left = NULL;
    }

    if (column_item->right != NULL) {
        free(column_item->right);
        column_item->right = NULL;
    }

    free(column_item);
    column_item = NULL; 
}

void releaseInsertAction(InsertAction* insert_action) {
    if (insert_action == NULL) return;

    if (insert_action->table_name != NULL) {
        free(insert_action->table_name);
        insert_action->table_name = NULL;
    }

    if (insert_action->columns != NULL) {
        releaseArray(insert_action->columns);
        insert_action->columns = NULL;
    }

    if (insert_action->value_list != NULL) {
        releaseInsertList(insert_action->value_list);
        insert_action->value_list = NULL;
    }

    free(insert_action);
}


void releaseArray(Array* array) {
    if (array == NULL) return;
    if (array->string_list_union.second.string_list != NULL) {
        releaseArray(array->string_list_union.second.string_list);
        array->string_list_union.second.string_list = NULL;
    }
    if (array->string_list_union.first.string != NULL) {
        free(array->string_list_union.first.string);
        array->string_list_union.first.string = NULL;
    }
    free(array);
}


void releaseSelectAction(SelectAction* select_action) {
	if (select_action == NULL) return;

	if (select_action->table_column_list != NULL) {
		releaseArray(select_action->table_column_list);  
	}

	free(select_action->table_name);

	if (select_action->where_objects != NULL) {
		releaseWhereObject(select_action->where_objects);
	}

	if (select_action->group_by_column_list != NULL) {
		releaseArray(select_action->group_by_column_list);
	}

	if (select_action->order_by_column_list != NULL) {
		releaseArray(select_action->order_by_column_list);
	}

	if (select_action->having_object != NULL) {
		releaseHavingObject(select_action->having_object);
	}

	if (select_action->join != NULL) {
		releaseJoin(select_action->join);
	}

	free(select_action);
}

void releaseWhereObject(WhereObject *where_object) {
    if (where_object == NULL) return;

    if (where_object->where_object_union.first.condition != NULL) {
        releaseCondition(where_object->where_object_union.first.condition);
        where_object->where_object_union.first.condition = NULL;
    }
    if (where_object->where_object_union.second.condition != NULL) {
        releaseCondition(where_object->where_object_union.second.condition);
        where_object->where_object_union.second.condition = NULL;
    }
    if (where_object->where_object_union.second.where_object != NULL) {
        releaseWhereObject(where_object->where_object_union.second.where_object);
        where_object->where_object_union.second.where_object = NULL;
    }
    free(where_object);
}

void releaseCondition(Condition* condition) {
	if (condition == NULL) {
        return;
    }
	free(condition->string);
	if (condition->value->type == VALUE_TYPE_STRING) {
		releaseValue(condition->value);
	}
	free(condition);
}


void releaseValue(Value* value) {
if (value == NULL) {
		return;
	}
	if (value->values.string != NULL) {
		free(value->values.string);
	}
	free(value);
}

void releaseJoin(Join* join) {
	if (join == NULL) return;
	releaseCondition(join->cond1);
	releaseCondition(join->cond2);
	free(join);
}

void releaseInsertList(InsertList* insert_list) {
    if (insert_list == NULL) return;
    if (insert_list->first.value_list != NULL) {
        releaseValueList(insert_list->first.value_list);
        insert_list->first.value_list = NULL;
    }
    if (insert_list->second.list != NULL) {
        releaseInsertList(insert_list->second.list);
        insert_list->second.list = NULL;
    }
    free(insert_list);
}


void releaseHavingObject(HavingObject* having_object) {
    if (having_object == NULL) return;

    if (having_object->having_object_union.first.condition != NULL) {
        releaseHavingCondition(having_object->having_object_union.first.condition);
        having_object->having_object_union.first.condition = NULL;
    }
    if (having_object->having_object_union.second.condition != NULL) {
        releaseHavingCondition(having_object->having_object_union.second.condition);
        having_object->having_object_union.second.condition = NULL;
    }
    if (having_object->having_object_union.second.having_object != NULL) {
        releaseHavingObject(having_object->having_object_union.second.having_object);
        having_object->having_object_union.second.having_object = NULL;
    }
    free(having_object);
}

void releaseHavingCondition(HavingCondition* having_condition) {
	if (having_condition == NULL) return;

	free(having_condition->string);

	if (having_condition->value->type == VALUE_TYPE_STRING) {
		releaseValue(having_condition->value);
	}
	free(having_condition);
}

void releaseValueList(ValueList* value_list) {
    if (value_list == NULL) return;

    if (value_list->value_list_union.first.value != NULL) {
        value_list->value_list_union.first.value = NULL;
    }

    if (value_list->value_list_union.second.value_list != NULL) {
        releaseValueList(value_list->value_list_union.second.value_list);
        value_list->value_list_union.second.value_list = NULL;
    }

    free(value_list);
}
