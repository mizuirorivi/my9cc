#include<ctype.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//
// Tokenizer
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
};

//Input program
char *user_input;

//現在着目しているトークン
Token *token;

//////////////////////////error block//////////////////////////////
//Reports an error and exit.
//takes same argument as printf
void error(char *fmt, ...){
    va_list ap;
    va_start(ap,fmt);
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    exit(1);
}

//Reports an error location and exit.
void error_at(char *loc,char *fmt, ...){
    va_list ap;
    va_start(ap,fmt);

    int pos = loc - user_input;
    fprintf(stderr,"%s\n",user_input);
    fprintf(stderr,"%*s",pos,"");//print pos spaces.
    fprintf(stderr,"^ ");
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    exit(1);
}
////////////////////////////////////////////////////////////////////


//次のトークンが期待している記号のときには、トークンを1つ読み進めて
//真を返す。それ以外の場合には偽を返す
bool consume(char op){
    if(token->kind != TK_RESERVED || token->str[0]!=op){
        return false;
    }
    token = token->next;
    return true;
}

//次のトークンが期待している記号の時には、トークンを1つ読み進める。
//それ以外の場合にはエラーを報告する。
void expect(char op){
    if(token->kind != TK_RESERVED || token->str[0] != op){
        //error("'%c'ではありません",op);
        error_at(token->str,"expected '%c'",op);
    }
    token = token->next;
}

//次のトークンが数値の時、トークンを一つ読み進めてその数値を返す。
//それ以外の場合にはエラーを報告する
int expect_number(){
    if(token->kind != TK_NUM){
        error_at(token->str,"expected a number");
     }
     int val = token->val;
     token = token->next;
     return val;
}


bool at_eof(){
    return token->kind == TK_EOF;
}


//新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind,Token *cur,char *str){
    Token *tok = calloc(1,sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

//入力文字列pをトークナイズしてそれを返す
Token *tokenize(){
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;
    while(*p){
        //空白文字をスキップ
        if(isspace(*p)){
            p++;
            continue;
        }
        if(strchr("+-*/()",*p)){
            //p++としてるのは次の文字を見る為
            //new_tokenにはpが渡されて、その後pは+1される
            cur = new_token(TK_RESERVED,cur,p++);
            continue;
        }
        if(isdigit(*p)){
            cur = new_token(TK_NUM,cur,p);
            cur->val = strtol(p,&p,10);
            continue;
        }
        error_at(p,"expected a number");
    }

    new_token(TK_EOF,cur,p);
    return head.next;
}

///
/// Parser
///

//抽象構文木のノードの種類
typedef enum{
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
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

Node *new_node(NodeKind kind){
    Node *node = calloc(1,sizeof(Node));
    node->kind = kind;
    return node;
}

Node *new_binary(NodeKind kind,Node *lhs,Node *rhs){
    Node *node = new_node(kind);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_num(int val){
    Node *node = new_node(ND_NUM);
    node->val = val;
    return node;
}

Node *expr();
Node *mul();
Node *primary();

// expr = mul ("+" mul | "-" mul")*
Node *expr(){
    Node *node = mul();
    for(;;){
        if(consume('+')){
            node = new_binary(ND_ADD,node,mul());
        }else if(consume('-')){
            node = new_binary(ND_SUB,node,mul());
        }else{
            return node;
        }
    }
}
// mul = primary("*" primary | "/" primary)*
Node *mul(){
    Node *node = primary();
    for(;;){
        if(consume('*')){
            node = new_binary(ND_MUL,node,primary());
        }else if(consume('/')){
            node = new_binary(ND_DIV,node,primary());
        }else{
            return node;
        }
    }
}
//primary = "(" expr ")" | num
Node *primary(){
    //次のトークンが"("なら、"(" expr ")"のはず
    if(consume('(')){
        Node *node = expr();
        expect(')');
        return node;
    }
    //それ以外
    return new_num(expect_number());
}


///
///Code generator
///

//式に応じたアセンブリの出力
void gen(Node *node){
    if(node->kind == ND_NUM){
        printf("    push %d\n",node->val);
        return;
    }
    gen(node->lhs);
    gen(node->rhs);
    
    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch(node->kind){
        case ND_ADD:
            printf("    add rax, rdi\n");
            break;
        case ND_SUB:
            printf("    sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("    imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("    cqo\n");
            printf("    idiv rdi\n");
            break;
    }
    printf("    push rax\n");
}
        
int main(int argc,char **argv){
    if(argc != 2){
        error("%s: invalid number of arguments",argv[0]);
        return 1;
    }
    //Tokenize and parse.
    user_input = argv[1];
    token = tokenize();
    Node *node = expr();
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    gen(node);
    //スタックトップに式全体の値が残っているはずなので
    //それはRAXにロードして関数からの返り値とする
    printf("    pop rax\n");
    printf("    ret\n");
    return 0;
}

