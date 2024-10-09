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
                COLUMNS COLON column_list COMMA
                FROM COLON STRING COMMA
                WHERE COLON where_object COMMA
                GROUP_BY COLON column_list COMMA
                HAVING COLON having_object
                RBRACE
                RBRACE                              { $$ = SelectActionSemanticAction($7, $11, $15, $19, $23); }
                ;

delete_action:
				LBRACE
				DELETE COLON LBRACE
				FROM COLON STRING COMMA
				WHERE COLON where_object
				RBRACE
				RBRACE                              { $$ = DeleteActionSemanticAction($7, $11); }
				;

add_action:
				LBRACE
				ADD COLON LBRACE
				TABLE COLON STRING COMMA
				VALUES COLON array
				RBRACE
				RBRACE                              { $$ = AddActionSemanticAction($7, $11); }
				;

update_action:
                LBRACE
                UPDATE COLON LBRACE
                TABLE COLON STRING COMMA
                SET COLON update_list COMMA
                WHERE COLON where_object
                RBRACE
                RBRACE                              { $$ = UpdateActionSemanticAction($7, $11, $15); }
                ;


column_object:
                LBRACE column_list RBRACE       { $$ = $2; }
                ;

column_list:
                STRING COLON STRING                      { $$ = ColumnDefinitionSemanticAction($1, $3); }
                | STRING COLON STRING COMMA column_list  { $$ = ColumnListSemanticAction($1, $3, $5); }
                ;

column_value:
				STRING COLON LBRACE
				TYPE COLON STRING COMMA
				NAME COLON STRING
				RBRACE                              { $$ = ColumnValueSemanticAction($6, $8); }
				| STRING COLON LBRACE
				TYPE COLON STRING COMMA
				NAME COLON STRING
				RBRACE COMMA column_value           { $$ = ColumnValueListSemanticAction($6, $8, $12); }
				;

update_list:
                LBRACE update_items RBRACE      { $$ = $2; }
                ;

update_items:
                STRING COLON value                       { $$ = UpdateItemSemanticAction($1, $3); }
                | STRING COLON value COMMA update_items  { $$ = UpdateItemListSemanticAction($1, $3, $5); }
                ;


string_list:
				STRING                                   { $$ = ColumnListSemanticAction($1, 1); }
				| STRING COMMA string_list               { $$ = ColumnListAppendSemanticAction($1, $3); }
				;

where_object:
            condition                                { $$ = $1; }
            | condition logical_op where_object      { $$ = LogicalConditionSemanticAction($1, $2, $3); }
            ;

having_object:
            condition                                { $$ = $1; }
            | condition logical_op having_object     { $$ = LogicalConditionSemanticAction($1, $2, $3); }
            ;

condition:
            STRING operator value                      { $$ = ConditionSemanticAction($1, EQUALS, $3); }
            | aggregate_function PARENTHESIS_OPEN STRING PARENTHESIS_CLOSE operatot value
                                                    { $$ = AggregateConditionSemanticAction($1, $3, $5, $6); }
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
				value                                    { $$ = ValueListSemanticAction($1, 1); }
				| value COMMA value_list                 { $$ = ValueListAppendSemanticAction($1, $3); }
				;

%%
