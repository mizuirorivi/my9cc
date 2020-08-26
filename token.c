#include"mizuirocc.h"
//
// Tokenizer
//





//次のトークンが期待している記号のときには、トークンを1つ読み進めて
//真を返す。それ以外の場合には偽を返す
bool consume(char *op){
    if(token->kind != TK_RESERVED || 
       strlen(op) != token->len ||
       memcmp(token->str,op,token->len)){
       
        return false;

    }
    token = token->next;
    return true;
}

//次のトークンが期待している記号の時には、トークンを1つ読み進める。
//それ以外の場合にはエラーを報告する。
void expect(char *op){
    if(token->kind != TK_RESERVED || 
       strlen(op) != token->len ||
       memcmp(token->str,op,token->len)){
        
        //error("'%c'ではありません",op);
        error_at(token->str,"expected \"%s\"",op);
  
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
Token *new_token(TokenKind kind,Token *cur,char *str,int len){
    Token *tok = calloc(1,sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}
//文字列判定

bool startswith(char *p,char *q){
    return memcmp(p,q,strlen(q)) == 0;
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
        if(startswith(p,"==") || startswith(p,"!=") ||
           startswith(p,"<=") || startswith(p,">=")){
            
            cur = new_token(TK_RESERVED,cur,p,2);
            p+=2;
            continue;

        }

        if(strchr("+-*/()<>",*p)){
            //p++としてるのは次の文字を見る為
            //new_tokenにはpが渡されて、その後pは+1される
            cur = new_token(TK_RESERVED,cur,p++,1);
            continue;
        }
        if(isdigit(*p)){
            cur = new_token(TK_NUM,cur,p,0);
            char *q = p;
            cur->val = strtol(p,&p,10);
            cur->len = p-q;
            continue;
        }
        error_at(p,"invalid token");
    }

    new_token(TK_EOF,cur,p,0);
    return head.next;
}

        