#ifndef TABLE_TREE
#define TABLE_TREE

#include "Map.h"
#include "ErrorParse.h"
#include "TreeNode.h"


struct TableTree{
	//GLOBAL
	TreeNode *root;
	DUNORDERED_MAP<std::string,TreeNode* > globals;
	typedef DUNORDERED_MAP<std::string,TreeNode* >::iterator ITVAR; 
	//
	void PushGlobal(const std::string& variable,TreeNode* node){
			globals[variable]=node;
	}
	TreeNode* GetGlobal(const std::string& variable){
		ITVAR it=globals.find(variable);
		if(globals.end()!=it) 
			return (*it).second;
		return NULL;
	}
	//function table
	struct FunctionTable{
		std::string name;
		TreeNode *root;
		DUNORDERED_MAP<std::string,TreeNode* > args;
		DUNORDERED_MAP<std::string,TreeNode* > locals;
		void PushArg(const std::string& variable, TreeNode* node){
			args[variable]=node;
		}
		void PushLocal(const std::string& variable,TreeNode* node){
			locals[variable]=node;
		}
		TreeNode* GetLocal(const std::string& variable){
			ITVAR it=locals.find(variable);
			if(locals.end()!=it) 
				return (*it).second;
			return NULL;
		}
		TreeNode* GetArg(const std::string& variable){
			ITVAR it=args.find(variable);
			if(args.end()!=it) 
				return (*it).second;
			return NULL;
		}
	};
	//function
	DUNORDERED_MAP<std::string,FunctionTable* > functions;	
	typedef DUNORDERED_MAP<std::string,FunctionTable* >::iterator ITFUN; 
	FunctionTable* GetFunction(const std::string& functionname){
		ITFUN it=functions.find(functionname);
		if(functions.end()!=it) 
			return (*it).second;
		return NULL;
	}
	void PushFunction(const std::string& functionname,FunctionTable *table){
		functions[functionname]=table;
	}
	void PushFunction(FunctionTable *table){
		functions[table->name]=table;
	}

};


#endif