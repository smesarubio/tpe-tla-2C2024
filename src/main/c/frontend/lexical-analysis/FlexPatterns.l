%{
#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack
/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT
/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */



%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"{"                         { return JsonBraceLexemeAction(createLexicalAnalyzerContext(), LBRACE); }
"}"                         { return JsonBraceLexemeAction(createLexicalAnalyzerContext(), RBRACE); }
"["                         { return JsonBracketLexemeAction(createLexicalAnalyzerContext(), BRACKET_OPEN); }
"]"                         { return JsonBracketLexemeAction(createLexicalAnalyzerContext(), BRACKET_CLOSE); }
":"                         { return JsonColonLexemeAction(createLexicalAnalyzerContext()); }
","                         { return JsonCommaLexemeAction(createLexicalAnalyzerContext()); }

"\"CREATE\""                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), CREATE); }
"\"SELECT\""                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), SELECT); }
"\"FROM\""                      { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), FROM); }
"\"WHERE\""                     { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), WHERE); }
"\"INSERT\""                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), INSERT); }
"\"VALUES\""                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), VALUES); }
"\"DELETE\""                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), DELETE); }
"\"UPDATE\""                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), UPDATE); }
"\"ADD\""                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), ADD); }
"\"SET\""                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), SET); }
"\"GROUP BY\""                  { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), GROUP_BY); }
"\"ORDER BY\""                  { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), ORDER_BY); }
"\"HAVING\""                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), HAVING); }
"\"COUNT\""                     { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), COUNT); }
"\"SUM\""                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), SUM); }
"\"AVG\""                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), AVG); }
"\"MAX\""                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), MAX); }
"\"MIN\""                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), MIN); }
"\"TABLE\""                     { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), TABLE); }
"\"EQUALS\""                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), EQUALS); }
"\"COLUMNS\""                   { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), COLUMNS); }
"\"all\""                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), all); }
"\"gt\""                        { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), GREATER_THAN); }
"\"lt\""                        { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), LESS_THAN); }
"\"OR\""                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), OR); }
"\"AND\""                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), AND); }
"\"JOIN\""                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), JOIN); }
"\"ON\""                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), ON); }


"null"                      { return NullLexemeAction(createLexicalAnalyzerContext()); }

\"(\\.|[^"\\])*\"           { return StringLexemeAction(createLexicalAnalyzerContext());}
[[:digit:]]+				{ return IntegerLexemeAction(createLexicalAnalyzerContext());}
[0-9]+"."[0-9]+             { return FloatLexemeAction(createLexicalAnalyzerContext()); }


%%

#include "FlexExport.h"
