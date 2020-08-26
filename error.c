#include"mizuirocc.h"
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
