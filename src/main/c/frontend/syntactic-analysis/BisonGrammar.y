%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */
	char* string;
	int integer;
	float float_value;
	Token token;

	/** Non-terminals. */
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
    AggFunc* aggregate_function;
    Operator* operator;
    LogOp* logical_op
}




/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
*/

/** Terminals. */
%token <string> STRING
%token <integer> INTEGER
%token <float> FLOAT
%token <token> SELECT
%token <token> DELETE
%token <token> FROM
%token <token> CREATE
%token <token> ADD
%token <token> WHERE
%token <token> COMMA
%token <token> COLON
%token <token> LBRACE
%token <token> RBRACE
%token <token> DOT
%token <token> BRACKET_OPEN
%token <token> BRACKET_CLOSE
%token <token> EQUALS
%token <token> GREATER_THAN
%token <token> LESS_THAN
%token <token> PARENTHESIS_OPEN
%token <token> PARENTHESIS_CLOSE
%token <token> AND
%token <token> OR
%token <token> GROUP_BY
%token <token> HAVING
%token <token> COUNT
%token <token> SUM
%token <token> AVG
%token <token> MAX
%token <token> MIN

/** Non-terminals. */
%type <json_query> json_query
%type <action> action
%type <create_action> create_action
%type <delete_action> delete_action
%type <select_action> select_action
%type <add_action> add_action
%type <update_action> update_action
%type <column_object> column_object
%type <column_list> column_list
%type <column_value> column_value
%type <update_list> update_list
%type <update_items> update_items
%type <string_list> string_list
%type <where_object> where_object
%type <having_object> having_object
%type <condition> condition
%type <value> value
%type <array> array
%type <value_list> value_list
%type <function> function
%type <operator> operator
%type <aggregate_function> aggregate_function
%type <logical_op> logical_op




/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left OR
%left AND
%left EQUALS GREATER_THAN LESS_THAN
%left ADD SUB
%left MUL DIV

//nuestro simbolo inicial

%start json_query

%%



json_query:     action[act]                                 { $$ = JsonQuerySemanticAction((Action) $act, NULL)}
                |LBRACE action COMMA json_query[query] RBRACE      { $$ = JsonQuerySemanticAction((Action) $act, $query)}

action:     create_action                                { $$ = $1; }
				| select_action                          { $$ = $1; }
				| delete_action                          { $$ = $1; }
				| add_action                             { $$ = $1; }
				| update_action                          { $$ = $1; }
				;

create_action:
                LBRACE
                CREATE COLON LBRACE
                TABLE COLON STRING COMMA
                COLUMNS COLON column_object[col_obj]
                RBRACE
                RBRACE                              { $$ = CreateActionSemanticAction($7, $col_obj); }
                ;

select_action:
				LBRACE
                SELECT COLON LBRACE
                COLUMNS COLON column_list[col_list] COMMA
                FROM COLON STRING COMMA
                WHERE COLON where_object[where_obj] COMMA
                GROUP_BY COLON column_list[col_list] COMMA
                HAVING COLON having_object[hav_obj]
                RBRACE
                RBRACE                              { $$ = SelectActionSemanticAction($col_list, $11, $where_obj, $col_list, $hav_obj); }
                ;

delete_action:
				LBRACE
				DELETE COLON LBRACE
				FROM COLON STRING COMMA
				WHERE COLON where_object[where_obj]
				RBRACE
				RBRACE                              { $$ = DeleteActionSemanticAction($7, $where_obj); }
				;

add_action:
				LBRACE
				ADD COLON LBRACE
				TABLE COLON STRING COMMA
				VALUES COLON array[arr]
				RBRACE
				RBRACE                              { $$ = AddActionSemanticAction($7, $arr); }
				;

update_action:
                LBRACE
                UPDATE COLON LBRACE
                TABLE COLON STRING COMMA
                SET COLON update_list[upd_list] COMMA
                WHERE COLON where_object[where_obj]
                RBRACE
                RBRACE                              { $$ = UpdateActionSemanticAction($7, $upd_list, $where_obj); }
                ;


column_object:
                LBRACE column_list[col_list] RBRACE       { $$ = $col_list; }
                ;

column_list:
                STRING COLON STRING                      { $$ = ColumnListSemanticAction($1, $3, NULL); }
                | STRING COLON STRING COMMA column_list[col_list]  { $$ = ColumnListSemanticAction($1, $3, $col_list); }
                ;

column_value:
				STRING COLON LBRACE
				TYPE COLON STRING COMMA
				NAME COLON STRING
				RBRACE                              { $$ = ColumnValueSemanticAction($6, $8, NULL); }
				| STRING COLON LBRACE
				TYPE COLON STRING[str] COMMA
				NAME COLON STRING
				RBRACE COMMA column_value[col_val]           { $$ = ColumnValueSemanticAction($str, $8, $col_val); }
				;

update_list:
                LBRACE update_items[upd_itmes] RBRACE      { $$ = $upd_itmes; }
                ;

update_items:
                STRING COLON value[val]                                  { $$ = UpdateItemSemanticAction($1, $val, NULL); }
                | STRING COLON value[val] COMMA update_items[upd_itmes]  { $$ = UpdateItemSemanticAction($1, $val, $upd_itmes); }
                ;


string_list:
				STRING                                             { $$ = ColumnListSemanticAction($1, 1); }
				| STRING COMMA string_list[str_list]               { $$ = ColumnListAppendSemanticAction($1, $str_list); }
				;

where_object:
            condition[cond]                                { $$ = $cond; }
            | condition[cond] logical_op[log_op] where_object[where_obj]     { $$ = WhereObjectSemanticAction($cond, $log_op, $where_obj); }
            | NOT where_object[where_obj]                       { $$ = WhereObjectSemanticAction(NULL, NOT, $where_obj); }

            ;

having_object:
            having_condition[hav_con]                                { $$ = $hav_con; }
            | having_condition[hav_con] logical_op[log_op] having_object[hav_obj]     { $$ = HavingObjectSemanticAction($hav_con, $log_op, $hav_obj); }
            | NOT having_object                            { $$ = HavingObjectSemanticAction(NULL, NOT, $3); }
            ;

having_condition: 
            aggregate_function[agg_func] PARENTHESIS_OPEN STRING[str] PARENTHESIS_CLOSE operator[op] value[value]
                                                    { $$ = HavingConditionSemanticAction($agg_func, $str, $op, $value); }
            ;

condition:
            STRING[str] operator value[value]                      { $$ = ConditionSemanticAction($str, EQUALS, $value); }
        
            ;

aggregate_function:
                COUNT | SUM | AVG | MAX | MIN            { $$ = $1; }
                ;


operator: 
                EQUALS | GREATER_THAN | LESS_THAN        {$$ = $1; }

value:
            STRING                                   { $$ = StringValueSemanticAction($1); }
            | INTEGER                                { $$ = IntegerValueSemanticAction($1); }
            | FLOAT                                  { $$ = FloatValueSemanticAction($1); }
            ;

array:
				BRACKET_OPEN value_list BRACKET_CLOSE    { $$ = $2; }
				;

value_list:
				value                                    { $$ = ValueListSemanticAction($1, NULL); }
				| value COMMA value_list                 { $$ = ValueListAppendSemanticAction($1, $3); }
				;
                
logical_op:     AND | OR;          { $$ = $1 }

%%
