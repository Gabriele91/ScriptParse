#include "SemanticsChecking.h"

/* STRING MUST TO BE OPERATOR: + || && == */
TreeNode* SemanticsChecking::ValidateExpString(TreeNode* node){
		switch (node->token)
		{
			//if + go up
			case Tokenizer::ADD:				
				return ValidateExpString(node->parent);
			break;
			//if -*/ < > <= >= < > ! error
			case Tokenizer::MIN:
			case Tokenizer::MUL:
			case Tokenizer::DIV:
			case Tokenizer::GT:
			case Tokenizer::LT:
			case Tokenizer::GTE:
			case Tokenizer::LTE:
			case Tokenizer::NOT:
				return node;
			break;
			//if == != || && return true
			case Tokenizer::OR:
			case Tokenizer::AND:
			case Tokenizer::EQ: 
			case Tokenizer::NOTEQ: 
				return NULL;
			break;
			//else return true
			default: return NULL; break;
		}
	}
TreeNode* SemanticsChecking::ValidExpString(TreeNode* node){
		
		//if is leafs and is a string
		if(node->Size()==0 && 
		   node->token==Tokenizer::STRING){
		   return ValidateExpString(node->parent);
		}
		//else go to leafs
		TreeNode* nodenotvalid=NULL;
		for(int i=0;i<node->Size() && nodenotvalid==NULL;++i){
			nodenotvalid=ValidExpString((*node)[i]);
		}
		return nodenotvalid;
	}	
/* RETURN MUST TO BE INTO FUNCTION */ 
TreeNode* SemanticsChecking::ValidReturn(TreeNode* node){		
		if(node->token==Tokenizer::RETURN){
			TreeNode* p=NULL;
			for(p=node->parent;p;p=p->parent){
				if(p->token==Tokenizer::DEF) break;
			}
			//errors
			if(p==NULL) return node;
			//
		}
		//else go to leafs
		TreeNode* nodenotvalid=NULL;
		for(int i=0;i<node->Size() && nodenotvalid==NULL;++i){
			nodenotvalid=ValidReturn((*node)[i]);
		}
		return nodenotvalid;
	}
/* A LESS NAME FUNCTION MUST TO BE ASSIGNAMENT OR CALLED */
TreeNode* SemanticsChecking::ValidLessNameFunction(TreeNode* node){
		//if is a function less name function
		if(node->token==Tokenizer::DEF &&
		   node->childs[0]->info==TreeNode::IS_LESSNAME_HEADER ){ 
			//errors
			if(!node->parent) return node;
			if(node->parent->info!=TreeNode::IS_LESSNAME_CALL &&
			   node->parent->token!=Tokenizer::ASSIGNAMENT) return node;
			//
		}
		//else go to leafs
		TreeNode* nodenotvalid=NULL;
		for(int i=0;i<node->Size() && nodenotvalid==NULL;++i){
			nodenotvalid=ValidLessNameFunction((*node)[i]);
		}
		return nodenotvalid;
	}
/* BREAK AND CONTINUE MUST TO BE INTO do-while, while or for block  */
TreeNode* SemanticsChecking::ValidBreakContinue(TreeNode* node){
		if(node->token==Tokenizer::BREAK||
		   node->token==Tokenizer::CONTINUE){
			TreeNode* p=NULL;
			for(p=node->parent;p;p=p->parent){
				if(p->token==Tokenizer::DO) break;
				if(p->token==Tokenizer::WHILE) break;
				if(p->token==Tokenizer::FOR) break;
				if(p->token==Tokenizer::DEF){ p=NULL; break; }
			}
			//errors
			if(p==NULL) return node;
			//
		}
		//else go to leafs
		TreeNode* nodenotvalid=NULL;
		for(int i=0;i<node->Size() && nodenotvalid==NULL;++i){
			nodenotvalid=ValidBreakContinue((*node)[i]);
		}
		return nodenotvalid;
	}
/* GLOBAL AND LOCAL MUST TO BE INTO def block  */
TreeNode* SemanticsChecking::ValidGlobalLocal(TreeNode* node){
		if((node->token==Tokenizer::LOCAL||
		    node->token==Tokenizer::GLOBAL) &&
			(!node->parent || 
			 node->parent->token!=Tokenizer::DEF)){
			return node;
		}
		//else go to leafs
		TreeNode* nodenotvalid=NULL;
		for(int i=0;i<node->Size() && nodenotvalid==NULL;++i){
			nodenotvalid=ValidGlobalLocal((*node)[i]);
		}
		return nodenotvalid;
	}
/************/
TreeNode* SemanticsChecking::Controll(TreeNode* tree,ErrorParse &errors){
		TreeNode* errornode=NULL;
		//find string exp error
		if((errornode=ValidExpString(tree))){
			errors.PushError(errornode->line,
							 errornode->column,
							 ErrorParse::SYNTAX,
						     "invalid exp string operator: "+errornode->name );
				delete tree;
				tree=NULL;
		}else 
		if((errornode=ValidReturn(tree))){
			errors.PushError(errornode->line,
							 errornode->column,
							 ErrorParse::SYNTAX,
							 "return must to be into a function");
				delete tree;
				tree=NULL;
		}else 
		if((errornode=ValidBreakContinue(tree))){
			errors.PushError(errornode->line,
							 errornode->column,
							 ErrorParse::SYNTAX,
							 "break/continue must to be into while/do-while/for statement");
			delete tree;
			tree=NULL;
		}else 
		if((errornode=ValidGlobalLocal(tree))){
			errors.PushError(errornode->line,
							 errornode->column,
							 ErrorParse::SYNTAX,
							 "global/local must to be into def statement");
			delete tree;
			tree=NULL;
		}else 
		if((errornode=ValidLessNameFunction(tree))){
			errors.PushError(errornode->line,
							 errornode->column,
							 ErrorParse::SYNTAX,
							 "invalid definition less name function");
			delete tree;
			tree=NULL;
		}
		//
		return tree;
	}
