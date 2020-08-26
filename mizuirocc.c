#include"mizuirocc.h"

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

