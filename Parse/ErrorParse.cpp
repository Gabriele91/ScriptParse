#include "ErrorParse.h"

void ErrorParse::PushError(unsigned int line,
						   unsigned char errorType, 
						   const std::string& addictioninfo){
		Error error;
		error.line=line;
		error.error=errorType;
		error.addictioninfo=addictioninfo;
		errors.push_back(error);
	}

std::string ErrorParse::ToString(){
	std::string out;
	for(int i=0;i<errors.size();++i){
		out+="error:"
			 +String::ToString(errors[i].line)+":"
			 +ErrorString[errors[i].error]
		     +(errors[i].addictioninfo.length()? std::string(", ")+errors[i].addictioninfo:"")
			 +"\n";
			}
			return out;
		}

const char *ErrorParse::ErrorString[]={
				//STATEMENT
				"invalid expression",//EXP,
				"invalid statement", //STATEMENT
				//
                "invalid parse",  //PARSE,
                "invalid syntax",  //SYNTAX,
                "invalid variable", //VARIABLE,
                "invalid number", //NUMBER,
                "invalid string", //STRING,

                "'do-while' parse error", //DO,
                "'if' parse error", //IF,
                "'elif' parse error", //ELIF,
                "'else' parse error", //ELSE,
                "'while' parse error", //WHILE,
                "'def' parse error", //DEF,
                "'return' parse error", //RETURN,
                "'break' parse error", //BREAK,
                "'continue' parse error", //CONTINUE,

                "'==' parse error",//EQ,
                "'>' parse error",//GT,
                "'<' parse error",//LT,
                "'>=' parse error",//GTE,
                "'<=' parse error",//LTE,

				"'&&' parse error",//AND,
				"'||' parse error",//OR,

				"'+' parse error",//ADD,
				"'-' parse error",//MIN,
				"'*' parse error",//MUL,
				"'/' parse error",//DIV,

				"'(' parse error",//LPR,
				"')' parse error",//RPR,
				"'{' parse error",//LS,
				"'}' parse error",//RS,
				
				"'=' parse error",//ASSIGNAMENT,
				"',' parse error",//COMMA,
				"EOF parse error",//END,
				"indeterminate parse error",//INVALID 
				"token->?NONE?<-token",//NONE    
};
