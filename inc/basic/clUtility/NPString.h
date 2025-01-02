#pragma once

#include "clUtilityCommon.h"
#include <string>
#include <vector>
using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

// Ϊ�˷����ڸ����ַ�������֮����ת����ʹ�õ��ࡣ
// NPString���Խ���˫�ֽڣ�wchar_t�����ֽڣ�char�����͵��ַ���ָ�롣˫�ֽ�ָ��Ĭ��ΪUnicode���͡����ֽ�ָ��Ĭ��ΪMultiByte���͡�
// �����ṩ���յ��ֽ�ָ��Ĺ��캯������Ҫ�����ṩ���Դ���ҳ��Ĭ��ΪANSI���͡�
// NPString�ṩ���õ��ַ����������棬�ڲ�ʵ����Ҫ����std::wstring��
// NPStringͨ��c_str()��������˫�ֽ�ָ�룬Ҳ��ͨ��GetMultiByteBufferUTF8()��������UTF-8���͵ĵ��ֽ�ָ�룬����GetMultiByteBufferANSI()��������ANSI���ֽ�ָ�롣
// ��Ϊ�������ֵ��ֽ�ָ�뻺�棬NPString�ڴ洢�ķ���Ҫ������ͨ��std::wstring��ʹ��ʱ��ע�⡣

// ����ע�⣡�� ���з�const�������������CalculateMultiByteBuffer()��ˢ�µ��ֽ�ָ�뻺�档
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

    // ���յ��ֽ�ָ�����std::string����Ҫ�����ṩ���Դ���ҳ�Ա�ת��Ϊ���ֽڣ�Ĭ��ΪANSI���͡�
    // EnProcessInputPointer������ʹ����ɺ��Զ�ɾ��������ַ���
    NPString(char* pStr, int codePage = 0, EnProcessInputPointer pip = LEAVE_INPUT_POINTER);
    NPString(const std::string& str, int codePage = 0);

// ���ֲ���������
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

// ��std::wstring��Ӧ�ĸ����ַ�������
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

// ���в���
public:
    void ReplaceSubString(const NPString& oldSubString, const NPString& newSubString);
    void RemoveQuotationMark();
    void RemoveFrontAndBackWhiteSpace();

	// ����ָ���ַ��ָ��ַ�����
	std::vector<std::wstring> SplitString(wchar_t delimiter, bool removeFrontAndBackWhitespace) const;
    std::vector<std::wstring> SplitString(const std::wstring& delimiter, bool removeFrontAndBackWhitespace) const;

    // ����ת������
    bool ToBoolValue() const;
    int ToIntValue() const;
    __int64 ToLongLongValue() const;
    double ToDoubleValue() const;

    static NPString FromBoolValue(bool boolValue);
    static NPString FromIntValue(int intValue);
    static NPString FromLongLongValue(__int64 llValue);
    static NPString FromDoubleValue(double doubleValue);

// ��ͬ�ַ���������ӿ�
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