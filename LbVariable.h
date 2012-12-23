#ifndef LBVARIABLE_H
#define LBVARIABLE_H

#include "LbType.h"
#include "LbError.h"
#include "LbString.h"

/**
* A LiteB Variable
* @addtogroup LbVariable
* @{
*/
enum ValueType{
    VALUE_ID,
    VALUE_NUMBER,
    VALUE_STRING,
	VALUE_FUNCTION,
	VALUE_CFUNCTION
};
typedef
struct LbVariable{

    union Value{
        ushort id;     ///< if is a id
        LbFloat number;  ///< if is a number
        LbString string; ///< if is a string
		int (*cfun) (VMliteB*);//if is a CFunction
    }var;///< variable


    uchar type;///< type variable

}LbVariable;

/**
* Ptint a variable
* @param LbVariable pointer
*/
extern void LbVariable_Print(LbVariable *ptr);
/**
* Alloc a variable
* @return LbVariable pointer
*/
extern LbVariable* LbVariable_Alloc();
/**
* Inizialize a variable
* @param LbVariable pointer
*/
extern void LbVariable_Init(LbVariable* ptr);
/**
* Alloc and inizialize a variable
* @return LbVariable pointer
*/
extern LbVariable* LbVariable_Create();
/**
* Dealloc a variable
* @param LbVariable pointer
*/
extern void LbVariable_Dealloc(LbVariable* ptr);
/**
* Clean a variable
* @param LbVariable pointer
*/
extern void LbVariable_Clean(LbVariable* ptr);
/**
* Copy a variable
* @param LbVariable pointer
*/
extern LbVariable* LbVariable_Copy(LbVariable *ptr);
/**
* Replace a variable
* @param LbVariable pointer
* @param LbVariable pointer
*/
extern void LbVariable_Replace(LbVariable *ptr1,LbVariable *ptr2);
/**
* Set string into variable
* @param LbVariable pointer
* @param cont char pointer (string)
*/
extern void LbVariable_SetString(LbVariable *ptr,const char* string);
/**
* Set number into variable
* @param LbVariable pointer
* @param LbFloat
*/
extern void LbVariable_SetNumber(LbVariable *ptr,LbFloat value);
/**
* Set function
* @param LbVariable pointer
* @param int
*/
extern void LbVariable_SetFunction(LbVariable *ptr,int value);
/**
* Set C function
* @param LbVariable pointer
* @param fuction pointer
*/
extern void LbVariable_SetCFunction(LbVariable *ptr,int (*cfun) (VMliteB*));
/**
* Operator -(void)
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_Cmd(LbVariable *ptr0,LbVariable *ptr1);
/**
* Operator +=
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_Add(LbVariable *ptr1,LbVariable *ptr2);
/**
* Operator -=
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_Sub(LbVariable *ptr1,LbVariable *ptr2);
/**
* Operator *=
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_Mul(LbVariable *ptr1,LbVariable *ptr2);
/**
* Operator /=
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_Div(LbVariable *ptr1,LbVariable *ptr2);
/**
* Operator ==
* @param LbVariable pointer
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_Eq(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2);
/**
* Operator >= (GT)
* @param LbVariable pointer
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_GtEq(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2);
/**
* Operator <= (LT)
* @param LbVariable pointer
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_LtEq(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2);
/**
* Operator > (GT)
* @param LbVariable pointer
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_Gt(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2);
/**
* Operator < (LT)
* @param LbVariable pointer
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_Lt(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2);
/**
* Operator and
* @param LbVariable pointer
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_And(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2);
/**
* Operator or
* @param LbVariable pointer
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_Or(LbVariable *ptr0,LbVariable *ptr1,LbVariable *ptr2);
/**
* Operator not
* @param LbVariable pointer
* @param LbVariable pointer
* @return uchar (get errors)
*/
extern uchar LbVariable_Not(LbVariable *ptr0,LbVariable *ptr1);
/**
* @}
*
*/


#endif