#include "globals.h"
#include "util.h"

void printToken(TokenType currentToken, const char* tokenString) {
    switch(currentToken) {
        case IF:
        case ELSE:
        case INT:
        case VOID:
        case RETURN:
        case WHILE:
            fprintf(outputfile, "keyword: %s\n", tokenString);
        break;
        case ID:
            fprintf(outputfile, "ID, name= %s\n", tokenString);
        break;
        case NUM: 
            fprintf(outputfile, "NUM, var= %s\n", tokenString);
        break;
        case ASSIGN:
        case PLUS: 
        case MINUS: 
        case TIMES: 
        case OVER:
        case LESSTHAN: 
        case LESSEQTHAN: 
        case GREATERTHAN: 
        case GREATEREQTHAN:
        case EQ:
        case NEQ:
        case SEMI: 
        case COMMA:
        case LRNDBRKT: 
        case RRNDBRKT:
        case LCURLBRKT:
        case RCURLBRKT:
        case LSQRBRKT:
        case RSQRBRKT:
            fprintf(outputfile, "%s\n", tokenString);
        break;
        case ENDFILE:
            fprintf(outputfile, "EOF\n");
        break;
        case ERROR:
            fprintf(outputfile, "ERROR, No such token \"%s\"\n", tokenString);
        break;
    }
}