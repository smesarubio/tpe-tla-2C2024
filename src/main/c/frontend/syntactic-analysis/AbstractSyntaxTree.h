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


/*
	char* string;
	int integer;
	float float_value;
	Token token;

	JsonQuery * json_query;
    Action * action;
    CreateAction * create_action;
    DeleteAction * delete_action;
    SelectAction * select_action;
    AddAction * add_action;
    UpdateAction * update_action;
    ColumnObject * column_object;
    ColumnList * column_list;
    ColumnValue * column_value;
    UpdateList * update_list;
    UpdateItems * update_items;
    StringList * string_list;
    WhereObject * where_object;
    HavingObject * having_object;
    Condition * condition;
    Value * value;
    Array * array;
    ValueList * value_list;     
    Function * function;

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
	}actions;
} Action ;


typedef struct CreateAction {
	String* table_name;
	ColumnObject* column_object;
} CreateAction;


typedef struct DeleteAction {
    String* table_name; 
	WhereObject* where_object;
} DeleteAction;


typedef struct SelectAction {
    
	ColumnList * table_column_list;
	String * table_name;
	WhereObject * where_objects;
	ColumnList * group_by_column_list;
	HavingObject *  having_object;
	
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
	union ColumnListUnion {
		
		struct clist_first {
			String* left;
			String* right;
		};
		struct clist_second {
			String* left;
			String* right;
			ColumnList* column_list;
		};
	};
} ColumnList;


typedef struct ColumnValue {
    // Definir los campos aquí
} ColumnValue;

typedef struct UpdateList {
    UpdateItems* update_items;
} UpdateList;


typedef struct UpdateItems {
    union UpdateItemsUnion {
		struct clist_first {
			String* string;
			Value* value;
		};
		struct clist_second {
			String* string;
			Value* value;
			UpdateItems* update_items;
		};
	};
} UpdateItems;


typedef struct StringList {
    union StringListUnion {
		struct clist_first {
			String* string;
		};
		struct clist_second {
			String* string;
			StringList* string_list;
		};
	};
} StringList;


typedef struct WhereObject {
    union whereObjectUnion{
		struct clist_first {
			Condition* condition;
		};
		struct clist_second {
			Condition* condition;
			LogOp* log_op;
			WhereObject* where_object;
		};
	};
} WhereObject;


typedef struct HavingObject {
    union havingObjectUnion{
		struct clist_first {
			Condition* condition;
		};
		struct clist_second {
			Condition* condition;
			LogOp* log_op;
			HavingObject* having_object;
		};
	};
} HavingObject;


typedef struct Condition {
    union ConditionUnion{
		struct cond_first {
			String* string;
			Operator* operator;
			Value* value;
		};
		struct cond_second {
			String* string;
			Value* value;
			AggFunc* agregate_function;
		};
	};
} Condition;


typedef struct Value {
	union Values
	{
		String * string;
		Integer * integer;
		Float * float_value
	};
} Value;


typedef struct Array {
	ValueList * value_list;	
} Array;


typedef struct ValueList {
    union ValueListUnion{
		struct clist_first {
			Value* value;
		};
		struct clist_second {
			Value* value;
			ValueList* value_list;
		};
	};
} ValueList;


typedef struct Function {
    // Definir los campos aquí
} Function;


typedef struct String {
	char * value;
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

typedef struct AggFunc{
	AggFuncType agg_func_value
}AggFunc;

typedef struct Operator{
	OperatorType operator_type;
}Operator;

typedef enum OperatorType{
	EQUALS,
	GREATER_THAN,
	LESS_THAN
}OperatorType;

typedef enum LogOpType {
	NOT,
	AND,
	OR
}LogOpType;

typedef enum AggFuncType{
	COUNT,
	SUM,
	AVG,
	MAX,
	MIN
}AggFuncType;



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



// struct Constant {
// 	int value;
// };

// struct Factor {
// 	union {
// 		Constant * constant;
// 		Expression * expression;
// 	};
// 	FactorType type;
// };

// struct Expression {
// 	union {
// 		Factor * factor;
// 		struct {
// 			Expression * leftExpression;
// 			Expression * rightExpression;
// 		};
// 	};
// 	ExpressionType type;
// };

// struct Program {
// 	Expression * expression;
// };



/**
 * Node recursive destructors.
 */
void releaseConstant(Constant * constant);
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseProgram(Program * program);


#endif
