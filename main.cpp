#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Parse/ParseTree.h"
#include "Parse/IntCode.h"
#include "Parse/GenByteCode.h"
#include "VMCpp.h"

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

	
	/*
	LbVM test;
	LbBytecode testbt;
	
	LbFunction fun;
	fun.locals=2;
	fun.args=0;	
	fun.PushCommands(LbLineCommands(1,3,LB_LOADARG,0));
	fun.PushCommands(LbLineCommands(1,5,LB_LOADCONST,10.0));
	fun.PushCommands(LbLineCommands(1,1,LB_ADD,0));
	fun.PushCommands(LbLineCommands(1,1,LB_RETURN,1));

	testbt.PushFunction(fun);
	testbt.PushVariable(LbVariable("function",0));
	testbt.PushVariable(LbVariable("a",1.0f));
	testbt.PushVariable(LbVariable("c",0.0f));
	testbt.PushCommands(LbLineCommands(1,3,LB_LOAD,0));
	testbt.PushCommands(LbLineCommands(1,5,LB_LOAD,1));
	testbt.PushCommands(LbLineCommands(1,1,LB_CALL,1));
	testbt.PushCommands(LbLineCommands(1,5,LB_LOADRETURN,0));
	testbt.PushCommands(LbLineCommands(1,1,LB_SAVE,2));
	


	test.Init(&testbt);

	if(!test.Execute()){
		std::cout<<test.GetLbError().ToString();
		system("pause");
	}
	*/
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
		LbVM vmscript;
		vmscript.Init(bytecode);
		if(!vmscript.Execute()){
			std::cout<<vmscript.GetLbError().ToString();
			system("pause");
			return 0;
		}
	}
	else{
		std::cout << parse.ErrorsToString();
	}
	
	system("pause");

	return 0;
}