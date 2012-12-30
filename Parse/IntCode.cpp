#include "IntCode.h"

	/* public */
ToIntCode::ToIntCode(){}
std::string ToIntCode::ToString(){
		std::string out="intermedie opcode:\n";
		//for all code in root
		for(int i=0;i<intCode.size();++i) out+=PrintIniCode(intCode[i],i);
		//for all function
		for(int f=0;f<functions.size();++f){
			//for all code in function
			out+="\nintermedie opcode ("+functions[f].name+"):\n";
			for(int i=0;i<functions[f].intCode.size();++i)
				out+=PrintIniCode(functions[f].intCode[i],i);
		}
		//
		return out;
	}	
std::string ToIntCode::ToStringBasic(){
		std::string out="intermedie opcode:\n";
		//for all code in root
		for(int i=0;i<intCode.size();++i) out+=PrintIniCodeBasic(intCode[i],i);
		//for all function
		for(int f=0;f<functions.size();++f){
			//for all code in function
			out+="\nintermedie opcode ("+functions[f].name+"):\n";
			for(int i=0;i<functions[f].intCode.size();++i)
				out+=PrintIniCodeBasic(functions[f].intCode[i],i);
		}
		//
		return out;
	}
void ToIntCode::ParseTree(TreeNode* node){
		int tmp_label=0;
		int tmp_label_if=0;
		this->root=node;
		//find function, and replace whit variable
		GetFunctionAndReplaceWithVariabe(node);
		//for all statements in root
		for(int i=0;i<root->Size();++i){
			GenInitBitecode(intCode,(*root)[i],tmp_label,tmp_label_if);
		}
		//Optimization
		MathOptimizazione(intCode);
		//for all function
		for(int f=0;f<functions.size();++f){
			bool tmp_islocal=true;
			//for all statements in function
			for(int i=1;i<functions[f].root->Size();++i){
				if((*functions[f].root)[i]->token!=Tokenizer::GLOBAL &&
				   (*functions[f].root)[i]->token!=Tokenizer::LOCAL    )//jmp local and global token
					GenInitBitecode(functions[f].intCode,(*functions[f].root)[i],tmp_label,tmp_label_if);
			}
			//Optimization
			MathOptimizazione(functions[f].intCode);
		}
	}
/* protected */
void ToIntCode::GetFunctionAndReplaceWithVariabe(TreeNode* node){	
		//if is a function function
		if(node->token==Tokenizer::DEF){ 
			
			/* create function */
			Function function;

			if(node->childs[0]->info==TreeNode::IS_LESSNAME_HEADER){
				//Anonymous function
				function.name="$Function_"+String::ToString(functions.size());
				//if is   <variable> = <function>
				//replace <variable> = $<variable>
				if(node->parent->token==Tokenizer::ASSIGNAMENT){
					/***
					*            <=>
					*           /   \
					*          /     \
					* <variable>     $<variable>
					*/
					(*node->parent)[1]=
						new TreeNode(node->line,node->column,Tokenizer::VARIABLE,function.name);
				}
				//if is <variable> = <function><less name call>
				else{
					/***
					*  <variable||IS_LESSNAME_CALL>
					*          |   \\\
					*          |    \\\
					*        <def>  <args>
					to
					*  <variable||IS_CALL>
					*        /|\
					*       / | \
					*      < args >
					*/			
					node->parent->name=function.name;
					node->parent->RemoveChild(0);
					node->parent->info=TreeNode::IS_CALL;
				}
			}
			else{	
				//search def/root
				TreeNode *p=NULL;
				for(p=node->parent;p!=NULL;p=p->parent){
					if(p->token==Tokenizer::DEF){
						break;
					}
				}
				//
				if(p!=NULL){
				/***
				* def function in function
				*
				*        <def>
				*          |   
				*          |    
				*        <def>
				to				
				*         <no root>
				*             |   
				*             |    
				*            <=>
				*           /   \
				*          /     \
				* <variable>     $<variable>
				*/
				//get name                 //header // name 
				//Anonymous function
				function.name="$Function_"+String::ToString(functions.size());
				//if is   <variable> = <function>
				std::string namefunctionvar=node->childs[0]->name;
				int index=node->parent->IndexChild(node);
				node->parent->childs[index]=
					  new TreeNode(node->line,node->column,Tokenizer::ASSIGNAMENT,"");
				node->parent->childs[index]->PushChild(
					  new TreeNode(node->line,node->column,Tokenizer::VARIABLE,namefunctionvar));
				node->parent->childs[index]->PushChild(
					  new TreeNode(node->line,node->column,Tokenizer::VARIABLE,function.name));
				}
				else{

					/***
					*        <root>
					*          |   
					*          |    
					*        <def>
					to	
					*        <root>
					*/

					//name function in first child node
					function.name=node->childs[0]->name;
					//dt node
					node->parent->RemoveChild(node);
				}

			}
			//
			function.root=node;
			//push into table
			functions.push_back(function);		
			/* create function */
		}	
		//else go to leafs
		int size_vc=node->Size();
		for(int i=0;i<size_vc;++i) {
			GetFunctionAndReplaceWithVariabe((*node)[i]);
			//if delete, redo....
			if(size_vc!=node->Size()){ --i; size_vc=node->Size(); }
		}
	}
void ToIntCode::GenInitBitecode(std::vector<ToIntCode::IntCode>& intCode,
								TreeNode* node,
								int& labelcount,
								int& labelcountif){
		
		IntCode tmp;
		tmp.token=node->token;
		tmp.line=node->line;
		tmp.column=node->column;
		//unused tmp.root_child_id=node->root_child_id;
		tmp.valid=true;
		tmp.label=false;
		tmp.info=node->info;
		
		int tmp_line;
		int tmp2_line;
		int tmp_code;

		std::string label_code;

		switch (node->token)
		{
		case Tokenizer::STRING:
			tmp.opcode=LB_LOAD;
			tmp.name=node->name;
			tmp.isconst=true;
			intCode.push_back(tmp);
			break;
		case Tokenizer::VARIABLE:
			//push val name
			tmp.opcode=LB_LOAD;
			tmp.name=node->name;
			tmp.isconst=false;
			intCode.push_back(tmp);
			if(node->info==TreeNode::IS_CALL){
				//push args
				for(int i=0;i<node->Size();++i){
					GenInitBitecode(intCode,(*node)[i],labelcount,labelcountif);
				}
				//push call
				tmp.opcode=LB_CALL;
				tmp.name=String::ToString(node->Size());
				tmp.isconst=false;
				intCode.push_back(tmp);
				//if a parent is no a def|if[elif|else]|for|do|while or root:
				if(node->parent->token!=Tokenizer::DEF &&
				   node->parent->token!=Tokenizer::FOR && 
				   node->parent->token!=Tokenizer::IF && 
				   node->parent->token!=Tokenizer::ELIF &&
				   node->parent->token!=Tokenizer::ELSE && 
				   node->parent->token!=Tokenizer::DO && 
				   node->parent->token!=Tokenizer::WHILE && 
				   node->parent->parent){
				   tmp.token=node->token;
				   tmp.line=node->line;
				   tmp.valid=true;
				   tmp.label=false;
				   tmp.opcode=LB_LOADRETURN;
				   tmp.name="1";
				   tmp.isconst=false;
				   intCode.push_back(tmp);
				}
			}
			break;
		case Tokenizer::NUMBER:
			tmp.opcode=LB_LOADCONST;
			tmp.name=node->name;
			tmp.isconst=true;
			intCode.push_back(tmp);
			break;

		#define BYTECODE_DUAL_OP(X,Y)\
		case Tokenizer::X:\
			GenInitBitecode(intCode,node->childs[0],labelcount,labelcountif);\
			GenInitBitecode(intCode,node->childs[1],labelcount,labelcountif);\
			tmp.opcode=Y;\
			tmp.name=node->name;\
			tmp.isconst=true;\
			intCode.push_back(tmp);\
			break;
		BYTECODE_DUAL_OP(ADD,LB_ADD)			
		case Tokenizer::MIN:
			if(node->Size()==2){
				GenInitBitecode(intCode,node->childs[0],labelcount,labelcountif);
				GenInitBitecode(intCode,node->childs[1],labelcount,labelcountif);
				tmp.opcode=LB_MIN;
				tmp.name=node->name;
				tmp.isconst=true;
				intCode.push_back(tmp);
			}
			else{				
				GenInitBitecode(intCode,node->childs[0],labelcount,labelcountif);
				tmp.opcode=LB_CMD;
				tmp.name=node->name;
				tmp.isconst=true;
				intCode.push_back(tmp);
			}
			break;
		BYTECODE_DUAL_OP(MUL,LB_MUL)
		BYTECODE_DUAL_OP(DIV,LB_DIV)
		
		BYTECODE_DUAL_OP(EQ,LB_EQ)
		BYTECODE_DUAL_OP(NOTEQ,LB_NOTEQ)
		BYTECODE_DUAL_OP(GT,LB_GT)
		BYTECODE_DUAL_OP(LT,LB_LT)
		BYTECODE_DUAL_OP(GTE,LB_GTEQ)
		BYTECODE_DUAL_OP(LTE,LB_LTEQ)

		BYTECODE_DUAL_OP(OR,LB_OR)
		BYTECODE_DUAL_OP(AND,LB_AND)
	
		case Tokenizer::NOT:
			GenInitBitecode(intCode,node->childs[0],labelcount,labelcountif);
			tmp.opcode=LB_NOT;
			tmp.name=node->name;
			tmp.isconst=true;
			intCode.push_back(tmp);
			break;		
			
		case Tokenizer::ASSIGNAMENT:
			GenInitBitecode(intCode,node->childs[1],labelcount,labelcountif);
			tmp.opcode=LB_SAVE;
			tmp.name=node->childs[0]->name;
			tmp.isconst=true;
			intCode.push_back(tmp);
			break;

		case Tokenizer::RETURN:
			/* exp */			
			GenInitBitecode(intCode,node->childs[0],labelcount,labelcountif);
			tmp.opcode=LB_RETURN;
			tmp.name=String::ToString(node->Size());
			tmp.isconst=true;
			intCode.push_back(tmp);	
			break;

		case Tokenizer::WHILE:	
			//increment count label
			++labelcount;
			/* save label */
			tmp_line=labelcount;
			/* push label */
			tmp.opcode=-1;
			tmp.name=".start$"+String::ToString(tmp_line);;
			tmp.isconst=true;
			tmp.label=true;
			intCode.push_back(tmp);	
			/* parse exp */
			GenInitBitecode(intCode,node->childs[0],labelcount,labelcountif);	
			/* push exp */
			tmp.opcode=LB_IF0;
			tmp.name=".end$"+String::ToString(tmp_line);
			tmp.isconst=true;
			tmp.label=false;
			intCode.push_back(tmp);	
			/* parse statements */
			for(int i=1;i<node->Size();++i)
				GenInitBitecode(intCode,node->childs[i],labelcount,labelcountif);		
			/* goto to exp (loop)*/
			tmp.opcode=LB_GOTO;
			tmp.name=".start$"+String::ToString(tmp_line);
			tmp.isconst=true;
			tmp.label=false;
			intCode.push_back(tmp);	
			/* push label exit while */
			label_code=".end$"+String::ToString(tmp_line);
			tmp.label=true;
			tmp.opcode=-1;
			tmp.name=label_code;
			tmp.isconst=true;
			intCode.push_back(tmp);	
			/* end */
			break;
		case Tokenizer::FOR:
			/* parse assignaments (first part) */
			for(int i=0;i<node->childs[0]->childs[0]->Size();++i)
				GenInitBitecode(intCode,node->childs[0]->childs[0]->childs[i],labelcount,labelcountif);
			//increment count label
			++labelcount;
			/* save label */
			tmp_line=labelcount;
			/* push label */
			tmp.opcode=-1;
			tmp.name=".startfor$"+String::ToString(tmp_line);
			tmp.isconst=true;
			tmp.label=true;
			intCode.push_back(tmp);	
			/* parse exp */
			GenInitBitecode(intCode,node->childs[0]->childs[1]->childs[0],labelcount,labelcountif);	
			/* push exp */
			tmp.opcode=LB_IF0;
			tmp.name=".end$"+String::ToString(tmp_line);
			tmp.isconst=true;
			tmp.label=false;
			intCode.push_back(tmp);	
			/* parse statements */
			for(int i=1;i<node->Size();++i)
				GenInitBitecode(intCode,node->childs[i],labelcount,labelcountif);	
			/* push label */
			tmp.opcode=-1;
			tmp.name=".start$"+String::ToString(tmp_line);;
			tmp.isconst=true;
			tmp.label=true;
			intCode.push_back(tmp);	
			/* parse assignaments (last part) */
			for(int i=0;i<node->childs[0]->childs[2]->Size();++i)
				GenInitBitecode(intCode,node->childs[0]->childs[2]->childs[i],labelcount,labelcountif);
			/* goto to exp (loop)*/
			tmp.opcode=LB_GOTO;
			tmp.name=".startfor$"+String::ToString(tmp_line);
			tmp.isconst=true;
			tmp.label=false;
			intCode.push_back(tmp);	
			/* push label exit while */
			label_code=".end$"+String::ToString(tmp_line);
			tmp.label=true;
			tmp.opcode=-1;
			tmp.name=label_code;
			tmp.isconst=true;
			intCode.push_back(tmp);	
			/* end */
			break;
		case Tokenizer::DO:			
			//increment count label
			++labelcount;
			/* save label */
			tmp_line=labelcount;
			/* push label */
			tmp.opcode=-1;
			tmp.name=".startdo$"+String::ToString(tmp_line);;
			tmp.isconst=true;
			tmp.label=true;
			intCode.push_back(tmp);				
			/* parse statements */
			for(int i=0;i<(node->Size()-1);++i)
				GenInitBitecode(intCode,node->childs[i],labelcount,labelcountif);			
			/* push label */
			tmp.opcode=-1;
			tmp.name=".start$"+String::ToString(tmp_line);;
			tmp.isconst=true;
			tmp.label=true;
			intCode.push_back(tmp);	
			/* parse exp */
			GenInitBitecode(intCode,node->childs[(node->Size()-1)],labelcount,labelcountif);	
			/* push exp */
			tmp.opcode=LB_IF;
			tmp.name=".startdo$"+String::ToString(tmp_line);
			tmp.isconst=true;
			tmp.label=false;
			intCode.push_back(tmp);				
			/* push label exit do-while */
			label_code=".end$"+String::ToString(tmp_line);
			tmp.label=true;
			tmp.opcode=-1;
			tmp.name=label_code;
			tmp.isconst=true;
			intCode.push_back(tmp);	
			break;
		case Tokenizer::IF:
			//increment count label
			++labelcountif;
			/* save label */
			tmp_line=labelcountif;
			/* push label */
			tmp.opcode=-1;
			tmp.name=".startif$"+String::ToString(tmp_line);
			tmp.isconst=true;
			tmp.label=true;
			intCode.push_back(tmp);	
			/* parse exp */
			GenInitBitecode(intCode,node->childs[0],labelcount,labelcountif);	
			/* push if */
			tmp.opcode=LB_IF0;
			tmp.name=".endif$"+String::ToString(tmp_line);
			tmp.isconst=true;
			tmp.label=false;
			tmp_code=intCode.size();
			intCode.push_back(tmp);	
			//parse statments			
			for(int i=1;i<node->Size();++i){	
				if(node->childs[i]->token==Tokenizer::ELIF){
					//* next label *//
					++labelcountif;
					//* last if/else if, go to there *//
					intCode[tmp_code].name=".startif$"+String::ToString(labelcountif);
					//* go to end if before if/elseif crack true*//
					tmp.opcode=LB_GOTO;
					tmp.name=".endif$"+String::ToString(tmp_line);
					tmp.isconst=true;
					tmp.label=false;
					intCode.push_back(tmp);						
					/* push label */
					tmp.opcode=-1;
					tmp.name=".startif$"+String::ToString(labelcountif);
					tmp.isconst=true;
					tmp.label=true;
					intCode.push_back(tmp);
					/* parse exp */
					GenInitBitecode(intCode,node->childs[i]->childs[0],labelcount,labelcountif);	
					/*************/
					++labelcountif;
					tmp.opcode=LB_IF0;
					tmp.name=".endif$"+String::ToString(tmp_line);
					tmp.isconst=true;
					tmp.label=false;
					tmp_code=intCode.size();
					intCode.push_back(tmp);	
					/* parse stament */		
					for(int efi=1;efi<node->childs[i]->Size();++efi)				
						GenInitBitecode(intCode,node->childs[i]->childs[efi],labelcount,labelcountif);
				}
				else if(node->childs[i]->token==Tokenizer::ELSE){	
					//Optimization (delete void else)
					if(node->childs[i]->Size()){
						//* next label *//
						++labelcountif;
						//* last if/else if, go to there *//
						intCode[tmp_code].name=".startif$"+String::ToString(labelcountif);
						//* go to end if before if/elseif crack true*//
						tmp.opcode=LB_GOTO;
						tmp.name=".endif$"+String::ToString(tmp_line);
						tmp.isconst=true;
						tmp.label=false;
						intCode.push_back(tmp);						
						/* push label */
						tmp.opcode=-1;
						tmp.name=".startif$"+String::ToString(labelcountif);
						tmp.isconst=true;
						tmp.label=true;
						intCode.push_back(tmp);
						//statements
						for(int ei=0;ei<node->childs[i]->Size();++ei)
							GenInitBitecode(intCode,node->childs[i]->childs[ei],labelcount,labelcountif);
					}	
				}				
				else{
					GenInitBitecode(intCode,node->childs[i],labelcount,labelcountif);				
				}
			}			
			/* push end label */
			label_code=".endif$"+String::ToString(tmp_line);
			tmp.label=true;
			tmp.opcode=-1;
			tmp.name=label_code;
			tmp.isconst=true;
			intCode.push_back(tmp);	
			
			break;

		case Tokenizer::BREAK:
			/* goto to end loop */
			tmp.opcode=LB_GOTO;
			tmp.name=".end$"+String::ToString(labelcount);
			tmp.isconst=true;
			tmp.label=false;
			intCode.push_back(tmp);
			break;
		case Tokenizer::CONTINUE:			
			/* goto to <exp> loop */
			tmp.opcode=LB_GOTO;
			tmp.name=".start$"+String::ToString(labelcount);
			tmp.isconst=true;
			tmp.label=false;
			intCode.push_back(tmp);
			break;


		/* errors */
		case Tokenizer::ELSE:
		case Tokenizer::ELIF:
		case Tokenizer::DEF: 
		case Tokenizer::LPR:
		case Tokenizer::RPR:
		case Tokenizer::LS:
		case Tokenizer::RS:
		case Tokenizer::COMMA:
		case Tokenizer::END:
		case Tokenizer::INVALID:
		case Tokenizer::NONE:
			throw ("faild: convertion tree to intermedie code");
		default: break;
		}
	}	
/* Constant Folding + lite math optimizazione */
void ToIntCode::MathOptimizazione(std::vector<ToIntCode::IntCode>& intCode){		
		IntCode tmp;
		for(int i=0;i<intCode.size();++i){
			switch (intCode[i].opcode){
			case LB_ADD:
			case LB_DIV:
			case LB_MIN:
			case LB_MUL:
				if(i>2){
					if(intCode[i-2].opcode==LB_LOADCONST &&
					   intCode[i-1].opcode==LB_LOADCONST){
					   if(intCode[i].opcode==LB_MUL)
						   tmp.name=String::ToString(atof(intCode[i-2].name.c_str())*atof(intCode[i-1].name.c_str()));
					   if(intCode[i].opcode==LB_DIV)
						    tmp.name=String::ToString(atof(intCode[i-2].name.c_str())/atof(intCode[i-1].name.c_str()));
					   if(intCode[i].opcode==LB_ADD)
						   tmp.name=String::ToString(atof(intCode[i-2].name.c_str())+atof(intCode[i-1].name.c_str()));
					   if(intCode[i].opcode==LB_MIN)
						    tmp.name=String::ToString(atof(intCode[i-2].name.c_str())-atof(intCode[i-1].name.c_str()));
					   tmp.token=Tokenizer::NUMBER;
					   tmp.opcode=LB_LOADCONST;
					   tmp.isconst=true;		
					   tmp.line=intCode[i].line;	
					   tmp.valid=true;
					   tmp.label=false;		   
					   intCode.erase(intCode.begin()+i);
					   intCode.erase(intCode.begin()+i-1);
					   //intCode.erase(intCode.begin()+i-2);
					   //intCode.insert(intCode.begin()+i-2,tmp);
					   intCode[i-2]=tmp;
					   i-=2;
					}
					else
					if(( 
					     intCode[i-2].token==Tokenizer::STRING && intCode[i-1].token==Tokenizer::NUMBER 
						 || 
						 intCode[i-2].token==Tokenizer::NUMBER && intCode[i-1].token==Tokenizer::STRING
						 ||						 
					     intCode[i-2].token==Tokenizer::STRING && intCode[i-1].token==Tokenizer::STRING 
					   )&&
					   intCode[i].opcode==LB_ADD){

					   tmp.token=Tokenizer::STRING;
					   tmp.opcode=LB_LOAD;
					   tmp.name=intCode[i-2].name+intCode[i-1].name;
					   tmp.isconst=true;		
					   tmp.line=intCode[i].line;	
					   tmp.valid=true;
					   tmp.label=false;	
					   intCode.erase(intCode.begin()+i);
					   intCode.erase(intCode.begin()+i-1);
					   //intCode.erase(intCode.begin()+i-2);
					   //intCode.insert(intCode.begin()+i-2,tmp);
					   intCode[i-2]=tmp;
					   i-=2;
					}
				}
				break;				
			case LB_CMD:
				if(i>0){
					if(intCode[i-1].opcode==LB_LOADCONST){
					   tmp.token=Tokenizer::NUMBER;
					   tmp.opcode=LB_LOADCONST;
					   tmp.isconst=true;		
					   tmp.line=intCode[i].line;
					   tmp.name="-"+intCode[i-1].name;
					   tmp.valid=true;
					   tmp.label=false;		
					   intCode.erase(intCode.begin()+i);
					   intCode[i-1]=tmp;
					   i-=1;
					}
				}
				break;
			default:
				break;
			}
		}
	}
/*                                            */
std::string ToIntCode::PrintIniCode(ToIntCode::IntCode intcode,int asm_line){
		std::string code=LbCommandsString[30];
		code+="{ "+String::ToString(intcode.line)+" | "+
				   String::ToString(asm_line)+" | "+
				   String::ToString(intcode.token)+" | "+
				   (intcode.opcode>-1 ? LbCommandsString[intcode.opcode] : "label")+" | "+
				   intcode.name+"}\n\n";
		return code;
	}
std::string ToIntCode::PrintIniCodeBasic(ToIntCode::IntCode intcode,int asm_line){
		std::string code;

		if(intcode.opcode>-1){
			std::string codevalue;
			std::string space;
			int lenspace=0;
			//calc space
			lenspace=20-strlen(LbCommandsString[intcode.opcode]);
			space=std::string(lenspace,' ');
			codevalue=codevalue+LbCommandsString[intcode.opcode]+space+" "+intcode.name+"\n";			
			//calc space
			std::string linenuber=String::ToString(intcode.line);
			lenspace=8-linenuber.size();
			space=std::string(lenspace,' ');
			//print
			code+=linenuber+space+" "+codevalue;
		}
		else 
			code="\n"+intcode.name+"\n";
		//return
		return code;
	}
