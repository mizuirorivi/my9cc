#include<ctype.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    int pos = 4;
    printf("<test>\n");
    fprintf(stdout,"%*s",pos,"a");
    printf("\n");
    return 0;
}
