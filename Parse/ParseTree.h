#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "Table.h"
#include "SyntaxTree.h"
#include "SemanticsChecking.h"

class ParseTree
{

	SyntaxTree lexer;
	ErrorParse errors;

public:

	TreeNode* StartParse(std::string &script);
	std::string ErrorsToString() const;	
	bool FindErrors() const;
	ErrorParse& GetErrors();

	ParseTree(){};
	~ParseTree(){};

};

#endif