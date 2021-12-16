#include "Differentiator.h"

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


int GetExp (char *exp, DiffNode **topNode) {
    assert (exp);
    assert (topNode);

    int res = 0;

    if (*topNode == nullptr) {
        *topNode = (DiffNode *)calloc(1, sizeof (DiffNode));
        exp++;
    }

    DiffNode *node = *topNode;

    while (1) {
        if (*exp == '(') {
            if (node->type == OPERATOR) {
                node->right = (DiffNode *)calloc (1, sizeof (DiffNode));
                node->right->parent = node;

                node = node->right;
            }
            else {
                node->left = (DiffNode *)calloc (1, sizeof (DiffNode));
                node->left->parent = node;

                node = node->left;
            }

            exp++;

            continue;
        }

        if (*exp == ')') {
            exp++;

            node = node->parent;

            continue;
        }

        char arg[50] = {};
        int wordLenth = 0;

        while (*exp != '(' && *exp != ')' && *exp != '\0') {
            arg[wordLenth++] = *exp;
            exp++;
        }


        if (*exp == '\0') break;

        arg [wordLenth] = '\0';

        res = FillData (node, arg);
        ASSERT_OK (res != OK, return INCORRECT_INPUT);
    }

    return OK;
}

char *SkipSpaceSymbols (char *str) {
    assert (str);

    while (isspace (*str)) {
        str++;
    }

    return str;
}

int GetFileSize (FILE *txtFile) {
    assert (txtFile);

    int numberOfSymbols = 0;

    fseek (txtFile, 0, SEEK_END);

    numberOfSymbols = ftell (txtFile);

    fseek (txtFile, 0, SEEK_SET);

    return numberOfSymbols;
}

void DumpTree (DiffNode *node, FILE *treeDump) {
    assert (node);

    fprintf (treeDump, "digraph D {\n");

    fprintf (treeDump, "    %lu [fillcolor=purple, style=\"rounded,filled\", label=\"", (unsigned long)node % 65536);
    DumpData (node, treeDump);
    fprintf (treeDump, "\"];\n");

    if (node->left != nullptr) {
        PreOrdSearch(node, node->left, treeDump);
    } 
    if (node->right != nullptr) {
        PreOrdSearch (node, node->right, treeDump);
    }
    
    fprintf (treeDump, "}");

    fclose (treeDump);

    system ("dot -Tpng DumpTree.txt -O\n"
            "xdg-open DumpTree.txt.png\n");
}

void PreOrdSearch (DiffNode *node, DiffNode *childNode, FILE *file) {
    assert (node);
    assert (childNode);

    fprintf (file, "    %lu -> %lu;\n\n", (unsigned long)node % 65536, (unsigned long)childNode % 65536);
    // fprintf (file, "    %lu -> %lu;\n\n", (unsigned long)childNode % 65536, (unsigned long)childNode->parent % 65536);

    if (childNode->left == nullptr) {
        fprintf (file, "    %lu [fillcolor=yellow, style=\"rounded,filled\", label=\"", (unsigned long)childNode % 65536);
        DumpData (childNode, file);
        fprintf (file, "\"];\n");

        return;
    }

    fprintf (file, "    %lu [fillcolor=cyan, style=\"rounded,filled\", label=\"", (unsigned long)childNode % 65536);
    DumpData (childNode, file);
    fprintf (file, "\"];\n");

    PreOrdSearch (childNode, childNode->left, file);

    if (childNode->right != nullptr) {
        PreOrdSearch (childNode, childNode->right, file);
    }

    return;
}

void DumpData (DiffNode *node, FILE *dump) {
    assert (node);

    switch (node->type) {
        case NUMBER: {
            fprintf (dump, "%lg", *(double *)node->data);

            break;
        }
        case FUNCTION: {
            fprintf (dump, "%s", (char *)node->data);

            break;
        }
        case CONSTANT: {
            fprintf (dump, "%s", (char *)node->data);

            break;
        }
        case OPERATOR: {
            fprintf (dump, "%c", *(char *)node->data);

            break;
        }
        case VARIABLE: {
            fprintf (dump, "%c", *(char *)node->data);

            break;
        }
        default: {
            fprintf (dump, "Unknown type!\n");
        }
    }
}

int FillData (DiffNode *topNode, char *arg) {
    assert (topNode);
    assert (arg);

    int length = strlen (arg);

    double val = 0;
    int n = 0;

    sscanf (arg, "%lg%n", &val, &n);

    if (n != 0) {
        topNode->type = NUMBER;

        topNode->data = calloc (1, sizeof (double));

        memcpy (topNode->data, (void *)(&val), sizeof (double));
        
        return OK;
    }

    if (STR_EQ (strE, arg) || STR_EQ (strPI, arg)) {
        topNode->type = CONSTANT;

        topNode->data = calloc (length + 1, sizeof (char));

        memcpy (topNode->data, arg, length + 1);

        return OK;
    }

    if (length > 1) {
        topNode->type = FUNCTION;

        topNode->data = calloc (length + 1, sizeof (char));

        memcpy (topNode->data, arg, length + 1);

        return OK;
    }

    if (length == 1 && (*arg == '+' || *arg == '*' || *arg == '-' || *arg == '/' || *arg == '^')) {
        topNode->type = OPERATOR;

        topNode->data = calloc (2, sizeof (char));

        memcpy (topNode->data, arg, length + 1);

        return OK;
    }

    if (length == 1) {
        topNode->type = VARIABLE;

        topNode->data = calloc (2, sizeof (char));

        memcpy (topNode->data, arg, length + 1);
        
        return OK;
    }

    return INCORRECT_INPUT;
}

void PrintData (DiffNode *node) {
    assert (node);

    switch (node->type) {
        case NUMBER: {
            printf ("%lg\n", *(double *)node->data);

            break;
        }
        case FUNCTION: {
            printf ("%s\n", (char *)node->data);

            break;
        }
        case CONSTANT: {
            printf ("%s\n", (char *)node->data);

            break;
        }
        case OPERATOR: {
            printf ("%c\n", *(char *)node->data);

            break;
        }
        case VARIABLE: {
            printf ("%c\n", *(char *)node->data);

            break;
        }
        default: {
            printf ("Unknown type!\n");
        }
    }
}

void DestructTree (DiffNode *topNode) {
    assert (topNode);

    free (topNode->data);

    if (topNode->left == nullptr) {

        return;
    }

    DestructTree (topNode->left);

    free (topNode->left);

    if (topNode->right != nullptr) {
        DestructTree (topNode->right);

        free (topNode->right);
    }
}

#define CREATE_LEFT(diffNode)                               diffNode->left = (DiffNode *)calloc (1, sizeof (DiffNode)); \
                                                            diffNode->left->parent = diffNode;

#define CREATE_RIGHT(diffNode)                              diffNode->right = (DiffNode *)calloc (1, sizeof (DiffNode)); \
                                                            diffNode->right->parent = diffNode;

#define ALLOC_DATA_FOR_STR(diffNode, oper, type_t)          diffNode->data = calloc (strlen (oper) + 1, sizeof (char));                     \
                                                            diffNode->type = type_t;                                      \
                                                            memcpy (diffNode->data, (void *)(&oper), strlen (oper) + 1);


int Differentiate (DiffNode *topNode, DiffNode *diffNode, const char *var) {
    assert (topNode);
    assert (diffNode);

    if (topNode->type == OPERATOR) {
        char op = *(char *)topNode->data;

        switch (op) {
            case '+': {
                ALLOC_DATA_FOR_STR (diffNode, strADD, OPERATOR);

                CREATE_LEFT (diffNode);
                CREATE_RIGHT (diffNode);

                Differentiate (topNode->left, diffNode->left, var);

                Differentiate (topNode->right, diffNode->right, var);

                break;
            }
            case '-': {
                ALLOC_DATA_FOR_STR (diffNode, strSUB, OPERATOR);

                CREATE_LEFT (diffNode);
                CREATE_RIGHT (diffNode);

                Differentiate (topNode->left, diffNode->left, var);

                Differentiate (topNode->right, diffNode->right, var);

                break;
            }
            case '*': {
                CREATE_LEFT (diffNode);
                CREATE_RIGHT (diffNode);

                CREATE_LEFT (diffNode->left);
                CREATE_RIGHT (diffNode->left);

                CREATE_LEFT (diffNode->right);
                CREATE_RIGHT (diffNode->right);

                ALLOC_DATA_FOR_STR (diffNode, strADD, OPERATOR);

                ALLOC_DATA_FOR_STR (diffNode->left, strMUL, OPERATOR);
                ALLOC_DATA_FOR_STR (diffNode->right, strMUL, OPERATOR);

                RecursiveCpy (diffNode->left->right, topNode->right);
                RecursiveCpy (diffNode->right->left, topNode->left);

                Differentiate (topNode->left, diffNode->left->left, var);

                Differentiate (topNode->right, diffNode->right->right, var);

                break;
            }
            case '/': {
                CREATE_LEFT (diffNode);
                CREATE_RIGHT (diffNode);

                CREATE_LEFT (diffNode->left);
                CREATE_RIGHT (diffNode->left);

                CREATE_LEFT (diffNode->left->left);
                CREATE_RIGHT (diffNode->left->left);

                CREATE_LEFT (diffNode->right);
                CREATE_RIGHT (diffNode->right);

                CREATE_LEFT (diffNode->left->right);
                CREATE_RIGHT (diffNode->left->right);

                ALLOC_DATA_FOR_STR (diffNode, strDIV, OPERATOR);
                ALLOC_DATA_FOR_STR (diffNode->left, strSUB, OPERATOR);
                ALLOC_DATA_FOR_STR (diffNode->left->left, strMUL, OPERATOR);
                ALLOC_DATA_FOR_STR (diffNode->left->right, strMUL, OPERATOR);
                ALLOC_DATA_FOR_STR (diffNode->right, strMUL, OPERATOR);

                RecursiveCpy (diffNode->left->left->right, topNode->right);
                RecursiveCpy (diffNode->left->right->left, topNode->left);

                RecursiveCpy (diffNode->right->left, topNode->right);

                RecursiveCpy (diffNode->right->right, topNode->right);

                Differentiate (topNode->left, diffNode->left->left->left, var);

                Differentiate (topNode->right, diffNode->left->right->right, var);

                break;
            }
            case '^': {
                if (topNode->right->type == NUMBER) {
                    CREATE_LEFT (diffNode);
                    CREATE_RIGHT (diffNode);

                    CREATE_LEFT (diffNode->right);
                    CREATE_RIGHT (diffNode->right);

                    CREATE_LEFT (diffNode->right->left);
                    CREATE_RIGHT (diffNode->right->left);

                    ALLOC_DATA_FOR_STR (diffNode, strMUL, OPERATOR);
                    ALLOC_DATA_FOR_STR (diffNode->right, strMUL, OPERATOR);
                    ALLOC_DATA_FOR_STR (diffNode->right->left, strDEG, OPERATOR);

                    double val = *(double *)topNode->right->data;

                    PushNumberToNode (diffNode->left, val);
                    PushNumberToNode (diffNode->right->left->right, val - 1);

                    RecursiveCpy (diffNode->right->left->left, topNode->left);

                    Differentiate (topNode->left, diffNode->right->right, var);
                }
                else {
                    DiffNode *treeLeft = (DiffNode *)calloc (1, sizeof (DiffNode));
                    DiffNode *treeRight = (DiffNode *)calloc (1, sizeof (DiffNode));

                    RecursiveCpy (treeLeft, topNode->left);
                    RecursiveCpy (treeRight, topNode->right);

                    DESTRUCT_TREE (topNode->left);
                    DESTRUCT_TREE (topNode->right);

                    CREATE_LEFT (topNode);
                    CREATE_RIGHT (topNode);

                    CREATE_LEFT (topNode->right);
                    CREATE_RIGHT (topNode->right);

                    CREATE_LEFT (topNode->right->left);

                    ALLOC_DATA_FOR_STR (topNode->left, strE, CONSTANT);
                    ALLOC_DATA_FOR_STR (topNode->right, strMUL, OPERATOR);
                    ALLOC_DATA_FOR_STR (topNode->right->left, strLN, FUNCTION);

                    RecursiveCpy (topNode->right->left->left, treeLeft);
                    RecursiveCpy (topNode->right->right, treeRight);

                    DESTRUCT_TREE (treeLeft);
                    DESTRUCT_TREE (treeRight);

                    CREATE_LEFT (diffNode);
                    CREATE_RIGHT (diffNode);

                    CREATE_LEFT (diffNode->left);
                    CREATE_RIGHT (diffNode->left);

                    ALLOC_DATA_FOR_STR (diffNode->left->left, strE, CONSTANT);
                    ALLOC_DATA_FOR_STR (diffNode, strMUL, OPERATOR);
                    ALLOC_DATA_FOR_STR (diffNode->left, strDEG, OPERATOR);

                    RecursiveCpy (diffNode->left->right, topNode->right);

                    Differentiate (topNode->right, diffNode->right, var);
                }

                break;
            }
        }
    }
    if (topNode->type == NUMBER) {

        PushNumberToNode (diffNode, 0);

        return OK;
    }
    if (topNode->type == CONSTANT) {
        
        PushNumberToNode (diffNode, 0);

        return OK;
    }
    if (topNode->type == VARIABLE) {

        if (STR_EQ (var, (char *)topNode->data)) {
            PushNumberToNode (diffNode, 1);
        }
        else {
           PushNumberToNode (diffNode,0);
        }

        return OK;
    }
    if (topNode->type == FUNCTION) {
        if (STR_EQ (strLN, (char *)(topNode->data))) {
            CREATE_LEFT (diffNode);
            CREATE_RIGHT (diffNode);

            CREATE_LEFT (diffNode->left);
            CREATE_RIGHT (diffNode->left);

            ALLOC_DATA_FOR_STR (diffNode, strMUL, OPERATOR);
            ALLOC_DATA_FOR_STR (diffNode->left, strDIV, OPERATOR);

            PushNumberToNode (diffNode->left->left, 1);

            RecursiveCpy (diffNode->left->right, topNode->left);
            Differentiate (topNode->left, diffNode->right, var);

            return OK;
        }
        if (STR_EQ (strSIN, (char *)(topNode->data))) {
            CREATE_LEFT (diffNode);
            CREATE_RIGHT (diffNode);

            CREATE_LEFT (diffNode->left);

            ALLOC_DATA_FOR_STR (diffNode, strMUL, OPERATOR);

            ALLOC_DATA_FOR_STR (diffNode->left, strCOS, FUNCTION);

            RecursiveCpy (diffNode->left->left, topNode->left);

            Differentiate (topNode->left, diffNode->right, var);

            return OK;
        }
        if (STR_EQ (strCOS, (char *)(topNode->data))) {
            CREATE_LEFT (diffNode);
            CREATE_RIGHT (diffNode);

            CREATE_LEFT (diffNode->left);
            CREATE_RIGHT (diffNode->left);

            CREATE_LEFT (diffNode->left->left);

            ALLOC_DATA_FOR_STR (diffNode, strMUL, OPERATOR);
            ALLOC_DATA_FOR_STR (diffNode->left, strMUL, OPERATOR);

            ALLOC_DATA_FOR_STR (diffNode->left->left, strSIN, FUNCTION);

            PushNumberToNode (diffNode->left->right, -1);

            RecursiveCpy (diffNode->left->left->left, topNode->left);

            Differentiate (topNode->left, diffNode->right, var);

            return OK;
        }
        if (STR_EQ (strSQRT, (char *)(topNode->data))) {
            CREATE_LEFT (diffNode);
            CREATE_RIGHT (diffNode);

            CREATE_LEFT (diffNode->left);
            CREATE_RIGHT (diffNode->left);

            CREATE_LEFT (diffNode->left->right);
            CREATE_RIGHT (diffNode->left->right);

            CREATE_LEFT (diffNode->left->right->right);

            ALLOC_DATA_FOR_STR (diffNode, strMUL, OPERATOR);
            ALLOC_DATA_FOR_STR (diffNode->left, strDIV, OPERATOR);
            ALLOC_DATA_FOR_STR (diffNode->left->right, strMUL, OPERATOR);

            PushNumberToNode (diffNode->left->left, 1);

            PushNumberToNode (diffNode->left->right->left, 2);

            ALLOC_DATA_FOR_STR (diffNode->left->right->right, strSQRT, FUNCTION);

            RecursiveCpy (diffNode->left->right->right->left, topNode->left);

            Differentiate (topNode->left, diffNode->right, var);
            
            return OK;
        }
    }

    return OK;
}

int RecursiveCpy (DiffNode *cpy, DiffNode *src) {
    assert (cpy);
    assert (src);

    PushDataToNode (src, cpy);
    cpy->type = src->type;

    if (src->left == nullptr && src->right == nullptr) {
        return OK;
    }

    cpy->left = (DiffNode *)calloc (1, sizeof (DiffNode));
    RecursiveCpy (cpy->left, src->left);
    cpy->left->parent = cpy;

    if (src->right != nullptr) {
        cpy->right = (DiffNode *)calloc (1, sizeof (DiffNode));
        RecursiveCpy (cpy->right, src->right);
        cpy->right->parent = cpy;
    }

    return OK;

}

int DoubleComp (const double val1, const double val2) {
    if (fabs (val1 - val2) <= EPS) {
        return EQUAL;
    }
    else if (val1 > val2) {
        return BIGGER;
    }
    else {
        return LESS;
    }
}

void PushDataToNode (DiffNode *node, DiffNode *dest) {
    assert (node);
    assert (dest);

    if (dest->data == nullptr) {
        dest->data = calloc (1, sizeof (double));
    }

    if (node->type == NUMBER) {
        dest->data = realloc (dest->data, 1 * sizeof (double));
        memcpy (dest->data, node->data, sizeof (double));
    }
    else if (node->type == VARIABLE || node->type == OPERATOR || node->type == FUNCTION || node->type == CONSTANT) {
        size_t length = strlen ((char *)node->data);

        dest->data = realloc (dest->data, (length + 1) * sizeof (char));
        memcpy (dest->data, node->data, length + 1);
    }
}

void PushNumberToNode (DiffNode *node, const double val) {
    assert (node);

    if (node->data == nullptr) {
        node->data = calloc (1, sizeof (double));
    }
    else {
        node->data = realloc (node->data, sizeof (double));
    }

    memcpy (node->data, (void *)&val, sizeof (double));
    node->type = NUMBER;
}

void TexDump (DiffNode *topNode, DiffNode *diffNode) {
    assert (topNode);

    FILE *tex = fopen ("texFile.tex", "w");

    fprintf (tex,   "\\documentclass{article}\n"
                    "\\usepackage[russian]{babel}\n"
                    "\\usepackage[letterpaper,top=2cm,bottom=2cm,left=3cm,right=3cm,marginparwidth=1.75cm]{geometry}\n"
                    "\\usepackage{amsmath}\n"
                    "\\usepackage{graphicx}\n"
                    "\\usepackage[colorlinks=true, allcolors=blue]{hyperref}\n"
                    "\\title{Учебник по дифференцированию}\n"
                    "\\author{Андрей Строчук}\n"
                    "\\begin{document}\n"
                    "\\maketitle\n"
                    "\\begin{abstract}\n"
                    "Вдохновлялся Петровичем...\n"
                    "\\end{abstract}\n"
                    "\\section{Дифференцирование}\n");

    fprintf (tex, "\\[f(x) = ");
    TexDumpNode (topNode, tex);
    fprintf (tex, "\\]\n");

    fprintf (tex, "\\[(f(x))' = ");
    TexDumpNode (diffNode, tex);
    fprintf (tex, "\\]\n");

    fprintf (tex, "\\end{document}\n");

    fclose (tex);
}

void TexDumpNode (DiffNode *node, FILE *tex) {
    assert (node);
    assert (tex);

    PrintData (node);

    switch (node->type) {
        case NUMBER: {
            fprintf (tex, "{%lg}", *(double *)node->data);

            break;
        }
        case CONSTANT: {
            fprintf (tex, "{%s}", (char *)node->data);

            break;
        }
        case OPERATOR: {
            switch (*(char *)node->data) {
                case '*': {
                    if (node->left->type == OPERATOR && (STR_EQ ((char *)node->left->data, strSUB) || STR_EQ ((char *)node->left->data, strADD))) {
                        fprintf (tex, "(");

                        TexDumpNode (node->left, tex);

                        fprintf (tex, ")");
                    }
                    else {
                        TexDumpNode (node->left, tex);
                    }

                    fprintf (tex, " \\cdot ");

                    if (node->right->type == OPERATOR && (STR_EQ ((char *)node->right->data, strADD) || STR_EQ ((char *)node->right->data, strSUB))) {
                        fprintf (tex, "(");

                        TexDumpNode (node->right, tex);

                        fprintf (tex, ")");
                    }
                    else {
                       TexDumpNode (node->right, tex);
                    }

                    break;
                }
                case '/': {
                    fprintf (tex, "\\frac{");

                    if (node->left->type == OPERATOR && (STR_EQ ((char *)node->left->data, strADD) || STR_EQ ((char *)node->left->data, strSUB))) {
                            fprintf (tex, "(");

                            TexDumpNode (node->left, tex);

                            fprintf (tex, ")");
                    }
                    else {
                         TexDumpNode (node->left, tex);
                    }

                    fprintf (tex, "}{");

                    if (node->right->type == OPERATOR && (STR_EQ ((char *)node->right->data, strADD) || STR_EQ ((char *)node->right->data, strSUB))) {
                        fprintf (tex, "(");

                        TexDumpNode (node->right, tex);

                        fprintf (tex, ")");
                    }
                    else {
                        TexDumpNode (node->right, tex);
                    }

                    fprintf (tex, "}");

                    break;
                }
                case '^': {
                    if (node->left->type != NUMBER && node->left->type != VARIABLE && node->left->type != CONSTANT) {
                        fprintf (tex, "({");

                        TexDumpNode (node->left, tex);

                        fprintf (tex, "})");
                    }
                    else {
                        TexDumpNode (node->left, tex);
                    }

                    fprintf (tex, "%s", strDEG);

                    fprintf (tex, "{");

                    TexDumpNode (node->right, tex);

                    fprintf (tex, "}");

                    break;
                }
                case '+': {
                    TexDumpNode (node->left, tex);

                    fprintf (tex, "%s", (char *)node->data);

                    TexDumpNode (node->right, tex);

                    break;
                }
            }

            break;
        }
        case VARIABLE: {
            fprintf (tex, "{%s}", (char *)node->data);

            break;
        }
        case FUNCTION: {
            fprintf (tex, "\\%s", (char *)node->data);

            if (node->left->type != NUMBER && node->left->type != VARIABLE) {
                fprintf (tex, "{");

                TexDumpNode (node->left, tex);

                fprintf (tex, "}");
            }
            else {
                TexDumpNode (node->left, tex);
            }

            break;
        }
    }

    return;
}

void DestructNode (DiffNode *node) {
    assert (node);

    if (node->data != nullptr) {
        free (node->data);
    }

    if (node->left != nullptr) {
        free (node->left);
    }

    if (node->right != nullptr) {
        free (node->right);
    }

    if (node->parent != nullptr) {
        free (node->parent);
    }

    free (node);
}

int SimplifyNode (DiffNode *node) {
    assert (node);

    if (node->left != nullptr) {
        SimplifyNode (node->left);
    }

    if (node->right != nullptr) {
        SimplifyNode (node->right);
    }

    if (node->type == OPERATOR && node->left->type == NUMBER && node->right->type == NUMBER) {
        double val = 0;

        double val1 = *(double *)node->left->data;
        double val2 = *(double *)node->right->data;

        if (STR_EQ ((char*)node->data, strMUL)) {
            val = val1 * val2;
        }
        if (STR_EQ ((char*)node->data, strDIV)) {
            val =val1 / val2;
        }
        if (STR_EQ ((char*)node->data, strADD)) {
            val = val1 + val2;
        }
        if (STR_EQ ((char*)node->data, strSUB)) {
            val = val1 - val2;
        }
        if (STR_EQ ((char*)node->data, strDEG)) {
            val = pow(val1, val2);
        }

        PushNumberToNode (node, val);
        DESTRUCT_TREE (node->left);
        DESTRUCT_TREE (node->right);

        node->left = nullptr;
        node->right = nullptr;

        return OK;
    }

    if (node->type == OPERATOR && STR_EQ ((char *)node->data, strMUL)) {
        if ((node->left->type == NUMBER && IsZero (node->left)) || (node->right->type == NUMBER && IsZero (node->right))) {
            PushNumberToNode (node, 0);
            DESTRUCT_TREE (node->left);
            DESTRUCT_TREE (node->right);

            node->left = nullptr;
            node->right = nullptr;

            return OK;
        }
        
        if (node->left->type == NUMBER && DoubleComp (*(double *)node->left->data, 1) == EQUAL) {
            DESTRUCT_TREE (node->left);
            node->left = nullptr;

            DiffNode *tmpNode = (DiffNode *)calloc (1, sizeof (DiffNode));
            RecursiveCpy (tmpNode, node->right);

            DESTRUCT_TREE (node->right);
            node->right = nullptr;

            RecursiveCpy (node, tmpNode);

            DESTRUCT_TREE (tmpNode);
            
            return OK;
        }
        if (node->right->type == NUMBER && DoubleComp (*(double *)node->right->data, 1) == EQUAL) {
            DESTRUCT_TREE (node->right);
            node->right = nullptr;

            DiffNode *tmpNode = (DiffNode *)calloc (1, sizeof (DiffNode));
            RecursiveCpy (tmpNode, node->left);

            DESTRUCT_TREE (node->left);
            node->left = nullptr;

            RecursiveCpy (node, tmpNode);

            DESTRUCT_TREE (tmpNode);

            return OK;
        }
    }
    if (node->type == OPERATOR && STR_EQ ((char *)node->data, strDEG)) {
        if ((node->left->type == NUMBER && IsZero (node->left))) {
            PushNumberToNode (node, 0);
            DESTRUCT_TREE (node->left);
            DESTRUCT_TREE (node->right);

            node->left = nullptr;
            node->right = nullptr;

            return OK;
        }
        if ((node->right->type == NUMBER && IsZero (node->right))) {
            PushNumberToNode (node, 1);
            DESTRUCT_TREE (node->left);
            DESTRUCT_TREE (node->right);

            node->left = nullptr;
            node->right = nullptr;

            return OK;
        }
    }
    if (node->type == OPERATOR && STR_EQ ((char *)node->data, strADD)) {
        if (node->left->type == NUMBER && DoubleComp (*(double *)node->left->data, 0) == EQUAL) {
            DESTRUCT_TREE (node->left);
            node->left = nullptr;            

            DiffNode *tmpNode = (DiffNode *)calloc (1, sizeof (DiffNode));
            RecursiveCpy (tmpNode, node->right);

            DESTRUCT_TREE (node->right);
            node->right = nullptr;

            RecursiveCpy (node, tmpNode);

            DESTRUCT_TREE (tmpNode);
            
            return OK;
        }
        if (node->right->type == NUMBER && DoubleComp (*(double *)node->right->data, 0) == EQUAL) {
            DESTRUCT_TREE (node->right);
            node->right = nullptr;

            DiffNode *tmpNode = (DiffNode *)calloc (1, sizeof (DiffNode));
            RecursiveCpy (tmpNode, node->left);

            DESTRUCT_TREE (node->left);
            node->left = nullptr;

            RecursiveCpy (node, tmpNode);

            DESTRUCT_TREE (tmpNode);

            return OK;
        }
    }
    
    return OK;
}

int IsZero (DiffNode *node) {
    assert (node);

    if (DoubleComp(*(double *)node->data, 0) == EQUAL) {
        return EQUAL;
    }

    return BIGGER;
}

void AssignVal (DiffNode *node) {
    assert (node);

    if (STR_EQ ((char *)node->data, strDEG)) {
        PushNumberToNode (node, 1);
    }
    if (STR_EQ ((char *)node->data, strMUL)) {
        PushNumberToNode (node, 0);
    }
}

int GetBuffer (char **buff, const char *fileName) {
    assert (buff);
    assert (fileName);

    FILE *sampleFile = fopen (fileName, "r");
    ASSERT_OK (sampleFile == nullptr, return UNABLETOOPENFILE);

    int fileSize = GetFileSize (sampleFile);

    fileSize = fread (*buff, sizeof (char), fileSize, sampleFile);
    fclose (sampleFile);

    return OK;
}

void PrintErrors (const int type) {
    switch (type) {
        case UNABLETOOPENFILE: {
            printf ("Unable to open file!\nError code: %d\n", type);

            break;
        }
        case INCORRECT_INPUT: {
            printf ("Incorrect input!\nError code: %d\n", type);

            break;
        }
        default: {
            printf ("Unknown error!\nError code: %d\n", type);
        }
    }
}

void RunDifferentiation (char *sample) {
    DiffNode *diffTree = (DiffNode *)calloc (1, sizeof (DiffNode));

    int res = GetExp (sample, &diffTree);
    ASSERT_OK (res != OK, PrintErrors (res); return);
    free (sample);

    while (1) {
        printf ("1 - differentiate whole function\n2 - differentiate function by variable\n");

        int type = 0;
        while (1) {
            int res = scanf ("%d", &type);

            if (res == 0) {
                while (getchar () != '\n') ;
                printf ("Please, type 1 or 2!\n");
            }
            else {
                break;
            }
        }

        switch (type) {
            case 1: {
                printf ("");
            }
        }
    }

        DiffNode *d = (DiffNode *)calloc (1, sizeof (DiffNode));
        Differentiate (diffTree, d, "y");

        SimplifyNode (d);
}