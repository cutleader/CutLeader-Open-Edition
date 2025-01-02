#pragma once

#include "clUtilityCommon.h"
#include <string>
#include <vector>
using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

// 为了方便在各种字符串类型之间做转换而使用的类。
// NPString可以接收双字节（wchar_t）或单字节（char）类型的字符串指针。双字节指针默认为Unicode类型。单字节指针默认为MultiByte类型。
// 另外提供接收单字节指针的构造函数，需要另外提供语言代码页。默认为ANSI类型。
// NPString提供常用的字符串操作界面，内部实现主要依赖std::wstring。
// NPString通过c_str()函数返回双字节指针，也可通过GetMultiByteBufferUTF8()函数返回UTF-8类型的单字节指针，或者GetMultiByteBufferANSI()函数返回ANSI单字节指针。
// 因为多了两种单字节指针缓存，NPString在存储耗费上要大于普通的std::wstring，使用时需注意。

// ！！注意！！ 所有非const函数都必须加上CalculateMultiByteBuffer()以刷新单字节指针缓存。
class ClUtility_Export NPString
{
public:
    enum EnProcessInputPointer
    {
        LEAVE_INPUT_POINTER = 0,
        TAKE_INPUT_POINTER
    };

public:
    NPString();
    NPString(const wchar_t* pStrW);
    NPString(const std::wstring& strW);

    // 接收单字节指针或者std::string。需要另外提供语言代码页以便转换为宽字节，默认为ANSI类型。
    // EnProcessInputPointer用于在使用完成后自动删除传入的字符串
    NPString(char* pStr, int codePage = 0, EnProcessInputPointer pip = LEAVE_INPUT_POINTER);
    NPString(const std::string& str, int codePage = 0);

// 各种操作符重载
public:
    NPString operator + (const NPString& _other) const;
    NPString& operator += (const NPString& _other);
    bool operator < (const NPString& _other) const { return getString() < _other.getString(); }
    bool operator == (const NPString& _other) const { return getString() == _other.getString(); }
    bool operator != (const NPString& _other) const { return getString() != _other.getString(); }
    NPString& operator << (const NPString& valString);
    NPString& operator << (int valInt);
    NPString& operator << (__int64 valInt64);
    NPString& operator << (double valDouble);

// 与std::wstring对应的各种字符串操作
public:
    static const int npos;
    static const NPString endl;
    bool empty() const { return getString().empty(); }
    int size() const { return (int)getString().size(); }
    int find(const NPString& _other, int pos) const { return (int)getString().find(_other.getString(), pos); }
    NPString substr(int pos, int n) const;

    NPString& assign(const NPString& _other);
    NPString& replace(int pos, int size, const NPString& newStr);
    void clear() { getStringFW().clear(); }

// 自有操作
public:
    void ReplaceSubString(const NPString& oldSubString, const NPString& newSubString);
    void RemoveQuotationMark();
    void RemoveFrontAndBackWhiteSpace();

	// 根据指定字符分割字符串。
	std::vector<std::wstring> SplitString(wchar_t delimiter, bool removeFrontAndBackWhitespace) const;
    std::vector<std::wstring> SplitString(const std::wstring& delimiter, bool removeFrontAndBackWhitespace) const;

    // 快速转换类型
    bool ToBoolValue() const;
    int ToIntValue() const;
    __int64 ToLongLongValue() const;
    double ToDoubleValue() const;

    static NPString FromBoolValue(bool boolValue);
    static NPString FromIntValue(int intValue);
    static NPString FromLongLongValue(__int64 llValue);
    static NPString FromDoubleValue(double doubleValue);

// 不同字符串的输出接口
public:
    const std::wstring& getString() const { return m_theString; }
    std::wstring& getStringFW() { return m_theString; }
    const wchar_t* c_str() const { return getString().c_str(); }
    const char* GetMultiByteBufferANSI() const;
    const char* GetMultiByteBufferUTF8() const;
    static char* CalculateMultiByteBuffer(int codePage, const NPString& theString);

private:
    void CalculateMultiByteBuffer();

private:
    std::wstring m_theString;
    std::string m_multiByteBufferANSI;
    std::string m_multiByteBufferUTF8;
};

END_CUTLEADER_NAMESPACE()