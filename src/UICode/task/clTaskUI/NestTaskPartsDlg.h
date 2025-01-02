#pragma once

#include "ClTaskUICommon.h"
#include "taoResource.h"
#include "NumEdit.h"
#include "CGridListCtrlGroups.h"
#include "SkinManager.h"


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

// ������Ի�����������δ����������
class ClTaskUI_Export NestTaskPartsDlg : public CLDialog
{
	DECLARE_DYNAMIC(NestTaskPartsDlg)

public:
	NestTaskPartsDlg(CWnd* pParent = NULL);
	virtual ~NestTaskPartsDlg();

	enum { IDD = IDD_clTaskUI_NestTaskParts };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

public:
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    afx_msg void OnNestAllParts();
    afx_msg void OnNestSelectedParts();
	afx_msg void OnBatchSetting();

    afx_msg void OnNestOnCurrentSheet();
    afx_msg void OnNestOnNewMaterial();
    afx_msg void OnAddMat();
    afx_msg void OnDeleteMat();

    afx_msg void OnEditNestParam();
	afx_msg void OnStartNesting();
    afx_msg void OnCancelNesting();

	DECLARE_MESSAGE_MAP()

public:
	void Init(TaskItemPtr pTaskItem, TaskPartListPtr pSelectedTaskParts, SheetPtr pActiveSheet);
    TaskPartsNestResultPtr GetTaskPartsNestResult() const { return m_pTaskPartsNestResult; }

private:
    // ��ʼ�������ʽ
    void InitGridStyle();


    /************************************************************************/
    // ������
    
	// �õ��Ű��������
	NestPartListPtr GetNestPartList(LONGLONG iParamConfigID) const;

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
    /************************************************************************/


    /************************************************************************/
    // �������

	// �õ����ϡ�
	NestMaterialListPtr GetNestMatList(SheetMaterialListPtr pSheetMaterials) const;

    // �Ƿ������������
    BOOL MatAdded(LONGLONG iMatSizeID);

    // ���б������һ�С�
    void AddNestMaterial(const IMaterialSize* pMaterialSize);

    // �ͷŲ��ϱ���е�����
    void ReleaseData_inSheetsInTaskGrid();
    void ReleaseData_inNewMaterialGrid();
    /************************************************************************/


	void DisplayInvalidParts(const std::vector<CString>& openBoundaryPartNames);

private: // for controls
    // ������
    int m_iNestedPartType;
	CGridListCtrlGroups m_partGrid;

    // �������
    int m_iUseMaterialType;
	CGridListCtrlGroups m_grid_sheetsInTask;
	CGridListCtrlGroups m_grid_newMaterial;

    SkinMFCButton m_btnBatchSetPart;
    SkinMFCButton m_btnAddMaterial;
    SkinMFCButton m_btnDeleteMaterial;
    SkinMFCButton m_btnNestParam;
    SkinMFCButton m_btnStartNest;
    SkinMFCButton m_btnCancel;

    CBrush m_brush;

private: // data members
	// ����������������Ҫ��δ����������
	TaskItemPtr m_pTaskItem;

	// ѡ�е����
	TaskPartListPtr m_pSelectedTaskParts;

	// ��ǰ�İ��
	SheetPtr m_pActiveSheet;

    // �Ű���
    TaskPartsNestResultPtr m_pTaskPartsNestResult;
};

END_CUTLEADER_NAMESPACE()
