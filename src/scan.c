#include "globals.h"
#include "util.h"
#include "scan.h"


#define BUFLEN 256

typedef enum {
    START, CHECKCOMMENT, INCOMMENT,BREAKCOMMENT,
    INID, INNUM, ERRORSTATE, INSYMBOL, DONE
}StateType;

typedef struct _Keyword{
    char* str;
    TokenType tok;
}Keyword;

static Keyword keyWords[MAXRESERVED] = {
    {"if", IF}, {"else", ELSE}, {"return", RETURN},
    {"int", INT}, {"void", VOID}, {"while", WHILE}
};

char tokenString[MAXTOKENLEN+1];

static char lineBuf[BUFLEN];
static int linepos = 0;
static int bufsize = 0;
static int EOF_flag = FALSE;

static int isDigit(int c) {
    c = c - '0';
    if((0 <= c ) && (c <= 9)) {
        return TRUE;
    }
    return FALSE;
}

static int isLetter(int c) {
    if((('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z'))) {
        return TRUE;
    }
    return FALSE;
}

static int getNextChar(void) {
    if(!(linepos < bufsize)) {
        lineno++;
        if(fgets(lineBuf, BUFLEN-1, inputfile)) {
            if(PrintScan) {
                fprintf(outputfile, "%4d: %s", lineno, lineBuf);
            }
            bufsize = strlen(lineBuf);
            // for some OS that takes newline other than \n
            lineBuf[bufsize-2] = '\n';
            linepos = 0;
            
            return lineBuf[linepos++];
        }
        else{
            EOF_flag = TRUE;
            return EOF;
        }
    }
    else {
        return lineBuf[linepos++];
    }
}

static void ungetNextChar(void) {
    if(!EOF_flag) linepos--;
}

static TokenType keywordLookup(char *s) {
    int i;
    for (i = 0; i < MAXRESERVED; ++i) {
        if(!strcmp(s, keyWords[i].str)) {
            return keyWords[i].tok;
        }
    }

    return ID;
}

TokenType getToken(void) {
    int tokenStringIndex = 0;
    TokenType currentToken;
    StateType state = START;
    int save;

    while(state != DONE) {
        int c = getNextChar();

        switch(state) {
            // initial state
            case START:
                save = TRUE;
                // start scanning NUM token
                if(isDigit(c)) {
                    state = INNUM;
                }
                
                // start scanning ID or KEYWORD token
                else if(isLetter(c)) {
                    state = INID;
                }
                
                // start scanning comment or OVER token
                else if(c == '/') {
                    state = CHECKCOMMENT;
                }

                // start scanning bi-SYMBOL token
                else if((c == '!') || (c == '<') || (c == '>') || (c == '=')) {
                    state = INSYMBOL;
                }

                // passing whitespaces
                else if((c == ' ') || (c == '\n') || (c == '\t')) {
                    save = FALSE;
                }

                // returning uni-SYMBOL tokens
                else {
                    state = DONE;
                    switch (c) {
                        case EOF:
                            save = FALSE;
                            currentToken = ENDFILE;
                        break;
                        case '+':
                            currentToken = PLUS;
                        break;
                        case '-':
                            currentToken = MINUS;
                        break;
                        case '*':
                            currentToken = TIMES;
                        break;
                        case '(':
                            currentToken = LRNDBRKT;
                        break;
                        case ')':
                            currentToken = RRNDBRKT;
                        break;
                        case '{':
                            currentToken = LCURLBRKT;
                        break;
                        case '}':
                            currentToken = RCURLBRKT;
                        break;
                        case '[':
                            currentToken = LSQRBRKT;
                        break;
                        case ']':
                            currentToken = RSQRBRKT;
                        break;
                        case ';':
                            currentToken = SEMI;
                        break;
                        case ',':
                            currentToken = COMMA;
                        break;
                        default:
                            currentToken = ERROR;
                        break;
                    } 
                }
            break;

            // start of scanning comment session
            // state for checking if '*' comes after '/'
            case CHECKCOMMENT:
                // '*' after '/', move to INCOMMENT state and remove '/' from tokenString
                if(c == '*') {
                    save = FALSE;
                    tokenStringIndex--;
                    state = INCOMMENT;
                }
                // other character after '/', return OVER token
                else {
                    ungetNextChar();
                    save = FALSE;
                    currentToken = OVER;
                    state = DONE;
                }
            break;
            // does nothing until '*' comes
            case INCOMMENT:
                if(c == '*') {
                    state = BREAKCOMMENT;
                }
                else if(c == EOF) {
                    state = DONE;
                    currentToken = ENDFILE;
                }
            break;
            // state for checking if '/' comes after '*'
            case BREAKCOMMENT:
                // '/' after '*', comment session over. move to START state
                if(c == '/') {
                    state = START;
                }
                else if(c == '*') {
                    // just consume a character;
                }
                else if(c == EOF) {
                    state = DONE;
                    currentToken = ENDFILE;
                }
                // other character after '*', go back to INCOMMENT state
                else {
                    state = INCOMMENT;
                }
            break;
            // end of scanning comment session

            // start of scanning bi-SYMBOL session
            case INSYMBOL:
                // '=' after '!', '<', '>', '=', return NEQ, LESSEQTHAN, GREATEREQTHAN, EQ
                if(c == '=') {
                    switch(tokenString[0]) {
                        case '!':
                            currentToken = NEQ;
                            state = DONE;
                        break;
                        case '<':
                            currentToken = LESSEQTHAN;
                            state = DONE;
                        break;
                        case '>':
                            currentToken = GREATEREQTHAN;
                            state = DONE;
                        break;
                        case '=':
                            currentToken = EQ;
                            state = DONE;
                        break;
                        // should never happen
                        default:
                            // DEBUGGING: report error state
                            fprintf(outputfile, "Scanner Bug: state = %d\n", state);
                            state = DONE;
                            currentToken = ERROR;
                        break;
                    }
                }
                // any other character after '!', '<', '>', '=', return ERROR, LESSTHAN, GREATERTHAN, 
                else {
                    ungetNextChar();
                    save = FALSE;
                    switch(tokenString[0]) {
                        case '!':
                            currentToken = ERROR;
                            state = DONE;
                        break;
                        case '<':
                            currentToken = LESSTHAN;
                            state = DONE;
                        break;
                        case '>':
                            currentToken = GREATERTHAN;
                            state = DONE;
                        break;
                        case '=':
                            currentToken = ASSIGN;
                            state = DONE;
                        break;
                        // should never happen
                        default:
                            // DEBUGGING: report error state
                            fprintf(outputfile, "Scanner Bug: state = %d\n", state);
                            state = DONE;
                            currentToken = ERROR;
                        break;
                    }
                }
            break;
            // end of scanning bi-SYMBOL session

            // start of scanning ID session
            // state for scanning letter for ID
            case INID:
                // keep scanning if another letter comes in
                if(isLetter(c)) {
                    state = INID;
                }
                // move to ERRORID state if digit comes in
                else if (isDigit(c)) {
                    state = ERRORSTATE;
                }
                // return ID if any other character comes in
                else {
                    ungetNextChar();
                    save = FALSE;
                    currentToken = ID;
                    state = DONE;
                }
            break;
            // state for scanning letters and digits for error token
            case ERRORSTATE:
                // keep scanning letters and digits
                if(isLetter(c) || isDigit(c)) {
                    state = ERRORSTATE;
                }
                // return error token if another character comes in
                else {
                    ungetNextChar();
                    save = FALSE;
                    currentToken = ERROR;
                    state = DONE;
                }
            break;
            // end of scanning ID session

            // start of scanning NUM session
            case INNUM:
                if(isDigit(c)) {
                    state = INNUM;
                }
                else if (isLetter(c)) {
                    state = ERRORSTATE;
                }
                else {
                    ungetNextChar();
                    save = FALSE;
                    currentToken = NUM;
                    state = DONE;
                }
            break;
            // end of scanning NUM session

            // should never happen
            case DONE:
            break;
            default: 
                // DEBUGGING: report error state
                fprintf(outputfile, "Scanner Bug: state = %d\n", state);
                state = DONE;
                currentToken = ERROR;
            break;
        }

        // save token character if save flag is on
        if((save) && (tokenStringIndex <= MAXTOKENLEN)) {
            tokenString[tokenStringIndex++] = (char) c;
        }

        // close token string and check if it is one of keywords
        if(state == DONE) {
            tokenString[tokenStringIndex] = '\0';
            if(currentToken == ID) {
                currentToken = keywordLookup(tokenString);
            }
        }
    }

    // report scanned token
    if(PrintScan) {
        fprintf(outputfile, "\t%d: ", lineno);
        printToken(currentToken, tokenString);
    }

    return currentToken;
}