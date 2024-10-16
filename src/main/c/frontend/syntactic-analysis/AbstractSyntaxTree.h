#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef struct JsonQuery JsonQuery;
typedef struct Action Action;
typedef struct CreateAction CreateAction;
typedef struct DeleteAction DeleteAction;
typedef struct SelectAction SelectAction;
typedef struct AddAction AddAction;
typedef struct InsertAction InsertAction;
typedef struct UpdateAction UpdateAction;
typedef struct ColumnObject ColumnObject;
typedef struct ColumnList ColumnList;
typedef struct ColumnItem ColumnItem;
typedef struct ColumnValue ColumnValue;
typedef struct UpdateList UpdateList;
typedef struct UpdateItems UpdateItems;
typedef struct StringList StringList;
typedef struct WhereObject WhereObject;
typedef struct HavingObject HavingObject;
typedef struct HavingCondition HavingCondition;
typedef struct Condition Condition;
typedef struct Value Value;
typedef struct Array Array;
typedef struct ValueList ValueList;
typedef struct Function Function;
// typedef struct Integer Integer;
// typedef struct Float Float;
typedef struct LogOp LogOp;
typedef struct AggFunc AggFunc;
typedef struct Operator Operator;
typedef struct Clause Clause;
typedef struct InsertList InsertList;
typedef struct Join Join;


typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;
typedef enum OperatorType OperatorType;
typedef enum LogOpType LogOpType;
typedef enum AggFuncType AggFuncType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;

typedef char * String;
typedef int Integer;
typedef float Float;


/**

 * Node types for the Abstract Syntax Tree (AST).
 */

struct JsonQuery {
    union {
        Action* action;
        struct {
            Action* action;
            struct JsonQuery* json_query;
        } node;
    } query;
};

struct Action {
    union {
        CreateAction* create_action;
        DeleteAction* delete_action;
        SelectAction* select_action;
        AddAction* add_action;
        UpdateAction* update_action;
    } actions;
};

struct InsertAction {
    String table_name;
    Array* columns;
    InsertList* value_list;
};

struct CreateAction {
    String table_name;
    ColumnObject* column_object;
};

struct DeleteAction {
    String table_name; 
    WhereObject* where_object;
};

struct Clause {
    WhereObject* where_object;
    HavingObject* having_object;
    Array* group_by_column_list;
    Array* order_by_column_list;
};

struct SelectAction {
    Array* table_column_list;
    String table_name;
    WhereObject* where_objects;
    Array* group_by_column_list;
    Array* order_by_column_list;
    HavingObject* having_object;
    Join* join;
};

struct AddAction {
    String table_name;
    ValueList* array; 
};

struct UpdateAction {
    String table_name;
    UpdateList* update_list;
    WhereObject* where_object;
};

struct ColumnObject {
    ColumnList* column_list;
};

struct ColumnList {
    union {
        struct {
            ColumnItem * column_item;
        }first;
        struct {
            ColumnItem * column_item;
            ColumnList * column_list;
        }second;
    } columnListUnion;
};

struct ColumnItem {
    String left;
    String right;
};

struct ColumnValue {
    // Definir los campos aquí
};

struct UpdateList {
    UpdateItems* update_items;
};

struct UpdateItems {
    union {
        struct {
            String string;
            Value* value;
        } first;
        struct {
            String string;
            Value* value;
            UpdateItems* update_items;
        } second;
    } update_items_union;
};


struct WhereObject {
    union {
        struct {
            Condition* condition;
        } first;
        struct {
            Condition* condition;
            LogOp* log_op;
            WhereObject* where_object;
        } second;
        struct {
            LogOp* log_op;
            WhereObject* where_object;
        } third;
    } where_object_union;
};

struct HavingObject {
    union {
        struct {
            HavingCondition* condition;
        } first;
        struct {
            HavingCondition* condition;
            LogOp* log_op;
            HavingObject* having_object;
        } second;
        struct {
            LogOp* log_op;
            HavingObject* having_object;
        } third;
    } having_object_union;
};

struct HavingCondition {
    String string;
    AggFunc* aggregate_func;
    Operator* operator;
    Value* value;
};

struct Condition {
    String string;
    Operator* operator;
    Value* value;
};

struct Value {
    union {
        String string;
        Integer integer;
        Float float_value;
    } values;
};

struct Array {
    union {
        struct {
            String string;
        } first;
        struct {
            String string;
            Array* string_list;
        } second;
    } string_list_union;  
};

struct InsertList{
    union {
        struct {
            ValueList* value_list;
        } first;
        struct {
            ValueList* value_list;
            InsertList* list;
        } second;
    };
};

struct ValueList {
    union {
        struct {
            Value* value;
        } first;
        struct {
            Value* value;
            ValueList* value_list;
        } second;
    } value_list_union;
};

struct Join {
    String table_name1;
    String table_name2;
    String cond1;
    String cond2;
};

struct Function {
    // Definir los campos aquí
};

// struct String {
//     char* value;
// };

// struct Integer {
//     int value;
// };

// struct Float {
//     float value;
// };


struct AggFunc {
    AggFuncType* agg_func_value;
};

struct Operator {
    OperatorType* operator_type;
};

struct LogOp {
    LogOpType * log_op_type;
};



enum OperatorType {
    E_EQUALS,
    E_GREATER_THAN,
    E_LESS_THAN
};

enum LogOpType {
    E_NOT,
    E_AND,
    E_OR
};

enum AggFuncType {
    E_COUNT,
    E_SUM,
    E_AVG,
    E_MAX,
    E_MIN
};

enum ActionType {
    E_CREATE,
    E_SELECT,
    E_DELETE,
    E_ADD,
    E_UPDATE
};

enum FactorType {
    E_CONSTANT,
    E_EXPRESSION
};

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant* constant);
void releaseExpression(Expression* expression);
void releaseFactor(Factor* factor);
void releaseProgram(Program* program);

void releaseInsertAction(InsertAction* insert_action);
void releaseArray(Array* array);
void releaseSelectAction(SelectAction* select_action);
void releaseWhereObject(WhereObject* where_object);
void releaseCondition(Condition* condition);
void releaseOperator(Operator* operator);
void releaseValue(Value* value);
void releaseJoin(Join* join);
void releaseInsertList(InsertList* insert_list);
void releaseHavingObject(HavingObject* having_object);
void releaseLogOp(LogOp* log_op);
void releaseHavingCondition(HavingCondition* having_condition);
void releaseAggFunc(AggFunc* agg_func);
void releaseValueList(ValueList* value_list);

#endif
