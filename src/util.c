#include "globals.h"
#include "util.h"

void printToken(TokenType currentToken, const char* tokenString) {
    switch(currentToken) {
        case IF:
            fprintf(outputfile, "keyword: %s\n", "if");
        break;
        case ELSE:
            fprintf(outputfile, "keyword: %s\n", "else");
        break;
        case INT:
            fprintf(outputfile, "keyword: %s\n", "int");
        break;
        case VOID:
            fprintf(outputfile, "keyword: %s\n", "void");
        break;
        case RETURN:
            fprintf(outputfile, "keyword: %s\n", "return");
        break;
        case WHILE:
            fprintf(outputfile, "keyword: %s\n", "while");
        break;
        case ID:
            fprintf(outputfile, "ID, name= %s\n", tokenString);
        break;
        case NUM: 
            fprintf(outputfile, "NUM, var= %s\n", tokenString);
        break;
        case ASSIGN:
            fprintf(outputfile, "%s\n", "=");
        break;
        case PLUS: 
            fprintf(outputfile, "%s\n", "+");
        break;
        case MINUS: 
            fprintf(outputfile, "%s\n", "-");
        break;
        case TIMES: 
            fprintf(outputfile, "%s\n", "*");
        break;
        case OVER:
            fprintf(outputfile, "%s\n", "/");
        break;
        case LESSTHAN: 
            fprintf(outputfile, "%s\n", "<");
        break;
        case LESSEQTHAN: 
            fprintf(outputfile, "%s\n", "<=");
        break;
        case GREATERTHAN: 
            fprintf(outputfile, "%s\n", ">");
        break;
        case GREATEREQTHAN:
            fprintf(outputfile, "%s\n", ">=");
        break;
        case EQ:
            fprintf(outputfile, "%s\n", "==");
        break;
        case NEQ:
            fprintf(outputfile, "%s\n", "!=");
        break;
        case SEMI: 
            fprintf(outputfile, "%s\n", ";");
        break;
        case COMMA:
            fprintf(outputfile, "%s\n", ",");
        break;
        case LRNDBRKT: 
            fprintf(outputfile, "%s\n", "(");
        break;
        case RRNDBRKT:
            fprintf(outputfile, "%s\n", ")");
        break;
        case LCURLBRKT:
            fprintf(outputfile, "%s\n", "{");
        break;
        case RCURLBRKT:
            fprintf(outputfile, "%s\n", "}");
        break;
        case LSQRBRKT:
            fprintf(outputfile, "%s\n", "[");
        break;
        case RSQRBRKT:
            fprintf(outputfile, "%s\n", "]");
        break;
        case ENDFILE:
            fprintf(outputfile, "EOF\n");
        break;
        case ERROR:
            fprintf(outputfile, "ERROR, No such token \"%s\"\n", tokenString);
        break;
    }

}

static int indentno = 0;

#define INDENT indentno+=2
#define UNINDENT indentno-=2

static void printSpaces(void) {
    int i = 0;
    for(i = 0; i < indentno; ++i) {
        fprintf(outputfile, " ");
    }
}
void printTree(TreeNode *t) {
    int i;
    char type[20];
    INDENT;

    while(t != NULL) {
        printSpaces();
        if(t->nodeKind == DecK) {
            if(t->type == Int) {
                strcpy(type, "int");
            }
            else if(t->type == Void) {
                strcpy(type, "void");
            }
            else {
                strcpy(type, "error, unknown type");
            }

            switch(t->kind.dec) {
                // var-declaration
                case VarDeclaration:
                    // var-declaration with array
                    if(t->arrayType) {
                        fprintf(outputfile, "Variable Declaration: %s %s in size [%d]\n", type, t->name, t->val);
                    }
                    // var-declaration without array
                    else {

                        fprintf(outputfile, "Varible Declaration: %s %s\n", type, t->name);
                    }
                break;
                // function-declaration
                case FunctionDeclaration:
                    fprintf(outputfile, "Function Declaration: %s %s\n", type, t->name);
                break;
                // param
                case ParamDeclaration:
                    // param with array
                    if(t->arrayType) {
                        fprintf(outputfile, "Param Declaration: %s %s[]\n", type, t->name);
                    }
                    // param without array
                    else {

                        fprintf(outputfile, "Param Declaration: %s %s\n", type, t->name);
                    }
                break;
                default:
                    fprintf(outputfile, "Unknown DecK\n");
                break;
            }
        }
        else if(t->nodeKind == StmtK) {
            switch(t->kind.stmt) {
                // compound-stmt
                case Compound:
                    fprintf(outputfile, "Compound: \n");
                break;
                // selection-stmt
                case Selection:
                    fprintf(outputfile, "If: \n");
                break;
                // iteration-stmt
                case Iteration:
                    fprintf(outputfile, "While: \n");
                break;
                // return-stmt
                case Return:
                    fprintf(outputfile, "Return: \n");
                break;
                // call
                case Call:
                    fprintf(outputfile, "Call: %s\n", t->name);
                break;
                default:
                    fprintf(outputfile, "Unknown StmtK\n");
                break;
            }
        }
        else if(t->nodeKind == ExpK) {
            switch (t->kind.exp) {
                case Op:
                    fprintf(outputfile, "Op: ");
                    printToken(t->op, "\0");
                break;
                case Id:
                    fprintf(outputfile, "Id: %s\n", t->name);
                break;
                case Assign:
                    fprintf(outputfile, "Assign: \n");
                break;
                case Constant:
                    fprintf(outputfile, "Const: %d\n", t->val);
                break;
                default:
                    fprintf(outputfile, "Unknown ExpK\n");
                break;
            }
        }
        else {
            // error unknown node kind
            fprintf(outputfile, "Unkwon node kind\n");
        }

        for(i = 0; i < MAXCHILDREN; ++i) {
            printTree(t->child[i]);
        }

        t = t->sibling;
    }

    UNINDENT;
}