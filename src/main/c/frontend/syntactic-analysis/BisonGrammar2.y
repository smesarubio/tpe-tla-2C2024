%{
#include "BisonActions.h"
%}

%define api.value.union.name SemanticValue

%union {
    /** Terminals. */
    char* string;
    int integer;
    float float_val;
    
    /** Non-terminals. */
    Action* action;
    Program* program;
    Column* column;
    Condition* condition;
    Value* value;
    char** string_array;
    int array_size;
}

/** Terminals. */
%token <string> STRING
%token <integer> INTEGER
%token <float_val> FLOAT
%token <string> RESERVED

%token CREATE SELECT DELETE ADD WHERE FROM TABLE COLUMNS VALUES AND OR

%token COLON COMMA OPEN_BRACE CLOSE_BRACE OPEN_BRACKET CLOSE_BRACKET
%token EQUALS GT LT

%token UNKNOWN

/** Non-terminals. */
%type <program> program
%type <action> action
%type <action> create_action select_action delete_action add_action
%type <column> column_object column_value
%type <condition> where_object condition
%type <value> value
%type <string_array> column_list
%type <array_size> array

%%

program: action                              { $$ = ProgramSemanticAction(currentCompilerState(), $1); }
    ;

action: create_action                        { $$ = $1; }
    | select_action                          { $$ = $1; }
    | delete_action                          { $$ = $1; }
    | add_action                             { $$ = $1; }
    ;

create_action: 
    OPEN_BRACE
    CREATE COLON OPEN_BRACE
    TABLE COLON STRING COMMA
    COLUMNS COLON column_object
    CLOSE_BRACE
    CLOSE_BRACE                              { $$ = CreateActionSemanticAction($7, $11); }
    ;

select_action:
    OPEN_BRACE
    SELECT COLON OPEN_BRACE
    COLUMNS COLON column_list COMMA
    FROM COLON STRING COMMA
    WHERE COLON where_object
    CLOSE_BRACE
    CLOSE_BRACE                              { $$ = SelectActionSemanticAction($7, $11, $15); }
    ;

delete_action:
    OPEN_BRACE
    DELETE COLON OPEN_BRACE
    FROM COLON STRING COMMA
    WHERE COLON where_object
    CLOSE_BRACE
    CLOSE_BRACE                              { $$ = DeleteActionSemanticAction($7, $11); }
    ;

add_action:
    OPEN_BRACE
    ADD COLON OPEN_BRACE
    TABLE COLON STRING COMMA
    VALUES COLON array
    CLOSE_BRACE
    CLOSE_BRACE                              { $$ = AddActionSemanticAction($7, $11); }
    ;

column_object:
    OPEN_BRACE column_value CLOSE_BRACE      { $$ = $2; }
    ;

column_value:
    STRING COLON RESERVED                    { $$ = ColumnValueSemanticAction($1, $3); }
    | STRING COLON RESERVED COMMA column_value { $$ = ColumnValueListSemanticAction($1, $3, $5); }
    ;

column_list:
    OPEN_BRACKET STRING CLOSE_BRACKET        { $$ = ColumnListSemanticAction($2, 1); }
    | OPEN_BRACKET string_list CLOSE_BRACKET { $$ = $2; }
    ;

string_list:
    STRING                                   { $$ = ColumnListSemanticAction($1, 1); }
    | STRING COMMA string_list               { $$ = ColumnListAppendSemanticAction($1, $3); }
    ;

where_object:
    condition                                { $$ = $1; }
    | condition logical_op where_object      { $$ = LogicalConditionSemanticAction($1, $2, $3); }
    ;

condition:
    STRING EQUALS value                      { $$ = ConditionSemanticAction($1, EQUALS, $3); }
    | STRING GT value                        { $$ = ConditionSemanticAction($1, GT, $3); }
    | STRING LT value                        { $$ = ConditionSemanticAction($1, LT, $3); }
    ;

logical_op:
    AND                                      { $$ = AND; }
    | OR                                     { $$ = OR; }
    ;

value:
    RESERVED                                 { $$ = ReservedValueSemanticAction($1); }
    | STRING                                 { $$ = StringValueSemanticAction($1); }
    | INTEGER                                { $$ = IntegerValueSemanticAction($1); }
    | FLOAT                                  { $$ = FloatValueSemanticAction($1); }
    ;

array:
    OPEN_BRACKET value_list CLOSE_BRACKET    { $$ = $2; }
    ;

value_list:
    value                                    { $$ = ValueListSemanticAction($1, 1); }
    | value COMMA value_list                 { $$ = ValueListAppendSemanticAction($1, $3); }
    ;

%%