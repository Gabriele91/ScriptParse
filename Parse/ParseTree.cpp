#include "ParseTree.h"

TreeNode* ParseTree::StartParse(std::string &script){
		TreeNode* tree=lexer.StartParse(script,&errors);
		if(tree){
			tree=SemanticsChecking::Controll(tree,errors);
		}
		return tree;
	}
std::string ParseTree::ErrorsToString() const{
		return errors.ToString();
	}	
bool ParseTree::FindErrors() const{
		return errors.errors.size();
	}
ErrorParse& ParseTree::GetErrors(){
		return errors;
	}