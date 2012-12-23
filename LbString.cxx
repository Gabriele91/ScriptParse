#include "LbString.h"

//string
LbString* LbString_Create(const char *str){
    LbString *out=(LbString*)malloc(sizeof(LbString));
    LbString_Init(out,str);
    //return
    return out;
}
void LbString_Init(LbString *in,const char *str){
    //get len
    in->len=strlen(str);
    //alloc memory
    if(in->len){
        in->str=(char*)malloc(sizeof(char)*(in->len+1));
        strcpy(in->str,str);
    }
    else{
        in->str=(char*)malloc(sizeof(char));
		strcpy(in->str,"");
	}
}

void LbString_Delete(LbString *string){

    LbString_Clean(string);
    free(string);
}
void LbString_Clean(LbString *string){
    if(string->str) free(string->str);
    string->str=0;
    string->len=0;
}

uint LbString_len(LbString *string){
    return string->len;
}
void LbString_AddString(LbString *outFirst,LbString *endLast){
    //
    LbString _tmp;
    LbString *out=&_tmp;
    //get len
    out->len=outFirst->len+endLast->len;
    //
    if(out->len){
        out->str=(char*)malloc(sizeof(char)*(out->len+1));
        strcpy(out->str,outFirst->str);
        strcpy(out->str+outFirst->len,endLast->str);
    }
    else
        out->str=0;
    //delete last string
    LbString_Clean(outFirst);
    //set new string
    outFirst->len=out->len;
    outFirst->str=out->str;
}
///////////////////////////