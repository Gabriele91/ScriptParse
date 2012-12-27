#ifndef LBERROR_H
#define LBERROR_H


#include "LbType.h"
#include "LbString.h"
/**
* Class error
* @addtogroup LbError
* @{
*/

enum EnLbValueError{
        VALUE_AN_ERROR=1,
        VALUE_NO_ERROR=0
};

enum EnLbErrorLog{
        ERROR_LOAD=0,
        ERROR_PUSH,
        ERROR_SAVE,
        /////////////
        ERROR_ADD,
        ERROR_MIN,
        ERROR_MUL,
        ERROR_DIV,
        /////////////
        ERROR_EQ,
        ERROR_NOTEQ,
        ERROR_GTEQ,
        ERROR_LTEQ,
        ERROR_GT,
        ERROR_LT,
        /////////////
        ERROR_AND,
        ERROR_OR,
        ERROR_NOT,
        /////////////
        ERROR_1ARG,
        ERROR_2ARG,
        ERROR_3ARG
};


//an error node
typedef
struct LbErrorNode{
        uint id;
        uint line;
        uint asmline;
        struct  LbErrorNode* next;
}LbErrorNode;
typedef
struct LbError{

    LbErrorNode *first;
    LbErrorNode *last;
    uint count;

}LbError;

extern const char *LbErrorLog[];

/**
* Init LbError (error list)
* @param LbError pointer
*/
void LbError_Init(LbError *ptr);
/**
* Push an error
* @param LbError pointer
* @param uint (id error)
* @param uint (line error)
*/
void LbError_Push(LbError *ptr,uint id,uint line,uint asmline);
/**
* Delete all'error in LbError
* @param LbError pointer
*/
void LbError_Clean(LbError *ptr);
/**
* @}
*/


#endif 