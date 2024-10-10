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
typedef struct String String;
typedef struct Integer Integer;
typedef struct Float Float;
typedef struct LogOp LogOp;
typedef struct AggFunc AggFunc;
typedef struct Operator Operator;

typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;
typedef enum OperatorType OperatorType;
typedef enum LogOpType LogOpType;
typedef enum AggFuncType AggFuncType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;

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

struct CreateAction {
    String* table_name;
    ColumnObject* column_object;
};

struct DeleteAction {
    String* table_name; 
    WhereObject* where_object;
};

struct SelectAction {
    ColumnList* table_column_list;
    String* table_name;
    WhereObject* where_objects;
    ColumnList* group_by_column_list;
    HavingObject* having_object;
};

struct AddAction {
    String* table_name;
    Array* array; 
};

struct UpdateAction {
    String* table_name;
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
    String* left;
    String* right;
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
            String* string;
            Value* value;
        } first;
        struct {
            String* string;
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
    AggFunc* aggregate_func;
    Operator* operator;
    Value* value;
};

struct Condition {
    String* string;
    Operator* operator;
    Value* value;
};

struct Value {
    union {
        String* string;
        Integer* integer;
        Float* float_value;
    } values;
};

struct Array {
    ValueList* value_list;    
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

struct Function {
    // Definir los campos aquí
};

struct String {
    char* value;
};

struct Integer {
    int value;
};

struct Float {
    float value;
};

struct LogOp {
    LogOpType* log_op_value;
};

struct AggFunc {
    AggFuncType* agg_func_value;
};

struct Operator {
    OperatorType* operator_type;
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

#endif
