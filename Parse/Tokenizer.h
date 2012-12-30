#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <string>

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
		FOR,    //for(<assignament>,<exp>, <assignament>){}
		RETURN, // return(<exp>)
		BREAK,    //break
		CONTINUE, //continue
		GLOBAL,   //global
		LOCAL,    //local
		//EXP
	    ADD,    // +
		MIN,    // -
		MUL,    // *
		DIV,    // /
		EQ,     // ==
		NOTEQ,  // !=
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
	int GetColumn();
	int GetLastLine();
	int GetLastColumn();
	void NextToken();
	std::string TokenValue();
	
	private:
	
	const char *pointer;
	const char *pointerline;
	Token type;
	int countline;
	int old_countline;
	int old_countcolumn;

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
	bool IsNOTEQ();
	bool IsGTE();
	bool IsLTE();
	void SkipNEGL();

	/* key words */
	bool IsIf();
	bool IsElif();
	bool IsElse();
	bool IsDo();
	bool IsWhile();
	bool IsDef();
	bool IsFor();
	bool IsReturn();
	bool IsBreak();
	bool IsContinue();
	bool IsGlobal();
	bool IsLocal();
	void SkipIf();
	void SkipElif();
	void SkipElse();
	void SkipDo();
	void SkipWhile();
	void SkipDef();
	void SkipFor();
	void SkipReturn();
	void SkipBreak();
	void SkipGlobal();
	void SkipLocal();
	void SkipContinue();
	

	/* Skeep */
	void SkipWhiteSpace();
	void SkiepComments();
	void DeterminateAndSkiepEndl();

	/* Token parse */
	void DeterminateToken();
	void SkipToken();
	
};

#endif