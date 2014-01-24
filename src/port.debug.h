#ifndef CFG_DEBUG_H_INCLUDE

#define CFG_DEBUG_H_INCLUDE

#include <iostream>
#include <cassert>

#ifdef TRACEON

#define BUG(x) std::cout<<"[DEBUG]"<<#x"=["<<x<<"]"<<"\n"
#define BUGX(x) x
#define TRACE(ARG) std::cout<<#ARG<<"\n";ARG
#define CODE_TRACE std::cout<<"[TRACE]"<<__FILE__<<"("<<__LINE__<<")";
//判断参数是否符合要求
#define REQUIRE(x) assert(x)
//判断结果是否符合要求
#define ASSURE(x) assert(x)

#else
#define BUG(x)
#define BUGX(x)
#define TRACE(ARG)
#define REQUIRE(x)
#define ASSURE(x)
#define CODE_TRACE

#endif

#endif



