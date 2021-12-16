#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define EPS 1e-6

#define ASSERT_OK(smth, doSmth)         do { if (smth) { doSmth;}} while (0)

#define STR_EQ(str1, str2)             (strncmp (str1, str2, strlen (str1)) == 0)

#define DESTRUCT_TREE(node)                     DestructTree (node);                                            \
                                                free (node);

struct DiffNode {
    char type;

    void *data;

    DiffNode *left;
    DiffNode *right;
    DiffNode *parent;
};

enum TYPESOFNODE {
    VARIABLE,
    OPERATOR,
    FUNCTION,
    NUMBER,
    CONSTANT
};

enum COMPARE {
    BIGGER,
    EQUAL,
    LESS
};

enum ERRORS {
    OK = -10,
    INCORRECT_INPUT,
    UNABLETOOPENFILE
};

int PreOrdSearch (DiffNode *topNode);

int DumpExp (DiffNode *topNode, FILE *expDump);

int GetExp (char *exp, DiffNode **topNode);

char *SkipSpaceSymbols (char *str);

int CheckForExp (char *str);

int GetFileSize (FILE *sample);

void DumpTree (DiffNode *node, FILE *treeDump);

void PreOrdSearch (DiffNode *node, DiffNode *childNode, FILE *file);

void DumpData (DiffNode *node, FILE *dump);

int FillData (DiffNode *topNode, char *arg);

void PrintData (DiffNode *node);

void DestructTree (DiffNode *topNode);

int Differentiate (DiffNode *topNode, DiffNode *diffNode, const char *var);

int RecursiveCpy (DiffNode *cpy, DiffNode *src);

void PushDataToNode (DiffNode *node, DiffNode *dest);

void PushNumberToNode (DiffNode *node, const double val);

void TexDump (DiffNode *topNode, DiffNode *diffNode);

void TexDumpNode (DiffNode *node, FILE *tex);

int DoubleComp (const double val1, const double val2);

void DestructNode (DiffNode *node);

int SimplifyNode (DiffNode *node);

double GetRealValue (DiffNode *node);

int IsZero (DiffNode *node);

void AssignVal (DiffNode *node);

void PrintErrors (const int type);

int GetBuffer (char **buff, const char *fileName);

void RunDifferentiation (char *sample);

#endif