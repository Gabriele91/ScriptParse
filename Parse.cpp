#ifndef PARSE_H
#define PARSE_H
#include <sstream>
#include <string>
#include <vector>

/**

    @autor Gabriele Di Bari
	@date  ( yyyy-mm-dd, hh:mm ) 2012-12-22 00:24
	@version 0.1

	//recursive descent parser (exp) (WORK!!!!)

	*   exp := [ <base> | <op_mono><base> ]	
	*	op_mono: '-'

	*   base  := <term> { <op_livel1> <term> }
	*	op_livel1 : '+' | '-'

	*   term := <factor> { <op_livel2> <factor> }
	*	op_livel2 : '*' | '/'

	*   factor := <NUMBER> | <VARIABLE> | '(' <exp> ')'

	struct REParse{
	    [...]
		Tokenizer tkobj;
	    [...]	
		bool ParserFactor();
		bool ParseTerm()
		bool ParseBase()
		bool ParseExp()

		bool StartParse(const std::string & script)
		
	    [...]
	}
	*** EXAPLE *********************************************************************************
	REParse testExp;
	std::cout <<( rep.StartParse(" (-9)+2*((2*3-5)*5)/10 ") ? "valid" : "invalid" )<< std::endl;
	********************************************************************************************
	
	//MAKE TREE 
	// exemple:
	//
	//   (1+2)*5
	//
	//       *
	//      / \
	//     +   5
	//    / \
	//   1   2
	//
	//   1+2*5
	//
	//     +
	//    / \
	//   1   *
	//      / \
	//     2   5
	//

	//into exp setp1: exp--->
	//                       |
	//				         v
	//    into base, setp1 : base->term->factor      '1' !number! Push Tree leaf  
	//    into base, step2 : base<-term<-factor      '+' !op!     Push Tree node 
	//    into base, step3 : base---------->
	//     |                               |
	//     |                               |
	//     |                               v
	//     v		  into term, step1 : term->factor            '2' !number! Push Tree leaf
	//     child--->  into term, step2 : term<-factor            '*' !op!     Push Tree node 
	//                into term, step3 : term->factor            '5' !number! Push Tree leaf

*/
namespace String{
	template<class T> std::string ToString(T dato){
		std::stringstream br;
		br << dato;
		return br.str();
	}
}
struct Tokenizer{

	enum Token{
	  //STRING,
	  //VARIABLE,
		NUMBER, // 12898303...
	    ADD,    // +
		MIN,    // -
		MUL,    // *
		DIV,    // /
		LPR,    // (
		RPR,    // )
		END,    // EOF
		INVALID // ???
	};

	/**
	* @param const char* start string
	*/
	void SetScript(const char *pointer){
		this->pointer=pointer;
		countline=0;
	}
	void Start(){
		countline=1;
		DeterminateToken();
	}
	Token GetToken(){
		return type;
	}
	int GetLine(){
		return countline;
	}
	void NextToken(){	
		SkipToken();
		DeterminateToken();
	}
	std::string TokenValue(){
		switch (type){
		case Tokenizer::NUMBER:		
			/* parse number */
			return GetNumber();
			break;
		case Tokenizer::ADD:
			return "+";
			break;
		case Tokenizer::MIN:
			return "-";
			break;
		case Tokenizer::MUL:
			return "*";
			break;
		case Tokenizer::DIV:
			return "/";
			break;
		case Tokenizer::LPR:
			return "(";
			break;
		case Tokenizer::RPR:
			return ")";
			break;
		case Tokenizer::END:
			return "\0";
			break;
		case Tokenizer::INVALID:
			return "";
			break;
		default:		
			return "";
			break;
		}

	}

	private:

	const char *pointer;
	Token type;
	int countline;

	bool IsCharNumber(){
		return (*pointer>47)&&(*pointer<58);
	}
	std::string GetNumber(){
			const char* tmp=pointer;
			while(IsCharNumber()) ++pointer;
			std::string tokenvalue(tmp,pointer-tmp);				
			pointer=tmp;
			return tokenvalue;
	}
	void SkipWhiteSpace(){
		while((*pointer)==' ' || (*pointer)=='\t'||
			  (*pointer)=='\r'|| (*pointer)=='\n'){
			  countline+=((*pointer)=='\n');
			  ++pointer;
		}
	}	
	
	// -->[112]-->[+]--> 
	void DeterminateToken(){
		//
		SkipWhiteSpace();
		//is a number
		if(IsCharNumber()) type=NUMBER; else 
		if(*pointer == '+') type=ADD; else
		if(*pointer == '-') type=MIN; else
		if(*pointer == '*') type=MUL; else
		if(*pointer == '/') type=DIV; else
		if(*pointer == '(') type=LPR; else
		if(*pointer == ')') type=RPR; else
		if(*pointer == '\0') type=END; else
		type=INVALID;
	}
	void SkipToken(){		
		SkipWhiteSpace();

		if(type==NUMBER) 
			while(IsCharNumber()) ++pointer;
		else
		if(type==ADD) ++pointer; else
		if(type==MIN) ++pointer; else
		if(type==MUL) ++pointer; else
		if(type==DIV) ++pointer; else
		if(type==LPR) ++pointer; else
		if(type==RPR) ++pointer;

	}

	
};


/**
*  Tree struct
*/

struct TreeNode{

		int line;
		Tokenizer::Token token;
		std::string name;
		std::vector<TreeNode*> childs;
		
		TreeNode()
			:line(0)
			,token(Tokenizer::Token::INVALID)
			,name(name){}
		TreeNode(int line,Tokenizer::Token token,const std::string &name)
			:line(line)
			,token(token)
			,name(name){
		}
		~TreeNode(){
			for(int i=0;i<Size();++i) 
					 delete childs[i];
		}

		TreeNode* PushChild(TreeNode* in){
			childs.push_back(in);
			return in;
		}
		void RemoveChild(TreeNode* in){
			for(int i=0;i<Size();++i) {
				if(childs[i]==in){
					childs.erase(childs.begin()+i);
					break;
				}
			}
		}
		void RemoveChild(int i){
			if(i<Size())
				childs.erase(childs.begin()+i);
		}
		void Change(int line,Tokenizer::Token token,const std::string &name){
			this->name=name; 
			this->line=line;
			this->token=token;
		}
		int Size() const{
			return childs.size();
		}
		TreeNode*& operator[](int i){
			return childs[i];
		}		
		TreeNode*  operator[](int i) const{
			return childs[i];
		}

		std::string ToString(int n=1,int leng=0){
			std::string out="|"+std::string(8*(n>1),'-')+
							">( "+name+","+String::ToString(line)+" ) ";
			for(int i=0;i<Size();++i){
				std::string space;
				space+=std::string(9*(n),' ');
				out+="\n"+space+childs[i]->ToString(n+1);
			}
			return out;
		}

};

//recursive descent parser (exp)
/* (First imp)
*   exp := <term> { <op_livel1> <term> }
*	op_livel1 : '+' | '-'
*   term := <factor> { <op_livel2> <factor> }
*	op_livel2 : '*' | '/'
*   factor := <NUMBER> | <VARIABLE> | '(' <exp> ')'
*
*/

struct REParse{

	
	#define NOT(X) !(X)
	std::string script;
	Tokenizer tkn;
	
	TreeNode* ParserFactor(){
		
		if(tkn.GetToken()==Tokenizer::NUMBER /* 1920.. or <VARIABLE> */ ){
			TreeNode *node=new TreeNode(tkn.GetLine(),
										tkn.GetToken(),
										 tkn.TokenValue());	
			tkn.NextToken();
			return node;
		}
		else
		if(tkn.GetToken()==Tokenizer::LPR){ // '('
			tkn.NextToken();
			TreeNode *node=ParseExp();
			if(tkn.GetToken()==Tokenizer::RPR){// ')'
				tkn.NextToken();
				return node; 
			}
			delete node;
			return NULL;
		}		 
		return NULL;
	}
	TreeNode* ParseTerm(){
		/* nodes */
		TreeNode *left=NULL;
		TreeNode *tree=NULL;
		TreeNode *right=NULL;
		/* nodes */
		if(NOT(left=ParserFactor())) return NULL; //<Factor>
		while(tkn.GetToken()==Tokenizer::MUL||
			  tkn.GetToken()==Tokenizer::DIV){ //'*' | '/'
			  //before tree				  
			  if(tree) left=tree;  
			  /* set tree */
			  tree= new TreeNode(tkn.GetLine(),
								 tkn.GetToken(),
								 tkn.TokenValue());

			  tkn.NextToken();
			  //<Factor>
			  if(NOT(right=ParserFactor())){ 				
					delete left;
					delete tree;
					return NULL;
			  } 
			  tree->PushChild(left);
			  tree->PushChild(right);
		}
		return tree ? tree : left;
	}
	TreeNode* ParseBase(){	
		/* nodes */
		TreeNode *left=NULL;
		TreeNode *tree=NULL;
		TreeNode *right=NULL;
		/* nodes */	
		if(NOT(left=ParseTerm())) return NULL; //<term>
		while(tkn.GetToken()==Tokenizer::ADD||
			  tkn.GetToken()==Tokenizer::MIN){ //'+' | '-' 
				//before tree				  
				if(tree) left=tree;
				/* make tree */
				tree= new TreeNode(tkn.GetLine(),
								   tkn.GetToken(),
								    tkn.TokenValue());

				tkn.NextToken();
				//<term>
				if(NOT(right=ParseTerm())){ 				
					delete left;
					delete tree;
					return NULL;
				} 
			  tree->PushChild(left);
			  tree->PushChild(right);
		}
		return tree ? tree : left;
	}
	TreeNode* ParseExp(){
		/* nodes */
		TreeNode *root=NULL;
		TreeNode *leaf=NULL;
		/* nodes */
		if(NOT(root=ParseBase())){ // <base>
			if(tkn.GetToken()==Tokenizer::MIN){//'-' <base>			  
			  TreeNode *left=new TreeNode(tkn.GetLine(),
								          tkn.GetToken(),
								          tkn.TokenValue());
			  tkn.NextToken();
			  if(NOT(leaf=ParseBase())){
				  delete root;
				  return NULL;
			  }
			  root->PushChild(leaf);
			}else
			   return NULL;
		}
		return root;
	}

	TreeNode* StartParse(const std::string & script){
		this->script=script;
		tkn.SetScript(this->script.c_str());
		tkn.Start();
		return ParseExp();
	}
	#undef NOT
};

#include <iostream>
#include <string>

std::string GetStringToken(Tokenizer::Token token){

	switch (token)
	{
	case Tokenizer::NUMBER:
		return "NUMBER";
		break;
	case Tokenizer::ADD:
		return "ADD +";
		break;
	case Tokenizer::MIN:
		return "MIN -";
		break;
	case Tokenizer::MUL:
		return "MUL *";
		break;
	case Tokenizer::DIV:
		return "DIV /";
		break;
	case Tokenizer::LPR:
		return "LPR (";
		break;
	case Tokenizer::RPR:
		return "RPR )";
		break;
	case Tokenizer::END:
		return "END";
		break;
	case Tokenizer::INVALID:
		return "INVALID";
		break;
	default:		
		return "???????";
		break;
	}

}

int main(){

	std::string scriptexp("1+2*5-(4-3)");
	std::cout << scriptexp << "\n\ntest Tokenizer:\n\n";
	/* test Tokenizer */
	Tokenizer tkn;
	tkn.SetScript(scriptexp.c_str());
	tkn.Start();
	while(tkn.GetToken()!=Tokenizer::END && tkn.GetToken()!=Tokenizer::INVALID){
		std::cout << GetStringToken(tkn.GetToken()) << std::endl;
		tkn.NextToken();
	}
	/* test REParse  */

	REParse testExp;
	TreeNode* testTree;
	std::cout <<"\ntest REParse:" <<((testTree=testExp.StartParse(scriptexp)) ? "valid" : "invalid" )<< std::endl;
	if(testTree){
		std::cout << "\nTree Parse:\n\n" <<testTree->ToString()<< std::endl;
	}
	

	return 0;

}

#endif