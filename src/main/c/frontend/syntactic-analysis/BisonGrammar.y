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
	Object* object;
	List* list;
	char* value;
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
%type <object> json_query
%type <object> object
%type <object> content
%type <object> action_object
%type <object> column_object
%type <object> column_value
%type <list> column_list
%type <object> where_object
%type <object> condition
%type <string> table_name
%type <value> value
%type <list> array
%type <token> LOGICAL_OP
%type <token> OPERATOR

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
%%

program:json_query                           { $$ = ProgramSemanticAction(currentCompilerState(), $1); }
				;

/**Que va a la derecha?*/

json_query: action
                |LBRACE action COMMA action RBRACE

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
                COLUMNS COLON column_object
                RBRACE
                RBRACE                              { $$ = CreateActionSemanticAction($7, $11); }
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
            | condition LOGICAL_OP where_object      { $$ = LogicalConditionSemanticAction($1, $2, $3); }
            ;

having_object:
            condition                                { $$ = $1; }
            | condition LOGICAL_OP having_object     { $$ = LogicalConditionSemanticAction($1, $2, $3); }
            ;

condition:
            STRING EQUALS value                      { $$ = ConditionSemanticAction($1, EQUALS, $3); }
            | STRING GREATER_THAN value              { $$ = ConditionSemanticAction($1, GREATER_THAN, $3); }
            | STRING LESS_THAN value                 { $$ = ConditionSemanticAction($1, LESS_THAN, $3); }
            | AGGREGATE_FUNCTION PARENTHESIS_OPEN STRING PARENTHESIS_CLOSE OPERATOR value
                                                    { $$ = AggregateConditionSemanticAction($1, $3, $5, $6); }
            ;

            AGGREGATE_FUNCTION:
                COUNT | SUM | AVG | MAX | MIN            { $$ = $1; }
                ;
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
