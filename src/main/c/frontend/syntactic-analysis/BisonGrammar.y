%{

#include "BisonActions.h"
#include "AbstractSyntaxTree.h"
%}

%code requires{

    #include "../../shared/Type.h"
}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */
	String string;
	int integer;
	float float_value;
	Token token;
    OperatorType operator;
    AggFuncType aggregate_function;
    LogOpType logical_op;
	/** Non-terminals. */
	JsonQuery * json_query;
    Action * action;
    InsertAction * insert_action;
    CreateAction * create_action;
    DeleteAction * delete_action;
    SelectAction * select_action;
    AddAction * add_action;
    UpdateAction * update_action;
    ColumnObject * column_object;
    UpdateObject * update_object;
    WhereObject * where_object;
    HavingObject * having_object;
    Condition * condition;
    Value * value;
    Array * string_list;
    ValueList * value_list;   
    Function * function;
    HavingCondition* having_condition;
    Array* array;
    Clause* clause;
    InsertList * insert_list;
    Join* join;
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

%destructor { releaseInsertAction($$); } <insert_action>
%destructor { releaseArray($$); } <array>
%destructor { releaseSelectAction($$); } <select_action>
%destructor { releaseWhereObject($$); } <where_object>
%destructor { releaseCondition($$); } <condition>
%destructor { releaseValue($$); } <value>
%destructor { releaseColumnObject($$); } <column_object>
%destructor { releaseCreateAction($$); } <create_action>
%destructor { releaseDeleteAction($$); } <delete_action>
%destructor { releaseAddAction($$); } <add_action>
%destructor { releaseUpdateObject($$); } <update_object>
%destructor { releaseUpdateAction($$); } <update_action>
%destructor { releaseInsertList($$); } <insert_list>
%destructor { releaseHavingObject($$); } <having_object>
%destructor { releaseHavingCondition($$); } <having_condition>
%destructor { releaseValueList($$); } <value_list>

/** Terminals. */
%token <string> STRING
%token <integer> INTEGER
%token <float_value> FLOAT
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
%token <token> BRACKET_OPEN
%token <token> BRACKET_CLOSE
%token <token> EQUALS
%token <token> GREATER_THAN
%token <token> LESS_THAN
%token <token> AND
%token <token> OR
%token <token> GROUP_BY
%token <token> ORDER_BY
%token <token> HAVING
%token <token> COUNT
%token <token> SUM
%token <token> AVG
%token <token> MAX
%token <token> MIN
%token <token> TABLE
%token <token> COLUMNS
%token <token> UPDATE
%token <token> VALUES
%token <token> SET
%token <token> NOT
%token <token> INSERT
%token <token> all
%token <token> ON
%token <token> JOIN
%token <token> NUL



/** Non-terminals. */
%type <json_query> json_query
%type <action> action
%type <insert_action> insert_action
%type <create_action> create_action
%type <delete_action> delete_action
%type <select_action> select_action
%type <add_action> add_action
%type <update_action> update_action
%type <column_object> column_object
%type <update_object> update_object
%type <where_object> where_object
%type <having_object> having_object
%type <condition> condition
%type <value> value
%type <value_list> value_list
%type <operator> operator
%type <aggregate_function> aggregate_function
%type <logical_op> logical_op
%type <having_condition> having_condition
%type <string_list> string_list
%type <where_object> where_clause
%type <array> group_by_clause
%type <array> order_by_clause
%type <having_object> having_clause
%type <clause> clause
%type <insert_list> insert_list

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left COMMA
%left WHERE 
%left GROUP_BY 
%left ORDER_BY 
%left HAVING
%left OR
%left AND
%left EQUALS GREATER_THAN LESS_THAN

//nuestro simbolo inicial

%start json_query

%%



json_query:     action[act]                                 { $$ = JsonQuerySemanticAction( currentCompilerState(), $act, NULL);}
                |LBRACE action[act] COMMA json_query[query] RBRACE      { $$ = JsonQuerySemanticAction(currentCompilerState(), $act, $query);}

action: create_action       { $$ = (Action *) malloc(sizeof(Action)); $$->type = E_CREATE; $$->actions.create_action = $1; }
      | select_action       { $$ = (Action *) malloc(sizeof(Action)); $$->type = E_SELECT; $$->actions.select_action = $1; }
      | delete_action       { $$ = (Action *) malloc(sizeof(Action)); $$->type = E_DELETE; $$->actions.delete_action = $1; }
      | add_action          { $$ = (Action *) malloc(sizeof(Action)); $$->type = E_ADD; $$->actions.add_action = $1; }
      | update_action       { $$ = (Action *) malloc(sizeof(Action)); $$->type = E_UPDATE; $$->actions.update_action = $1; }
      | insert_action       { $$ = (Action *) malloc(sizeof(Action)); $$->type = E_INSERT; $$->actions.insert_action = $1; }
;



insert_action:
                LBRACE
                INSERT COLON STRING[str] COMMA
                COLUMNS COLON BRACKET_OPEN string_list[col_list] BRACKET_CLOSE COMMA
                VALUES COLON BRACKET_OPEN  insert_list[arr_list]  BRACKET_CLOSE
                RBRACE
                                              { $$ = InsertActionSemanticAction($str, $col_list, $arr_list); }

create_action:
                LBRACE
                CREATE COLON LBRACE
                TABLE COLON STRING COMMA
                COLUMNS COLON LBRACE column_object[col_obj] RBRACE
                RBRACE
                RBRACE                              { $$ = CreateActionSemanticAction($7, $col_obj); }
                ;

select_action:
    LBRACE SELECT COLON BRACKET_OPEN string_list[col_list] BRACKET_CLOSE COMMA
    FROM COLON STRING[str] 
    clause[clause_block]
    RBRACE
        { $$ = SelectActionSemanticAction($col_list, $str, $clause_block->where_object, $clause_block->group_by_column_list, $clause_block->order_by_column_list, $clause_block->having_object, NULL); }
    |
    LBRACE SELECT COLON BRACKET_OPEN string_list[col_list] BRACKET_CLOSE COMMA
    FROM  COLON STRING[str] 
    COMMA JOIN COLON LBRACE STRING[str2] COLON LBRACE ON COLON LBRACE STRING[cond1] COLON STRING[cond2] RBRACE RBRACE RBRACE
    clause[clause_block]
    RBRACE
        { $$ = SelectActionSemanticAction($col_list, $str, $clause_block->where_object, $clause_block->group_by_column_list, $clause_block->order_by_column_list, $clause_block->having_object, JoinSemanticAction($str2, $str, $cond1, $cond2)); }
    |
    LBRACE SELECT COLON all COMMA FROM COLON STRING[str] RBRACE
        { $$ = SelectAllActionSemanticAction($str); }
    ;

clause:
    where_clause[where_obj] group_by_clause[group_list] order_by_clause[order_list] having_clause[hav_obj]
        { $$ = ClauseSemanticAction($where_obj, $group_list, $order_list, $hav_obj); }
    | where_clause[where_obj] group_by_clause[group_list] order_by_clause[order_list]
        { $$ = ClauseSemanticAction($where_obj, $group_list, $order_list, NULL); }
    | where_clause[where_obj] group_by_clause[group_list]
        { $$ = ClauseSemanticAction($where_obj, $group_list, NULL, NULL); }
    | where_clause[where_obj] order_by_clause[order_list]
        { $$ = ClauseSemanticAction($where_obj, NULL, $order_list, NULL); }
    | where_clause[where_obj]
        { $$ = ClauseSemanticAction($where_obj, NULL, NULL, NULL); }
    | where_clause[where_obj] group_by_clause[group_list] having_clause[hav_obj]
        { $$ = ClauseSemanticAction($where_obj, $group_list, NULL, $hav_obj); }
    | order_by_clause[order_list]
        { $$ = ClauseSemanticAction(NULL, NULL, $order_list, NULL); }
    | 
        { $$ = ClauseSemanticAction(NULL, NULL, NULL, NULL); }
    ;
where_clause:
    COMMA WHERE COLON LBRACE where_object[where_obj] RBRACE
        { $$ = $where_obj; }
    | COMMA WHERE COLON LBRACE logical_op[log_op] COLON LBRACE condition[c1] COMMA condition[c2] RBRACE RBRACE
        { $$ = WhereObjectSemanticAction($c1, $log_op, (WhereObject*)$c2); }
    ;


group_by_clause:
    COMMA GROUP_BY COLON BRACKET_OPEN string_list[group_list] BRACKET_CLOSE
        { $$ = $group_list; };

order_by_clause:
    COMMA ORDER_BY COLON BRACKET_OPEN string_list[order_list] BRACKET_CLOSE
        { $$ = $order_list; };

having_clause:
    COMMA HAVING COLON BRACKET_OPEN LBRACE having_object[hav_obj] RBRACE BRACKET_CLOSE
        { $$ = $hav_obj; } ;

delete_action:
				LBRACE
				DELETE COLON LBRACE
				FROM COLON STRING[str] where_clause[where_obj] 
				RBRACE
				RBRACE                              { $$ = DeleteActionSemanticAction($str, $where_obj); }
				;


add_action:
				LBRACE
				ADD COLON LBRACE
				TABLE COLON STRING COMMA
				COLUMNS COLON LBRACE column_object[col_obj] RBRACE
				RBRACE
				RBRACE                              { $$ = AddActionSemanticAction($7, $col_obj); }
				;

update_action:
                LBRACE
                UPDATE COLON LBRACE
                TABLE COLON STRING[str] COMMA
                SET COLON LBRACE 
                update_object[upd_list] 
                RBRACE 
                where_clause[where_obj]
                RBRACE
                RBRACE                              { $$ = UpdateActionSemanticAction($str, $upd_list, $where_obj); }
                ;


column_object:
    STRING COLON STRING                              { $$ = ColumnObjectSemanticAction($1, $3, NULL); }
    | STRING COLON STRING COMMA column_object[col_obj] { $$ = ColumnObjectSemanticAction($1, $3, $col_obj); }
    ;


update_object:
                condition[cond]                                  { $$ = UpdateObjectSemanticAction($cond, NULL); }
                | condition[cond] COMMA update_object[upd_itmes]  { $$ = UpdateObjectSemanticAction($cond, $upd_itmes); }
                ;

where_object:
    condition[cond]
        { $$ = WhereObjectSemanticAction($cond, E_NONE, NULL); }
    | condition[cond] COMMA where_object[where_obj]
        { $$ = WhereObjectSemanticAction($cond, E_AND, $where_obj); }
    | NOT where_object[where_obj]
        { $$ = WhereObjectSemanticAction(NULL, E_NOT, $where_obj); }
    ;


having_object:
            having_condition[hav_con]                                { $$ = HavingObjectSemanticAction($hav_con, E_NONE, NULL); }
            | having_condition[hav_con] COMMA having_object[hav_obj]  { $$ = HavingObjectSemanticAction($hav_con, E_AND, $hav_obj); }
            ;

having_condition: 
            aggregate_function[agg_func] COLON STRING[str] COMMA operator[op] COLON value[val]
                                                    { $$ = HavingConditionSemanticAction($agg_func, $str, $op, $val); }
            ;

condition:
            STRING[str] COLON value[val]                    { $$ = ConditionSemanticAction($str,E_EQUALS, $val); }
            | STRING[str] COLON LBRACE operator[op] COLON value[val] RBRACE                    { $$ = ConditionSemanticAction($str,$op, $val); }
            ;


aggregate_function:
                COUNT   { $$ = E_COUNT; }
                | SUM   { $$ = E_SUM; }
                | AVG   { $$ = E_AVG; }
                | MAX   { $$ = E_MAX; }
                | MIN   { $$ = E_MIN; }
                ;   


operator: 
                EQUALS             {$$ = E_EQUALS; }
                | GREATER_THAN     {$$ = E_GREATER_THAN; }
                | LESS_THAN        {$$ = E_LESS_THAN; }
                ;

value:
            STRING                                   { $$ = StringValueSemanticAction($1);  }
            | INTEGER                                { $$ = IntegerValueSemanticAction($1); }
            | FLOAT                                  { $$ = FloatValueSemanticAction($1); }
            ;

insert_list:    BRACKET_OPEN value_list[val_list] BRACKET_CLOSE COMMA insert_list[ins_list] { $$ = MultipleInsertListSemanticAction($val_list, $ins_list); }
                | BRACKET_OPEN value_list[val_list] BRACKET_CLOSE                           { $$ = SimpleInsertListSemanticAction($val_list); }
                ;

value_list:
				value                                    { $$ = ValueListSemanticAction($1, NULL); }
				| value COMMA value_list                 { $$ = ValueListSemanticAction($1, $3); }
				;

string_list: 
                STRING                                    { $$ = ArraySemanticAction($1, NULL); }
                | STRING COMMA string_list                { $$ = ArraySemanticAction($1, $3); }
                ;                                


logical_op:     AND     { $$ = E_AND; }
                | OR    { $$ = E_OR; }
                ;          

%%
