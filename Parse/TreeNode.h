#ifndef TREE_NODE_H
#define TREE_NODE_H
#include "Tokenizer.h"
#include <vector>

struct TreeNode{

		enum AdditionalInfo{
			IS_CALL,
			IS_LESSNAME_CALL,
			IS_HEADER,
			IS_LESSNAME_HEADER,
			IS_FOR_HEADER,
			IS_TOKEN,
		};
		/***************************/
		int line;
		int column;
		Tokenizer::Token token;
		std::string name;
		AdditionalInfo info;
		/*************************/
		TreeNode *parent;
		std::vector<TreeNode*> childs;
		
		TreeNode();
		TreeNode(int line,int column,Tokenizer::Token token,const std::string &name,AdditionalInfo info=IS_TOKEN);
		~TreeNode();

		TreeNode* PushChild(TreeNode* in);
		void RemoveChild(TreeNode* in);
		int IndexChild(TreeNode* in);
		void RemoveChild(int i);
		void Change(int line,int column,Tokenizer::Token token,const std::string &name);
		int Size() const;
		TreeNode*& operator[](int i);
		TreeNode*  operator[](int i) const;
		std::string ToString(int n=1,int leng=0);

};


#endif