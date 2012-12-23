#ifndef UTILITY_STRING_H
#define UTILITY_STRING_H

#include <string>
#include <sstream>

namespace String{
	template<class T> std::string ToString(T dato){
		std::stringstream br;
		br << dato;
		return br.str();
	}
}

#endif