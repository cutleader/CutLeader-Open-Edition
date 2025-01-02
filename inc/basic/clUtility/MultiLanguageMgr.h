#pragma once

#include "clUtilityCommon.h"
#include <map>
#include <set>


BEGIN_CUTLEADER_NAMESPACE()


// �������Ͷ���
enum LANG_TYPE
{
	LANG_CHS	= 0, // ����
	LANG_ENG	= 1, // Ӣ��
	LANG_TR		= 2, // ��������
	LANG_SPA	= 3, // ��������
	LANG_UKR    = 4, // �ڿ�����
	LANG_POL    = 5, // ������
	LANG_GER    = 6, // ����
	LANG_FRA    = 7, // ����
	LANG_RUS    = 8, // ����
	LANG_BGR    = 9, // ��������
	LANG_ROK    = 10, // ����
	LANG_NED    = 11, // ������
	LANG_CRO    = 12, // ���޵�����
	LANG_ROU    = 13, // ����������
	LANG_POR    = 14, // ��������
	LANG_JPN    = 15, // ����
	LANG_SVK    = 16, // ˹�工����
	LANG_XYL    = 17, // ��������
	LANG_ITA    = 18, // �������
	LANG_VIE    = 19, // Խ����
	LANG_ZWF    = 20, // ���ķ���
};


// �����
class TranslationLib
{
public:
    TranslationLib(const std::map<CString, std::map<LANG_TYPE, CString>>& translationLib_1, const std::map<int, std::map<LANG_TYPE, CString>>& translationLib_2);
    ~TranslationLib();

public:
    const std::map<CString, std::map<LANG_TYPE, CString>>& Get_TranslationLib_1() const { return m_translationLib_1; }
    const std::map<int, std::map<LANG_TYPE, CString>>& Get_TranslationLib_2() const { return m_translationLib_2; }

private:
    // ��һ���ַ����Ķ����Է��롣
    // ע����¼�˻���Ӣ���ַ���������������Զ�Ӧ���ַ����� ���磬<"Library", (�������ͣ����⡱) (���������ͣ���???��) ... >��
    std::map<CString, std::map<LANG_TYPE, CString>> m_translationLib_1;

    // �ڶ����ַ����Ķ����Է��롣
    // ע����¼�˻����ַ���ID������������Զ�Ӧ���ַ�����������Ӣ�ĵģ�Ӣ�ĵļ�¼��clResourceģ���У����磬 <12345�� (�������ͣ����⡱) (���������ͣ���???��) ... >
    std::map<int, std::map<LANG_TYPE, CString>> m_translationLib_2;
};


// ��Ӧ��ǰ���Եķ�������
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
    // ��һ���ַ�����Ӧ��ǰ���Եķ��롣
    // ע����ʽ��<Ӣ���ַ�������ǰ���Զ�Ӧ���ַ���>������ <"Library", ���⡱>�������ǰ������Ӣ�ģ����map�е�ֵ�ͷ�Ӣ���ַ�������
    std::map<CString, CString> m_translationData_1;

    // �ڶ����ַ�����Ӧ��ǰ���Եķ��롣
    // ע��<�ַ���ID����ǰ���Զ�Ӧ���ַ���>������ <12345, "Library")��
    std::map<int, CString> m_translationData_2;
};


// �ַ����������������ӷ�����м������еĹ��ʻ����ݣ���ʹ���߲�ѯ����ϸ��Ϣ��ο��ĵ���
// ע��
//  1) ������ڲ�Ҫ����Ҫ��������Ի���ֻ����Ӣ�ġ�
//  2) ���̽���ʱ���������ɾ������ڴ棬������ܱ��ڴ�й©��
class ClUtility_Export MultiLanguageMgr
{
private:
	MultiLanguageMgr(void);

public:
	~MultiLanguageMgr(void);

public: // static functions.
	static MultiLanguageMgr* GetInstance();

    // �õ�cutleaderĿǰ֧�ֵ��������ͣ����صĸ�ʽ<�������ͣ����Ե��ַ���ID>��
    static std::map<LANG_TYPE, int> GetAllSupportedLangs();

public:
	// �õ���ǰ���Եķ�������
    const TranslationData& GetTranslationData() const { return m_translationData; }

	// �ڳ�������ʱ���������ص�ǰ���Զ�Ӧ�ķ������ݡ�
	// ע��
	//  1) ÿ�������ڵ���������еķǾ�̬����ǰ�����ȵ������������
	void LoadTranslationData(CString strTranslationLibPath, LANG_TYPE iLanguageType);

	// �����һ���ַ�����
	CString TranslateString(const CString& stringWillBeTranslated);

	// ����ڶ����ַ�����
	CString TranslateString(int iStringID);

    // ����öԻ����µľ�̬�ַ���
    // ע��
    //  1) ԭ����ֻҪ����Ի�����⡢label��group��check��radio��button����ľ�̬�ַ���������������������ӿؼ����з��룬
    //     ����༭�������ʱ��û����ֵ������Ҳ���ᱻ���롣
    void TranslateDialog(CWnd* pWnd);

	// ������ԴID����һ���˵���Ŀǰֻ�����˵�һ���˵���
	// ע��
	//  1) ����ǰȷ��������˵���Դ���������û�м���쳣�����
	HMENU CreateTranslatedMenu(UINT nIDResource);

	// ����˵���Ŀǰֻ�����˵�һ���˵���
	void TranslateMenu(CMenu* pMenu);

	// ���µ�һ��͵ڶ���ķ��뵽Excel��������ɺ����������Ҫ���¼��ص�ǰ���Զ�Ӧ�ķ������ݡ�
	// ע��
	//  1) m_translationData_1_modified��m_translationData_2_modified�Ǳ��޸Ĺ��ķ������ݡ�
	bool UpdateTranslationData(const std::map<CString, CString>& translationData_1_modified, const std::map<int, CString>& translationData_2_modified, LANG_TYPE iLanguageType);

	// ����������һ����Ŀ���������Լ�ά��һ��m_translationData_1��m_translationData_2��
	bool InsertRecord2TranslationLib(CString strKey); // ��һ���ַ���
	bool InsertRecord2TranslationLib(int iStringID); // �ڶ����ַ���

private:
	// ���ط���⡣
	static TranslationLib LoadTranslationLib(CString strTranslationLibPath);

private: // static data.
	static MultiLanguageMgr* pMultiLanguageMgr;

private:
	// ������·�����·�����Ժ�Ͳ���ÿ�δ������ˡ�
	CString m_strTranslationLibPath;

    // ��Ӧ��ǰ���Եķ�������
    TranslationData m_translationData;
};

END_CUTLEADER_NAMESPACE()
