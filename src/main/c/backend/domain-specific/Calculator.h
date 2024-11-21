#ifndef CALCULATOR_HEADER
#define CALCULATOR_HEADER

/**
 * We reuse the types from the AST for convenience, but you should separate
 * the layers of the backend and frontend using another group of
 * domain-specific models or DTOs (Data Transfer Objects).
 */
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include <limits.h>

/** Initialize module's internal state. */
void initializeCalculatorModule();

/** Shutdown module's internal state. */
void shutdownCalculatorModule();

/**
 * The result of a computation. It's considered valid only if "succeed" is
 * true.
 */
typedef struct {
	boolean succeed;
	char *sql;
} ComputationResult;

typedef ComputationResult (*BinaryOperator)(const int, const int);

/**
 * Computes the final string of a sql expression.
 */
ComputationResult computeAction(Action *action);


#endif
