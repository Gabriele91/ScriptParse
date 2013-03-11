#ifndef TINYSCRIPT_H
#define TINYSCRIPT_H

#include <string>
#include "Parse/ParseTree.h"
#include "Parse/IntCode.h"
#include "Parse/GenByteCode.h"
#include "VMCpp.h"


class TinyScript{
	
	public :

	std::string script;
	ParseTree pTree;
	ToIntCode iCode;
	GenByteCode bGen;
	LbBytecode  *bytecode;
	LbVM vm;

	bool excError;


		TinyScript();		
		void PushCFunction(const std::string& name,LbCfunction fun);
		bool DoStript(const std::string& script);
		bool FindErrors() const;
		std::string Errors();
		bool Execute();
		void Clear(){};

};


#endif