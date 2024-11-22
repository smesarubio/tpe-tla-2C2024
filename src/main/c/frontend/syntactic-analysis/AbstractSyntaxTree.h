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

/**

 * Node types for the Abstract Syntax Tree (AST).
 */


typedef enum actionType ActionType;

enum actionType {
    E_CREATE,
    E_SELECT,
    E_SELECT_ALL,
    E_DELETE,
    E_ADD,
    E_UPDATE, 
    E_INSERT
};


enum OperatorType {
    E_EQUALS,
    E_GREATER_THAN,
    E_LESS_THAN
};

enum LogOpType {
    E_NOT,
    E_AND,
    E_OR, 
    E_NONE
};

enum AggFuncType {
    E_COUNT,
    E_SUM,
    E_AVG,
    E_MAX,
    E_MIN
};

typedef enum {
    VALUE_TYPE_STRING,
    VALUE_TYPE_INTEGER,
    VALUE_TYPE_FLOAT
} ValueType;

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
    ActionType type;
    union {
        CreateAction* create_action;
        DeleteAction* delete_action;
        SelectAction* select_action;
        AddAction* add_action;
        UpdateAction* update_action;
        InsertAction* insert_action;
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
    ColumnObject* column_object;
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
            LogOpType log_op;
            WhereObject* where_object;
        } second;
        struct {
            LogOpType log_op;
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
            LogOpType log_op;
            HavingObject* having_object;
        } second;
        struct {
            LogOpType log_op;
            HavingObject* having_object;
        } third;
    } having_object_union;
};

struct HavingCondition {
    String string;
    AggFuncType aggregate_func;
    OperatorType operator;
    Value* value;
};

struct Condition {
    String string;
    OperatorType operator;
    Value* value;
};

struct Value {
    ValueType type;
    union {
        String string;
        int integer;
        float float_value;
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


void releaseProgram(JsonQuery* program);

void releaseInsertAction(InsertAction* insert_action);
void releaseArray(Array* array);
void releaseSelectAction(SelectAction* select_action);
void releaseWhereObject(WhereObject* where_object);
void releaseCondition(Condition* condition);
void releaseValue(Value* value);
void releaseJoin(Join* join);
void releaseInsertList(InsertList* insert_list);
void releaseHavingObject(HavingObject* having_object);
void releaseHavingCondition(HavingCondition* having_condition);
void releaseValueList(ValueList* value_list);
void releaseAction(Action *action);
void releaseCreateAction(CreateAction *create_action);
void releaseDeleteAction(DeleteAction *delete_action);
void releaseAddAction(AddAction *add_action);
void releaseUpdateAction(UpdateAction *update_action);
void releaseColumnObject(ColumnObject *column_object);
void releaseColumnList(ColumnList *column_list);
void releaseColumnItem(ColumnItem *column_item);
void releaseUpdateList(UpdateList *update_list);
void releaseUpdateItems(UpdateItems *update_items);

#endif
