#include "RecDescent.h"

static const double POISON = 1488228;

DiffNode *GetG (char **exp) {
    assert (exp);

    DiffNode *node = GetE (exp);

    if (**exp == '\n') {
        return node;
    }
    else {
        return 0;
    }
}

DiffNode *GetE (char **exp) {
    assert (exp);

    DiffNode *node1 = GetT (exp);

    while (**exp == '+' || **exp == '-') {
        char sign = **exp;
        (*exp)++;

        DiffNode *node2 = GetT (exp);

        DiffNode *nodeOp = (DiffNode *)calloc (1, sizeof (DiffNode));

        if (sign == '+') {
            ALLOC_DATA_FOR_STR (nodeOp, "+", OPERATOR);
        }
        else {
            ALLOC_DATA_FOR_STR (nodeOp, "-", OPERATOR);
        }

        nodeOp->left = node1;
        nodeOp->right = node2;

        node1->parent = nodeOp;
        node2->parent = nodeOp;

        node1 = nodeOp;
    }

    return node1;
}

DiffNode *GetT (char **exp) {
    assert (exp);

    DiffNode *node1 = GetExponent (exp);

    while (**exp == '*' || **exp == '/') {
        char symbol = **exp;

        (*exp)++;

        DiffNode *node2 = GetExponent (exp);

        DiffNode *nodeOp = (DiffNode *)calloc (1, sizeof (DiffNode));

        if (symbol == '*') {
            ALLOC_DATA_FOR_STR (nodeOp, "*", OPERATOR);
        }
        else {
            ALLOC_DATA_FOR_STR (nodeOp, "/", OPERATOR);
        }

        nodeOp->left = node1;
        nodeOp->right = node2;

        node1->parent = nodeOp;
        node2->parent = nodeOp;

        node1 = nodeOp;
    }

    return node1;
}

DiffNode *GetP (char **exp) {
    assert (exp);

    if (**exp == '(') {
        (*exp)++;

        DiffNode *node = GetE (exp);

        Require (exp, ')');

        return node;

    }
    else if (isalpha (**exp)) {
        char func[50] = {};

        GetString (func, exp);

        int res = CheckFunction (func);

        DiffNode *node = (DiffNode *)calloc (1, sizeof (DiffNode));

        if (res == NOTFOUND) {
            ALLOC_DATA_FOR_STR (node, func, VARIABLE);

            node->left = nullptr;
            node->right = nullptr;
        }
        else {
            Require (exp, '(');

            DiffNode *nodeFunArg = GetE(exp);

            Require (exp, ')');

            ALLOC_DATA_FOR_STR (node, func, FUNCTION);

            nodeFunArg->parent = node;
            node->right = nullptr;
            node->left = nodeFunArg;
        }

        return node;
    }
    else {
        return GetN (exp);
    }
}

DiffNode *GetN (char **exp) {
    assert (exp);

    double val = 0;
    int n = 0;

    sscanf (*exp, "%lg%n", &val, &n);

    (*exp) += n;

    DiffNode *node = (DiffNode *)calloc (1, sizeof (DiffNode));

    PushNumberToNode (node, val);

    node->left = nullptr;
    node->right = nullptr;

    return node;
}

DiffNode *GetExponent (char **exp) {
    assert (exp);

    DiffNode *node1 = GetP (exp);

    while (**exp == '^') {
        (*exp)++;

        DiffNode *node2 = GetP (exp);

        DiffNode *nodeOp = (DiffNode *)calloc (1, sizeof (DiffNode));

        ALLOC_DATA_FOR_STR (nodeOp, "^", OPERATOR);

        node1->parent = nodeOp;
        node2->parent = nodeOp;

        nodeOp->left = node1;
        nodeOp->right = node2;

        node1 = nodeOp;
    }

    return node1;
}

void Require (char **exp, char symbol) {
    assert (exp);

    if (**exp != symbol) {
        printf ("Syntax error!\n");
        abort ();
    }

    (*exp)++;
}

int GetString (char *str, char **exp) {
    assert (str);
    assert (exp);

    int length = 0;

    while (isalpha (**exp)) {
        str [length] = **exp;
        (*exp)++;
        length++;
    }

    str [length] = '\0';

    return length;
}

double DoMathFunction (char *funcName, double val) {
    assert (funcName);

    if (STR_EQ (strSIN, funcName)) {
        return sin (val);
    }
    else if (STR_EQ (strCOS, funcName)) {
        return cos (val);
    }
    else if (STR_EQ (strLN, funcName)) {
        return log (val);
    }
    else {
        SyntaxError ();
    }

    return POISON;
}

int CheckFunction (char *funcName) {
    assert (funcName);

    if (STR_EQ (strSIN, funcName)) {
        return SIN;
    }
    else if (STR_EQ (strCOS, funcName)) {
        return COS;
    }
    else if (STR_EQ (strLN, funcName)) {
        return LN;
    }
    else if (STR_EQ (strSQRT, funcName)) {
        return SQRT;
    }
    else {
        return NOTFOUND;
    }
}

void SyntaxError () {
    printf ("Syntax error!\n");

    exit (1);
}
