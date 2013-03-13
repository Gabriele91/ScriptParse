#include "TinyScript.h"
#include <iostream>

TinyScript::TinyScript(){
	excError=false;
	bytecode=NULL;
}
void TinyScript::PushCFunction(const std::string& name,LbCfunction fun){
	bGen.PushCFunction(name,fun);
}
bool TinyScript::DoStript(const std::string& scr){
	script=scr;
	//parse tree
	TreeNode* tree=pTree.StartParse(this->script);
	if(pTree.FindErrors()) return false; 
	//intermedie code
	iCode.ParseTree(tree);
	std::cout <<"\n\n"<<iCode.ToStringBasic();
	//gen bytecode
	bGen.ParseIntermedieCode(&iCode);
	std::cout <<"\n\n"<<iCode.ToStringBasic();
	//get bytecode
	bytecode=bGen.AllocLbBytecode();
	int i=0;
	for(auto& v:bytecode->variables){
		std::cout<< i <<" : " << v.GetName() << " ";
		if(v.IsNumber())
			std::cout<< " number:" << v.GetNumber();
		else if(v.IsString())
			std::cout<< " string:"<< v.GetString();
		else if(v.IsFunction())
			std::cout<< " function:"<< v.GetFunction();
		std::cout << std::endl;
		++i;
	}
	i=0;
	for(auto& f:bytecode->function){
		std::cout<< i <<" : function(" << f.args << ")" << std::endl;
		++i;
	}
	//
	return true;
}
bool TinyScript::FindErrors() const{
	return pTree.FindErrors()||excError;
}
std::string TinyScript::Errors(){
	std::string out;
	if(pTree.FindErrors()){
		out+=pTree.ErrorsToString();
	}
	if(excError){		
		out+=vm.GetLbError().ToString();
	}
	return out;
}
bool TinyScript::Execute(){
	//init bytecode
	vm.Init(bytecode);
	//execute
	excError=!vm.Execute();
	//return
	return !excError;
}