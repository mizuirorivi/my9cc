#include<ctype.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Input program
extern char *user_input;
//現在着目しているトークン

///////////////////Details in error.c////////////////
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
///////////////////////////////////////////////////


// 
// tokenize.c
// 

//type of token
typedef enum{
    TK_RESERVED,    //symbol
    TK_NUM,         //Integer literals
    TK_EOF,         //EOFfile literals
    TokenKind_NUM   //number of TokenKind
}TokenKind;

typedef struct Token Token;
// Token type
struct Token{
    TokenKind kind; //トークンの型
    Token *next;    //次の入力トークン
    int val;        //kindがTK_NUMの場合,その数値
    char *str;      //トークン文字列
    int len;        //トークンの長さ
};
extern Token *token;




bool consume(char *);//consume character
void expect(char *);// expect and advance character
int expect_number(void);// expect number
Token *new_token(TokenKind kind,Token *cur,char *str,int len); // create new token
bool startswith(char *p,char *q);// string identification
Token *tokenize(void); //tokenization 

//
// parse.c
// 

//AST table
typedef enum{
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_EQ,  // == 
    ND_NE,  // !=
    ND_LT,  // <
    ND_LE,  // <=
    ND_NUM, //整数
}NodeKind;

//AST node type
typedef struct Node Node;

//抽象構文木のノードの型
struct Node{
    NodeKind kind; //ノードの型
    Node *lhs; //左辺
    Node *rhs; //右辺
    int val; //kindがND_NUMの場合のみ使う
};

Node *new_node(NodeKind kind);
Node *new_binary(NodeKind kind,Node *lhs,Node *rhs);
Node *new_num(int val);
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();
//
//codegen.c
//

void gen(Node *node);


