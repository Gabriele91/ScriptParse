#include "LbVariable.h"

//static variable
static LbVariable _InvalidVariable;
//get invalid variable
LbVariable& LbVariable::GetInvalidVariable(){ 
	_InvalidVariable.type=INVALID; 
	return _InvalidVariable; 
}

/**
* Constructors
* @param const cppstring&
* @param values
*/
LbVariable::LbVariable():name(""),type(NUMBER){	
	this->var.number=0;
}
LbVariable::LbVariable(const cppstring &name,float number):name(name),type(NUMBER){
	this->var.number=number;
}
LbVariable::LbVariable(const cppstring &name,const cppstring &str):name(name),type(STRING){
	this->string=str;
}
LbVariable::LbVariable(const cppstring &name,const char* str):name(name),type(STRING){
	this->string=str;
}
LbVariable::LbVariable(const cppstring &name,int function):name(name),type(FUNCTION){
	this->var.function=function;
}
LbVariable::LbVariable(const cppstring &name,LbCfunction cfunction):name(name),type(CFUNCTION){
	this->var.cfunction=cfunction;
}
LbVariable::LbVariable(const cppstring &name,void *object):name(name),type(OBJECT){
	this->var.object=object;
}

/**
* Ptint a variable
*/
void LbVariable::Print() const{

}
/**
* Clean a variable
*/
void LbVariable::Clear(){
	type=NUMBER;
	var.number=0.0;
	name.clear();
	string.clear();
}
/**
* Replace a variable
* @param LbVariable reference
*/
void LbVariable::Replace(const LbVariable &ptr){ 	
	type=ptr.type;
	var=ptr.var;
	name=ptr.name;
	string=ptr.string;
}
void LbVariable::ReplaceValue(const LbVariable &ptr){ 	
	type=ptr.type;
	var=ptr.var;
	string=ptr.string;
}
/**
* Set name variable
* @param const cppstring&
*/
void LbVariable::SetName(const cppstring& str){ 
	name=str;
}
/**
* Set string into variable
* @param const cppstring&
*/
void LbVariable::SetString(const cppstring& str){ 
	string=str;
	type=STRING;
}
/**
* Set number into variable
* @param LbFloat
*/
void LbVariable::SetNumber(LbFloat value){
	var.number=value;
	type=NUMBER;
}
/**
* Set function
* @param int
*/
void LbVariable::SetFunction(int value){ 
	var.function=value;
}
/**
* Set C function
* @param LbCfunction
*/
void LbVariable::SetCFunction(LbCfunction cfun){
	var.cfunction=cfun;
}
/**
* Operator -(void)
* @return bool (get errors)
*/
bool LbVariable::Cmd(){
	if(IsNumber()){
		var.number*=-1;
		return true;
	}
	return false;
}
/**
* Operator +=
* @param LbVariable reference
* @return bool (get errors)
*/
bool LbVariable::Add(const LbVariable &ptr){
	if(IsNumber() && ptr.IsNumber()){
		var.number+=ptr.var.number;
		return true;
	}
	else if(IsString() && ptr.IsString()){
		string+=ptr.string;
		return true;
	}
	else if(IsString() && ptr.IsNumber()){
		string+=String::ToString(ptr.var.number);
		return true;
	}
	else if(IsNumber() && ptr.IsString()){
		string=String::ToString(var.number)+ptr.string;
		type=STRING;
		return true;
	}
	return false;
}
/**
* Operator -=
* @param LbVariable reference
* @return bool (get errors)
*/
bool LbVariable::Sub(const LbVariable &ptr){	
	if(IsNumber() && ptr.IsNumber()){
		var.number-=ptr.var.number;
		return true;
	} 
	return false;
}
/**
* Operator *=
* @param LbVariable reference
* @return bool LbVariable::(get errors)
*/
bool LbVariable::Mul(const LbVariable &ptr){	
	if(IsNumber() && ptr.IsNumber()){
		var.number*=ptr.var.number;
		return true;
	} 
	return false;
}
/**
* Operator /=
* @param LbVariable reference
* @return bool LbVariable::(get errors)
*/
bool LbVariable::Div(const LbVariable &ptr){	
	if(IsNumber() && ptr.IsNumber()){
		var.number/=ptr.var.number;
		return true;
	} 
	return false;
}
/**
* Operator ==
* @param LbVariable reference
* @param LbVariable reference
* @return bool LbVariable::(get errors)
*/
bool LbVariable::Eq(const LbVariable &ptr0,LbVariable &ptr1) const{ 
	ptr1.Clear();
	ptr1.type=NUMBER;
	
	if(type!=ptr0.type)
		ptr1.var.number=0;
	else
	if(IsString()&&ptr0.IsString())
		ptr1.var.number=string==ptr0.string;
	else
	if(IsNumber()&&ptr0.IsNumber())
		ptr1.var.number=var.number==ptr0.var.number;
	else
	if(IsFunction()&&ptr0.IsFunction())
		ptr1.var.number=var.function==ptr0.var.function;
	else
	if(IsCFunction()&&ptr0.IsCFunction())
		ptr1.var.number=var.cfunction==ptr0.var.cfunction;
	else
	if(IsObject()&&ptr0.IsObject())
		ptr1.var.number=var.object==ptr0.var.object;
	
	return true;
}
/**
* Operator !=
* @param LbVariable reference
* @param LbVariable reference
* @return bool LbVariable::(get errors)
*/
bool LbVariable::NotEq(const LbVariable &ptr0,LbVariable &ptr1) const{
	ptr1.Clear();
	ptr1.type=NUMBER;
	if(type!=ptr0.type)
		ptr1.var.number=1;
	else
	if(IsString()&&ptr0.IsString())
		ptr1.var.number=string!=ptr0.string;
	else
	if(IsNumber()&&ptr0.IsNumber())
		ptr1.var.number=var.number!=ptr0.var.number;
	else
	if(IsFunction()&&ptr0.IsFunction())
		ptr1.var.number=var.function!=ptr0.var.function;
	else
	if(IsCFunction()&&ptr0.IsCFunction())
		ptr1.var.number=var.cfunction!=ptr0.var.cfunction;
	else
	if(IsObject()&&ptr0.IsObject())
		ptr1.var.number=var.object!=ptr0.var.object;
	
	return true;
}
/**
* Operator >= (GT)
* @param LbVariable reference
* @param LbVariable reference
* @return bool (get errors)
*/
bool LbVariable::GtEq(const LbVariable &ptr0,LbVariable &ptr1) const{
	ptr1.Clear();
	ptr1.type=NUMBER;
	if(IsNumber()&&ptr0.IsNumber()){
		ptr1.var.number=var.number>=ptr0.var.number;
		return true;
	}
	return false;
}
/**
* Operator <= (LT)
* @param LbVariable reference
* @param LbVariable reference
* @return bool (get errors)
*/
bool LbVariable::LtEq(const LbVariable &ptr0,LbVariable &ptr1) const{
	ptr1.Clear();
	ptr1.type=NUMBER;
	if(IsNumber()&&ptr0.IsNumber()){
		ptr1.var.number=var.number<=ptr0.var.number;
		return true;
	}
	return false;
}
/**
* Operator > (GT)
* @param LbVariable reference
* @param LbVariable reference
* @return bool (get errors)
*/
bool LbVariable::Gt(const LbVariable &ptr0,LbVariable &ptr1) const{
	ptr1.Clear();
	ptr1.type=NUMBER;
	if(IsNumber()&&ptr0.IsNumber()){
		ptr1.var.number=var.number>ptr0.var.number;
		return true;
	}
	return false;
}
/**
* Operator < (LT)
* @param LbVariable reference
* @param LbVariable reference
* @return bool (get errors)
*/
bool LbVariable::Lt(const LbVariable &ptr0,LbVariable &ptr1) const{
	ptr1.Clear();
	ptr1.type=NUMBER;
	if(IsNumber()&&ptr0.IsNumber()){
		ptr1.var.number=var.number<ptr0.var.number;
		return true;
	}
	return false;
}
/**
* Operator and
* @param LbVariable reference
* @param LbVariable reference
* @return bool (get errors)
*/
bool LbVariable::And(const LbVariable &ptr0,LbVariable &ptr1) const{
	ptr1.Clear();
	ptr1.type=NUMBER;
	if(IsNumber()&&ptr0.IsNumber())
		ptr1.var.number=var.number&&ptr0.var.number;
	else
		ptr1.var.number=true;
	return true;
}
/**
* Operator or
* @param LbVariable reference
* @param LbVariable reference
* @return uchar (get errors)
*/
bool LbVariable::Or(const LbVariable &ptr0,LbVariable &ptr1) const{
	ptr1.Clear();
	ptr1.type=NUMBER;
	if(IsNumber()&&ptr0.IsNumber())
		ptr1.var.number=var.number||ptr0.var.number;
	else
		ptr1.var.number=true;
	return true;
}
/**
* Operator not
* @return uchar (get errors)
*/
bool LbVariable::Not(LbVariable &ptr1) const{
	ptr1.Clear();
	ptr1.type=NUMBER;
	if(IsNumber())
		ptr1.var.number=!var.number;
	else
		ptr1.var.number=false;
	return true;
}