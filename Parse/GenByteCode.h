#ifndef GEN_BYTE_CODE_H
#define GEN_BYTE_CODE_H

#include <vector>
#include "Map.h"
#include "IntCode.h"
#include "../VMCpp.h"

struct GenByteCode{

	/* map variable */
	struct Variable{
		int id_var;
		int line;
		bool is_string;
		std::string value;
	};
	DUNORDERED_MAP<std::string,Variable> global_variable_map;
	typedef DUNORDERED_MAP<std::string,Variable>  mapvariable;
	typedef DUNORDERED_MAP<std::string,Variable>::iterator  mapvariableit;
	typedef std::pair<std::string,Variable>  mapvariableinsert;
	/* map label */ 
	typedef DUNORDERED_MAP<std::string,int>  maplabel;
	typedef DUNORDERED_MAP<std::string,int>::iterator  maplabelit;
	typedef std::pair<std::string,int>  maplabelinsert;
	/* map functions */
	struct Function{	
		TreeNode *funroot;	
		std::vector<ToIntCode::IntCode>* intcode;
		int id_var;
		std::string name;
		/* scope */
		std::vector<TreeNode*> vGlobalLocal;
		void BuildListGLobalLocal(TreeNode *node=NULL){	
			if(node==NULL){ node=funroot; }
			else
			if(node->token==Tokenizer::GLOBAL||
			   node->token==Tokenizer::LOCAL) 
				vGlobalLocal.push_back(node);

			for(int i=0;i<node->Size();++i){
				BuildListGLobalLocal((*node)[i]);
			}
		}
		bool GetLineScopeGlobal(const std::string& var,int line,int column){
			const TreeNode& node=*funroot;
			bool isglobal=false;
			for(int i=0;i<vGlobalLocal.size();++i){
				if(vGlobalLocal[i]->line>line) break;
				if(vGlobalLocal[i]->line==line && vGlobalLocal[i]->column>column) break;
				for(int v=0;v<(*vGlobalLocal[i]).Size();++v)
						if(var==(*vGlobalLocal[i])[v]->name)
								isglobal=vGlobalLocal[i]->token==Tokenizer::GLOBAL;
				
			}
			return isglobal;
		}
		/*  */
		DUNORDERED_MAP<std::string,Variable> args_map;
		DUNORDERED_MAP<std::string,Variable> local_map;
	};
	DUNORDERED_MAP<std::string,Function> global_function_map;
	typedef DUNORDERED_MAP<std::string,Function>::iterator  mapfunctionit;
	typedef std::pair<std::string,Function>  mapfunctioninsert;
	/* opcode */
	ToIntCode *intermediecode;

	GenByteCode(){}
	void ParseIntermedieCode(ToIntCode *itc);
	LbBytecode* AllocLbBytecode();

	void InitCommands(std::vector<LbLineCommands>& commands,
					  std::vector<ToIntCode::IntCode>& itc);

protected:

	void SetVariableGlobalMap(int& id_var_count,
							  std::vector<ToIntCode::IntCode>& itc);

	void GetArgsVariableFunctionMap(TreeNode *funroot,
									Function& function);

	void SetVariableFunctionMap(int& id_var_count,
								Function& function,
								std::vector<ToIntCode::IntCode>& itc);

	void MappingAndRemoveLabels(maplabel& maplb,std::vector<ToIntCode::IntCode>& itc);
	void SetLabelId(maplabel& maplb,std::vector<ToIntCode::IntCode>& itc);
};

#endif