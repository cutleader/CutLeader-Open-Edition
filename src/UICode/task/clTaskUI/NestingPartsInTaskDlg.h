#pragma once

#include "ClTaskUICommon.h"
#include "taoResource.h"
#include "NumEdit.h"
#include "CGridListCtrlGroups.h"
#include "CLResizableDialog.h"


DECLARE_CUTLEADER_CLASS(TaskItem)
DECLARE_CUTLEADER_CLASS(Sheet)
DECLARE_CUTLEADER_CLASS(TaskPartList)
DECLARE_CUTLEADER_CLASS(NestPartList)
DECLARE_CUTLEADER_CLASS(NestMaterialList)
DECLARE_CUTLEADER_CLASS(SheetMaterialList)
DECLARE_CUTLEADER_CLASS(NestJob)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(TaskPartsNestResult)


BEGIN_CUTLEADER_NAMESPACE()

// ������Ի���ȷ��Ҫ����������Щ�����
class ClTaskUI_Export NestingPartsInTaskDlg : public CLResizableDialog
{
	DECLARE_DYNAMIC(NestingPartsInTaskDlg)

public:
	NestingPartsInTaskDlg(CWnd* pParent = NULL);
	virtual ~NestingPartsInTaskDlg();

	enum { IDD = IDD_clTaskUI_NestingPartsInTask };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public:
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg void OnNestAllParts();
    afx_msg void OnNestSelectedParts();

	DECLARE_MESSAGE_MAP()

public:
	void Init(TaskItemPtr pTaskItem, TaskPartListPtr pSelectedTaskParts);

    // �õ��Ű��������
    NestPartListPtr GetNestPartList(LONGLONG iParamConfigID) const;

private:
    // ��ʼ�������ʽ
    void InitGridStyle();

    // �������ǲ����к͡��������á����׵Ĺ������ݡ�
    // ע��
    //   1) Ҫ��û�У�����һ���µĹ������ݡ�
    //   2) ���ع�������ID��
    LONGLONG CheckPart(LONGLONG iParamConfigID, LONGLONG iPartID) const;

    // ���������
    void FillPartGrid_withAllPartsInTask();
    void FillPartGrid_withSelectedPartsInTask(); // ����֮ǰ��Ҫȷ��ѡ�е��������δ��������

    // �ͷ��������е�����
    void ReleaseData_inPartGrid();

private: // for controls
    // ������
    int m_iNestedPartType;
	CGridListCtrlGroups m_partGrid;

    CBrush m_brush;

private: // data members
	// ����������������Ҫ��δ����������
	TaskItemPtr m_pTaskItem;

	// ѡ�е����
	TaskPartListPtr m_pSelectedTaskParts;
};

END_CUTLEADER_NAMESPACE()
