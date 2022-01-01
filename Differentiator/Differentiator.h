#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

static const char strSIN[] = "sin";
static const char strCOS[] = "cos";
static const char strLN[] = "ln";
static const char strPI[] = "pi";
static const char strE[] = "e";
static const char strLG[] = "lg";
static const char strSQRT[] = "sqrt";
static const char strMUL[] = "*";
static const char strDIV[] = "/";
static const char strADD[] = "+";
static const char strSUB[] = "-";
static const char strDEG[] = "^";

#define EPS 1e-6

#define ASSERT_OK(smth, doSmth)         do { if (smth) { doSmth;}} while (0)

#define STR_EQ(str1, str2)              (strncmp (str1, str2, strlen (str1)) == 0)

#define DESTRUCT_TREE(node)             DestructTree (node); \
                                        free (node);

#define CREATE_LEFT(diffNode)                               diffNode->left = (DiffNode *)calloc (1, sizeof (DiffNode)); \
                                                            diffNode->left->parent = diffNode;

#define CREATE_RIGHT(diffNode)                              diffNode->right = (DiffNode *)calloc (1, sizeof (DiffNode)); \
                                                            diffNode->right->parent = diffNode;

#define ALLOC_DATA_FOR_STR(diffNode, oper, type_t)          diffNode->data = calloc (strlen (oper) + 1, sizeof (char));  \
                                                            diffNode->type = type_t;                                     \
                                                            memcpy (diffNode->data, (void *)(&oper), strlen (oper) + 1);


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