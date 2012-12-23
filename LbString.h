#ifndef LBSTRING_H
#define LBSTRING_H

#include "LbType.h"

/**
* A LiteB String
* @addtogroup LbString
* @{
*/
typedef
struct LbString{
    char* str;
    uint len;
}LbString;
/**
* Make a string
* @param const char
* @return LbString pointer
*/
extern LbString* LbString_Create(const char *str);
/**
* Inizialize a string
* @param LbString pointer
* @param const char
*/
extern void LbString_Init(LbString *in,const char *str);
/**
* Delete a string
* @param LbString pointer
*/
extern void LbString_Delete(LbString *string);
/**
* Clean a string
* @param LbString pointer
*/
extern void LbString_Clean(LbString *string);
/**
* Get len of a string
* @param LbString pointer
* @return unsigned int
*/
extern uint LbString_len(LbString *string);
/**
* Add to first string the second string
* @param LbString pointer
* @param LbString pointer
*/
extern void LbString_AddString(LbString *outFirst,LbString *endLast);
/**
* @}
*/


#endif