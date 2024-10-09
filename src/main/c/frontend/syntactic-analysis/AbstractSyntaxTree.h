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

typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

typedef struct JsonQuery {
    union {
        Action* action;
        struct {
            Action* action;
            struct JsonQuery* json_query;
        } node;
    } query;
} JsonQuery;

typedef struct Action {
    union {
        CreateAction* create_action;
        DeleteAction* delete_action;
        SelectAction* select_action;
        AddAction* add_action;
        UpdateAction* update_action;
    } actions;
} Action;

typedef struct CreateAction {
    String* table_name;
    ColumnObject* column_object;
} CreateAction;

typedef struct DeleteAction {
    String* table_name; 
    WhereObject* where_object;
} DeleteAction;

typedef struct SelectAction {
    ColumnList* table_column_list;
    String* table_name;
    WhereObject* where_objects;
    ColumnList* group_by_column_list;
    HavingObject* having_object;
} SelectAction;

typedef struct AddAction {
    String* table_name;
    Array* array; 
} AddAction;

typedef struct UpdateAction {
    String* table_name;
    UpdateList* update_list;
    WhereObject* where_object;
} UpdateAction;

typedef struct ColumnObject {
    ColumnList* column_list;
} ColumnObject;

typedef struct ColumnList {
    union {
        struct {
            String* left;
            String* right;
        } first;
        struct {
            String* left;
            String* right;
            struct ColumnList* column_list;
        } second;
    } u;
} ColumnList;

typedef struct ColumnValue {
    // Definir los campos aquí
} ColumnValue;

typedef struct UpdateList {
    UpdateItems* update_items;
} UpdateList;

typedef struct UpdateItems {
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
} UpdateItems;

typedef struct StringList {
    union {
        struct {
            String* string;
        } first;
        struct {
            String* string;
            StringList* string_list;
        } second;
    } string_list_union;
} StringList;

typedef struct WhereObject {
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
        }third;
    } where_object_union;
} WhereObject;

typedef struct HavingObject {
    union {
        struct {
            Condition* condition;
        } first;
        struct {
            Condition* condition;
            LogOp* log_op;
            HavingObject* having_object;
        } second;
        struct {
            LogOp* log_op;
            HavingObject* having_object;
        }third;
    } having_object_union;
} HavingObject;

typedef struct HavingCondition{
    AggFunc* aggregate_func;
    Operator* operator;
    Value* value;
}HavingCondition;

typedef struct Condition {
    String* string;
    Operator* operator;
    Value* value;
} Condition;

typedef struct Value {
    union {
        String* string;
        Integer* integer;
        Float* float_value;
    } values;
} Value;

typedef struct Array {
    ValueList* value_list;    
} Array;

typedef struct ValueList {
    union {
        struct {
            Value* value;
        } first;
        struct {
            Value* value;
            ValueList* value_list;
        } second;
    } value_list_union;
} ValueList;

typedef struct Function {
    // Definir los campos aquí
} Function;

typedef struct String {
    char* value;
} String;

typedef struct Integer {
    int value;
} Integer;

typedef struct Float {
    float value;
} Float;

typedef struct LogOp {
    LogOpType log_op_value;
} LogOp;

typedef struct AggFunc {
    AggFuncType agg_func_value;
} AggFunc;

typedef struct Operator {
    OperatorType operator_type;
} Operator;

typedef enum OperatorType {
    EQUALS,
    GREATER_THAN,
    LESS_THAN
} OperatorType;

typedef enum LogOpType {
    NOT,
    AND,
    OR
} LogOpType;

typedef enum AggFuncType {
    COUNT,
    SUM,
    AVG,
    MAX,
    MIN
} AggFuncType;

enum ActionType {
    CREATE,
    SELECT,
    DELETE,
    ADD,
    UPDATE
};

enum FactorType {
    CONSTANT,
    EXPRESSION
};

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant* constant);
void releaseExpression(Expression* expression);
void releaseFactor(Factor* factor);
void releaseProgram(Program* program);

#endif
