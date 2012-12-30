#ifndef LBERROR_H
#define LBERROR_H

#include "LbType.h"

class LbError{
	
	public:	

	enum LbErrorType{
		LB_STACK=0,
		LB_CONTEXT,

	    LB_LOADCONST,
        LB_LOAD,
        LB_PUSH,
        LB_LPUSH,

        LB_SAVE,
        LB_POP,

        LB_CMD,
        LB_ADD,
        LB_MIN,
        LB_MUL,
        LB_DIV,
		
        LB_EQ,
        LB_NOTEQ,
        LB_GTEQ,
        LB_LTEQ,
        LB_GT,
        LB_LT,

        LB_AND,
        LB_OR,
        LB_NOT,

        LB_IF,
        LB_IF0,
        LB_GOTO,
        LB_EXIT,

		//function
		LB_CALL,
		LB_LOADCALL,
		LB_LOADLOCAL,
		LB_LOADARG,
		LB_SAVELOCAL,
		LB_SAVEARG,
		LB_RETURN,
		LB_LOADRETURN
	};


	DFORCEINLINE uint Size(){ return errors.size();  }
	DFORCEINLINE void Clear(){ errors.clear(); }

	void PushError(LbErrorType error,
				   unsigned int line, 
				   unsigned int column,
				   unsigned int asmline, 
				   const std::string& addictioninfo);


	std::string ToString();
	
	private:

	struct Error{ 
				LbErrorType   error;
				unsigned int  line;
				unsigned int  column; 
				unsigned int  asmline; 
				std::string   addictioninfo;
			};
	std::vector<Error> errors;
	static const char *ErrorString[];
};


#endif