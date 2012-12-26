#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Parse/ParseTree.h"
#include "Parse/IntCode.h"
#include "Parse/GenByteCode.h"

//#define TEST_VM
#define TEST_PARSE

#ifdef TEST_PARSE
std::string GetStringToken(Tokenizer::Token token){

	switch (token)
	{
	case Tokenizer::IF:
		return "IF";
		break;
	case Tokenizer::ELIF:
		return "ELIF";
		break;
	case Tokenizer::ELSE:
		return "ELSE";
		break;
	case Tokenizer::DO:
		return "DO";
		break;
	case Tokenizer::WHILE:
		return "WHILE";
		break;
	case Tokenizer::DEF:
		return "DEF";
		break;
	case Tokenizer::RETURN:
		return "RETURN";
		break;
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
	case Tokenizer::LS:
		return "LPR {";
		break;
	case Tokenizer::RS:
		return "RPR }";
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
	case Tokenizer::ASSIGNAMENT:
		return "ASSIGNAMENT =";
		break;
	case Tokenizer::END:
		return "END";
		break;
	case Tokenizer::INVALID:
		return "INVALID";
		break;
	case Tokenizer::NONE:
		return "NONE";
		break;
	default:		
		return "???????";
		break;
	}

}

int main(){
	std::ifstream t("test.pl");
	std::string scriptexp((std::istreambuf_iterator<char>(t)),
						   std::istreambuf_iterator<char>());

	std::cout << scriptexp << "\n\ntest Tokenizer:\n\n";
	/* test Tokenizer */
	Tokenizer tkn;
	tkn.SetScript(scriptexp.c_str());
	tkn.Start();
	while(tkn.GetToken()!=Tokenizer::END && tkn.GetToken()!=Tokenizer::INVALID){
		std::cout << GetStringToken(tkn.GetToken()) << std::endl;
		tkn.NextToken();
	}
	/* test SyntaxTree  */

	ParseTree parse;
	TreeNode* tree=parse.StartParse(scriptexp);

	std::cout <<"\nParse:" <<(!parse.FindErrors() ? "valid" : "invalid" )<< std::endl;
	
	if(!parse.FindErrors()){
		std::cout << "\nTree Parse:\n\n" <<tree->ToString()<< std::endl;
		
		ToIntCode tobytecode;
		tobytecode.ParseTree(tree);
		
		std::cout <<"\n\n"<<tobytecode.ToString();
		std::cout <<"\n\n"<<tobytecode.ToStringBasic();

		GenByteCode gen;
		gen.ParseIntermedieCode(&tobytecode);
		std::cout <<"\n\n"<<tobytecode.ToStringBasic();
		LbBytecode *bytecode=gen.AllocLbBytecode();
		VMliteB *liteB=VMliteB_Create(bytecode->commands,bytecode->lenCommands,
									  bytecode->variables,bytecode->variablescount,
									  bytecode->functions,bytecode->functionscount);
		VMliteB_Exec(liteB);
		VMliteB_Print(liteB);
		VMliteB_Free(liteB);
	}
	else{
		std::cout << parse.ErrorsToString();
	}
	
	system("pause");
	return 0;

}
#endif

/* TEST VM */

#ifdef TEST_VM
#include "VMliteB.h"
#include <string>

extern void PrintTestMain();

LbLineCommands program[]={	
   /*0 */ {0,LB_GOTO,15}, //TO MAIN
   ////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////
   /*1 */ {0,LB_LOADARG,0},   //LOAD ARG0
   /*2 */ {0,LB_LOADARG,0},   //RE LOAD ARG0
   /*3 */ {0,LB_ADD,0},       //STACK[0]<-arg+arg
   /*4 */ {0,LB_SAVELOCAL,0}, //LOCAL[0]<-STACK[0]
   ////////////////////////////////////////////////////
   /*5 */ {0,LB_LOADLOCAL,0},	   //LOCAL[0] (arg+arg)
   /*6 */ {0,LB_LOADCONST,100},    //LOAD 100.0
   /*7 */ {0,LB_LT,0},             //arg+arg < 100.0
   /*8 */ {0,LB_IF0,11},           //if(!(arg+arg < 100.0)) return
   /*9 */ {0,LB_LOADLOCAL,0},      //STACK[1]=LOCAL[0] (arg+arg)
   /*10 */{0,LB_RETURN,0},         //RETURN   
   ////////////////////////////////////////////////////
   /*11 */ {0,LB_LOAD,0},     //LOAD FUNCTION STACK[0]=val[0]
   /*12 */ {0,LB_LOADLOCAL,0},//STACK[1]=LOCAL[0] (arg+arg)
   /*13 */ {0,LB_CALL,1},     //CALL FUNCTION val[0] 1 arg
   /*14 */ {0,LB_RETURN,0},   //RETURN   
   ////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////
   /*15 */ {0,LB_LOAD,0}, //LOAD FUNCTION val[0]
   /*16 */ {0,LB_LOAD,1}, //LOAD VAR val[1]
   /*17 */ {0,LB_CALL,1}, //CALL FUNCTION, 1 arg
   /*18 */ {0,LB_SAVE,1}, //SAVE RETURN to val[1]
};


int main(){
    LbVariable vlist[2];
	LbVariable_SetFunction(&vlist[0],0);
    LbVariable_SetNumber(&vlist[1],5.0);
	LbFunction funList[1];
	//////////////////////////////
	funList[0].args=1;
	funList[0].lineCode=1;
	funList[0].locals=1;
	funList[0].rcount=1;
	//////////////////////////////



    VMliteB *liteB=VMliteB_Create(program,sizeof(program)/sizeof(LbLineCommands),
                                  vlist  ,sizeof(vlist)/sizeof(LbVariable),
								  funList,sizeof(funList)/sizeof(LbFunction));
    VMliteB_Exec(liteB);
    VMliteB_Print(liteB);
    VMliteB_Free(liteB);
	*/

    return 0;
}
#endif
