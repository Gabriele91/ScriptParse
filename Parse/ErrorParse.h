#ifndef ERROR_PERSE_H
#define ERROR_PERSE_H

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
				SYNTAX,
                VARIABLE,
                NUMBER,
                STRING,

                DO,
                IF,
                ELIF,
                ELSE,
                WHILE,
				DEF,
				FOR,
				RETURN,
				BREAK,
				CONTINUE,
				GLOBAL,
				LOCAL,

                EQ,
				NOTEQ,
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
				unsigned int column; 
				unsigned char error;
				std::string   addictioninfo;
			};
	
	std::vector<Error> errors;
	static const char *ErrorString[];

	void PushError(unsigned int line, 
				   unsigned int column, 
				   unsigned char errorType, 
				   const std::string& addictioninfo);

	std::string ToString();

};

#endif