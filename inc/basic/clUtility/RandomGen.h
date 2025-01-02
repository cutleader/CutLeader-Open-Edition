#pragma once

#include "clUtilityCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// �̰߳�ȫ���������������ʹ�õĽ�����Ҫ����Initialize/UnInitialize������
class ClUtility_Export RandomGen
{
public:
	// ��ʼ������
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
	// generates a random number on [0, 2^64-1]-interval����������������ĺ������ǵ�����������ġ�
	static unsigned long long genrand64_int64(void);
};

END_CUTLEADER_NAMESPACE()
