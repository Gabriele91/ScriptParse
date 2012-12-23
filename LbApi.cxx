#include "LbApi.h"
////////////////////////////////////////
/////////////// C API //////////////////
typedef int (*LSFuncPointer) (VMliteB*);
//GET
int LS_CountArgs(VMliteB *B){
	return (B->stackcount-B->topCallStack)+1;
}
LbVariable* LS_GetArgs(VMliteB *B,int i){
	return &B->stack[B->topCallStack+i-1];
}
const char* LS_StringArgs(VMliteB *B,int i){
	return LS_GetArgs(B,i)->var.string.str;
}
int LS_IntArgs(VMliteB *B,int i){
	return (int)(LS_GetArgs(B,i)->var.number);
}
LbFloat LS_NumberArgs(VMliteB *B,int i){
	return LS_GetArgs(B,i)->var.number;
}
int LS_FunctionArgs(VMliteB *B,int i){
	return LS_GetArgs(B,i)->var.id;
}
 LSFuncPointer LS_CFunctionArgs(VMliteB *B,int i){
	 return LS_GetArgs(B,i)->var.cfun;
}
//Is a ?
bool LS_IsANumber(VMliteB *B,int i){
	return LS_GetArgs(B,i)->type==VALUE_NUMBER;
}
bool LS_IsAString(VMliteB *B,int i){
	return LS_GetArgs(B,i)->type==VALUE_STRING;
}
bool LS_IsAFunction(VMliteB *B,int i){
	return LS_GetArgs(B,i)->type==VALUE_FUNCTION;
}
bool LS_IsACFunction(VMliteB *B,int i){
	return LS_GetArgs(B,i)->type==VALUE_CFUNCTION;
}
//Return
void LS_ReturnNumber(VMliteB *B,LbFloat v){
	 ++B->returncount;
	 LbVariable_SetNumber(&B->returnvariables[B->returncount],v);
}
void LS_ReturnString(VMliteB *B,const char* s){
	 ++B->returncount;
	 LbVariable_SetString(&B->returnvariables[B->returncount],s);
}
void LS_ReturnFunction(VMliteB *B,int i){
	 ++B->returncount;
	 LbVariable_SetFunction(&B->returnvariables[B->returncount],i);
}
void LS_ReturnCFunction(VMliteB *B,LSFuncPointer cfun){
	 ++B->returncount;
	 LbVariable_SetCFunction(&B->returnvariables[B->returncount],cfun);
}