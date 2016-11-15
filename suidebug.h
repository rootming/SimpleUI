#pragma once
#include <cstdio>
#include "config.h"

namespace sui {

#define BRED				"\E[41m"
#define BGREEN				"\E[42m"
#define BYELLOW				"\E[43m"
#define BBLUE				"\E[44m"
#define BWHITE				"\E[40m"



#ifdef SUI_DEBUG

#define SUI_DEBUG_ERROR(...)		fprintf(stderr, "%s[ERROR]%s%s[FileName]%s%s%s[Line]%s%d\n\t" ,BRED, BWHITE, BBLUE,	BWHITE, __FILE__, BBLUE, BWHITE,__LINE__) ;\
fprintf(stderr, __VA_ARGS__);

#define SUI_DEBUG_WORRY(...)		fprintf(stderr, "%s[WORRY]%s%s[FileName]%s%s%s[Line]%s%d\n\t" ,BYELLOW, BWHITE, BBLUE, BWHITE, __FILE__, BBLUE, BWHITE,__LINE__) ;\
fprintf(stderr, __VA_ARGS__);

#define SUI_DEBUG_INFO(...)		fprintf(stdout, "%s[INFO]%s%s[FileName]%s%s%s[Line]%s%d\n\t" ,BGREEN, BWHITE, BBLUE, BWHITE, __FILE__, BBLUE, BWHITE,__LINE__) ;\
fprintf(stdout, __VA_ARGS__);

#else

#define SUIDEBUG_ERROR(...)
#define SUIDEBUG_WORRY(...)
#define SUIDEBUG_INFO(...)

#endif

class SUIDebug
{
public:
    SUIDebug();
};

}

