#ifndef VMCPP_H
#define VMCPP_H
#include <string>
#include "LbVariable.h"
#include "LbError.h"
#include "LbFunction.h"
#include "LbBytecode.h"



/**
* Class commands
* @addtogroup LbLineCommands
* @{
*/
typedef struct LbLineCommands{
    uint  line;       ///< source line
    uint  column;     ///< source column
    uchar command;    ///< command
    LbFloat id;       ///< value
	
	LbLineCommands(uint line,uint column,uint command,LbFloat id)
		:line(line)
		,column(column)
		,command(command)
		,id(id){}

}LbLineCommands;
enum Commands{

        LB_LOADCONST=0,
        LB_LOAD,
        LB_PUSH,
        LB_LPUSH,

        LB_SAVE,
        LB_POP,

        LB_CMD,
        LB_ADD,
        LB_MIN,
        LB_MUL,
        LB_DIV,
		
        LB_EQ,
        LB_NOTEQ,
        LB_GTEQ,
        LB_LTEQ,
        LB_GT,
        LB_LT,

        LB_AND,
        LB_OR,
        LB_NOT,

        LB_IF,
        LB_IF0,
        LB_GOTO,
        LB_EXIT,

		//function
		LB_CALL,
		LB_LOADCALL,
		LB_LOADLOCAL,
		LB_LOADARG,
		LB_SAVELOCAL,
		LB_SAVEARG,
		LB_RETURN,
		LB_LOADRETURN
};
extern const char* LbCommandsString[];
/**
* @}
*/



class LbVM{
	//program counter
    uint pc;
	//bytecode
	LbBytecode *bytecode;
	//current
	std::vector<LbLineCommands>* commands; 
	//stack
	short countstack;
    LbVariable stack[VMB_SIZE_STACK];
	//function context
	short countcontext;
    LbFunctionContext functionContext[VMB_SIZE_CONTEXT];
	short topstackfunction;
	//return variable
	LbVariable returnvariable;
	//errors;
    LbError errors;
	//methos:
	public:
	
	DFORCEINLINE short GetCountStack() const { return countstack; }
	DFORCEINLINE short GetCountContext() const { return countcontext; }
	DFORCEINLINE LbError&  GetLbError() { return errors; }

	DFORCEINLINE uint  GetPC() const { return pc; }
	DFORCEINLINE void  SetPC(uint _pc) { pc=_pc; }
	DFORCEINLINE const LbVariable&  GetReturn() const{ return returnvariable; }
	DFORCEINLINE void  SetReturn(const LbVariable& value) { returnvariable=value; }
	/* push error */
	DFORCEINLINE void PushError(LbError::LbErrorType type,const std::string& info){
		errors.PushError(type,(*commands)[pc].line,(*commands)[pc].column,pc,info);
	}
	DFORCEINLINE void PushError(LbError::LbErrorType type){
		errors.PushError(type,(*commands)[pc].line,(*commands)[pc].column,pc,"");
	}
	DFORCEINLINE void PushErrorIsFalse(bool iffalse,LbError::LbErrorType type,const std::string& info){
		if(!iffalse) errors.PushError(type,(*commands)[pc].line,(*commands)[pc].column,pc,info);
	}
	DFORCEINLINE void PushErrorIsFalse(bool iffalse,LbError::LbErrorType type){
		if(!iffalse) errors.PushError(type,(*commands)[pc].line,(*commands)[pc].column,pc,"");
	}
	/* variables/function */
	DFORCEINLINE LbVariable& VariableAt(int i){
		if(i>=this->bytecode->variables.size()){
			PushError(LbError::LB_CONTEXT,"id variable not valid"); 
			return LbVariable::GetInvalidVariable();
		}
		return bytecode->variables[i];
	}	
	DFORCEINLINE LbFunction& FunctionAt(int i){
		if(i>=this->bytecode->function.size()){
			PushError(LbError::LB_CONTEXT,"id function not valid"); throw(1);
		}
		return bytecode->function[i];
	}
	/* commands */
	DFORCEINLINE LbLineCommands& At(int i=0){
		if(i>=commands->size()){
			PushError(LbError::LB_CONTEXT,"id command not valid"); throw(1);
		}
		return (*commands)[i];
	}	
	DFORCEINLINE LbLineCommands& Command(){
		return (*commands)[pc];
	}
	DFORCEINLINE void SetMainCommands(){
		if(bytecode) commands=&(bytecode->commands);
	}
	DFORCEINLINE void SetCommands(std::vector<LbLineCommands>* commands){
		this->commands=commands;
	}
	DFORCEINLINE std::vector<LbLineCommands>* GetCommands(){
		return commands;
	}
	/* stack function */
	DFORCEINLINE LbVariable& Push(){
		++countstack;
		if(countstack>=VMB_SIZE_STACK){
			PushError(LbError::LB_STACK,"stack overflow");
			return LbVariable::GetInvalidVariable();
		}
		return stack[countstack];
	}	
	DFORCEINLINE LbVariable& Top(){
		return stack[countstack];
	}
	DFORCEINLINE LbVariable& StackAt(int i=0){
		if(countstack-i<0){
			PushError(LbError::LB_STACK,"invalid id"); 
			return LbVariable::GetInvalidVariable();
		}
		return stack[countstack-i];
	}
	DFORCEINLINE void Pop(){
		--countstack;
	}
	DFORCEINLINE void Pops(int v=1){
		countstack-=v;
	}
	/* Function and Context */
	DFORCEINLINE LbFunctionContext& PushFunctionContext(){
		++countcontext;
		if(countcontext>=VMB_SIZE_CONTEXT){
			PushError(LbError::LB_CONTEXT,"context overflow"); throw(1);
		}
		return  functionContext[countcontext];		
	}
	DFORCEINLINE LbFunctionContext* PushFunctionContextPtr(){
		++countcontext;
		if(countcontext>=VMB_SIZE_CONTEXT){
			PushError(LbError::LB_CONTEXT,"context overflow"); throw(1);
		}
		return  &functionContext[countcontext];		
	}
	DFORCEINLINE LbFunctionContext& GetFunctionContext(){
		return  functionContext[countcontext];		
	}
	DFORCEINLINE LbFunctionContext* GetFunctionContextPtr(){
		return  &functionContext[countcontext];		
	}
	DFORCEINLINE void PopFunctionContext(){
		functionContext[countcontext].Clear();
		--countcontext;
	}
	DFORCEINLINE LbFunction& GetFunction(ushort i){
		if(i>=bytecode->function.size()){
			PushError(LbError::LB_CONTEXT,"id function not valid"); throw(1);
		}
		return bytecode->function[i];
	}
	/* CFunction */
	DFORCEINLINE void SetTopArgs(short top){
		topstackfunction=top;
	}
	DFORCEINLINE short GetTopArgs(){
		return topstackfunction;
	}

	LbVM();
	void Init(LbBytecode *bytecode);
	bool Execute();
	bool Clear();



};

#endif