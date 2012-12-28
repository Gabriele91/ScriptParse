#include "LbFunction.h"

// function
LbFunctionContext* LbFunctionContext_Create(LbFunction* ptr,int args){
		args= ptr->args>args ? ptr->args : args;
		LbFunctionContext *out=(LbFunctionContext*)malloc(sizeof(LbFunctionContext));
		out->context=(LbVariable*)malloc(sizeof(LbVariable)*ptr->locals);
		for(int i=0;i<ptr->locals;++i)
			LbVariable_Init(&out->context[i]);
		out->args=(LbVariable*)malloc(sizeof(LbVariable)*args);
		out->countargs=args;
		out->countcontext=ptr->locals;
		return out;
}
void LbFunctionContext_Delete(LbFunctionContext* ptr){
	free(ptr->context);
	free(ptr->args);
	free(ptr);
}