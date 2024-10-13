%{

#include "BisonActions.h"

%}

%code requires{

    #include "../../shared/Type.h"
}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */
	String string;
	Integer integer;
	Float float_value;
	Token token;

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
    ColumnList * column_list;
    ColumnItem * column_item;
    UpdateList * update_list;
    UpdateItems * update_items;
    WhereObject * where_object;
    HavingObject * having_object;
    Condition * condition;
    Value * value;
    Array * string_list;
    ValueList * value_list;   
    Function * function;
    AggFunc* aggregate_function;
    Operator* operator;
    LogOp* logical_op;
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
%destructor { releaseOperator($$); } <operator>
%destructor { releaseValue($$); } <value>
%destructor { releaseJoin($$); } <join>
%destructor { releaseInsertList($$); } <insert_list>
%destructor { releaseHavingObject($$); } <having_object>
%destructor { releaseLogOp($$); } <logical_op>
%destructor { releaseHavingCondition($$); } <having_condition>
%destructor { releaseAggFunc($$); } <aggregate_function>
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
%type <column_list> column_list
%type <column_item> column_item
%type <update_list> update_list
%type <update_items> update_items
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

action:     create_action                                { $$ = (Action *) $1; }
				| select_action                          { $$ = (Action *) $1; }
				| delete_action                          { $$ = (Action *) $1; }
				| add_action                             { $$ = (Action *) $1; }
				| update_action                          { $$ = (Action *) $1; }
                | insert_action                          { $$ = (Action *) $1; }
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
        |  COMMA WHERE COLON LBRACE logical_op[log_op] COLON LBRACE condition[c1] COMMA condition[c2] RBRACE RBRACE
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
				VALUES COLON BRACKET_OPEN value_list[arr] BRACKET_CLOSE
				RBRACE
				RBRACE                              { $$ = AddActionSemanticAction($7, $arr); }
				;

update_action:
                LBRACE
                UPDATE COLON LBRACE
                TABLE COLON STRING COMMA
                SET COLON update_list[upd_list] where_clause[where_obj]
                RBRACE
                RBRACE                              { $$ = UpdateActionSemanticAction($7, $upd_list, $where_obj); }
                ;


column_object:
                column_list[col_list]        { $$ = (ColumnObject *) $col_list; }
                ;

column_list:
                column_item                             { $$ = ColumnListSemanticAction($1, NULL); }
                | column_list COMMA column_item         { $$ = ColumnListSemanticAction($3, $1); }
                
                ;

column_item:
                STRING COLON STRING                     { $$ = ColumnItemSemanticAction($1, $3); }
                |STRING COMMA STRING                     { $$ = ColumnItemSemanticAction($1, $3); }
                ;


update_list:
                LBRACE update_items[upd_items] RBRACE      { $$ = (UpdateList *) $upd_items; }
                ;

update_items:
                STRING COLON value[val]                                  { $$ = UpdateItemSemanticAction($1, $val, NULL); }
                | STRING COLON value[val] COMMA update_items[upd_itmes]  { $$ = UpdateItemSemanticAction($1, $val, $upd_itmes); }
                ;


where_object:
             condition[cond]                               { $$ = (WhereObject *) $cond; }
            |  condition[cond] COMMA where_object[where_obj]     { $$ = WhereObjectSemanticAction($cond, LogOpSemanticAction(AND), $where_obj); }
            |  NOT where_object[where_obj]                       { $$ = WhereObjectSemanticAction(NULL, E_NOT, $where_obj); }
            ;

having_object:
            having_condition[hav_con]                                { $$ = (HavingObject *) $hav_con; }
            | having_condition[hav_con] COMMA having_object[hav_obj]  { $$ = HavingObjectSemanticAction($hav_con, LogOpSemanticAction(AND), $hav_obj); }
            ;

having_condition: 
            aggregate_function[agg_func] COLON STRING[str] COMMA operator[op] COLON value[val]
                                                    { $$ = HavingConditionSemanticAction($agg_func, $str, $op, $val); }
            ;

condition:
            STRING[str] COLON value[val]                    { $$ = ConditionSemanticAction($str,NULL, $val); }
            | operator COLON  value[val]                     { $$ = ConditionSemanticAction(NULL,$1, $3); }
            ;


aggregate_function:
                COUNT   { $$ = (AggFunc *) COUNT; }
                | SUM   { $$ = (AggFunc *)  SUM; }
                | AVG   { $$ = (AggFunc *)  AVG; }
                | MAX   { $$ = (AggFunc *)  MAX; }
                | MIN   { $$ = (AggFunc *) MIN; }
                ;   


operator: 
                EQUALS             {$$ = (Operator *) EQUALS; }
                | GREATER_THAN     {$$ = (Operator *) GREATER_THAN; }
                | LESS_THAN        {$$ = (Operator *) LESS_THAN; }
                ;

value:
            STRING                                   { $$ = StringValueSemanticAction($1); }
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


logical_op:     AND     { $$ = LogOpSemanticAction(AND); }
                | OR    { $$ = LogOpSemanticAction(OR); }
                ;          

%%
