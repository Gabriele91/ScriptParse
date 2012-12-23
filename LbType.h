#ifndef LBTYPE_H
#define LBTYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>
#include <string.h>
//bool
#ifndef __cplusplus
    #ifndef bool
        typedef unsigned char bool;
        #define false '\0'
        #define true 1
    #endif
#endif


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
#ifdef LITEB_FLOAT
	typedef float LbFloat;
	#define LB_FLOAT_PRINT "%.6g"
#else
	typedef double LbFloat;
	#define LB_FLOAT_PRINT "%.6g"
#endif
	
////Struct declaretion
struct VMliteB;
typedef struct VMliteB VMliteB;
////
////
struct LbLineCommands;
typedef struct LbLineCommands LbLineCommands;
////

#endif