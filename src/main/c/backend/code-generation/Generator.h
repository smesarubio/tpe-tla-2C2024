#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include <stdarg.h>
#include <stdio.h>

/** Initialize module's internal state. */
void initializeGeneratorModule();

/** Shutdown module's internal state. */
void shutdownGeneratorModule();

/**
 * Generates the final output using the current compiler state.
 */
 // Declarations for static functions used in Generator.c
static void _generateSQL(JsonQuery *json_query);
static void _generateAddAction(AddAction *addAction);
static void _generateCreateAction(CreateAction *createAction);
static void _generateDeleteAction(DeleteAction *deleteAction);
static void _generateSelectAction(SelectAction *selectAction);
static void _generateUpdateAction(UpdateAction *updateAction);

static void _generateValueList(ValueList *valueList);
static void _generateValue(Value *value);
static void _generateColumnObject(ColumnObject *columnObject);
static void _generateColumnList(ColumnList *columnList);
static void _generateColumnItem(ColumnItem *columnItem);
static void _generateWhereObject(WhereObject *whereObject);
static void _generateCondition(Condition *condition);
// static void _generateHavingObject(HavingObject *havingObject);
static void _generateArray(Array *array);
void generate(CompilerState * compilerState);

#endif
