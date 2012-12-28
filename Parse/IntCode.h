#ifndef INTCODE_H
#define INTCODE_H

#include <vector>
#include "UtilityString.h"
#include "Table.h"
#include "SyntaxTree.h"
#include "ParseTree.h"
#include "../VMliteB.h"


struct ToIntCode{


	struct IntCode{
		bool label;
		bool isconst;
		bool valid;
		short opcode;
		int line;
		int column;
		int root_child_id;
		Tokenizer::Token token;
		TreeNode::AdditionalInfo info;
		std::string name;
	};
	struct Function{
		std::string name;
		TreeNode *root;
		std::vector<IntCode> intCode;
	};
	
	TreeNode *root;
	std::vector<IntCode> intCode;
	std::vector<Function> functions;
	public:
		

	ToIntCode();
	std::string ToString();	
	std::string ToStringBasic();
	void ParseTree(TreeNode* node);

protected:

	void GetFunctionAndReplaceWithVariabe(TreeNode* node);
	void GenInitBitecode(std::vector<IntCode>& intCode,TreeNode* node,
						 int& labelcount,
						 int& labelcountif);	
	/* Constant Folding + lite math optimizazione */
	void MathOptimizazione(std::vector<IntCode>& intCode);
	/*                                            */
	std::string PrintIniCode(IntCode intcode,int asm_line);
	std::string PrintIniCodeBasic(IntCode intcode,int asm_line);

};


#endif