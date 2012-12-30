#include "GenByteCode.h"

void GenByteCode::ParseIntermedieCode(ToIntCode *itc){
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
			//seep 4.2, take global/local variables;
			(*it).second.BuildListGLobalLocal();
			//step 4.5, take args map
			GetArgsVariableFunctionMap((*it).second.funroot,(*it).second);			
			//take local map and set id		
			SetVariableFunctionMap(count_global_vars,
								   (*it).second,
								   *(*it).second.intcode);			
			//step 5, remove label
			tmplabel.clear();
			MappingAndRemoveLabels(tmplabel,*(*it).second.intcode);
			SetLabelId(tmplabel,*(*it).second.intcode);
			//set return
			ToIntCode::IntCode tmp;
			tmp.line=0;
			tmp.column=0;
			tmp.opcode=LB_RETURN;
			tmp.label=false;
			tmp.isconst=false;
			tmp.name="0";
			(*(*it).second.intcode).push_back(tmp);
		}

	}
LbBytecode* GenByteCode::AllocLbBytecode(){
		LbBytecode *out=new LbBytecode();
		/* global opcode */
		InitCommands(out->commands,intermediecode->intCode);
		/* local opcode */
		for(mapfunctionit it=global_function_map.begin();
			it!=global_function_map.end();
			it++){
			out->PushFunction();
			out->GetLastPush().args=(*it).second.args_map.size();
			out->GetLastPush().locals=(*it).second.local_map.size();
			InitCommands(out->GetLastPush().commands,*(*it).second.intcode);
		}
		/* set variable */
		for(mapfunctionit it=global_function_map.begin();
			it!=global_function_map.end();it++){
			out->PushVariable(LbVariable((*it).second.name,(*it).second.id_var));
		}
		for(mapvariableit it=global_variable_map.begin();
			it!=global_variable_map.end();it++){
				if((*it).second.is_string)
					out->PushVariable(LbVariable("$string",(*it).second.value));
				else
					out->PushVariable(LbVariable((*it).second.value,0.0f));
		}
		return out;
	}

void GenByteCode::InitCommands(std::vector<LbLineCommands>& commands,
					           std::vector<ToIntCode::IntCode>& itc){

		for(int i=0;i<itc.size();++i){
			commands.push_back(LbLineCommands(itc[i].line,
											  itc[i].column,
											  itc[i].opcode,
											  atof(itc[i].name.c_str())));
		}

	}


void GenByteCode::SetVariableGlobalMap(int& id_var_count,
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
				bool isstring=itc[i].token==Tokenizer::STRING;
				std::string tmpname=isstring ? "@"+itc[i].name : itc[i].name;
				mapvariableit itglobal=global_variable_map.find(tmpname);				
				if(itglobal!=global_variable_map.end()){
					itc[i].name=String::ToString((*itglobal).second.id_var);
					continue;
				}
				/* if not found */
				++id_var_count;
				Variable tmp;
				tmp.id_var=id_var_count;
				tmp.line=itc[i].line;
				tmp.is_string=isstring;	
				tmp.value=itc[i].name;				
				global_variable_map.insert(mapvariableinsert(tmpname,tmp));
				itc[i].name=String::ToString(tmp.id_var);
			}
		}
	}

void GenByteCode::GetArgsVariableFunctionMap(TreeNode *funroot,
									         Function& function){

		Variable tmp;
		/**/
		int coutargs=funroot->childs[0]->Size();
		for(int i=0;i<coutargs;++i){
			tmp.id_var=i;
			tmp.value=funroot->childs[0]->childs[i]->name;
			tmp.is_string=false;
			tmp.line=funroot->childs[i]->line;
			function.args_map.insert(mapvariableinsert(funroot->childs[0]->childs[i]->name,tmp));
		}
	}

void GenByteCode::SetVariableFunctionMap(int& id_var_count,
								         Function& function,
								         std::vector<ToIntCode::IntCode>& itc){

		for(int i=0;i<itc.size();++i){
			if(itc[i].opcode==LB_LOAD || 
			   itc[i].opcode==LB_SAVE){				  

				if(itc[i].token==Tokenizer::STRING||function.GetLineScopeGlobal(itc[i].name,itc[i].line,itc[i].column)){
					/* search in global */				
					bool isstring=itc[i].token==Tokenizer::STRING;
					std::string tmpname=isstring ? "@"+itc[i].name : itc[i].name;
					mapvariableit itglobal=global_variable_map.find(tmpname);					
					if(itglobal!=global_variable_map.end()){
						itc[i].name=String::ToString((*itglobal).second.id_var);
						continue;
					}
					//if is not a string...
					if(itc[i].token!=Tokenizer::STRING){
						/* search in functions names, if is a call */ 
						if(!itc[i].isconst){ //function isn't a const (or a string)
							mapfunctionit itfn=global_function_map.find(itc[i].name);
							if(itfn!=global_function_map.end()){
								itc[i].name=String::ToString((*itfn).second.id_var);
								continue;
							}
						}
					}
					/* if not found */
					++id_var_count;
					Variable tmp;
					tmp.id_var=id_var_count;
					tmp.line=itc[i].line;
					tmp.is_string=isstring;	
					tmp.value=itc[i].name;						
					global_variable_map.insert(mapvariableinsert(tmpname,tmp));
					itc[i].name=String::ToString(tmp.id_var);
					continue;
				}
				/* search in args */ 
				mapvariableit itargs=function.args_map.find(itc[i].name);		
				if(itargs!=function.args_map.end()){
					itc[i].opcode= itc[i].opcode==LB_LOAD? LB_LOADARG : LB_SAVEARG;
					itc[i].name=String::ToString((*itargs).second.id_var);
					continue;
				}
				/* search in local */ 
				mapvariableit itlocal=function.local_map.find(itc[i].name);				
				if(itlocal!=function.local_map.end()){
					itc[i].opcode= itc[i].opcode==LB_LOAD? LB_LOADLOCAL : LB_SAVELOCAL;
					itc[i].name=String::ToString((*itlocal).second.id_var);
					continue;
				}
				/* if is this function args */
				if(itc[i].opcode==LB_LOAD && function.name==itc[i].name){
					itc[i].name=String::ToString(function.id_var);
					itc[i].opcode=LB_LOADCALL;
					continue;
				}
				/* search in functions names, if is a call */ 
				if(itc[i].info==TreeNode::IS_CALL||itc[i].name[0]=='$'){					

					/* or other function */
					mapfunctionit itfn=global_function_map.find(itc[i].name);
					if(itfn!=global_function_map.end()){
						itc[i].name=String::ToString((*itfn).second.id_var);
						continue;
					}
				}
				/* if not found */
				Variable tmp;
				tmp.line=itc[i].line;
				tmp.value=itc[i].name;	
				tmp.is_string=false;	
				if(!tmp.is_string){
					tmp.id_var=function.local_map.size();
					function.local_map.insert(mapvariableinsert(itc[i].name,tmp));
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

void GenByteCode::MappingAndRemoveLabels(maplabel& maplb,std::vector<ToIntCode::IntCode>& itc){	
		for(int i=0;i<itc.size();++i){
			if(itc[i].label){
				maplb.insert(maplabelinsert(itc[i].name,i));
				itc.erase(itc.begin()+i);
				i=(i-1)*(i>0);//break if i>0
			}
		}
	}
void GenByteCode::SetLabelId(maplabel& maplb,std::vector<ToIntCode::IntCode>& itc){
		for(int i=0;i<itc.size();++i){
			if(itc[i].opcode==LB_IF ||
			   itc[i].opcode==LB_IF0||
			   itc[i].opcode==LB_GOTO ){
			   itc[i].name=String::ToString(maplb[itc[i].name]);
			}
		}	
	}