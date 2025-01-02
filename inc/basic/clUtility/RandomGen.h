#pragma once

#include "clUtilityCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// 线程安全的随机数生成器。使用的进程需要调用Initialize/UnInitialize函数。
class ClUtility_Export RandomGen
{
public:
	// 初始化函数
	static void Initialize(unsigned long long seed);
	static void UnInitialize();

	/* generates a random number on [0, 2^63-1]-interval */
	static unsigned long long genrand64_int63(void);

	/* generates a random number on [0,1]-real-interval */
	static double genrand64_real1(void);

	/* generates a random number on [0,1)-real-interval */
	static double genrand64_real2(void);

	/* generates a random number on (0,1)-real-interval */
	static double genrand64_real3(void);

private:
	// generates a random number on [0, 2^64-1]-interval。上面生成随机数的函数都是调用这个函数的。
	static unsigned long long genrand64_int64(void);
};

END_CUTLEADER_NAMESPACE()
