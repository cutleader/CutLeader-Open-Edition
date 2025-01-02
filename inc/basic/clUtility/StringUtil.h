#pragma once

#include "clUtilityCommon.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

// this class will perform some conversions about the string.
class ClUtility_Export StringUtil
{
public:
	// convert CString object to char*
	// notes: 
	//   1) for Unicode env, we need to convert.
	//   2) "pszData" will be allocated by the caller.
	static void UnicodeToAscii(const CString &csFilestr, char* pszData);

	// notes: allocate "wchars" and "schars" by the caller.
	static void WideCharToMulByte(WCHAR* wchars, CHAR* schars, int scharsLen);

	// notes: allocate "schars" by the caller.
	static CString MulByteToWideChar(char* schars);

	// convert utf-8 string to unicode.
	static CString UTF8ToUnicode(const char* pszUtf8, UINT iLength);

	// convert unicode string to utf-8.
	// notes:
	//   1) "pszData" will be allocated by the caller.
	static void UnicodeToUtf8(CString strUnicode, char* pszData);

	// �����򵥵��ַ������Ӻ�����
	static CString ConnectStrings(CString str1, CString str2);
	static CString ConnectStrings(CString str1, CString str2, CString str3);
	static CString ConnectStrings(CString str1, CString str2, CString str3, CString str4);
	static CString ConnectStringAndInteger(CString str, int i); // ��"-"�����ַ�����������
	static CString ConnectStringAndInteger_2(CString str, int i); // �����ַ�����������

	// ����ֵת��Ϊ�ַ�����
	static CString Integer2String(unsigned int i);
	static CString Double2String(double dVal, CString strFormat);
	// ���ȡ��λ���֣����һ������0������2.1200��ת��Ϊ"2.12"��12.121314000�ᱻתΪ"12.1213"��12.10000000�ᱻתΪ"12.1"��
	static CString Double2String_2(double dVal);

	// ���շָ����ָ��ַ��������÷������ظ�ʽ���磺��12��23��34��.
	static std::vector<CString> SplitString(const CString& strContent, const CString& strSeperator);
};

END_CUTLEADER_NAMESPACE()
