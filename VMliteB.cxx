#include "VMliteB.h"
#include <string.h>

//////////////////////////
//Commands

#define CR_VARIABLE_CP(B,I) LbVariable_Copy(&(B)->variables[(I)])
#define CR_VARIABLE(B,I) ((B)->variables[(I)])

#define CR_SET_PC(B,I) (B)->pc=(I)
#define CR_RPL(D,F) LbVariable_Replace(&(D),&(F))

#define CR_I_STACK(B,I) (B)->stack[(I)]
#define CR_ID_STACK(B) ((B)->stackcount)
#define CR_STACK(B) (B)->stack[(B)->stackcount]
#define CR_STACKPP(B) ++((B)->stackcount)
#define CR_STACKMM(B) --((B)->stackcount)

#define CR_OPREGISTER(B) (B)->opRegister
#define CR_OPtoSTACK(B) LbVariable_Replace(&(CR_STACK(B)),&((B)->opRegister))

#define CR_COMMAND(B) (B)->commands[(B)->pc]
#define CR_COMMAND_LINE(B) (B)->commands[(B)->pc].line
#define CR_COMMAND_CM(B) (B)->commands[(B)->pc].command
#define CR_COMMAND_ID(B) ((int)(B)->commands[(B)->pc].id)
#define CR_COMMAND_FLOAT_ID(B) ((B)->commands[(B)->pc].id)


#define CR_IF_ERROR(B,X,ID) if((X)){ LbError_Push(&((B)->errors),(ID),CR_COMMAND_LINE(B),(B)->pc); }
#define CR_IF_ERROR_RT(B,X,ID) if((X)){ LbError_Push(&((B)->errors),(ID),CR_COMMAND_LINE(B),(B)->pc); return; }

void LbCmd_LOADCONST(VMliteB *B){
   CR_STACKPP(B);
   //stack[i] <- variable[ value ]
   LbVariable_Clean(&CR_STACK(B));
   CR_STACK(B).var.number=CR_COMMAND_FLOAT_ID(B);
   CR_STACK(B).type=VALUE_NUMBER;
}
void LbCmd_LOAD(VMliteB *B){
   CR_STACKPP(B);
   //
   CR_IF_ERROR_RT(B,CR_COMMAND_ID(B)>B->variablescount,ERROR_LOAD)
   //stack[i] <- variable[ value ]
   CR_RPL(CR_STACK(B),CR_VARIABLE(B,CR_COMMAND_ID(B)));
}
void LbCmd_PUSH(VMliteB *B){
   CR_STACKPP(B);
   //
   CR_IF_ERROR_RT(B,(CR_ID_STACK(B)-CR_COMMAND_ID(B))<0||
                    (CR_ID_STACK(B)-CR_COMMAND_ID(B))>VMB_SIZE_STACK,ERROR_PUSH)
   //stack[i] <- stack[ i - value ]
   CR_RPL(CR_STACK(B),
          CR_I_STACK(B,CR_ID_STACK(B)-CR_COMMAND_ID(B)));
}
void LbCmd_LPUSH(VMliteB *B){
   CR_STACKPP(B);
   //
   CR_IF_ERROR_RT(B,(CR_ID_STACK(B)-CR_COMMAND_ID(B))<0||
                    (CR_ID_STACK(B)-CR_COMMAND_ID(B))>VMB_SIZE_STACK,ERROR_PUSH)
   CR_IF_ERROR_RT(B,CR_I_STACK(B,CR_ID_STACK(B)-CR_COMMAND_ID(B)).var.id<0||
                    CR_I_STACK(B,CR_ID_STACK(B)-CR_COMMAND_ID(B)).var.id>=B->variablescount,ERROR_PUSH)
   //
   //stack[i] <- variable[ stack[i - value] ]
   CR_RPL(CR_STACK(B),
          CR_VARIABLE(B,
                      CR_I_STACK(B,
                                 CR_ID_STACK(B)-CR_COMMAND_ID(B)
                                 ).var.id
                      )
          );
}

void LbCmd_SAVE(VMliteB *B){
   CR_RPL(CR_VARIABLE(B,CR_COMMAND_ID(B)),CR_STACK(B));
   CR_STACKMM(B);

   CR_IF_ERROR(B,CR_COMMAND_ID(B)>B->variablescount,ERROR_SAVE)
}
void LbCmd_POP(VMliteB *B){
   CR_STACKMM(B);
}
// stack[i] = -stack[i]
void LbCmd_CMD(VMliteB *B){
    if(CR_ID_STACK(B)>=0){

        CR_IF_ERROR(B,
        LbVariable_Cmd(&CR_OPREGISTER(B),
                       &CR_I_STACK(B,CR_ID_STACK(B)))
        ,ERROR_NOT);

        CR_OPtoSTACK(B);
    }else CR_IF_ERROR(B,true,ERROR_1ARG)
}
// stack[i-1] op =stack[i] --i
#define OP_VAR(OP,EOP)\
if(CR_ID_STACK(B)>0){\
   CR_IF_ERROR(B, \
   LbVariable_##OP(&CR_I_STACK(B,CR_ID_STACK(B)-1),\
                   &CR_I_STACK(B,CR_ID_STACK(B)))\
    ,ERROR_##EOP);\
    CR_STACKMM(B);\
}else CR_IF_ERROR(B,true,ERROR_2ARG)

void LbCmd_ADD(VMliteB *B){
    OP_VAR(Add,ADD)
}
void LbCmd_MIN(VMliteB *B){
    OP_VAR(Sub,MIN)
}
void LbCmd_MUL(VMliteB *B){
    OP_VAR(Mul,MUL)
}
void LbCmd_DIV(VMliteB *B){
    OP_VAR(Div,DIV)
}

// stack[i-1]=stack[i-1] op stack[i] --i
#define OP_VAR3(OP,EOP)\
if(CR_ID_STACK(B)>0){\
        CR_IF_ERROR(B, \
        LbVariable_##OP(&CR_OPREGISTER(B),\
                        &CR_I_STACK(B,CR_ID_STACK(B)-1),\
                        &CR_I_STACK(B,CR_ID_STACK(B)))\
        ,ERROR_##EOP);\
        CR_STACKMM(B);\
        CR_OPtoSTACK(B);\
    }else CR_IF_ERROR(B,true,ERROR_2ARG)

void LbCmd_EQ(VMliteB *B){
    OP_VAR3(Eq,EQ)
}
void LbCmd_NOTEQ(VMliteB *B){
    OP_VAR3(NotEq,NOTEQ)
}
void LbCmd_GTEQ(VMliteB *B){
    OP_VAR3(GtEq,GTEQ)
}
void LbCmd_LTEQ(VMliteB *B){
    OP_VAR3(LtEq,LTEQ)
}
void LbCmd_GT(VMliteB *B){
    OP_VAR3(Gt,GT)
}
void LbCmd_LT(VMliteB *B){
    OP_VAR3(Lt,LT)
}

void LbCmd_AND(VMliteB *B){
    OP_VAR3(And,AND)
}
void LbCmd_OR(VMliteB *B){
    OP_VAR3(Or,OR)
}

void LbCmd_NOT(VMliteB *B){
    //stack[i]!=stack[i]
    if(CR_ID_STACK(B)>=0){

        CR_IF_ERROR(B,
        LbVariable_Not(&CR_OPREGISTER(B),
                       &CR_I_STACK(B,CR_ID_STACK(B)))
        ,ERROR_NOT);

        CR_OPtoSTACK(B);
    }else CR_IF_ERROR(B,true,ERROR_1ARG)
}

void LbCmd_IF(VMliteB *B){
   if( CR_STACK(B).var.number!=0.0 ) CR_SET_PC(B,CR_COMMAND_ID(B)-1);
   CR_STACKMM(B);
}
void LbCmd_IF0(VMliteB *B){
   if( CR_STACK(B).var.number==0.0 ) CR_SET_PC(B,CR_COMMAND_ID(B)-1);
   CR_STACKMM(B);
}
void LbCmd_GOTO(VMliteB *B){
    CR_SET_PC(B,CR_COMMAND_ID(B)-1);
}
void LbCmd_EXIT(VMliteB *B){
    CR_SET_PC(B,B->lenCommands);
}
///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
#define CR_VARIABLE_ARG(B,I) B->fncontext[B->fncontextcount]->args[(I)]
#define CR_VARIABLE_LOCAL(B,I) B->fncontext[B->fncontextcount]->context[(I)]
#define CR_VARIABLE_ARG_COUNT(B) B->fncontext[B->fncontextcount]->countargs
#define CR_VARIABLE_LOCAL_COUNT(B) B->fncontext[B->fncontextcount]->countcontext
///////////////////////////////////////////
void LbCmd_CALL(VMliteB *B){
	uint args=CR_COMMAND_ID(B);
	if(CR_I_STACK(B,CR_ID_STACK(B)-args).type==VALUE_FUNCTION){
		uint function=CR_I_STACK(B,CR_ID_STACK(B)-args).var.id;
		//make context
		++(B->fncontextcount);
		B->fncontext[B->fncontextcount]=LbFunctionContext_Create(&(B->functions[function]),args);
		//set args
		uint ui=0;
		for(;ui<args;++ui){ //copy stack in args
		 CR_RPL(CR_VARIABLE_ARG(B,args-ui-1),CR_STACK(B));
		 CR_STACKMM(B);
		 CR_IF_ERROR(B,CR_COMMAND_ID(B)>CR_VARIABLE_ARG_COUNT(B),ERROR_SAVE);
		}
		//set return value
		B->fncontext[B->fncontextcount]->lineReturn=B->pc;
		B->fncontext[B->fncontextcount]->rtCommands=B->commands;
		B->fncontext[B->fncontextcount]->rtLenCommands=B->lenCommands;
		//delete function name from stack
		CR_STACKMM(B);
		//change commands
		//Jump	to function's opcode
		B->commands=B->functions[function].commands;
		B->lenCommands=B->functions[function].lenCommands;
		CR_SET_PC(B,-1);
	}
	else if(CR_I_STACK(B,CR_ID_STACK(B)-args).type==VALUE_CFUNCTION)
	{
		//save ARGS
		B->topCallStack=CR_ID_STACK(B)-args+1;
		//call
		CR_I_STACK(B,CR_ID_STACK(B)-args).var.cfun(B);	
		//delete args from stack
		uint ui=0;
		for(;ui<args;++ui){ CR_STACKMM(B);}
		B->topCallStack=-1;
		//delete function name from stack
		CR_STACKMM(B);
	}
	//else error...
}

void LbCmd_LOADLOCAL(VMliteB *B){
   //
   CR_STACKPP(B);
   //     
   CR_IF_ERROR_RT(B,CR_COMMAND_ID(B)>CR_VARIABLE_LOCAL_COUNT(B),ERROR_LOAD)
   //stack[i] <- variable_local[ value ]
   CR_RPL(CR_STACK(B),CR_VARIABLE_LOCAL(B,CR_COMMAND_ID(B)));
   //   
}
void LbCmd_LOADARG(VMliteB *B){
   //
   CR_STACKPP(B);
   //     
   CR_IF_ERROR_RT(B,CR_COMMAND_ID(B)>CR_VARIABLE_ARG_COUNT(B),ERROR_LOAD)
   //stack[i] <- variable_arg[ value ]
   CR_RPL(CR_STACK(B),CR_VARIABLE_ARG(B,CR_COMMAND_ID(B)));
   //   
}
void LbCmd_SAVELOCAL(VMliteB *B){
   CR_RPL(CR_VARIABLE_LOCAL(B,CR_COMMAND_ID(B)),CR_STACK(B));
   CR_STACKMM(B);
   CR_IF_ERROR(B,CR_COMMAND_ID(B)>CR_VARIABLE_LOCAL_COUNT(B),ERROR_SAVE)
}
void LbCmd_SAVEARG(VMliteB *B){
   CR_RPL(CR_VARIABLE_ARG(B,CR_COMMAND_ID(B)),CR_STACK(B));
   CR_STACKMM(B);
   CR_IF_ERROR(B,CR_COMMAND_ID(B)>CR_VARIABLE_ARG_COUNT(B),ERROR_SAVE)
}
void LbCmd_RETURN(VMliteB *B){
	//Save return	
   CR_IF_ERROR_RT(B,(CR_ID_STACK(B)-CR_COMMAND_ID(B))<-1 ||
					CR_COMMAND_ID(B)<0||
                    (CR_ID_STACK(B)-CR_COMMAND_ID(B))>VMB_SIZE_STACK,ERROR_PUSH)					
   //returnvars[0..i] <- stack[ i - value[i..0] ]
   int vars=CR_COMMAND_ID(B);
   for(;vars;--vars){
	   ++B->returncount;
	    CR_RPL(B->returnvariables[B->returncount], 
	 		   CR_I_STACK(B,CR_ID_STACK(B)-(vars-1)));
    }
   for(vars=CR_COMMAND_ID(B);vars;--vars) CR_STACKMM(B);

	//jump back	and set commands return
	CR_SET_PC(B,B->fncontext[B->fncontextcount]->lineReturn);
	B->commands=B->fncontext[B->fncontextcount]->rtCommands;
	B->lenCommands=B->fncontext[B->fncontextcount]->rtLenCommands;
	//delete context
	LbFunctionContext_Delete(B->fncontext[B->fncontextcount]);
	--(B->fncontextcount);	
}
void LbCmd_LOADRETURN(VMliteB *B){
   //
   CR_STACKPP(B);
   //     
   CR_IF_ERROR_RT(B,B->returncount<0,ERROR_LOAD)
   //stack[i] <- variable_return[ ir ]
   CR_RPL(CR_STACK(B),B->returnvariables[B->returncount]);
   --B->returncount;
   //   
}
void LbCmd_ENDCALL(VMliteB *B){
   //
   B->returncount=0;
   //   
}

LbFnCommands listCommands[]={
        LbCmd_LOADCONST,
        LbCmd_LOAD,
        LbCmd_PUSH,
        LbCmd_LPUSH,

        LbCmd_SAVE,
        LbCmd_POP,
		
        LbCmd_CMD,
        LbCmd_ADD,
        LbCmd_MIN,
        LbCmd_MUL,
        LbCmd_DIV,
        LbCmd_EQ,
		LbCmd_NOTEQ,
        LbCmd_GTEQ,
        LbCmd_LTEQ,
        LbCmd_GT,
        LbCmd_LT,
        LbCmd_AND,
        LbCmd_OR,
        LbCmd_NOT,
        LbCmd_IF,
        LbCmd_IF0,
        LbCmd_GOTO,
        LbCmd_EXIT,

        LbCmd_CALL,
        LbCmd_LOADLOCAL,
        LbCmd_LOADARG,
		LbCmd_SAVELOCAL,
		LbCmd_SAVEARG,
		LbCmd_RETURN,
		LbCmd_LOADRETURN,
		LbCmd_ENDCALL
};


extern const char* LbCommandsString[]={
	 "LB_LOADCONST",
     "LB_LOAD",
     "LB_PUSH",
     "LB_LPUSH",

     "LB_SAVE",
     "LB_POP",

     "LB_CMD",
     "LB_ADD",
     "LB_MIN",
     "LB_MUL",
     "LB_DIV",
	 
     "LB_EQ",
     "LB_NOTEQ",
     "LB_GTEQ",
     "LB_LTEQ",
     "LB_GT",
     "LB_LT",

      "LB_AND",
      "LB_OR",
      "LB_NOT",

      "LB_IF",
      "LB_IF0",
      "LB_GOTO",
      "LB_EXIT",
	  
	  "LB_CALL",
	  "LB_LOADLOCAL",
	  "LB_LOADARG",
	  "LB_SAVELOCAL",
	  "LB_SAVEARG",
	  "LB_RETURN",
	  "LB_LOADRETURN",
	  "LB_ENDCALL"
};

VMliteB* VMliteB_Create(LbLineCommands *commands,uint lenCommands,
                        LbVariable  *variables,uint variablescount,
						LbFunction  *functions,uint functionscount){

        VMliteB* out=(VMliteB*)malloc(sizeof(VMliteB));
        out->pc=0;
        out->stackcount=-1;

        out->lenCommands=lenCommands;
        out->variablescount=variablescount;


        out->commands=commands;
        out->variables=variables;
		
		out->functions=functions;
		out->functionscount=functionscount;
		out->fncontextcount=-1;
		out->returncount=-1;
		//C call
	    out->topCallStack=-1;

        uint i=0;
        for(;i<VMB_SIZE_STACK;++i)
            LbVariable_Init(&out->stack[i]);
        LbVariable_Init(&out->opRegister);

        LbError_Init(&out->errors);


        return out;
}
void VMliteB_Exec(VMliteB *vm){
    for(;vm->pc<vm->lenCommands;++vm->pc)
        (listCommands[vm->commands[vm->pc].command]).func(vm);
}
void VMliteB_Print(VMliteB *B){

    printf("\nVMLiteB:\n");
    printf("Pc:%d\n",B->pc);
    printf("Stack.size: %d\n",B->stackcount+1);

    int i=0;
    for(;i<=B->stackcount && 0<=B->stackcount;++i){
       printf("Stack[%d]:",i);
       LbVariable_Print(&B->stack[i]);
       printf("\n");
    }

    printf("opRegister:");
    LbVariable_Print(&B->opRegister);
    printf("\n");

    printf("variables:\n");
    uint ui=0;
    for(ui=0;ui<B->variablescount;++ui){
        printf("var(%d): (",ui);
        LbVariable_Print(&B->variables[ui]);
        printf(")\n");
     }

     if(B->errors.count){
         printf("\n------------------------------\n");
         printf("errors(%d):\n\n",B->errors.count);
         LbErrorNode *tmp=B->errors.first;
         for(ui=1;tmp;tmp=tmp->next){
                printf("Error(%d) Line(%d,%d): %s\n",ui,tmp->line,tmp->asmline,LbErrorLog[tmp->id]);
                ++ui;
         }
         printf("\n------------------------------\n");
     }

}
extern void VMliteB_PrintStack(VMliteB *B){
    printf("Stack[\t");
    uint i=0;
	for(;i<=B->stackcount && 0<=B->stackcount;++i){
       LbVariable_Print(&B->stack[i]);
       printf("\t");
    }
	printf("]");
}
void VMliteB_PrintCommands(VMliteB *B){
    printf("\n-------------MAIN-------------\n");
	uint ui;

	for(ui=0;ui<B->lenCommands;++ui)
		printf("%d, %d  %s %.6g\n",B->commands[ui].line,ui,LbCommandsString[B->commands[ui].command],B->commands[ui].id);
	
    printf("\n------------------------------\n");
	
    printf("\n-----------FUNCTIONS-----------\n");
	
	int f;
	for(f=0;f<B->functionscount;++f){
		printf("\n-----------FUNCTION[%d]-----------\n",f+1);
		for(ui=0;ui<B->functions[f].lenCommands;++ui){
			printf("%d, %d  %s %.6g\n",B->functions[f].commands[ui].line,ui,LbCommandsString[B->functions[f].commands[ui].command],B->functions[f].commands[ui].id);
			
		}
		printf("\n------------------------------\n");
	}

    printf("\n------------------------------\n");
}
void VMliteB_Free(VMliteB *vm){

    uint i=0;
    //clean(/dealloc) variables
    for(i=0;i<VMB_SIZE_STACK;++i)
            LbVariable_Clean(&vm->stack[i]);
        LbVariable_Clean(&vm->opRegister);
    //clean(/dealloc) functioncontext
	for(;vm->fncontextcount>-1;--vm->fncontextcount)
		LbFunctionContext_Delete(vm->fncontext[vm->fncontextcount]);
    //clean(/dealloc) error list
    LbError_Clean(&vm->errors);
    //dealloc vm
    free(vm);
}

