#define _CRT_SECURE_NO_WARNINGS

#include "globals.h"
#include "util.h"
#include "scan.h"

FILE *inputfile, *outputfile;
int lineno = 0;

int main(int argc, const char * argv[]) {
    TokenType result;
    // open inputfile & outputfile
    inputfile = fopen(argv[1], "r");
    outputfile = fopen(argv[2], "w");
    
    while((result = getToken()) != ENDFILE) {
        // keep getting token until EOF
    }
    // close inputfile & outputfile
    fclose(inputfile);
    fclose(outputfile);
}