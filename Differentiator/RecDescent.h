#ifndef CALCULATOR_H
#define CALCULATOR_H

#define ASSERT_OK(smth, doSmth)         do { if (smth) { doSmth;}} while (0)

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "Differentiator.h"

#define STR_EQ(str1, str2)             (strncmp (str1, str2, strlen (str1)) == 0)

enum FUNCTION_TYPES {
    NOTFOUND,
    COS,
    SIN,
    LN,
    SQRT
};

DiffNode *GetP (char **exp);

void Require (char **exp, char symbol);

int CheckFunction (char *func);

int GetString (char *str, char **exp);

double DoMathFunction (char *funcName, double val);

void SyntaxError ();

DiffNode *GetN (char **exp);

DiffNode *GetG (char **exp);

DiffNode *GetE (char **exp);

DiffNode *GetT (char **exp);

DiffNode *GetExponent (char **exp);

#endif
