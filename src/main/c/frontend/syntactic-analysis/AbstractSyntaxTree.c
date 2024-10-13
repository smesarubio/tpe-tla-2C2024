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

/** PUBLIC FUNCTIONS */

// void releaseConstant(Constant * constant) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (constant != NULL) {
// 		free(constant);
// 	}
// }

// void releaseExpression(Expression * expression) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (expression != NULL) {
// 		switch (expression->type) {
// 			case ADDITION:
// 			case DIVISION:
// 			case MULTIPLICATION:
// 			case SUBTRACTION:
// 				releaseExpression(expression->leftExpression);
// 				releaseExpression(expression->rightExpression);
// 				break;
// 			case FACTOR:
// 				releaseFactor(expression->factor);
// 				break;
// 		}
// 		free(expression);
// 	}
// }

// void releaseFactor(Factor * factor) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (factor != NULL) {
// 		switch (factor->type) {
// 			case CONSTANT:
// 				releaseConstant(factor->constant);
// 				break;
// 			case EXPRESSION:
// 				releaseExpression(factor->expression);
// 				break;
// 		}
// 		free(factor);
// 	}
// }



// void releaseProgram(Program * program) {
// 	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
// 	if (program != NULL) {
// 		releaseExpression(program->expression);
// 		free(program);
// 	}
// }




// void releaseAction(Action* action) {
//     if (action == NULL) return;
    
//     switch (action->type) {
//         case CREATE_ACTION:
//             free(action->action.create.tableName);
//             for (int i = 0; i < action->action.create.columnCount; i++) {
//                 free(action->action.create.columns[i].name);
//             }
//             free(action->action.create.columns);
//             break;
//     }
//     free(action);
// }

// void releaseProgram(Program* program) {
//     if (program == NULL) return;
//     releaseAction(program->action);
//     free(program);
// }




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
	if (operator == NULL) return;
	free(operator->operator_type);
	free(operator);
}

void releaseValue(Value* value) {
	if (value == NULL) return;
	free(value->values.string);  
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
