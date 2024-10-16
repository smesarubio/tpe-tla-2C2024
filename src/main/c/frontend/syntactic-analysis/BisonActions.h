#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

// Constant * IntegerConstantSemanticAction(const int value);
// Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
// Expression * FactorExpressionSemanticAction(Factor * factor);
// Factor * ConstantFactorSemanticAction(Constant * constant);
// Factor * ExpressionFactorSemanticAction(Expression * expression);
// Program * ExpressionProgramSemanticAction(CompilerState * compilerState, Expression * expression);

JsonQuery * JsonQuerySemanticAction(CompilerState * compilerState, Action * action, JsonQuery * jsonQuery);
InsertAction * InsertActionSemanticAction(String table_name, Array* columns, InsertList* value_list);
CreateAction * CreateActionSemanticAction(String table_name, ColumnObject* col_object);
UpdateAction * UpdateActionSemanticAction(String table_name, UpdateList* update_list, WhereObject* where_object);
AddAction * AddActionSemanticAction(String table_name, ValueList* array);
DeleteAction * DeleteActionSemanticAction(String table_name, WhereObject* where_object);
SelectAction* SelectActionSemanticAction(Array* table_column_list, String table_name, WhereObject* where_object, Array* groupby_column_list, Array* order_by_column_list, HavingObject* having_object, Join* join);
SelectAction* SelectAllActionSemanticAction(String table_name);
ColumnList * ColumnListSemanticAction(ColumnItem* column_item,  ColumnList* column_list);
ColumnItem * ColumnItemSemanticAction(String left, String right);
UpdateItems * UpdateItemSemanticAction(String string, Value * value, UpdateItems * updateItems);
WhereObject * WhereObjectSemanticAction(Condition * condition, LogOp* logical_op, WhereObject* where_object);
HavingObject * HavingObjectSemanticAction(HavingCondition* having_condition, LogOp* logical_op, HavingObject* having_object);
Condition * ConditionSemanticAction(String string, Operator* operator, Value* value);
Value * StringValueSemanticAction(String string);
Value * IntegerValueSemanticAction(Integer integer);
Value * FloatValueSemanticAction(Float float_value);
ValueList * ValueListSemanticAction(Value* value, ValueList* value_list);
Array * ArraySemanticAction(String str, Array* string_list);
HavingCondition* HavingConditionSemanticAction(AggFunc * agg_func, String string, Operator* operator, Value* value);
LogOp *LogOpSemanticAction(LogOpType logOpType);
Clause * ClauseSemanticAction(WhereObject* where_object, Array* group_by_column_list, Array* order_by_column_list, HavingObject* having_object);
InsertList * SimpleInsertListSemanticAction(ValueList* value_list);
InsertList * MultipleInsertListSemanticAction(ValueList* value_list, InsertList* list);
Join * JoinSemanticAction(String table_name1, String table_name2, String cond1, String cond2);

#endif
