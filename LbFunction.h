#ifndef LBFUNCTION_H
#define LBFUNCTION_H

#include "LbType.h"
#include "LbError.h"
#include "LbString.h"
#include "LbVariable.h"

/**
* A LiteB Function
* @{
*/
typedef struct LbFunction{
			LbLineCommands *commands; //< commands
			uint lenCommands; //< commands count
			//args
			uint args;
			//locals
			uint locals;
}LbFunction; 
/**
* Alloc a function
* @return LbFunction pointer
*/
extern LbFunction* LbFunction_Alloc();
/**
* Inizialize a function
* @param LbFunction pointer
*/
extern void LbFunction_Init(LbVariable* ptr);
/**
* Alloc and inizialize a variable
* @return LbFunction pointer
*/
extern LbFunction* LbFunction_Create();
/**
* Dealloc a variable
* @param LbFunction pointer
*/
extern void LbFunction_Dealloc(LbFunction* ptr);
/**
* @}
*/

/**
* A LiteB FunctionContext
* @{
*/
typedef struct LbFunctionContext{
			LbVariable *args;
			LbVariable *context;
			uint countcontext;
			uint countargs;
			///RETURN VALUE
			uint lineReturn;
			LbLineCommands *rtCommands;
			uint rtLenCommands;
			///
}LbFunctionContext;
/**
* Alloc and inizialize a function context
* @param LbFunction pointer
* @param int (if args>function args, alloc more value)
*/
extern LbFunctionContext* LbFunctionContext_Create(LbFunction* ptr,int args=0);
/**
* Dealloc a function context
* @param LbFunctionContext pointer
*/
extern void LbFunctionContext_Delete(LbFunctionContext* ptr);

/**
* @}
*/

#endif