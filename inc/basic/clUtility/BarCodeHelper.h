#pragma once

#include "clUtilityCommon.h"
struct zint_symbol;

BEGIN_CUTLEADER_NAMESPACE()

// ����������ࡣ
class ClUtility_Export BarCodeHelper
{
public:
	// �������������һ���ļ���
	static bool DrawBarCode2File(BARCODE_TYPE iType, CString strContent, int iHeight, CString strFilePath);

private:
	static int EscapeCharProcess(zint_symbol *my_symbol, unsigned char input_string[], int length);
};

END_CUTLEADER_NAMESPACE()
