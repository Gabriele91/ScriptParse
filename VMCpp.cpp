#include "VMCpp.h"

extern const char* LbCommandsString[]={
	 "LB_LOADCONST",
     "LB_LOAD",
     "LB_PUSH",
     "LB_LPUSH",

     "LB_SAVE",
     "LB_POP",

     "LB_CMD",
     "LB_ADD",
     "LB_MIN",
     "LB_MUL",
     "LB_DIV",
	 
     "LB_EQ",
     "LB_NOTEQ",
     "LB_GTEQ",
     "LB_LTEQ",
     "LB_GT",
     "LB_LT",

      "LB_AND",
      "LB_OR",
      "LB_NOT",

      "LB_IF",
      "LB_IF0",
      "LB_GOTO",
      "LB_EXIT",
	  
	  "LB_CALL",
	  "LB_LOADCALL",
	  "LB_LOADLOCAL",
	  "LB_LOADARG",
	  "LB_SAVELOCAL",
	  "LB_SAVEARG",
	  "LB_RETURN",
	  "LB_LOADRETURN"
};

void LbCmd_LOADCONST(LbVM *vm){
	vm->Push();
	vm->Top().SetNumber(vm->Command().id);
	vm->Top().SetName(String::ToString(vm->Command().id));
}
void LbCmd_LOAD(LbVM *vm){
	vm->Push()=vm->VariableAt(vm->Command().id);
}
void LbCmd_PUSH(LbVM *vm){
	vm->Push()=vm->StackAt(vm->Command().id);
}
void LbCmd_LPUSH(LbVM *vm){
	vm->Push()=vm->VariableAt(vm->StackAt(vm->Command().id).GetNumber());
}
void LbCmd_SAVE(LbVM *vm){
	vm->VariableAt(vm->Command().id).ReplaceValue(vm->Top());
	vm->Pop();
}
void LbCmd_POP(LbVM *vm){
	vm->Pop();
}
void LbCmd_CMD(LbVM *vm){
	vm->PushErrorIsFalse(vm->Top().Cmd(),
						 LbError::LB_CMD,
						 "-"+vm->Top().GetName());
}
void LbCmd_ADD(LbVM *vm){	
	vm->PushErrorIsFalse(vm->StackAt(1).Add(vm->Top()),
						 LbError::LB_ADD,
						 vm->StackAt(1).GetName()+"+"+vm->Top().GetName());
	vm->Pop();
}
void LbCmd_MIN(LbVM *vm){
	vm->PushErrorIsFalse(vm->StackAt(1).Sub(vm->Top()),
						 LbError::LB_MIN,
						 vm->StackAt(1).GetName()+"-"+vm->Top().GetName());
	vm->Pop();
}
void LbCmd_MUL(LbVM *vm){
	vm->PushErrorIsFalse(vm->StackAt(1).Mul(vm->Top()),
						 LbError::LB_MUL,
						 vm->Top().GetName()+"*"+vm->StackAt(1).GetName());
	vm->Pop();
}
void LbCmd_DIV(LbVM *vm){
	vm->PushErrorIsFalse(vm->StackAt(1).Div(vm->Top()),
						 LbError::LB_DIV,
						 vm->StackAt(1).GetName()+"/"+vm->Top().GetName());
	vm->Pop();
}
void LbCmd_EQ(LbVM *vm){	
	LbVariable tmp=vm->StackAt(1);
	vm->PushErrorIsFalse(tmp.Eq(vm->Top(),vm->StackAt(1)),
						 LbError::LB_EQ,
						 tmp.GetName()+"=="+vm->Top().GetName());
	vm->Pop();
}
void LbCmd_NOTEQ(LbVM *vm){	
	LbVariable tmp=vm->StackAt(1);
	vm->PushErrorIsFalse(tmp.NotEq(vm->Top(),vm->StackAt(1)),
						 LbError::LB_NOTEQ,
						 tmp.GetName()+"!="+vm->Top().GetName());
	vm->Pop();
}
void LbCmd_GTEQ(LbVM *vm){	
	LbVariable tmp=vm->StackAt(1);
	vm->PushErrorIsFalse(tmp.GtEq(vm->Top(),vm->StackAt(1)),
						 LbError::LB_GTEQ,
						 tmp.GetName()+">="+vm->Top().GetName());
	vm->Pop();
}
void LbCmd_LTEQ(LbVM *vm){
	LbVariable tmp=vm->StackAt(1);
	vm->PushErrorIsFalse(tmp.LtEq(vm->Top(),vm->StackAt(1)),
						 LbError::LB_LTEQ,
						 tmp.GetName()+"<="+vm->Top().GetName());
	vm->Pop();
}
void LbCmd_GT(LbVM *vm){
	LbVariable tmp=vm->StackAt(1);
	vm->PushErrorIsFalse(tmp.Gt(vm->Top(),vm->StackAt(1)),
						 LbError::LB_GT,
						 tmp.GetName()+">"+vm->Top().GetName());
	vm->Pop();
}
void LbCmd_LT(LbVM *vm){
	LbVariable tmp=vm->StackAt(1);
	vm->PushErrorIsFalse(tmp.Lt(vm->Top(),vm->StackAt(1)),
						 LbError::LB_LT,
						 tmp.GetName()+"<"+vm->Top().GetName());
	vm->Pop();
}
void LbCmd_AND(LbVM *vm){
	LbVariable tmp=vm->StackAt(1);
	vm->PushErrorIsFalse(tmp.And(vm->Top(),vm->StackAt(1)),
						 LbError::LB_AND,
						 tmp.GetName()+"&&"+vm->Top().GetName());
	vm->Pop();
}
void LbCmd_OR(LbVM *vm){
	LbVariable tmp=vm->StackAt(1);
	vm->PushErrorIsFalse(tmp.Or(vm->Top(),vm->StackAt(1)),
						 LbError::LB_OR,
						 tmp.GetName()+"||"+vm->Top().GetName());
	vm->Pop();
}
void LbCmd_NOT(LbVM *vm){
	vm->PushErrorIsFalse(vm->Top().Cmd(),
						 LbError::LB_NOT,
						 "!"+vm->Top().GetName());
}
void LbCmd_IF(LbVM *vm){
	if(!vm->Top().IsZero()) vm->SetPC(vm->Command().id-1);
	vm->Pop();
}
void LbCmd_IF0(LbVM *vm){
	if(vm->Top().IsZero()) vm->SetPC(vm->Command().id-1);
	vm->Pop();
}
void LbCmd_GOTO(LbVM *vm){
	vm->SetPC(vm->Command().id-1);
}
void LbCmd_EXIT(LbVM *vm){
	vm->SetPC(vm->GetCommands()->size());
}


void LbCmd_CALL(LbVM *vm){
	//get argc
	int argc=vm->Command().id;
	//a script function
	if(vm->StackAt(argc).IsFunction()){
		//add function context
		LbFunctionContext* functionctx=vm->PushFunctionContextPtr();
		//set call variable
		functionctx->varcall=vm->StackAt(argc);
		//copy args
		for(uint ui=0;ui<argc;++ui){ //copy stack in args
			functionctx->PushArg(vm->StackAt(argc-ui-1));
		}
		int max_arg=vm->GetFunction(vm->StackAt(argc).GetFunction()).args;
		if(argc<max_arg) functionctx->arg.resize(max_arg,LbVariable("arg",0.0f));
		//set local variables
		int max_locals=vm->GetFunction(vm->StackAt(argc).GetFunction()).locals;
		functionctx->local.resize(max_locals,LbVariable("local",0.0f));
		//save return point
		functionctx->rtPc=vm->GetPC();
		functionctx->rtCommands=vm->GetCommands();
		//set function context
		vm->SetCommands(&vm->GetFunction(vm->StackAt(argc).GetFunction()).commands);
		vm->SetPC(-1);
		//pop values and function
		vm->Pops(argc+1);
	}
	//is a CFunction?
	else
	if(vm->StackAt(argc).IsCFunction()){
		//"save args"
		vm->SetTopArgs(argc);
		//call
		vm->StackAt(argc).GetCFunction()(vm);
		//pop values and function
		vm->Pops(argc+1);
	}
	//else, error
	else
		vm->PushError(LbError::LB_CALL,vm->StackAt(argc+1).GetName()+" is not a function");
}
void LbCmd_LOADCALL(LbVM *vm){
	vm->Push()=vm->GetFunctionContextPtr()->varcall;
}
void LbCmd_LOADLOCAL(LbVM *vm){
	vm->Push()=vm->GetFunctionContextPtr()->LocalAt(vm,vm->Command().id);
}
void LbCmd_LOADARG(LbVM *vm){
	vm->Push()=vm->GetFunctionContextPtr()->ArgAt(vm,vm->Command().id);
}
void LbCmd_SAVELOCAL(LbVM *vm){	
	vm->GetFunctionContextPtr()->LocalAt(vm,vm->Command().id).ReplaceValue(vm->Top());
	vm->Pop();
}
void LbCmd_SAVEARG(LbVM *vm){	
	vm->GetFunctionContextPtr()->ArgAt(vm,vm->Command().id).ReplaceValue(vm->Top());
	vm->Pop();
}
void LbCmd_RETURN(LbVM *vm){
	//get return
	if(vm->Command().id>0){
		vm->SetReturn(vm->Top());
		vm->Pop();
	}
	//set old context
	LbFunctionContext* functionctx=vm->GetFunctionContextPtr();
	vm->SetPC(functionctx->rtPc);
	vm->SetCommands(functionctx->rtCommands);
	//delete context
	vm->PopFunctionContext();
}
void LbCmd_LOADRETURN(LbVM *vm){
	vm->Push()=vm->GetReturn();
}

LbFnCommands listCommands[]={
        LbCmd_LOADCONST,
        LbCmd_LOAD,
        LbCmd_PUSH,
        LbCmd_LPUSH,

        LbCmd_SAVE,
        LbCmd_POP,
		
        LbCmd_CMD,
        LbCmd_ADD,
        LbCmd_MIN,
        LbCmd_MUL,
        LbCmd_DIV,
        LbCmd_EQ,
		LbCmd_NOTEQ,
        LbCmd_GTEQ,
        LbCmd_LTEQ,
        LbCmd_GT,
        LbCmd_LT,
        LbCmd_AND,
        LbCmd_OR,
        LbCmd_NOT,
        LbCmd_IF,
        LbCmd_IF0,
        LbCmd_GOTO,
        LbCmd_EXIT,

        LbCmd_CALL,
        LbCmd_LOADCALL,
        LbCmd_LOADLOCAL,
        LbCmd_LOADARG,
		LbCmd_SAVELOCAL,
		LbCmd_SAVEARG,
		LbCmd_RETURN,
		LbCmd_LOADRETURN
};

LbVM::LbVM():pc(0)
			 ,commands(0)
			 ,countstack(-1)
			 ,countcontext(-1)
{

}
void LbVM::Init(LbBytecode *bytecode){

	this->bytecode=bytecode;
	this->pc=0;
	this->SetMainCommands();
	this->countcontext=-1;
	this->countstack=-1;

}
bool LbVM::Execute(){
	try{
		for(pc;commands 
					&& 
					pc<commands->size() 
					&& 
					!errors.Size();    
			++pc) 
				listCommands[(int)(*commands)[pc].command](this);
	}
	catch(int i){
		return false;
	}
	return !errors.Size();
}
bool LbVM::Clear(){
	return false;
}
