#ifndef LBBYTECODE_H
#define LBBYTECODE_H


class LbBytecode{

	public:

	std::vector<LbLineCommands> commands;
	std::vector<LbVariable>		variables;
	std::vector<LbFunction>		function;

	DFORCEINLINE void PushVariable(const LbVariable& var){
		variables.push_back(var);
	}
	DFORCEINLINE void PushFunction(const LbFunction& fun){
		function.push_back(fun);
	}
	DFORCEINLINE void PushFunction(){
		function.push_back(LbFunction());
	}
	DFORCEINLINE LbFunction& GetLastPush(){
		return function[function.size()-1];
	}
	DFORCEINLINE void PushCommands(const LbLineCommands& com){
		commands.push_back(com);
	}
	
};

#endif