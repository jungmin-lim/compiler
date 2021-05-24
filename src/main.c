#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

FILE *inputfile, *outputfile;
int lineno = 0;
int Error = FALSE;
int PrintScan = FALSE;

int main(int argc, const char * argv[]) {
    TreeNode *tree;

    // open inputfile & outputfile
    inputfile = fopen(argv[1], "r");
    outputfile = fopen(argv[2], "w");

    // get syntax tree
    tree = parse();
    if(tree != NULL) {
        fprintf(outputfile, "<<Syntax Tree>>\n");
        printTree(tree);
    }

    // close inputfile & outputfile
    fclose(inputfile);
    fclose(outputfile);
}