#include "UtilityString.h"
#include <stdlib.h>
double String::ToFloat(const std::string& str){
	return atof(str.c_str());
}
