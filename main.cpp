#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Parse/ParseTree.h"
#include "Parse/IntCode.h"
#include "Parse/GenByteCode.h"
#include "VMCpp.h"
#include "TinyScript.h"

int VM_print(LbVM *vm){
	for(int i=1;i<=vm->GetTopArgs();++i){
		if(vm->GetArg(i).IsString())
			std::cout << vm->GetArg(i).GetString();
		else if(vm->GetArg(i).IsNumber())
			std::cout << String::ToString(vm->GetArg(i).GetNumber());
		else{
			vm->PushError(LbError::LB_CALL,
				std::string("print, invalid tipe arg:")+String::ToString(i));
		}
	}
	std::cout << std::endl;
	return 0;
}
int VM_input(LbVM *vm){
	for(int i=1;i<=vm->GetTopArgs();++i){
		if(vm->GetArg(i).IsString())
			std::cout << vm->GetArg(i).GetString();
		else if(vm->GetArg(i).IsNumber())
			std::cout << String::ToString(vm->GetArg(i).GetNumber());
		else{
			vm->PushError(LbError::LB_CALL,
				std::string("print, invalid tipe arg:")+String::ToString(i));
		}
	}
	std::cout << std::endl;
	std::string valuein;
	std::cin >> valuein;
	vm->SetReturn(LbVariable("$const",valuein));
	return 0;
}
int VM_system(LbVM *vm){
	if(vm->GetTopArgs()!=1) 
		vm->PushError(LbError::LB_CALL,"system, invalid args number");	
	else{ 
		if(vm->GetArg(1).IsString())
			system(vm->GetArg(1).GetString().c_str());
		else
			vm->PushError(LbError::LB_CALL,"system, invalid tipe arg:1");
	}
	return 0;
}
int VM_openfile(LbVM *vm){	
	if(vm->GetTopArgs()!=2) 
		vm->PushError(LbError::LB_CALL,"open file, invalid args number");	
	else{
		if(!vm->GetArg(1).IsString()||!vm->GetArg(2).GetString().c_str()){
			vm->PushError(LbError::LB_CALL,"open file, invalid tipe args[1,2]");	
			return 0;
		}
		FILE *pfile=fopen(vm->GetArg(1).GetString().c_str(),
						  vm->GetArg(2).GetString().c_str());
		vm->SetReturn(LbVariable("open file",(void*)pfile));
		return 1;
	}
	return 0;
}
int VM_closefile(LbVM *vm){	
	if(vm->GetTopArgs()!=1) 
		vm->PushError(LbError::LB_CALL,"close file, invalid args number");	
	else{
		if(!vm->GetArg(1).IsObject()){
			vm->PushError(LbError::LB_CALL,"close file, invalid tipe args[1]");	
			return 0;
		}
		fclose((FILE*)vm->GetArg(1).GetObject());
	}
	return 0;
}
int VM_writefile(LbVM *vm){
	std::string out;
	if(vm->GetTopArgs()<2) 
		vm->PushError(LbError::LB_CALL,"write file, invalid args number");	
	else{
		if(!vm->GetArg(1).IsObject()){
			vm->PushError(LbError::LB_CALL,"write file, invalid tipe args[1]");	
			return 0;
		}
		for(int i=2;i<=vm->GetTopArgs();++i){
			if(vm->GetArg(i).IsString())
				out += vm->GetArg(i).GetString();
			else if(vm->GetArg(i).IsNumber())
				out += String::ToString(vm->GetArg(i).GetNumber());
			else{
				vm->PushError(LbError::LB_CALL,
					std::string("write file, invalid tipe arg:")+String::ToString(i));
			}
		}
		fwrite(out.c_str(),out.size(),1,(FILE*)vm->GetArg(1).GetObject());
	}
	return 0;
}
int VM_tofloat(LbVM *vm){
	if(vm->GetTopArgs()!=1) 
		vm->PushError(LbError::LB_CALL,"tofloat, invalid args number");	
	else{ 
		if(vm->GetArg(1).IsString()){
			float tmp=(LbFloat)(String::ToFloat(vm->GetArg(1).GetString()));
			vm->SetReturn(LbVariable("$const",tmp));
			return 1;		
		}else if(vm->GetArg(1).IsNumber()){
			float tmp=vm->GetArg(1).GetNumber();
			vm->SetReturn(LbVariable("$const",tmp));
			return 1;
		}else
			vm->PushError(LbError::LB_CALL,"tofloat, invalid tipe arg:1");
	}
	return 0;
}
int VM_toint(LbVM *vm){
	if(vm->GetTopArgs()!=1) 
		vm->PushError(LbError::LB_CALL,"toint, invalid args number");	
	else{ 
		if(vm->GetArg(1).IsString()){
			float tmp=(LbFloat)((int)String::ToFloat(vm->GetArg(1).GetString()));
			vm->SetReturn(LbVariable("$const",tmp));
			return 1;
		}else if(vm->GetArg(1).IsNumber()){
			float tmp=(LbFloat)((int)vm->GetArg(1).GetNumber());
			vm->SetReturn(LbVariable("$const",tmp));
			return 1;
		}else
			vm->PushError(LbError::LB_CALL,"toint, invalid tipe arg:1");
	}
	return 0;
}
int VM_tostring(LbVM *vm){
	if(vm->GetTopArgs()!=1) 
		vm->PushError(LbError::LB_CALL,"tostring, invalid args number");	
	else{ 
		if(vm->GetArg(1).IsString()){
			vm->SetReturn(LbVariable("$const",vm->GetArg(1).GetString()));
			return 1;
		}else if(vm->GetArg(1).IsNumber()){
			vm->SetReturn(LbVariable("$const",String::ToString(vm->GetArg(1).GetNumber())));
			return 1;
		}
			vm->PushError(LbError::LB_CALL,"tostring, invalid tipe arg:1");
	}
	return 0;
}

int main(){
	//read file
	std::ifstream sfile("test.pl");
	std::string scriptexp((std::istreambuf_iterator<char>(sfile)),
						   std::istreambuf_iterator<char>());
	//inizialize script
	TinyScript tiny;
	tiny.PushCFunction("input",VM_input);
	tiny.PushCFunction("print",VM_print);
	tiny.PushCFunction("system",VM_system);
	tiny.PushCFunction("openfile",VM_openfile);
	tiny.PushCFunction("writefile",VM_writefile);
	tiny.PushCFunction("closefile",VM_closefile);
	tiny.PushCFunction("tofloat",VM_tofloat);
	tiny.PushCFunction("toint",VM_toint);
	tiny.PushCFunction("tostring",VM_tostring);
	tiny.DoStript(scriptexp);
	tiny.Execute();


	if(tiny.FindErrors()){
		std::cout <<"errors:\n"<<tiny.Errors()<<std::endl; 
		system("pause");
	}
	return 0;
}

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

int __main(){

	
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