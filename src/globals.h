#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXRESERVED 6

typedef enum {
    ENDFILE, ERROR,
    IF, ELSE, INT, RETURN, VOID, WHILE,
    ID, NUM,
    ASSIGN, PLUS, MINUS, TIMES, OVER,
    LESSTHAN, LESSEQTHAN, GREATERTHAN, GREATEREQTHAN,
    EQ, NEQ, SEMI, COMMA,
    LRNDBRKT, RRNDBRKT, LCURLBRKT, RCURLBRKT, LSQRBRKT, RSQRBRKT,
}TokenType;

extern FILE* inputfile;
extern FILE* outputfile;

extern int lineno;

#endif