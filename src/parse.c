#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token;

static TreeNode *createNewNode(void);

static TreeNode *declarationList(void);
static TreeNode *declaration(void);
static TreeNode *varDeclaration(void);
static TreeNode *typeSpecifier(void);
static TreeNode *funDeclaration(void);
static TreeNode *params(void);
static TreeNode *paramList(void);
static TreeNode *param(void);
static TreeNode *compoundStmt(void);
static TreeNode *localDeclaration(void);
static TreeNode *statementList(void);
static TreeNode *statement(void);
static TreeNode *expressionStmt(void);
static TreeNode *selectionStmt(void);
static TreeNode *iterationStmt(void);
static TreeNode *returnStmt(void);
static TreeNode *expression(void);

static TreeNode *varCall(void);

static TreeNode *var(void);
static TreeNode *simpleExpression(TreeNode *n);
static TreeNode *relop(void);
static TreeNode *additiveExpression(TreeNode *n);
static TreeNode *addop(void);
static TreeNode *term(TreeNode *n);
static TreeNode *mulop(void);
static TreeNode *factor(TreeNode *n);
static TreeNode *call(void);
static TreeNode *args(void);
static TreeNode *argList(void);


static void syntaxError(char *message) {
    fprintf(outputfile, "\n>>> ");
    fprintf(outputfile, "Syntax error at line %d: %s", lineno, message);
    Error = TRUE;
}

char *copyString(const char *s) {
    int n;
    char *t;
    if(s == NULL) {
        return NULL;
    }
    n = strlen(s) + 1;
    t = (char*)malloc(sizeof(char)*n);
    if(t == NULL) {
        fprintf(stderr, "memory allocation error. exiting...\n");
        exit(EXIT_FAILURE);
    }
    strcpy(t, s);
    return t;
}

static void match(TokenType expected) {
    if(token == expected) {
        token = getToken();
    }
    else {
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        fprintf(outputfile, "      ");
    }
}

static TreeNode *createNewNode(void) {
    TreeNode *t = NULL;
    int i = 0;

    t = (TreeNode *)malloc(sizeof(TreeNode));
    if(t == NULL) {
        fprintf(stderr, "memory allocation error. exiting...\n");
        exit(EXIT_FAILURE);
    }
    else {
        for(i = 0; i < MAXCHILDREN; ++i) {
            t->child[i] = NULL;
        }
        t->sibling = NULL;
        t->lineno = lineno;
        t->arrayType = FALSE;
    }
    return t;
}

TreeNode *declarationList(void) {
    TreeNode *t = declaration();
    TreeNode *p = t;
    TreeNode *q = NULL;

    while(token != ENDFILE) {
        q = declaration();
        if(q != NULL) {
            if(t == NULL) {
                p = q;
                t = p;
            }
            else {
                p->sibling = q;    
                p = q;
            }
        }
    }

    return t;
}

static TreeNode *declaration(void) {
    TreeNode *t = createNewNode();
    char *n = NULL;

    // get type of declaration
    if(token == INT) {
        t->type = Int;
        match(INT);
    }
    else if (token == VOID) {
        t->type = Void;
        match(VOID);
    }
    else {
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
    }

    // get name of declared variable/function
    n = copyString(tokenString);
    match(ID);
    t->name = n;

    // case ';': var-declaration without array
    if(token == SEMI) {
        t->nodeKind = DecK;
        t->kind.dec = VarDeclaration;
        match(SEMI);
    }
    // case '[': var-declaration with array
    else if(token == LSQRBRKT) {
        t->nodeKind = DecK;
        t->kind.dec = VarDeclaration;
        t->arrayType = TRUE;

        match(LSQRBRKT);

        t->val = atoi(tokenString);
        match(NUM);

        match(RSQRBRKT);
        match(SEMI);
    }
    // case '(': function-declaration
    else if(token == LRNDBRKT) {
        t->nodeKind = DecK;
        t->kind.dec = FunctionDeclaration;

        match(LRNDBRKT);
        t->child[0] = paramList();
        match(RRNDBRKT);
        t->child[1] = compoundStmt();
    }
    // else: unexpected token
    else{ 
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
    }

    return t;
}

static TreeNode *varDeclaration(void) {
    TreeNode *t = createNewNode();
    char *n = NULL;

    // get type of declaration
    if(token == INT) {
        t->type = Int;
        match(INT);
    }
    else if (token == VOID) {
        t->type = Void;
        match(VOID);
    }
    else {
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
    }

    // get name of declared variable
    n = copyString(tokenString);
    match(ID);

    // case ';': variable declaration without array;
    if(token == SEMI) {
        t->nodeKind = DecK;
        t->kind.dec = VarDeclaration;
        
        t->name = n;
        match(SEMI);
    }
    else if(token == LSQRBRKT) {
        t->nodeKind = DecK;
        t->kind.dec = VarDeclaration;
        t->arrayType = TRUE;

        match(LSQRBRKT);

        t->name = n;
        t->val = atoi(tokenString);
        match(NUM);

        match(RSQRBRKT);
        match(SEMI);
    }
    else {
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
    }

    return t;
}

static TreeNode *typeSpecifier(void) {
    // No need. Implemented when needed
}

static TreeNode *funDeclaration(void) {
    // No need. Implemented in declaration(void)
}

static TreeNode *params(void) {
    // No need. Implemented in paramList(void)
}

static TreeNode *paramList(void) {
    TreeNode *t = NULL;
    TreeNode *p = NULL;
    TreeNode *q = NULL;

    // implementation of params->void. represent void in NULL
    if(token == VOID) {
        match(VOID);
        return NULL;
    }

    // get parameters
    t = param();
    p = t;

    while(token == COMMA) {
        match(COMMA);
        q = param();

        if(q != NULL) {
            p->sibling = q;
            p = q;
        }
    }

    return t;
}

static TreeNode *param(void) {
    TreeNode *t = createNewNode();
    char *n = NULL;

    // get type of declaration
    if(token == INT) {
        t->type = Int;
        match(INT);
    }
    else if(token == VOID) {
        t->type = Void;
        match(VOID);
    }
    else {
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
    }

    // get name of parameter
    n = copyString(tokenString);
    match(ID);

    // case '[': parameter declaration with array
    if(token == LSQRBRKT) {
        t->nodeKind = DecK;
        t->kind.dec = ParamDeclaration;
        t->arrayType = TRUE;
        
        t->name = n;
        match(LSQRBRKT);
        match(RSQRBRKT);
    }
    else {
        t->nodeKind = DecK;
        t->kind.dec = ParamDeclaration;
        t->name = n;
    }

    return t;
}

static TreeNode *compoundStmt(void) {
    TreeNode *t = createNewNode();

    t->nodeKind = StmtK;
    t->kind.stmt = Compound;

    match(LCURLBRKT);
    // case !'}': compound statement with some statments
    if(token != RCURLBRKT) {
        // compound statment with local declarations
        if((token == INT) || (token == VOID)) {
            t->child[0] = localDeclaration();
        }
        // compound statement with statments
        if(token != RCURLBRKT) {
            t->child[1] = statementList();
        }
    }
    match(RCURLBRKT);

    return t;
}

static TreeNode *localDeclaration(void) {
    TreeNode *t = NULL;
    TreeNode *p = NULL;
    TreeNode *q = NULL;

    t = varDeclaration();
    p = t;
    while((token == INT) || (token == VOID)) {
        q = varDeclaration();
        if(q != NULL) {
            p->sibling = q;
            p = q;
        }
    }

    return t;
}

static TreeNode *statementList(void) {
    TreeNode *t = NULL;
    TreeNode *p = NULL;
    TreeNode *q = NULL;

    t = statement();
    p = t;

    while(token != RCURLBRKT) {
        q = statement();

        if(q != NULL) {
            p->sibling = q;
            p = q;
        }
    }

    return t;
}

static TreeNode *statement(void) {
    TreeNode *t = NULL;

    // case #1: get expression statement
    if((token == SEMI) || (token == ID) || (token == LRNDBRKT) || (token == NUM)) {
        t = expressionStmt();
    }
    // case #2: get compound statement
    else if(token == LCURLBRKT) {
        t = compoundStmt();
    }
    // case #3: get selection statement
    else if(token == IF) {
        t = selectionStmt();
    }
    // case #4: get iteration statement
    else if(token == WHILE) {
        t = iterationStmt();
    }
    // case #5: get return statement
    else if(token == RETURN) {
        t = returnStmt();
    }

    return t;
}

static TreeNode *expressionStmt(void) {
    TreeNode *t = NULL;

    // case ';': expression with no information. no need to create new node
    if(token == SEMI) {
        match(SEMI);
    }
    else {
        t = expression();
        match(SEMI);
    }

    return t;
}

static TreeNode *selectionStmt(void) {
    TreeNode *t = createNewNode();

    t->nodeKind = StmtK;
    t->kind.stmt = Selection;

    match(IF);
    match(LRNDBRKT);
    t->child[0] = expression();
    match(RRNDBRKT);
    t->child[1] = statement();

    // case ELSE: get if statement with else part
    if(token == ELSE) {
        match(ELSE);
        t->child[2] = statement();
    }

    return t;
}

static TreeNode *iterationStmt(void) {
    TreeNode *t = createNewNode();

    t->nodeKind = StmtK;
    t->kind.stmt = Iteration;

    match(WHILE);
    match(LRNDBRKT);
    t->child[0] = expression();
    match(RRNDBRKT);
    t->child[1] = statement();

    return t;
}

static TreeNode *returnStmt(void) {
    TreeNode *t = createNewNode();

    t->nodeKind = StmtK;
    t->kind.stmt = Return;

    match(RETURN);
    // get expression to return
    if(token != SEMI) {
        t->child[0] = expression();
    }
    match(SEMI);

    return t;
}

static TreeNode *expression(void) {
    TreeNode *t = NULL;
    TreeNode *l = NULL;

    if(token == ID) {
        l = varCall();
    }

    // case '=': get assignment expression
    if((l != NULL) && (token == ASSIGN)) {
        // if l is var
        if((l->nodeKind == ExpK) && (l->kind.exp == Id)) {
            t = createNewNode();
            
            t->nodeKind = ExpK;
            t->kind.exp = Assign;
            match(ASSIGN);

            t->child[0] = l;
            t->child[1] = expression();
        }
        // if l is not var
        else {
            syntaxError("unexpected token -> ");
            printToken(token, tokenString);
            token = getToken();
        }
    }
    else {
        t = simpleExpression(l);
    }

    return t;
}

static TreeNode *varCall(void) {
    TreeNode *t = createNewNode();
    char *n = NULL;

    t->nodeKind = ExpK;
    t->kind.exp = Id;

    n = copyString(tokenString);
    match(ID);
    t->name = n;

    // case '(': get call
    if(token == LRNDBRKT) {
        t->nodeKind = StmtK;
        t->kind.stmt = Call;

        match(LRNDBRKT);
        t->child[0] = args();
        match(RRNDBRKT);
    }
    // case '[': get var with array
    else if(token == LSQRBRKT) {
        t->arrayType = TRUE;

        match(LSQRBRKT);
        t->child[0] = expression();
        match(RSQRBRKT);
    }

    return t;
}

static TreeNode *var(void) {
    // No need. Implemented in varCall()
}

static TreeNode *simpleExpression(TreeNode *n) {
    TreeNode *t;
    TreeNode *l;
    
    l = additiveExpression(n);
    if((token == LESSTHAN) || (token == LESSEQTHAN) || (token == GREATERTHAN) || 
        (token == GREATEREQTHAN) || (token == EQ) || (token == NEQ)) {

        t = createNewNode();
        t->nodeKind = ExpK;
        t->kind.exp = Op;
        t->op = token;
        match(token);

        t->child[0] = l;
        t->child[1] = additiveExpression(NULL);
    }
    else {
        t = l;
    }

    return t;
}

static TreeNode *relop(void) {
    // No need. Implemented in simpleExpression(TreeNode *)
}

static TreeNode *additiveExpression(TreeNode *n) {
    TreeNode *t = NULL;
    TreeNode *p = NULL;

    t = term(n);
    while((token == PLUS) || (token == MINUS)) {
        p = createNewNode();
        p->nodeKind = ExpK;
        p->kind.exp = Op;

        p->child[0] = t;
        p->op = token;

        t = p;
        match(token);
        t->child[1] = term(NULL);
    }

    return t;
}

static TreeNode *addop(void) {
    // No need. Implemented in additiveExpression(TreeNode *)
}

static TreeNode *term(TreeNode *n) {
    TreeNode *t = NULL;
    TreeNode *p = NULL;

    t = factor(n);
    while((token == TIMES) || (token == OVER)) {
        p = createNewNode();
        p->nodeKind = ExpK;
        p->kind.exp = Op;

        p->child[0] = t;
        p->op = token;

        t = p;
        match(token);
        t->child[1] = factor(NULL);
    }

    return t;
}

static TreeNode *mulop(void) {
    // No need. Implemented in term(TreeNode *)
}

static TreeNode *factor(TreeNode *n) {
    TreeNode *t = NULL;
    TreeNode *p = NULL;

    // n is factor->call. Return n
    if(n != NULL) {
        return n;
    }

    // case ID: get variable/call
    if(token == ID) {
        t = varCall();
    }
    // case '(': get ( expression )
    else if(token == LRNDBRKT) {
        match(LRNDBRKT);
        t = expression();
        match(RRNDBRKT);
    }
    // case NUM: get NUM
    else if(token == NUM) {
        t = createNewNode();
        
        t->nodeKind = ExpK;
        t->kind.exp = Constant;
        t->val = atoi(tokenString);
        t->type = Int;

        match(NUM);
    }
    else {
        syntaxError("unexpected token -> ");
        printToken(token, tokenString);
        token = getToken();
    }

    return t;
}

static TreeNode *call(void) {
    // No need. Implemented in varCall()
}

static TreeNode *args(void) {
    TreeNode *t = NULL;

    // case !')': nonempty args
    if(token != RRNDBRKT) {
        t = argList();
    }

    return t;
}

static TreeNode *argList(void) {
    TreeNode *t = NULL;
    TreeNode *p = NULL;
    TreeNode *q = NULL;

    t = expression();
    p = t;
    while(token == COMMA) {
        match(COMMA);

        q = expression();
        p->sibling = q;
        p = q;
    }

    return t;
}

TreeNode *parse(void) {
    TreeNode *t = NULL;

    token = getToken();
    t = declarationList();

    if(token != ENDFILE) {
        syntaxError("Code ends before file\n");
    }

    return t;
}