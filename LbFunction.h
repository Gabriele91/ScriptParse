#ifndef LBFUNCTION_H
#define LBFUNCTION_H

#include "LbType.h"
#include "LbVariable.h"

class LbFunction{

	public:

	std::vector<LbLineCommands> commands;
	uint locals;
	uint args; 	
	
	DFORCEINLINE void PushCommands(const LbLineCommands& com){
		commands.push_back(com);
	}
	LbFunction():locals(0),args(0){}
	LbFunction(uint locals,uint args):locals(locals),args(args){}

};

class LbFunctionContext{

	public:

	LbVariable					varcall;
	std::vector<LbVariable>		arg;
	std::vector<LbVariable>		local;
	DFORCEINLINE void Clear(){
		varcall.Clear();
		arg.clear();
		local.clear();
	}
	void PushArg(const LbVariable& variable){
		arg.push_back(variable);
	}
	void Pushlocal(const LbVariable& variable){
		local.push_back(variable);
	}
	///RETURN VALUE
	uint rtPc;
	std::vector<LbLineCommands> *rtCommands;
	///get vars	
	LbVariable& LocalAt(LbVM *vm,int i);
	LbVariable& ArgAt(LbVM *vm,int i);
};

#endif