#ifndef ERROR_PERSE_H
#define ERROR_PARSE_H

#include "UtilityString.h"
#include <vector>

/**
*  Class management errors
*/
struct ErrorParse{

	enum ErrorType{
				//STATEMENT
				EXP=0,
				STATEMENT,
				//
                PARSE,
                VARIABLE,
                NUMBER,
                STRING,

                DO,
                IF,
                EIF,
                ELSE,
                WHILE,
				DEF,
				RETURN,

                EQ,
                GT,
                LT,
                GTE,
                LTE,

				AND,
				OR,

				ADD,
				MIN,
				MUL,
				DIV,

				LPR,
				RPR,
				LS,
				RS,

				ASSIGNAMENT,
				COMMA,
				END,
				INVALID,
				NONE     


            };
	struct Error{ 
				unsigned int line; 
				unsigned char error;
				const char    *addictioninfo;
			};
	
	std::vector<Error> errors;
	static const char *ErrorString[];

	void PushError(unsigned int line, 
				   unsigned char errorType, 
				   const char *addictioninfo);

	std::string ToString();

};

#endif