/**
*   @autor Gabriele Di Bari
* 	@date  ( yyyy-mm-dd, hh:mm ) 2012-12-22 00:24
* 	@version 0.1
*
*/
#include "LbType.h"
#include "LbError.h"
#include "LbString.h"
#include "LbVariable.h"
#include "LbFunction.h"

/**
* Class function commands
* @struct LbFnCommands
*/
typedef struct LbFnCommands{
    void (*func)(VMliteB*);
}LbFnCommands;
extern LbFnCommands listCommands[];
////
/**
* Class commands
* @addtogroup LbLineCommands
* @{
*/
typedef struct LbLineCommands{
    uint  line;    ///< source line
    uchar command; ///< command
    LbFloat id;      ///< value
}LbLineCommands;
enum Commands{

        LB_LOADCONST=0,
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
		LB_LOADLOCAL,
		LB_LOADARG,
		LB_SAVELOCAL,
		LB_SAVEARG,
		LB_RETURN,
		LB_LOADRETURN,
		LB_ENDCALL
};
extern const char* LbCommandsString[];
/**
* @}
*/

/**
* Class VMliteB
* @addtogroup VMliteB
* @{
*/
typedef
struct VMliteB{
        //program counter;
        uint pc;
        LbLineCommands *commands;
        uint lenCommands;
        //opration register
        LbVariable opRegister;
        //stack
        #ifndef VMB_SIZE_STACK
        #define VMB_SIZE_STACK 1024
        #endif
        short stackcount;
        LbVariable stack[VMB_SIZE_STACK];
        //
        uint variablescount;
        LbVariable  *variables;
		//
        uint functionscount;
		LbFunction* functions;
		//        
		#ifndef VMB_SIZE_FUNCONTEXT
        #define VMB_SIZE_FUNCONTEXT 1024
        #endif
		short fncontextcount;
		LbFunctionContext *fncontext[VMB_SIZE_FUNCONTEXT];
		//return values
        #ifndef VMB_SIZE_VAR_RETURN
        #define VMB_SIZE_VAR_RETURN 1
        #endif
        short returncount;
        LbVariable returnvariables[VMB_SIZE_VAR_RETURN];
		//Set top stack C call
		int topCallStack;
		//
        LbError errors;
        //
}VMliteB;

/**
* Create a virtual machine
* @param LbLineCommands pointer
* @param uint (n commands)
* @param LbVariable pointer
* @param uint (n variables)
* @return VMliteB pointer
*/
extern VMliteB* VMliteB_Create(LbLineCommands *commands,uint lenCommands,
                               LbVariable  *variables,uint variablescount,
							   LbFunction  *functions,uint functionscount);
/**
* Execute a program
* @param VMliteB pointer
*/
extern void VMliteB_Exec(VMliteB *vm);
/**
* Print a virtual machine (context)
* @param VMliteB pointer
*/
extern void VMliteB_Print(VMliteB *B);
extern void VMliteB_PrintStack(VMliteB *B);
/**
* Print opcode
* @param VMliteB pointer
*/
extern void VMliteB_PrintCommands(VMliteB *B);
/**
* Dealloc a virtual machine
* @param VMliteB pointer
*/
extern void VMliteB_Free(VMliteB *vm);
/**
* @}
*/


