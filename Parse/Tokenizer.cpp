#include "Tokenizer.h"

	void Tokenizer::SetScript(const char *pointer){
		this->pointer=pointer;
		countline=0;
	}
	void Tokenizer::Start(){
		countline=1;
		DeterminateToken();
	}
	Tokenizer::Token Tokenizer::GetToken(){
		return type;
	}	
	Tokenizer::Token Tokenizer::GetNextToken(){
		//save current state
		const char *tpointer=pointer;
		Token ttype=type;
		//get next token
		SkipToken();
		DeterminateToken();
		//save token
		Token rtoken=type;
		//resume state		
		pointer=tpointer;
		type=ttype;
		//return next state
		return rtoken;
	}
	int Tokenizer::GetLine(){
		return countline;
	}
	void Tokenizer::NextToken(){	
		SkipToken();
		DeterminateToken();
	}
	std::string Tokenizer::TokenValue(){

		std::string tmp;

		switch (type){
		case Tokenizer::IF:
			return "if";
			break;
		case Tokenizer::ELIF:
			return "elif";
			break;
		case Tokenizer::ELSE:
			return "else";
			break;
		case Tokenizer::DO:
			return "do";
		case Tokenizer::WHILE:
			return "while";
			break;		
		case Tokenizer::DEF:
			return "def";
			break;		
		case Tokenizer::RETURN:
			return "return";
			break;		
		case Tokenizer::BREAK:
			return "break";
			break;		
		case Tokenizer::CONTINUE:
			return "continue";
			break;		
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
		case Tokenizer::ASSIGNAMENT:
			return "=";
			break;
		case Tokenizer::COMMA:
			return ",";
			break;
		case Tokenizer::END:
			return "\0";
			break;
		case Tokenizer::INVALID:			
			return "_INVALID_";
			break;
		case Tokenizer::NONE:
			return "_NONE_";
			break;
		default:		
			return "";
			break;
		}

	}
	
	const char *pointer;
	Tokenizer::Token type;
	int countline;

	/* VARIABLESSS */
	bool Tokenizer::IsNumber(){
		bool isnumber=(*pointer>47)&&(*pointer<58);// 0-9
		bool isvalidpoint=(*pointer==46) && 
						  (*(pointer+1)>47) &&
						  (*(pointer+1)<58); // .0-9
		return isnumber||isvalidpoint;
	}
	std::string Tokenizer::GetNumber(){
			const char* tmp=pointer;
			while(IsNumber()) ++pointer;
			std::string tokenvalue(tmp,pointer-tmp);				
			pointer=tmp;
			return tokenvalue;
	}
	void Tokenizer::SkipNumber(){		
		while(IsNumber()) ++pointer;
	}

	bool Tokenizer::IsVariable(){
		return ((*pointer>='A')&&(*pointer<='Z')) ||
			   ((*pointer>='a')&&(*pointer<='z')) ||
			   ((*pointer)=='_');
	}	
	bool Tokenizer::IsVariableAfter(){
		return ((*pointer>='A')&&(*pointer<='Z')) ||
			   ((*pointer>='a')&&(*pointer<='z'))||
			   ((*pointer>='0')&&(*pointer<='9'))||
			   ((*pointer)=='_');
	}
	std::string Tokenizer::GetVariable(){
			const char* tmp=pointer;		
			if(IsVariable()) ++pointer;
			while(IsVariableAfter()) ++pointer;
			std::string tokenvalue(tmp,pointer-tmp);				
			pointer=tmp;
			return tokenvalue;
	}
	void Tokenizer::SkipVariable(){		
		if(IsVariable()) ++pointer;
		while(IsVariableAfter()) ++pointer;
	}

	bool Tokenizer::IsString(){
		const char *tmp=pointer;
		if(SkipString()){
			pointer=tmp;
			return true;
		}
		return false;
	}		
	bool Tokenizer::GetCString(std::string& out){
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
	bool Tokenizer::SkipString(){
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

	/* EXP */

	bool Tokenizer::IsLogicAnd(){
		return (*pointer=='&') && (*(pointer+1)=='&');
	}
	bool Tokenizer::IsLogicOr(){
		return (*pointer=='|') && (*(pointer+1)=='|');
	}
	void Tokenizer::SkipLogicAO(){
		pointer+=2;
	}
	
	bool Tokenizer::IsEQ(){
		return (*pointer=='=') && (*(pointer+1)=='=');
	}
	bool Tokenizer::IsGTE(){
		return (*pointer=='>') && (*(pointer+1)=='=');
	}	
	bool Tokenizer::IsLTE(){
		return (*pointer=='<') && (*(pointer+1)=='=');
	}
	void Tokenizer::SkipEGL(){		
		pointer+=2;
	}

	/* key words */
	bool Tokenizer::IsIf(){
		return (*pointer=='i') && (*(pointer+1)=='f');		
	}	
	bool Tokenizer::IsElif(){
		return (*pointer=='e') && 
			   (*(pointer+1)=='l') && 
			   (*(pointer+2)=='i') && 
			   (*(pointer+3)=='f');		
	}	
	bool Tokenizer::IsElse(){
		return (*pointer=='e') && 
			   (*(pointer+1)=='l') && 
			   (*(pointer+2)=='s') && 
			   (*(pointer+3)=='e');		
	}
	bool Tokenizer::IsDo(){
		return (*pointer=='d') && (*(pointer+1)=='o');		
	}
	bool Tokenizer::IsWhile(){
		return (*pointer=='w') && 
			   (*(pointer+1)=='h') && 
			   (*(pointer+2)=='i') && 
			   (*(pointer+3)=='l') && 
			   (*(pointer+4)=='e');		
	}
	bool Tokenizer::IsDef(){
		return (*pointer=='d') && 
			   (*(pointer+1)=='e') && 
			   (*(pointer+2)=='f');		
	}	
	bool Tokenizer::IsReturn(){
		return (*pointer=='r') && 
			   (*(pointer+1)=='e') && 
			   (*(pointer+2)=='t') && 
			   (*(pointer+3)=='u') && 
			   (*(pointer+4)=='r') && 
			   (*(pointer+5)=='n');		
	}	
	bool Tokenizer::IsBreak(){
		return (*pointer=='b') && 
			   (*(pointer+1)=='r') && 
			   (*(pointer+2)=='e') && 
			   (*(pointer+3)=='a') && 
			   (*(pointer+4)=='k');		
	}	
	bool Tokenizer::IsContinue(){
		return (*pointer=='c') && 
			   (*(pointer+1)=='o') && 
			   (*(pointer+2)=='n') && 
			   (*(pointer+3)=='t') && 
			   (*(pointer+4)=='i') && 
			   (*(pointer+5)=='n') && 
			   (*(pointer+6)=='u') && 
			   (*(pointer+7)=='e');		
	}	
	void Tokenizer::SkipIf(){
		pointer+=2;
	}
	void Tokenizer::SkipElif(){
		pointer+=4;
	}
	void Tokenizer::SkipElse(){
		pointer+=4;
	}
	void Tokenizer::SkipDo(){
		pointer+=2;
	}
	void Tokenizer::SkipWhile(){
		pointer+=5;
	}
	void Tokenizer::SkipDef(){
		pointer+=3;
	}
	void Tokenizer::SkipReturn(){
		pointer+=6;
	}
	void Tokenizer::SkipBreak(){
		pointer+=5;
	}
	void Tokenizer::SkipContinue(){
		pointer+=8;
	}
	

	/* Skeep */
	void Tokenizer::SkipWhiteSpace(){
		while((*pointer)==' ' || (*pointer)=='\t'||
			  (*pointer)=='\r'|| (*pointer)=='\n'){

			  if((*pointer)=='\r')
			  { ++countline; pointer+=(*(pointer+1)=='\n'); }
			  else 
			  countline+=((*pointer)=='\n');

			  ++pointer;
		}
	}	
	
	/* Token parse */
	void Tokenizer::DeterminateToken(){
		//
		SkipWhiteSpace();
		/* IMPORTANT: KEYWORDs SEARCHs BEFORE VARIABLEs */
		if(IsIf())			type=IF; else 
		if(IsElif())			type=ELIF; else 
		if(IsElse())		type=ELSE; else 
		if(IsDo())			type=DO; else 
		if(IsWhile())		type=WHILE; else
		if(IsDef())			type=DEF; else
		if(IsReturn())		type=RETURN; else
		if(IsBreak())		type=BREAK; else
		if(IsContinue())	type=CONTINUE; else
		/* IMPORTANT: KEYWORDs SEARCHs BEFORE VARIABLEs */
		if(IsVariable())    type=VARIABLE; else 
		if(IsNumber())		type=NUMBER; else 
		if(IsString())		type=STRING; else 

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
		if(*pointer == '{') type=LS; else
		if(*pointer == '}') type=RS; else

		/* special key to be searched after exp */			
		if(*pointer == '=')  type=ASSIGNAMENT; else	
		if(*pointer == ',')  type=COMMA; else
		if(*pointer == '\0') type=END; else
		type=INVALID;
	}
	void Tokenizer::SkipToken(){		
		SkipWhiteSpace();
		/* IMPORTANT: KEYWORDs SEARCHs BEFORE VARIABLEs */
		if(type==IF)	 SkipIf(); else
		if(type==ELIF)	 SkipElif(); else
		if(type==ELSE)   SkipElse(); else
		if(type==DO)	 SkipDo(); else
		if(type==WHILE)  SkipWhile(); else
		if(type==DEF)    SkipDef(); else
		if(type==RETURN)   SkipReturn(); else
		if(type==BREAK)    SkipBreak(); else
		if(type==CONTINUE) SkipContinue(); else
		/* IMPORTANT: KEYWORDs SEARCHs BEFORE VARIABLEs */		
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
		if(type==RPR) ++pointer; else
		if(type==LS) ++pointer; else
		if(type==RS) ++pointer; else			
		/* special key to be searched after exp */
		if(type==ASSIGNAMENT) ++pointer; else
		if(type==COMMA) ++pointer;

	}