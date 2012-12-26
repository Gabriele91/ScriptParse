#ifndef GEN_BYTE_CODE_H
#define GEN_BYTE_CODE_H

#include "Map.h"
#include "IntCode.h"
#include "../LbBycode.h"

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
		DUNORDERED_MAP<std::string,Variable> args_map;
		DUNORDERED_MAP<std::string,Variable> local_map;
	};
	DUNORDERED_MAP<std::string,Function> global_function_map;
	typedef DUNORDERED_MAP<std::string,Function>::iterator  mapfunctionit;
	typedef std::pair<std::string,Function>  mapfunctioninsert;
	/* opcode */
	ToIntCode *intermediecode;

	GenByteCode(){}
	void ParseIntermedieCode(ToIntCode *itc){
		this->intermediecode=itc;
		//step 1, vector<function> to map
		int count_global_vars=itc->functions.size()-1;
		for(int i=0 ;i<itc->functions.size();++i){
			Function tmp;
			tmp.id_var=i;
			tmp.funroot=itc->functions[i].root;
			tmp.name=itc->functions[i].name;
			tmp.intcode=&(itc->functions[i].intCode);
			global_function_map
				.insert(mapfunctioninsert(itc->functions[i].name,tmp));
		}
		//step 2, take global variable, and set id
		SetVariableGlobalMap(count_global_vars,itc->intCode);		
		//setp 3, remove label (global)
		maplabel tmplabel;
		MappingAndRemoveLabels(tmplabel,itc->intCode);
		SetLabelId(tmplabel,itc->intCode);

		//stp 4,5 take args/local variable, and set id, and remove label
		for(mapfunctionit it=global_function_map.begin();
			it!=global_function_map.end();
			it++){
			//step 4.5, take args map
			GetArgsVariableFunctionMap((*it).second.funroot,(*it).second.args_map);			
			//take local map and set id		
			SetVariableFunctionMap(count_global_vars,
								   (*it).second.args_map,
								   (*it).second.local_map,
								   *(*it).second.intcode);			
			//step 5, remove label
			tmplabel.clear();
			MappingAndRemoveLabels(tmplabel,*(*it).second.intcode);
			SetLabelId(tmplabel,*(*it).second.intcode);
		}

	}
	LbBytecode* AllocLbBytecode(){
		LbBytecode *out=(LbBytecode*)malloc(sizeof(LbBytecode));
		/* global opcode */
		out->lenCommands=intermediecode->intCode.size();
		out->commands=(LbLineCommands*)malloc(sizeof(LbLineCommands)*out->lenCommands);
		InitCommands(out->lenCommands,out->commands,intermediecode->intCode);
		/* local opcode */
		out->functionscount=global_function_map.size();
		out->functions=(LbFunction*)malloc(sizeof(LbFunction)*out->functionscount);
		for(mapfunctionit it=global_function_map.begin();
			it!=global_function_map.end();
			it++){
			out->functions[(*it).second.id_var].lenCommands=intermediecode->intCode.size();
			out->functions[(*it).second.id_var].commands=(LbLineCommands*)malloc(sizeof(LbLineCommands)*out->functions[(*it).second.id_var].lenCommands);
			InitCommands(out->functions[(*it).second.id_var].lenCommands,
						 out->functions[(*it).second.id_var].commands,
						 *(*it).second.intcode);
			out->functions[(*it).second.id_var].args=(*it).second.args_map.size();
			out->functions[(*it).second.id_var].locals=(*it).second.local_map.size();
		}
		/* set variable */
		out->variablescount=global_variable_map.size()+out->functionscount;
		out->variables=(LbVariable*)malloc(sizeof(LbVariable)*out->variablescount);
		for(mapfunctionit it=global_function_map.begin();
			it!=global_function_map.end();
			it++){
			LbVariable_Init(&out->variables[(*it).second.id_var]);
			LbVariable_SetFunction(&out->variables[(*it).second.id_var],(*it).second.id_var);
		}
		for(mapvariableit it=global_variable_map.begin();
			it!=global_variable_map.end();
			it++){
				LbVariable_Init(&out->variables[(*it).second.id_var]);
				if((*it).second.is_string)
					LbVariable_SetString(&out->variables[(*it).second.id_var],(*it).second.value.c_str());
				else
					LbVariable_SetNumber(&out->variables[(*it).second.id_var],0);
		}
		return out;
	}

	void InitCommands(unsigned int& lenCommands,
					  LbLineCommands*& commands,
					  std::vector<ToIntCode::IntCode>& itc){

		lenCommands=itc.size();
		commands=(LbLineCommands*)malloc(sizeof(LbLineCommands)*lenCommands);
		for(int i=0;i<lenCommands;++i){
			//save command (opcode)
			commands[i].command=itc[i].opcode;
			//save id
			commands[i].id=atof(itc[i].name.c_str());
			//save line
			commands[i].line=itc[i].line;
		}

	}

protected:

	void SetVariableGlobalMap(int& id_var_count,
							  std::vector<ToIntCode::IntCode>& itc){


		for(int i=0;i<itc.size();++i){
			if(itc[i].opcode==LB_LOAD || 
			   itc[i].opcode==LB_SAVE){				   
				/* search in functions names */ 
				mapfunctionit itfn=global_function_map.find(itc[i].name);
				if(itfn!=global_function_map.end()){
					itc[i].name=String::ToString((*itfn).second.id_var);
					continue;
				}
				/* search in global */ 
				mapvariableit itglobal=global_variable_map.find(itc[i].name);				
				if(itglobal!=global_variable_map.end()){
					itc[i].name=String::ToString((*itglobal).second.id_var);
					continue;
				}
				/* if not found */
				++id_var_count;
				Variable tmp;
				tmp.id_var=id_var_count;
				tmp.line=itc[i].line;
				tmp.value=itc[i].name;	
				tmp.is_string=itc[i].token==Tokenizer::STRING;					
				global_variable_map.insert(mapvariableinsert(itc[i].name,tmp));
				itc[i].name=String::ToString(tmp.id_var);
			}
		}
	}

	void GetArgsVariableFunctionMap(TreeNode *funroot,mapvariable& varmapargs){
		int coutargs=funroot->childs[0]->Size();
		for(int i=0;i<coutargs;++i){
			Variable tmp;
			tmp.id_var=i;
			tmp.value=funroot->childs[0]->childs[i]->name;
			tmp.is_string=false;
			tmp.line=funroot->childs[i]->line;
			varmapargs.insert(mapvariableinsert(funroot->childs[0]->childs[i]->name,tmp));
		}
	}

	void SetVariableFunctionMap(int& id_var_count,
								mapvariable& varmapargs,
								mapvariable& varmaplocal,
								std::vector<ToIntCode::IntCode>& itc){

		for(int i=0;i<itc.size();++i){
			if(itc[i].opcode==LB_LOAD || 
			   itc[i].opcode==LB_SAVE){
				/* search in functions names */ 
				mapfunctionit itfn=global_function_map.find(itc[i].name);
				if(itfn!=global_function_map.end()){
					itc[i].name=String::ToString((*itfn).second.id_var);
					continue;
				}
				/* search in global */ 
				mapvariableit itglobal=global_variable_map.find(itc[i].name);				
				if(itglobal!=global_variable_map.end()){
					itc[i].name=String::ToString((*itglobal).second.id_var);
					continue;
				}	
				/* search in args */ 
				mapvariableit itargs=varmapargs.find(itc[i].name);		
				if(itargs!=varmapargs.end()){
					itc[i].opcode= itc[i].opcode==LB_LOAD? LB_LOADARG : LB_SAVEARG;
					itc[i].name=String::ToString((*itargs).second.id_var);
					continue;
				}
				/* search in local */ 
				mapvariableit itlocal=varmaplocal.find(itc[i].name);				
				if(itlocal!=varmaplocal.end()){
					itc[i].opcode= itc[i].opcode==LB_LOAD? LB_LOADLOCAL : LB_SAVELOCAL;
					itc[i].name=String::ToString((*itlocal).second.id_var);
					continue;
				}
				/* if not found */
				Variable tmp;
				tmp.line=itc[i].line;
				tmp.value=itc[i].name;	
				tmp.is_string=itc[i].token==Tokenizer::STRING;	
				if(!tmp.is_string){
					tmp.id_var=varmaplocal.size();
					varmaplocal.insert(mapvariableinsert(itc[i].name,tmp));
					itc[i].name=String::ToString(tmp.id_var);
					itc[i].opcode= itc[i].opcode==LB_LOAD ? LB_LOADLOCAL : LB_SAVELOCAL;
				}
				/* push a string in global (is a const) */
				else{
					++id_var_count;
					tmp.id_var=id_var_count;
					global_variable_map.insert(mapvariableinsert(itc[i].name,tmp));
					itc[i].name=String::ToString(tmp.id_var);				
				}
			}
		}
	}

	void MappingAndRemoveLabels(maplabel& maplb,std::vector<ToIntCode::IntCode>& itc){	
		for(int i=0;i<itc.size();++i){
			if(itc[i].label){
				maplb.insert(maplabelinsert(itc[i].name,i));
				itc.erase(itc.begin()+i);
				i=(i-1)*(i>0);//break if i>0
			}
		}
	}
	void SetLabelId(maplabel& maplb,std::vector<ToIntCode::IntCode>& itc){
		for(int i=0;i<itc.size();++i){
			if(itc[i].opcode==LB_IF ||
			   itc[i].opcode==LB_IF0||
			   itc[i].opcode==LB_GOTO ){
			   itc[i].name=String::ToString(maplb[itc[i].name]);
			}
		}	
	}
};

#endif