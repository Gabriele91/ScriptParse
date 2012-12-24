#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "UtilityString.h"

struct Tokenizer{

	enum Token{
		//VARIABLESSS
		STRING,
	    VARIABLE,
		NUMBER, // 12898303...
		//KEYWORD
		WHILE, //while (){}
		DO,    //do{}while()
		IF,	   //if(){}
		ELSE,  //else{}
		ELIF,    //<if>elif(){}
		DEF,    //def(<args>){}
		RETURN, // return(<exp>)
		BREAK,    //break
		CONTINUE, //continue
		//EXP
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
		LS,     // {
		RS,     // }
		//SPECIAL TOKEN
		ASSIGNAMENT,// =
		COMMA,		// ,
		END,        // EOF
		INVALID,     // ???
		NONE		 // ???
	};

	/**
	* @param const char* start string
	*/
	void SetScript(const char *pointer);
	void Start();
	Token GetToken();
	Token GetNextToken();
	int GetLine();
	void NextToken();
	std::string TokenValue();
	
	private:

	const char *pointer;
	Token type;
	int countline;

	/* VARIABLESSS */
	bool IsNumber();
	std::string GetNumber();
	void SkipNumber();
	
	bool IsVariable();
	bool IsVariableAfter();
	std::string GetVariable();
	void SkipVariable();

	bool IsString();
	bool GetCString(std::string& out);
	bool SkipString();
	/* EXP */

	bool IsLogicAnd();
	bool IsLogicOr();
	void SkipLogicAO();
	bool IsEQ();
	bool IsGTE();
	bool IsLTE();
	void SkipEGL();

	/* key words */
	bool IsIf();
	bool IsElif();
	bool IsElse();
	bool IsDo();
	bool IsWhile();
	bool IsDef();
	bool IsReturn();
	bool IsBreak();
	bool IsContinue();
	void SkipIf();
	void SkipElif();
	void SkipElse();
	void SkipDo();
	void SkipWhile();
	void SkipDef();
	void SkipReturn();;
	void SkipBreak();
	void SkipContinue();
	

	/* Skeep */
	void SkipWhiteSpace();
	
	/* Token parse */
	void DeterminateToken();
	void SkipToken();
	
};

#endif