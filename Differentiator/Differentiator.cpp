#include "Differentiator.h"

int main (int argc, const char **argv) {
    ASSERT_OK (argv[1] == nullptr, PrintErrors (UNABLETOOPENFILE); return 0);
    
    char *sample = nullptr;
    int res = GetBuffer (&sample, argv [1]);
    ASSERT_OK (res != OK, PrintErrors (res); return 0);

    DiffNode *diffTree = nullptr;

    int res = GetExp (sample, &diffTree);
    ASSERT_OK (res != OK, return 0);
    free (sample);

    DiffNode *d = (DiffNode *)calloc (1, sizeof (DiffNode));
    Differentiate (diffTree, d, "y");

    SimplifyNode (d);

    FILE *dump = fopen ("DumpTree.txt", "w");

    DumpTree (d, dump);

    TexDump (diffTree, d);

    DESTRUCT_TREE (diffTree);

    DESTRUCT_TREE (d);

    return 0;
}
