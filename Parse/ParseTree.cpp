#include "ParseTree.h"

TreeNode* ParseTree::StartParse(std::string &script){
		TreeNode* tree=lexer.StartParse(script,&errors);
		if(tree){
			tree=SemanticsChecking::Controll(tree,errors);
		}
		return tree;
	}
std::string ParseTree::ErrorsToString(){
		return errors.ToString();
	}	
bool ParseTree::FindErrors(){
		return errors.errors.size();
	}
ErrorParse& ParseTree::GetErrors(){
		return errors;
	}