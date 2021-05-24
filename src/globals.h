#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#define _CRT_SECURE_NO_WARNINGS

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

#define MAXTOKENLEN 45
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
extern int PrintScan;

typedef enum {DecK, StmtK, ExpK} NodeKind;
typedef enum {VarDeclaration, FunctionDeclaration, ParamDeclaration} DecKind;
typedef enum {Compound, Selection, Iteration, Return, Call} StmtKind;
typedef enum {Op, Id, Assign, Constant} ExpKind;

typedef enum {Void, Int} ExpType;

#define MAXCHILDREN 4

typedef struct treeNode {
    struct treeNode *child[MAXCHILDREN];
    struct treeNode *sibling;
    int lineno;
    NodeKind nodeKind;
    union { DecKind dec; StmtKind stmt; ExpKind exp; } kind;
    TokenType op; 
    int val; 
    char *name;
    ExpType type;
    int arrayType;
} TreeNode;

extern int Error;
#endif