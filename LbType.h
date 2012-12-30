#ifndef LBTYPE_H
#define LBTYPE_H
#include "UtilityString.h"
#include <vector>
#include <string>
#include <sstream>

////Class declaretion
class LbVM;
class LbVariable;
class LbError;
class LbFunction;
class LbFunctionContext;
struct LbLineCommands;
////
////type definiction
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef std::string  cppstring;
typedef int (*LbCfunction) (LbVM*);
typedef void (*LbFnCommands) (LbVM*);

////type float
#ifdef LITEB_FLOAT
	typedef float LbFloat;
	#define LB_FLOAT_PRINT "%.6g"
#else
	typedef double LbFloat;
	#define LB_FLOAT_PRINT "%.6g"
#endif
//define stack size
#ifndef VMB_SIZE_STACK
	#define VMB_SIZE_STACK 1024
#endif
#ifndef VMB_SIZE_CONTEXT
	#define VMB_SIZE_CONTEXT 1024
#endif

#define DINLINE __inline
#define DFORCEINLINE __forceinline



#endif