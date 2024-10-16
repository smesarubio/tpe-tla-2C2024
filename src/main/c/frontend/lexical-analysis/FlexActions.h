#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
//new
Token JsonBraceLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token JsonBracketLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token JsonColonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token JsonCommaLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token SqlKeywordLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token StringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token BooleanLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token NullLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token FloatLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IdentifierLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

#endif
