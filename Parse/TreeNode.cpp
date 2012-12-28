#include "TreeNode.h"


TreeNode::TreeNode()
			:line(0)
			,token(Tokenizer::Token::NONE)
			,name(name)
			,parent(0){}
TreeNode::TreeNode(int line,int column,Tokenizer::Token token,const std::string &name,AdditionalInfo info)
			:line(line)
			,column(column)
			,token(token)
			,name(name)
			,info(info)
			,parent(0){
		}
TreeNode::~TreeNode(){
		for(int i=0;i<Size();++i) 
			 delete childs[i];
}

TreeNode* TreeNode::PushChild(TreeNode* in){
			in->parent=this;
			childs.push_back(in);
			return in;
		}
void TreeNode::RemoveChild(TreeNode* in){
			for(int i=0;i<Size();++i) {
				if(childs[i]==in){
					childs.erase(childs.begin()+i);
					break;
				}
			}
		}
int TreeNode::IndexChild(TreeNode* in){
		for(int i=0;i<Size();++i) if(childs[i]==in) return i;
		return -1;		
}
void TreeNode::RemoveChild(int i){
			if(i<Size())
				childs.erase(childs.begin()+i);
		}
void TreeNode::Change(int line,int column,Tokenizer::Token token,const std::string &name){
			this->name=name; 
			this->line=line;
			this->column=column;
			this->token=token;
		}
int TreeNode::Size() const{
			return childs.size();
		}
TreeNode*& TreeNode::operator[](int i){
			return childs[i];
		}		
TreeNode*  TreeNode::operator[](int i) const{
		return childs[i];
}
std::string TreeNode::ToString(int n,int leng){
		std::string out="|"+std::string(8*(n>1),'-')+
						">( "+name+","+String::ToString(line)+" ) ";
			for(int i=0;i<Size();++i){
				std::string space;
				space+=std::string(9*(n),' ');
				out+="\n"+space+childs[i]->ToString(n+1);
			}
			return out;
	}