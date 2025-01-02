#pragma once

#include "clUtilityCommon.h"
#include "clUILibCommon.h"
#include "XGridProperty.h"
#include <vector>

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr<std::vector<CString>> StringListPtr;

// the basic class for the bars.
class ClUILib_Export XBarBase : public CDockablePane
{
public:
	XBarBase(void);
	virtual ~XBarBase(void);

public:
	virtual BOOL CanBeClosed() const { return FALSE; }
	virtual BOOL CanFloat() const { return FALSE; }

public:
	// only display or hide bar����������������������һЩ��������顣
	// ע��
	// 1) bDelay��Ϊtrueʱ��ܲ�������������壬��ͬʱ��ʾ������һ�����ʱ������ô����
	// 2) ����ʾ���ʱ�ú������ǻἤ�������ú���û���ṩ����ShowPane����bActivate�����Ŀ��ܣ�
	//    ����Ҫ����dock��һ�������е�һ�����ʱ������Ҫ����ShowPane(TRUE, FALSE, TRUE)��
	virtual void ShowBar(BOOL bShow, BOOL bDelay = FALSE);

protected:
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point) {} // ��Ӧ���������Ŀ���ǽ��ø������������Ҽ��˵���

	DECLARE_MESSAGE_MAP()

public:
	// get the relative view of this prop bar.
	// notes:
	//   1) actually, the relative view is the current active view.
	CWnd* GetRelativeView() const;

	// �õ����ID��
	int GetPanelID() const { return m_nID; }

protected:
	// ��������һ�������͵��������������������
	XGridProperty* AddDoublePropItem(CMFCPropertyGridProperty* pGroup, CString strName, double dVal, int iPrecision, BOOL bEnable = TRUE, BOOL bHighlight = FALSE);

	// ��������һ���ַ����͵��������������������
	XGridProperty* AddStringPropItem(CMFCPropertyGridProperty* pGroup, CString strName, CString strVal, BOOL bEnable = TRUE, BOOL bHighlight = FALSE);

	// ��������һ����Ͽ����͵��������������������
	XGridProperty* AddComboPropItem(CMFCPropertyGridProperty* pGroup, CString strName, CString strVal, StringListPtr pStrValList, BOOL bHighlight = FALSE);
	// ������/���á���Ͽ�
	XGridProperty* AddComboPropItemA(CMFCPropertyGridProperty* pGroup, CString strName, BOOL bVal, BOOL bHighlight = FALSE);

	// ��������һ����ѡ�����͵��������������������
	XGridProperty* AddCheckPropItem(CMFCPropertyGridProperty* pGroup, CString strName, BOOL bVal, DWORD_PTR dwData = 0, BOOL bHighlight = FALSE);

	// ��������һ��ָ�����͵��������������������
	XGridProperty* AddPropItem(CMFCPropertyGridProperty* pGroup, BASIC_DATA_TYPE iDataType, int iPrecision, const CString& strName,
		const COleVariant& var, BOOL bEnable = TRUE, BOOL bHighlight = FALSE);

protected:
	// the main window of the app.
	CWnd* m_pMainWnd;
};

END_CUTLEADER_NAMESPACE()
