#ifndef BYTECODE_H
#define BYTECODE_H

#include "LbType.h"
#include "LbError.h"
#include "LbString.h"
#include "LbVariable.h"
#include "LbFunction.h"

/**
* A LiteB Bytecode struct
* @addtogroup LbBytecode
* @{
*/
typedef
struct LbBytecode{

    LbLineCommands *commands,
	uint lenCommands,    
	LbVariable  *variables,
	uint variablescount,	
	LbFunction  *functions,
	uint functionscount;

}LbBytecode;
/**
* @}
*/

#endif