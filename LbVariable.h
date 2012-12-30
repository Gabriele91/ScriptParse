#ifndef LBVARIABLE_H
#define LBVARIABLE_H
#include "LbType.h"
/**
* @class LbVariable
*
* @brief A LiteB Variable.
*
* @author Gabriele Di Bari
*/
class LbVariable{

		enum ValueType{
			INVALID,
			NUMBER,
			STRING,
			FUNCTION,
			OBJECT,
			CFUNCTION
		};

		union Value{
			ushort function;       ///< if is script function
			LbFloat number;        ///< if is a number
			LbCfunction cfunction; ///<if is a CFunction
			void* object;          ///< if is a object
		}var;
    
		cppstring string; ///< if is a string
		cppstring name;   ///< variable name
		ValueType type;   ///< type variable

	public:
	
	/* Query */
	static LbVariable& GetInvalidVariable();
	DINLINE bool IsInvalid() const { return type==INVALID; }
	DINLINE bool IsString() const { return type==STRING; }
	DINLINE bool IsFunction() const { return type==FUNCTION; }
	DINLINE bool IsCFunction() const { return type==CFUNCTION; }
	DINLINE bool IsNumber() const { return type==NUMBER; }
	DINLINE bool IsObject() const { return type==OBJECT; }
	DINLINE bool IsZero() const { return type==NUMBER && var.number==0; }
	DINLINE const cppstring& GetName(){ return name; }
	DINLINE const cppstring& GetString(){ return string; }
	DINLINE const LbFloat GetNumber(){ return var.number; }
	DINLINE const ushort GetFunction(){ return var.function; }
	DINLINE const LbCfunction GetCFunction(){ return var.cfunction; }
	DINLINE const void* GetObject(){ return var.object; }

	/**
	* Constructors
	* @param const cppstring&
	* @param values
	*/
	LbVariable();
	LbVariable(const cppstring &name,float number);
	LbVariable(const cppstring &name,const cppstring &function);
	LbVariable(const cppstring &name,const char* str);
	LbVariable(const cppstring &name,int function);
	LbVariable(const cppstring &name,LbCfunction cfunction);
	LbVariable(const cppstring &name,void *object);
	/**
	* Print a variable
	*/
	void Print() const;
	/**
	* Clear a variable
	*/
	void Clear();
	/**
	* Replace a variable
	* @param LbVariable reference
	*/
	void Replace(const LbVariable &ptr);
	void ReplaceValue(const LbVariable &ptr);
	/**
	* Set name variable
	* @param const cppstring&
	*/
	void SetName(const cppstring& str);
	/**
	* Set string into variable
	* @param const cppstring&
	*/
	void SetString(const cppstring& str);
	/**
	* Set number into variable
	* @param LbFloat
	*/
	void SetNumber(LbFloat value);
	/**
	* Set function
	* @param int
	*/
	void SetFunction(int value);
	/**
	* Set C function
	* @param LbCfunction
	*/
	void SetCFunction(LbCfunction cfun);
	/**
	* Operator -(void)
	* @return bool (get errors)
	*/
	bool Cmd();
	/**
	* Operator +=
	* @param LbVariable reference
	* @return bool (get errors)
	*/
	bool Add(const LbVariable &ptr);
	/**
	* Operator -=
	* @param LbVariable reference
	* @return bool (get errors)
	*/
	bool Sub(const LbVariable &ptr);
	/**
	* Operator *=
	* @param LbVariable reference
	* @return bool (get errors)
	*/
	bool Mul(const LbVariable &ptr);
	/**
	* Operator /=
	* @param LbVariable reference
	* @return bool (get errors)
	*/
	bool Div(const LbVariable &ptr);
	/**
	* Operator ==
	* @param LbVariable reference
	* @param LbVariable reference
	* @return bool (get errors)
	*/
	bool Eq(const LbVariable &ptr0,LbVariable &ptr1) const;
	/**
	* Operator !=
	* @param LbVariable reference
	* @param LbVariable reference
	* @return bool (get errors)
	*/
	bool NotEq(const LbVariable &ptr0,LbVariable &ptr1) const;
	/**
	* Operator >= (GT)
	* @param LbVariable reference
	* @param LbVariable reference
	* @return bool (get errors)
	*/
	bool GtEq(const LbVariable &ptr0,LbVariable &ptr1) const;
	/**
	* Operator <= (LT)
	* @param LbVariable reference
	* @param LbVariable reference
	* @return uchar (get errors)
	*/
	bool LtEq(const LbVariable &ptr0,LbVariable &ptr1) const;
	/**
	* Operator > (GT)
	* @param LbVariable reference
	* @param LbVariable reference
	* @return bool  (get errors)
	*/
	bool Gt(const LbVariable &ptr0,LbVariable &ptr1) const;
	/**
	* Operator < (LT)
	* @param LbVariable reference
	* @param LbVariable reference
	* @return bool  (get errors)
	*/
	bool Lt(const LbVariable &ptr0,LbVariable &ptr1) const;
	/**
	* Operator and
	* @param LbVariable reference
	* @param LbVariable reference
	* @return bool  (get errors)
	*/
	bool And(const LbVariable &ptr0,LbVariable &ptr1) const;
	/**
	* Operator or
	* @param LbVariable reference
	* @param LbVariable reference
	* @return bool (get errors)
	*/
	bool Or(const LbVariable &ptr0,LbVariable &ptr1) const;
	/**
	* Operator not
	* @param LbVariable reference
	* @return bool (get errors)
	*/
	bool Not(LbVariable &ptr1) const;

};
#endif