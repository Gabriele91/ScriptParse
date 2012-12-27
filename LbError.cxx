#include "LbError.h"


const char *LbErrorLog[]={
        "can't load variable, variable doesn't exist",
        "push id isn't valid",
        "can't save variable, variable doesn't exist",

        "stack[i-1]+stack[i]",
        "stack[i-1]-stack[i], must to be numbers",
        "stack[i-1]*stack[i], must to be numbers",
        "stack[i-1]/stack[i], must to be numbers",
		
        "stack[i-1]==stack[i]",
        "stack[i-1]!=stack[i]",
        "stack[i-1]>=stack[i], must to be numbers",
        "stack[i-1]<=stack[i], must to be numbers",
        "stack[i-1]>stack[i], must to be numbers",
        "stack[i-1]<stack[i], must to be numbers",

        "\'stack[i-1] and stack[i]\', must to be numbers",
        "\'stack[i-1] or stack[i]\', must to be numbers",
        "\'!stack[i]\', must to be a number",

        "in stack isn't have an argument",
        "in stack there aren't the two argument",
        "in stack there aren't the three argument"
};


///////////////////////////
//error
//
void LbError_Init(LbError *ptr){
    ptr->count=0;
    ptr->first=0;
    ptr->last=0;
}
void LbError_Push(LbError *ptr,uint id,uint line,uint asmline){

   LbErrorNode *tmp=(LbErrorNode*)malloc(sizeof(LbErrorNode));
   tmp->id=id;
   tmp->asmline=asmline;
   tmp->line=line;
   tmp->next=0;

   if(ptr->first==0) ptr->first=tmp;
   if(ptr->last!=0)  ptr->last->next=tmp;

   ptr->last=tmp;
   ++(ptr->count);
}
void LbError_Clean(LbError *ptr){
    LbErrorNode *tmp=ptr->first;
    //dealloc
    for(;tmp;tmp=tmp->next) free(tmp);
    //reset
    ptr->count=0;
    ptr->first=0;
    ptr->last=0;
}