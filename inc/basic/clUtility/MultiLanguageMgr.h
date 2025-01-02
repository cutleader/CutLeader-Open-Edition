#pragma once

#include "clUtilityCommon.h"
#include <map>
#include <set>


BEGIN_CUTLEADER_NAMESPACE()


// 语言类型定义
enum LANG_TYPE
{
	LANG_CHS	= 0, // 中文
	LANG_ENG	= 1, // 英文
	LANG_TR		= 2, // 土耳其文
	LANG_SPA	= 3, // 西班牙文
	LANG_UKR    = 4, // 乌克兰文
	LANG_POL    = 5, // 波兰文
	LANG_GER    = 6, // 德语
	LANG_FRA    = 7, // 法语
	LANG_RUS    = 8, // 俄语
	LANG_BGR    = 9, // 保加利亚
	LANG_ROK    = 10, // 韩语
	LANG_NED    = 11, // 荷兰语
	LANG_CRO    = 12, // 克罗地亚语
	LANG_ROU    = 13, // 罗马尼亚语
	LANG_POR    = 14, // 葡萄牙语
	LANG_JPN    = 15, // 日语
	LANG_SVK    = 16, // 斯洛伐克语
	LANG_XYL    = 17, // 匈牙利语
	LANG_ITA    = 18, // 意大利语
	LANG_VIE    = 19, // 越南语
	LANG_ZWF    = 20, // 中文繁体
};


// 翻译库
class TranslationLib
{
public:
    TranslationLib(const std::map<CString, std::map<LANG_TYPE, CString>>& translationLib_1, const std::map<int, std::map<LANG_TYPE, CString>>& translationLib_2);
    ~TranslationLib();

public:
    const std::map<CString, std::map<LANG_TYPE, CString>>& Get_TranslationLib_1() const { return m_translationLib_1; }
    const std::map<int, std::map<LANG_TYPE, CString>>& Get_TranslationLib_2() const { return m_translationLib_2; }

private:
    // 第一类字符串的多语言翻译。
    // 注：记录了基于英文字符串翻译的其他语言对应的字符串。 例如，<"Library", (中文类型，“库”) (土耳其类型，“???”) ... >。
    std::map<CString, std::map<LANG_TYPE, CString>> m_translationLib_1;

    // 第二类字符串的多语言翻译。
    // 注：记录了基于字符串ID翻译的其他语言对应的字符串，不包含英文的，英文的记录在clResource模块中，例如， <12345， (中文类型，“库”) (土耳其类型，“???”) ... >
    std::map<int, std::map<LANG_TYPE, CString>> m_translationLib_2;
};


// 对应当前语言的翻译数据
class TranslationData
{
public:
    TranslationData();
    ~TranslationData();

public:
    void Set_TranslationData_1(const std::map<CString, CString>& translationData_1) { m_translationData_1 = translationData_1; }
    const std::map<CString, CString>& Get_TranslationData_1() const { return m_translationData_1; }
    std::map<CString, CString>& Get_TranslationData_1_FW() { return m_translationData_1; }

    void Set_TranslationData_2(const std::map<int, CString>& translationData_2) { m_translationData_2 = translationData_2; }
    const std::map<int, CString>& Get_TranslationData_2() const { return m_translationData_2; }
    std::map<int, CString>& Get_TranslationData_2_FW() { return m_translationData_2; }

private:
    // 第一类字符串对应当前语言的翻译。
    // 注：格式是<英文字符串，当前语言对应的字符串>，例如 <"Library", “库”>，如果当前语言是英文，这个map中的值就放英文字符串本身。
    std::map<CString, CString> m_translationData_1;

    // 第二类字符串对应当前语言的翻译。
    // 注：<字符串ID，当前语言对应的字符串>，例如 <12345, "Library")。
    std::map<int, CString> m_translationData_2;
};


// 字符串翻译器。这个类从翻译库中加载所有的国际化数据，供使用者查询。详细信息请参考文档。
// 注：
//  1) 这个类内部要是需要弹出警告对话框，只能用英文。
//  2) 进程结束时，最好主动删除这块内存，否则可能报内存泄漏。
class ClUtility_Export MultiLanguageMgr
{
private:
	MultiLanguageMgr(void);

public:
	~MultiLanguageMgr(void);

public: // static functions.
	static MultiLanguageMgr* GetInstance();

    // 得到cutleader目前支持的语言类型，返回的格式<语言类型，语言的字符串ID>。
    static std::map<LANG_TYPE, int> GetAllSupportedLangs();

public:
	// 得到当前语言的翻译数据
    const TranslationData& GetTranslationData() const { return m_translationData; }

	// 在程序启动时，主动加载当前语言对应的翻译数据。
	// 注：
	//  1) 每个进程在调用这个类中的非静态函数前必须先调用这个函数。
	void LoadTranslationData(CString strTranslationLibPath, LANG_TYPE iLanguageType);

	// 翻译第一类字符串。
	CString TranslateString(const CString& stringWillBeTranslated);

	// 翻译第二类字符串。
	CString TranslateString(int iStringID);

    // 翻译该对话框下的静态字符串
    // 注：
    //  1) 原则上只要翻译对话框标题、label、group、check、radio、button上面的静态字符串，不过这里遍历所有子控件进行翻译，
    //     比如编辑框在这个时候还没有填值，所以也不会被翻译。
    void TranslateDialog(CWnd* pWnd);

	// 根据资源ID创建一个菜单，目前只翻译了第一级菜单。
	// 注：
	//  1) 调用前确保有这个菜单资源，这个函数没有检查异常情况。
	HMENU CreateTranslatedMenu(UINT nIDResource);

	// 翻译菜单，目前只翻译了第一级菜单。
	void TranslateMenu(CMenu* pMenu);

	// 更新第一类和第二类的翻译到Excel，更新完成后这个函数需要重新加载当前语言对应的翻译数据。
	// 注：
	//  1) m_translationData_1_modified和m_translationData_2_modified是被修改过的翻译数据。
	bool UpdateTranslationData(const std::map<CString, CString>& translationData_1_modified, const std::map<int, CString>& translationData_2_modified, LANG_TYPE iLanguageType);

	// 往翻译库插入一个条目，调用完自己维护一下m_translationData_1和m_translationData_2。
	bool InsertRecord2TranslationLib(CString strKey); // 第一类字符串
	bool InsertRecord2TranslationLib(int iStringID); // 第二类字符串

private:
	// 加载翻译库。
	static TranslationLib LoadTranslationLib(CString strTranslationLibPath);

private: // static data.
	static MultiLanguageMgr* pMultiLanguageMgr;

private:
	// 这里存下翻译库的路径，以后就不用每次传参数了。
	CString m_strTranslationLibPath;

    // 对应当前语言的翻译数据
    TranslationData m_translationData;
};

END_CUTLEADER_NAMESPACE()
