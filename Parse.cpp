#ifndef PARSE_H
#define PARSE_H
#include <sstream>
#include <string>
#include <vector>

/**

    @autor Gabriele Di Bari
	@date  ( yyyy-mm-dd, hh:mm ) 2012-12-22 00:24
	@date  ( yyyy-mm-dd, hh:mm ) 2012-12-22 21:48
	@version 0.2

	//recursive descent parser (exp) (WORK!!!!)

	*  <statement> := <statement_if> | <statement_while> | <statement_assignament>
	*  <statement_if> := if  <exp> '{' <statement>  '}'
	*  <statement_while> := while  <exp> '{' <statement>  '}'
	*  <statement_assignament> := <VARIABLE> '=' <exp>
	*
	************************************************
	*** EXPRESCION PARSE ***************************
	************************************************
	*   exp :=  <compare> { <op_logic> <compare> }	
	*	op_logic: '&&' | '||'

	*   compare := <base> { <op_compare> <base> }
	*   op_compare := '=='| '<' | '>' | '<=' | '>='

	*   base  := <term> { <op_livel1> <term> }
	*	op_livel1 : '+' | '-'

	*   term := <factor> { <op_livel2> <factor> }
	*	op_livel2 : '*' | '/'

	*   factor := [<unary_op>] <NUMBER> | [<unary_op>] <VARIABLE> | <STRING> | '(' <exp> ')'
	*   unary_op := '-' | '!'

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
	    STRING,
	    VARIABLE,
		NUMBER, // 12898303...
	    ADD,    // +
		MIN,    // -
		MUL,    // *
		DIV,    // /
		EQ,     // ==
		GT,     // >
		LT,     // <
		GTE,    // >=
		LTE,    // <=
		NOT,    // !
		OR,     // ||
		AND,    // &&
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

		std::string tmp;

		switch (type){
		case Tokenizer::VARIABLE:		
			/* parse variable */
			return GetVariable();
			break;
		case Tokenizer::NUMBER:		
			/* parse number */
			return GetNumber();
			break;
		case Tokenizer::STRING:		
			/* parse string */
			GetCString(tmp);
			return tmp;
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
		case Tokenizer::EQ:
			return "==";
			break;		
		case Tokenizer::GT:
			return ">";
			break;		
		case Tokenizer::LT:
			return "<";
			break;		
		case Tokenizer::GTE:
			return ">=";
			break;		
		case Tokenizer::LTE:
			return "<=";
			break;
		case Tokenizer::NOT:
			return "!";
			break;		
		case Tokenizer::AND:
			return "&&";
			break;		
		case Tokenizer::OR:
			return "||";
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

	bool IsNumber(){
		bool isnumber=(*pointer>47)&&(*pointer<58);// 0-9
		bool isvalidpoint=(*pointer==46) && 
						  (*(pointer+1)>47) &&
						  (*(pointer+1)<58); // .0-9
		return isnumber||isvalidpoint;
	}
	std::string GetNumber(){
			const char* tmp=pointer;
			while(IsNumber()) ++pointer;
			std::string tokenvalue(tmp,pointer-tmp);				
			pointer=tmp;
			return tokenvalue;
	}
	void SkipNumber(){		
		while(IsNumber()) ++pointer;
	}

	bool IsVariable(){
		return ((*pointer>='A')&&(*pointer<='Z')) ||
			   ((*pointer>='a')&&(*pointer<='z')) ||
			   ((*pointer)=='_');
	}	
	bool IsVariableAfter(){
		return ((*pointer>='A')&&(*pointer<='Z')) ||
			   ((*pointer>='a')&&(*pointer<='z'))||
			   ((*pointer>='0')&&(*pointer<='9'))||
			   ((*pointer)=='_');
	}
	std::string GetVariable(){
			const char* tmp=pointer;		
			if(IsVariable()) ++pointer;
			while(IsVariableAfter()) ++pointer;
			std::string tokenvalue(tmp,pointer-tmp);				
			pointer=tmp;
			return tokenvalue;
	}
	void SkipVariable(){		
		if(IsVariable()) ++pointer;
		while(IsVariableAfter()) ++pointer;
	}

	bool IsString(){
		const char *tmp=pointer;
		if(SkipString()){
			pointer=tmp;
			return true;
		}
		return false;
	}		
	bool GetCString(std::string& out){
			const char *tmp=pointer;
			out="";
			if((*tmp)=='\"'){ //["...."]
				++tmp;  //[...."]
				while((*tmp)!='\"'&&(*tmp)!='\n'){
					if((*tmp)=='\\'){//[\.]
						++tmp;  //[.]
						switch(*tmp){
							case 'n': out+='\n'; break;
							case 't': out+='\t'; break;
							case 'b': out+='\b'; break;
							case 'r': out+='\r'; break;
							case 'f': out+='\f'; break;
							case 'a': out+='\a'; break;
							case '\\': out+='\\'; break;
							case '?':  out+='\?'; break;
							case '\'': out+='\''; break;
							case '\"': out+='\"'; break;
							case '\n': /* jump unix */ break;
							case '\r': /* jump mac */
								if((*(tmp+1))=='\n') ++tmp; /* jump window (\r\n)*/
							break;
								default: return false; break;
							}
						}
						else{
							if((*tmp)!='\0') out+=(*tmp);
							else return false;
						}
						++tmp;//next char
					}
					if((*tmp)=='\n') return false;
					return true;
				}
			return false;
	}
	bool SkipString(){
			if((*pointer)=='\"'){ //["...."]
				++pointer;  //[...."]
				while((*pointer)!='\"'&&(*pointer)!='\n'){
					if((*pointer)=='\\'){//[\.]
						++pointer;  //[.]
						switch(*pointer){
							case 'n':
							case 't':
							case 'b':
							case 'r': 
							case 'f':
							case 'a': 
							case '\\':
							case '?':  
							case '\'': 
							case '\"': break;
							case '\n': /* jump unix */ 
								++this->countline;
							break;
							case '\r': /* jump mac OS */
								if((*(pointer+1))=='\n') ++pointer; /* jump window (\r\n)*/
								++this->countline; 
							break;
								default: return false; break;
							}
						}
						else{
							if((*pointer)!='\0');
							else return false;
						}
						++pointer;//next char
					}
					if((*pointer)=='\n') return false;
					//SKEEP! \"
					++pointer;
					//
					return true;
				}
			return false;
	}
	
	bool IsLogicAnd(){
		return (*pointer=='&') && (*(pointer+1)=='&');
	}
	bool IsLogicOr(){
		return (*pointer=='|') && (*(pointer+1)=='|');
	}
	void SkipLogicAO(){
		pointer+=2;
	}
		
	bool IsEQ(){
		return (*pointer=='<') && (*(pointer+1)=='=');
	}
	bool IsGTE(){
		return (*pointer=='>') && (*(pointer+1)=='=');
	}	
	bool IsLTE(){
		return (*pointer=='<') && (*(pointer+1)=='=');
	}
	void SkipEGL(){		
		pointer+=2;
	}

	void SkipWhiteSpace(){
		while((*pointer)==' ' || (*pointer)=='\t'||
			  (*pointer)=='\r'|| (*pointer)=='\n'){

			  if((*pointer)=='\r')
			  { ++countline; pointer+=(*(pointer+1)=='\n'); }
			  else 
			  countline+=((*pointer)=='\n');

			  ++pointer;
		}
	}	
	
	// -->[112]-->[+]--> 
	void DeterminateToken(){
		//
		SkipWhiteSpace();
		//is a number
		if(IsVariable())  type=VARIABLE; else 
		if(IsNumber())  type=NUMBER; else 
		if(IsString())  type=STRING; else 
		if(*pointer == '+') type=ADD; else
		if(*pointer == '-') type=MIN; else
		if(*pointer == '*') type=MUL; else
		if(*pointer == '/') type=DIV; else
		if(*pointer == '!') type=NOT; else
		if(*pointer == '>') type=GT; else
		if(*pointer == '<') type=LT; else
		if(IsEQ())			type=EQ; else
		if(IsGTE())			type=GTE; else
		if(IsLTE())			type=LTE; else
		if(IsLogicAnd())    type=AND; else
		if(IsLogicOr())     type=OR; else
		if(*pointer == '(') type=LPR; else
		if(*pointer == ')') type=RPR; else
		if(*pointer == '\0') type=END; else
		type=INVALID;
	}
	void SkipToken(){		
		SkipWhiteSpace();
		
		if(type==VARIABLE) SkipVariable(); else
		if(type==NUMBER) SkipNumber(); else
		if(type==STRING) SkipString(); else
		if(type==ADD) ++pointer; else
		if(type==MIN) ++pointer; else
		if(type==MUL) ++pointer; else
		if(type==DIV) ++pointer; else
		if(type==NOT) ++pointer; else
		if(type==GT)  ++pointer; else
		if(type==LT)  ++pointer; else
		if(type==EQ)  SkipEGL(); else
		if(type==GTE) SkipEGL(); else
		if(type==LTE) SkipEGL(); else
		if(type==AND) SkipLogicAO(); else
		if(type==OR)  SkipLogicAO(); else
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
		/* unary op, '-' | '!' */
		TreeNode *newnode=NULL;
		if(tkn.GetToken()==Tokenizer::MIN ||  tkn.GetToken()==Tokenizer::NOT){
			newnode=new TreeNode(tkn.GetLine(),
							     tkn.GetToken(),
							     tkn.TokenValue());	
			tkn.NextToken();	
		}

		if(tkn.GetToken()==Tokenizer::NUMBER ||
		  (tkn.GetToken()==Tokenizer::STRING && NOT(newnode)) ||
		   tkn.GetToken()==Tokenizer::VARIABLE ){
			TreeNode *node=new TreeNode(tkn.GetLine(),
										tkn.GetToken(),
										tkn.TokenValue());			
			tkn.NextToken();
			/* after 
			   number/variable/string 
			   can't following another  
			   number/variable/string */
			if(	tkn.GetToken()==Tokenizer::NUMBER ||
				tkn.GetToken()==Tokenizer::STRING ||
				tkn.GetToken()==Tokenizer::VARIABLE ){
				delete newnode;
				delete node;
				return NULL;
			}
			/* if not unary op */
			if(NOT(newnode)) 
				return node;
			/* else after a unary op */
			newnode->PushChild(node);
			return newnode;
		}

		if(NOT(newnode) && tkn.GetToken()==Tokenizer::LPR){ // '('
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
	TreeNode* ParseCompare(){	
		/* nodes */
		TreeNode *left=NULL;
		TreeNode *tree=NULL;
		TreeNode *right=NULL;
		/* nodes */	
		if(NOT(left=ParseBase())) return NULL; //<base>
		while(tkn.GetToken()==Tokenizer::EQ ||
			  tkn.GetToken()==Tokenizer::GT ||
			  tkn.GetToken()==Tokenizer::LT ||
			  tkn.GetToken()==Tokenizer::GTE||
			  tkn.GetToken()==Tokenizer::LTE){ //'==' | '>' | '<' | '>=' | '<=' 
				//before tree				  
				if(tree) left=tree;
				/* make tree */
				tree= new TreeNode(tkn.GetLine(),
								   tkn.GetToken(),
								    tkn.TokenValue());

				tkn.NextToken();
				//<base>
				if(NOT(right=ParseBase())){ 				
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
		TreeNode *left=NULL;
		TreeNode *tree=NULL;
		TreeNode *right=NULL;
		/* nodes */	
		if(NOT(left=ParseCompare())) return NULL; //<compare>
		while(tkn.GetToken()==Tokenizer::AND||
			  tkn.GetToken()==Tokenizer::OR){ //'&&' | '||' 
				//before tree				  
				if(tree) left=tree;
				/* make tree */
				tree= new TreeNode(tkn.GetLine(),
								   tkn.GetToken(),
								   tkn.TokenValue());

				tkn.NextToken();
				//<compare>
				if(NOT(right=ParseCompare())){ 				
					delete left;
					delete tree;
					return NULL;
				} 
			  tree->PushChild(left);
			  tree->PushChild(right);
		}
		return tree ? tree : left;
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
	case Tokenizer::VARIABLE:
		return "VARIABLE";
		break;
	case Tokenizer::NUMBER:
		return "NUMBER";
		break;
	case Tokenizer::STRING:
		return "STRING";
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
	case Tokenizer::EQ:
		return "EQ ==";
		break;
	case Tokenizer::GT:
		return "GT >";
		break;
	case Tokenizer::LT:
		return "LT <";
		break;
	case Tokenizer::GTE:
		return "GTE >=";
		break;
	case Tokenizer::LTE:
		return "LTE <=";
		break;
	case Tokenizer::LPR:
		return "LPR (";
		break;
	case Tokenizer::RPR:
		return "RPR )";
		break;
	case Tokenizer::NOT:
		return "NOT !";
		break;
	case Tokenizer::AND:
		return "AND &&";
		break;
	case Tokenizer::OR:
		return "OR ||";
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

	std::string scriptexp("-2*-A > 5+1");
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