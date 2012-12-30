#include "LbError.h"

/**
* Push an error
*/
void LbError::PushError(LbErrorType error,
						unsigned int line, 
						unsigned int column,
						unsigned int asmline, 
						const std::string& addictioninfo){
	Error tmp;
	tmp.error=error;
	tmp.line=line;
	tmp.column=column;
	tmp.asmline=asmline;
	tmp.addictioninfo=addictioninfo;
	errors.push_back(tmp);
}


std::string LbError::ToString(){
	std::string out;
	for(int i=0;i<Size();++i){

		std::string addinfo=errors[i].addictioninfo.size()? 
							", "+errors[i].addictioninfo 
							: 
							std::string("");

		out+="error:"+String::ToString(errors[i].line)+
			":"+String::ToString(errors[i].column)+
			":"+String::ToString(errors[i].asmline)+
			":"+ErrorString[errors[i].error]+
			 addinfo+"\n";
	}
	return out;
}



const char *LbError::ErrorString[]={
	
		"STACK, error in stack",//LB_STACK,
		"CONTEXT, error in conext",//LB_CONTEXT,
	    "LOADCONST, load value",//LB_LOADCONST=0,
        "LOAD, load value",//LB_LOAD,
        "LOAD, push value",//LB_PUSH,
        "LLOAD, push value",//LB_LPUSH,

        "SAVE, save value",//LB_SAVE,
        "POP, pop value",//LB_POP,

        "CMD, -value",//LB_CMD,
        "ADD, value+value",//LB_ADD,
        "MIN, value-value",//LB_MIN,
        "MUL, value*value",//LB_MUL,
        "DIV, value/value",//LB_DIV,
		
        "EQ, value==value",//LB_EQ,
        "NOTEQ, value!=value",//LB_NOTEQ,
        "GTEQ, value>=value",//LB_GTEQ,
        "LTEQ, value<=value",//LB_LTEQ,
        "GT, value>value",//LB_GT,
        "TE, value<value",//LB_LT,

        "AND, value&&value",//LB_AND,
        "OR, value||value",//LB_OR,
        "NOT, !value",//LB_NOT,

        "IF, if(1)->jmp",//LB_IF,
        "IF0, if(0)->jmp",//LB_IF0,
        "GOTO, jmp",//LB_GOTO,
        "EIXT, end program",//LB_EXIT,

		//function
		"CALL, invalid call function",//LB_CALL,
		"LOADCALL, load value",//LB_LOADCALL,
		"LOADLOCAL, load local value",//LB_LOADLOCAL,
		"LOADARG, load argument value",//LB_LOADARG,
		"SAVELOCAL, save local value",//LB_SAVELOCAL,
		"SAVEARG, save argument value",//LB_SAVEARG,
		"RETURN, return value",//LB_RETURN,
		"LOADRETURN, load return value"//LB_LOADRETURN
};