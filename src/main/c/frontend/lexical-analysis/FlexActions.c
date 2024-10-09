#include "FlexActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule() {
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext) {
	char * escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
		functionName,
		escapedLexeme,
		lexicalAnalyzerContext->currentContext,
		lexicalAnalyzerContext->length,
		lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->integer = atoi(lexicalAnalyzerContext->lexeme);
	return INTEGER;
}

Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->token = token;
	return token;
}

/*
Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return UNKNOWN;
}*/



/* Add new action functions for JSON and SQL tokens */

Token JsonBraceLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token JsonBracketLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token JsonColonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return COLON;
}

Token JsonCommaLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return COMMA;
}

Token SqlKeywordLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->token = token;
    return token;
}

Token StringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    // Remove quotes and handle escape characters
    char* unescaped = unescapeString(lexicalAnalyzerContext->lexeme + 1, lexicalAnalyzerContext->length - 2);
    lexicalAnalyzerContext->semanticValue->string = unescaped;
    return STRING;
}

/*

"true"                      { return BooleanLexemeAction(createLexicalAnalyzerContext()); }
"false"                     { return BooleanLexemeAction(createLexicalAnalyzerContext()); }

Token BooleanLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->boolean = (strcmp(lexicalAnalyzerContext->lexeme, "true") == 0);
    return BOOL;
}
*/
Token NullLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    return NULL;
}

Token FloatLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->float_value = atof(lexicalAnalyzerContext->lexeme);
    return FLOAT;
}
/*
Esto estaba en el .l
[a-zA-Z_][a-zA-Z0-9_]*      { return IdentifierLexemeAction(createLexicalAnalyzerContext()); }

y esto aca
Token IdentifierLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->string = strdup(lexicalAnalyzerContext->lexeme);
    return IDENTIFIER;
}
*/
