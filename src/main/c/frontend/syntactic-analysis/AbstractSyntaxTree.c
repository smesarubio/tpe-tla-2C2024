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
    if (program->query.action != NULL) {
        releaseAction(program->query.action);
    }
    // if (program->query.node.action != NULL) {
    //     releaseAction(program->query.node.action);
    // }
    logDebugging(_logger, "Releasing program memory");
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

    // Free table_name if allocated
    if (add_action->table_name != NULL) {
        free(add_action->table_name);
        add_action->table_name = NULL; // Prevent double free
    }

    // Release column_object if allocated
    if (add_action->column_object != NULL) {
        releaseColumnObject(add_action->column_object);
        add_action->column_object = NULL; // Prevent double free
    }

    free(add_action);
    add_action = NULL; // Avoid dangling pointer
}
void releaseUpdateObject(UpdateObject* UpdateObject){
    if (UpdateObject == NULL) return;

    if (UpdateObject->update_items_union.first.condition != NULL) {
        releaseCondition(UpdateObject->update_items_union.first.condition);
    }

    // if (UpdateObject->update_items_union.second.condition != NULL) {
    //     releaseCondition(UpdateObject->update_items_union.second.condition);
    // }

    // if (UpdateObject->update_items_union.second.update_object != NULL) {
    //     releaseUpdateObject(UpdateObject->update_items_union.second.update_object);
    // }
    free(UpdateObject);

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

    // Release column_list if allocated
    if (column_object->column_list != NULL) {
        releaseColumnList(column_object->column_list);
        column_object->column_list = NULL; // Prevent double free
    }

    free(column_object);
    column_object = NULL; // Avoid dangling pointer
}



void releaseColumnList(ColumnList *column_list) {
    if (column_list == NULL) return;

    // Free current column_item
    if (column_list->columnListUnion.first.column_item != NULL) {
        releaseColumnItem(column_list->columnListUnion.first.column_item);
        column_list->columnListUnion.first.column_item = NULL; // Prevent double free
    }

    // Recursively release the next column_list
    if (column_list->columnListUnion.second.column_list != NULL) {
        releaseColumnList(column_list->columnListUnion.second.column_list);
        column_list->columnListUnion.second.column_list = NULL; // Prevent double free
    }

    free(column_list);
    column_list = NULL; // Avoid dangling pointer
}



void releaseColumnItem(ColumnItem *column_item) {
    if (column_item == NULL) return;

    // Free left string
    if (column_item->left != NULL) {
        free(column_item->left);
        column_item->left = NULL; // Prevent double free
    }

    // Free right string
    if (column_item->right != NULL) {
        free(column_item->right);
        column_item->right = NULL; // Prevent double free
    }

    free(column_item);
    column_item = NULL; // Avoid dangling pointer
}




void releaseInsertAction(InsertAction* insert_action) {
    if (insert_action == NULL) return;

    // Free the table_name
    if (insert_action->table_name != NULL) {
        free(insert_action->table_name);
        insert_action->table_name = NULL;
    }

    // Release the columns array
    if (insert_action->columns != NULL) {
        releaseArray(insert_action->columns);
        insert_action->columns = NULL;
    }

    // Release the value list
    if (insert_action->value_list != NULL) {
        releaseInsertList(insert_action->value_list);
        insert_action->value_list = NULL;
    }

    free(insert_action);
}


void releaseArray(Array* array) {
    if (array == NULL) return;

    // Free the nested array if it exists
    if (array->string_list_union.second.string_list != NULL) {
        releaseArray(array->string_list_union.second.string_list);
        array->string_list_union.second.string_list = NULL;
    }

    // Free the current string
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

    // Release the first case: a single condition
    if (where_object->where_object_union.first.condition != NULL) {
        releaseCondition(where_object->where_object_union.first.condition);
        where_object->where_object_union.first.condition = NULL;
    }
    // Release the second case: a condition, log_op, and nested where_object
    if (where_object->where_object_union.second.condition != NULL) {
        releaseCondition(where_object->where_object_union.second.condition);
        where_object->where_object_union.second.condition = NULL;
    }
    if (where_object->where_object_union.second.where_object != NULL) {
        releaseWhereObject(where_object->where_object_union.second.where_object);
        where_object->where_object_union.second.where_object = NULL;
    }
    // TODO:
    // Release the third case: only a log_op and nested where_object
    // if (where_object->where_object_union.third.where_object != NULL) {
    //     logCritical(_logger, "que hace aca");
    //     releaseWhereObject(where_object->where_object_union.third.where_object);
    //     where_object->where_object_union.third.where_object = NULL;
    // }  
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

	free(join->table_name1);
	free(join->table_name2);
	free(join->cond1);
	free(join->cond2);

	free(join);
}

void releaseInsertList(InsertList* insert_list) {
    if (insert_list == NULL) return;

    // Release the current value list
    if (insert_list->first.value_list != NULL) {
        releaseValueList(insert_list->first.value_list);
        insert_list->first.value_list = NULL;
    }

    // Recursively release the next list
    if (insert_list->second.list != NULL) {
        releaseInsertList(insert_list->second.list);
        insert_list->second.list = NULL;
    }

    free(insert_list);
}


void releaseHavingObject(HavingObject* having_object) {
	if (having_object == NULL) {
        return;
    }

    if (having_object->having_object_union.first.condition != NULL) {
        releaseHavingCondition(having_object->having_object_union.first.condition);
    }

    // if (having_object->having_object_union.second.condition != NULL) {
    //     releaseHavingCondition(having_object->having_object_union.second.condition);
    // }
    // if (having_object->having_object_union.second.having_object != NULL) {
    //     releaseHavingObject(having_object->having_object_union.second.having_object);
    // }

    // if (having_object->having_object_union.third.having_object != NULL) {
    //     releaseHavingObject(having_object->having_object_union.third.having_object);
    // }

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

    // Release the current value
    if (value_list->value_list_union.first.value != NULL) {
        free(value_list->value_list_union.first.value); // Assuming Value is dynamically allocated
        value_list->value_list_union.first.value = NULL;
    }

    // Recursively release the next value list
    if (value_list->value_list_union.second.value_list != NULL) {
        releaseValueList(value_list->value_list_union.second.value_list);
        value_list->value_list_union.second.value_list = NULL;
    }

    free(value_list);
}
