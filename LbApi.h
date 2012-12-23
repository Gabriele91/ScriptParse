#ifndef LBAPI_H
#define LBAPI_H
#include "VMliteB.h"

/**
* Class CAPI_liteB
* @addtogroup CAPI_liteB
* @{
*/
////////////////////////////////////////
/////////////// C API //////////////////
typedef int (*LSFuncPointer) (VMliteB*);
//GET
extern int LS_CountArgs(VMliteB *B);
extern LbVariable* LS_GetArgs(VMliteB *B,int i);
extern const char* LS_StringArgs(VMliteB *B,int i);
extern int LS_IntArgs(VMliteB *B,int i);
extern LbFloat LS_NumberArgs(VMliteB *B,int i);
extern int LS_FunctionArgs(VMliteB *B,int i);
extern LSFuncPointer LS_CFunctionArgs(VMliteB *B,int i);
//Is a ?
extern bool LS_IsANumber(VMliteB *B,int i);
extern bool LS_IsAString(VMliteB *B,int i);
extern bool LS_IsAFunction(VMliteB *B,int i);
extern bool LS_IsACFunction(VMliteB *B,int i);
//Return
extern void LS_ReturnNumber(VMliteB *B,LbFloat v);
extern void LS_ReturnString(VMliteB *B,const char* s);
extern void LS_ReturnFunction(VMliteB *B,int i);
extern void LS_ReturnCFunction(VMliteB *B,LSFuncPointer cfun);
/**
* @}
*/
#endif