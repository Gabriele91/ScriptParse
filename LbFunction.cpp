#include "LbType.h"
#include "LbVariable.h"
#include "LbFunction.h"
#include "VMCpp.h"

LbVariable& LbFunctionContext::LocalAt(LbVM *vm,int i){
	if(i>=local.size()){
		vm->PushError(LbError::LB_CONTEXT,"id variable not valid in "+varcall.GetName()); throw(1);
	}
	return local[i];
}
	
LbVariable& LbFunctionContext::ArgAt(LbVM *vm,int i){
	if(i>=arg.size()){
		vm->PushError(LbError::LB_CONTEXT,"id variable not valid in "+varcall.GetName()); throw(1);
	}
	return arg[i];
}