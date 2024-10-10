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

"CREATE"                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), CREATE); }
"SELECT"                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), SELECT); }
"FROM"                      { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), FROM); }
"WHERE"                     { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), WHERE); }
"INSERT"                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), INSERT); }
"INTO"                      { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), INTO); }
"VALUES"                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), VALUES); }
"DELETE"                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), DELETE); }
"UPDATE"                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), UPDATE); }
"SET"                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), SET); }
"GROUP BY"                  { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), GROUP_BY); }
"HAVING"                    { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), HAVING); }
"COUNT"                     { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), COUNT); }
"SUM"                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), SUM); }
"AVG"                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), AVG); }
"MAX"                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), MAX); }
"MIN"                       { return SqlKeywordLexemeAction(createLexicalAnalyzerContext(), MIN); }


"null"                      { return NullLexemeAction(createLexicalAnalyzerContext()); }

\"(\\.|[^"\\])*\"           { return StringLexemeAction(createLexicalAnalyzerContext());}
[[:digit:]]+				{ return IntegerLexemeAction(createLexicalAnalyzerContext());}
[0-9]+"."[0-9]+             { return FloatLexemeAction(createLexicalAnalyzerContext()); }


%%

#include "FlexExport.h"
