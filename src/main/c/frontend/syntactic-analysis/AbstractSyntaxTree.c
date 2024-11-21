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

void releaseProgram(JsonQuery* program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (program == NULL) return;
    releaseAction(program->query.action);
    free(program);
}


////////////// gpt EMPIEZA
#include "AbstractSyntaxTree.h"



void releaseAction(Action *action) {
    if (action == NULL) return;

    if (action->actions.create_action != NULL) {
        releaseCreateAction(action->actions.create_action);
    } else if (action->actions.select_action != NULL) {
        releaseSelectAction(action->actions.select_action);
    } else if (action->actions.delete_action != NULL) {
        releaseDeleteAction(action->actions.delete_action);
    } else if (action->actions.add_action != NULL) {
        releaseAddAction(action->actions.add_action);
    } else if (action->actions.update_action != NULL) {
        releaseUpdateAction(action->actions.update_action);
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

    free(add_action->table_name);
    if (add_action->array != NULL) {
        releaseValueList(add_action->array);
    }

    free(add_action);
}

void releaseUpdateAction(UpdateAction *update_action) {
    if (update_action == NULL) return;

    free(update_action->table_name);
    if (update_action->update_list != NULL) {
        releaseUpdateList(update_action->update_list);
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
    }
    free(column_object);
}

void releaseColumnList(ColumnList *column_list) {
    if (column_list == NULL) return;

    if (column_list->columnListUnion.second.column_list != NULL) {
        releaseColumnList(column_list->columnListUnion.second.column_list);
    }

    if (column_list->columnListUnion.second.column_item != NULL) {
        releaseColumnItem(column_list->columnListUnion.second.column_item);
    }

    if (column_list->columnListUnion.first.column_item != NULL) {
        releaseColumnItem(column_list->columnListUnion.first.column_item);
    }

    free(column_list);
}

void releaseColumnItem(ColumnItem *column_item) {
    if (column_item == NULL) return;

    free(column_item->left);
    free(column_item->right);

    free(column_item);
}

void releaseUpdateList(UpdateList *update_list) {
    if (update_list == NULL) return;
    if (update_list->update_items != NULL) {
        releaseUpdateItems(update_list->update_items);
    }
    free(update_list);
}

void releaseUpdateItems(UpdateItems *update_items) {
    if (update_items == NULL) return;

    if (update_items->update_items_union.second.update_items != NULL) {
        releaseUpdateItems(update_items->update_items_union.second.update_items);
    }

    if (update_items->update_items_union.first.value != NULL) {
        releaseValue(update_items->update_items_union.first.value);
    }

    free(update_items);
}

////////////// gpt TERMINA


void releaseInsertAction(InsertAction* insert_action) {
	if (insert_action == NULL) return;
	
	free(insert_action->table_name);
	
	if (insert_action->columns != NULL) {
		releaseArray(insert_action->columns); 
	}

	if (insert_action->value_list != NULL) {
		releaseInsertList(insert_action->value_list); 
	}

	free(insert_action);
}

void releaseArray(Array* array) {
	if (array == NULL) return;

	if (array->string_list_union.second.string_list != NULL) {
		releaseArray(array->string_list_union.second.string_list);  
	}

	free(array->string_list_union.first.string);
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

void releaseWhereObject(WhereObject* where_object) {
	if (where_object == NULL) return;

	if (where_object->where_object_union.second.where_object != NULL) {
		releaseWhereObject(where_object->where_object_union.second.where_object);
	}

	if (where_object->where_object_union.second.log_op != NULL) {
		releaseLogOp(where_object->where_object_union.second.log_op);
	}

	if (where_object->where_object_union.second.condition != NULL) {
		releaseCondition(where_object->where_object_union.second.condition);
	}

	free(where_object);
}

void releaseCondition(Condition* condition) {
	if (condition == NULL) return;

	free(condition->string);

	if (condition->operator != NULL) {
		releaseOperator(condition->operator);
	}

	if (condition->value != NULL) {
		releaseValue(condition->value);
	}

	free(condition);
}

void releaseOperator(Operator* operator) {
	if (operator == NULL) {
		return;
	}

	if (operator->operator_type != NULL) {
		free(operator->operator_type);
	}

	free(operator);
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
	if (insert_list == NULL) {
        return;
    }

    if (insert_list->first.value_list != NULL) {
        releaseValueList(insert_list->first.value_list);
    }

    if (insert_list->second.value_list != NULL) {
        releaseValueList(insert_list->second.value_list);
    }

    if (insert_list->second.list != NULL) {
        releaseInsertList(insert_list->second.list);
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

    if (having_object->having_object_union.second.condition != NULL) {
        releaseHavingCondition(having_object->having_object_union.second.condition);
    }

    if (having_object->having_object_union.second.log_op != NULL) {
        releaseLogOp(having_object->having_object_union.second.log_op);
    }

    if (having_object->having_object_union.second.having_object != NULL) {
        releaseHavingObject(having_object->having_object_union.second.having_object);
    }

    if (having_object->having_object_union.third.log_op != NULL) {
        releaseLogOp(having_object->having_object_union.third.log_op);
    }

    if (having_object->having_object_union.third.having_object != NULL) {
        releaseHavingObject(having_object->having_object_union.third.having_object);
    }

    free(having_object);
}

void releaseLogOp(LogOp* log_op) {
	if (log_op == NULL) {
		return;
	}

	if (log_op->log_op_type != NULL) {
        free(log_op->log_op_type);
    }

    free(log_op);
}

void releaseHavingCondition(HavingCondition* having_condition) {
	if (having_condition == NULL) {
        return;
    }

    if (having_condition->string != NULL) {
        free(having_condition->string);
    }

    if (having_condition->aggregate_func != NULL) {
        releaseAggFunc(having_condition->aggregate_func);
    }

    if (having_condition->operator != NULL) {
        releaseOperator(having_condition->operator);
    }

    if (having_condition->value != NULL) {
        releaseValue(having_condition->value);
    }

    free(having_condition);
}

void releaseAggFunc(AggFunc* agg_func) {
	if (agg_func == NULL) {
		return;
	}

	if (agg_func->agg_func_value != NULL) {
        free(agg_func->agg_func_value);
    }

	free(agg_func);
}

void releaseValueList(ValueList* value_list) {
	if (value_list == NULL) {
		return;
	}

	if (value_list->value_list_union.first.value != NULL) {
		releaseValue(value_list->value_list_union.first.value);
	}

	if (value_list->value_list_union.second.value != NULL) {
		releaseValue(value_list->value_list_union.second.value);
	}

	if (value_list->value_list_union.second.value_list != NULL) {
		releaseValueList(value_list->value_list_union.second.value_list);
	}

	free(value_list);
}