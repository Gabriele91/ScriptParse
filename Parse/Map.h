#ifndef __MAP_H //__ (include <map> header)
#define __MAP_H

#if defined(__APPLE__) || defined(ANDROID)
# include <tr1/unordered_map>
#else
# include <unordered_map>
#endif

#if defined(NACL)
# define DUNORDERED_MAP    std::unordered_map
# define DUNORDERED_MULTIMAP    std::unordered_multimap
#else
# define DUNORDERED_MAP    std::tr1::unordered_map
# define DUNORDERED_MULTIMAP    std::tr1::unordered_multimap
#endif
/**
IF unordered_map and unordered_unordered_multimap not are supported
set define:
DUNORDERED_MULTIMAP std::map
DUNORDERED_MULTIMAP std::multimap
**/

#endif