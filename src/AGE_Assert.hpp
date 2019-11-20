#ifndef AGE_ASSERT_H
#define AGE_ASSERT_H
#include <iostream>
#include <assert.h>





#define AGE_ASSERT(_EXPR)   assert(_EXPR)                               // You can override the default assert handler by editing imconfig.h

#ifdef AGE_PROFILER
	#define AGE_PRINTCONSLE(_M)  這裡輸入Profiler的函式
#endif // AGE_PROFILER
	#define AGE_PRINTCONSLE(_M)  std::cout<<_M<<std::endl;

#endif // !AGE_ASSERT_H
