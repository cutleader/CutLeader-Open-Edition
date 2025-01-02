#pragma once

#include "clNestUICommon.h"
#include "LineArc2DStaticControl.h"
#include "taoresource.h"
#include "NestPriority.h"
#include "CLDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(FolderNode)
DECLARE_CUTLEADER_CLASS(IMaterialSize)
DECLARE_CUTLEADER_CLASS(PartItem)
DECLARE_CUTLEADER_CLASS(NestPartList)
DECLARE_CUTLEADER_CLASS(NestMaterialList)
DECLARE_CUTLEADER_CLASS(NestJob)

BEGIN_CUTLEADER_NAMESPACE()

class ClNestUI_Export NestScheduleDlg : public CLDialog
{
	DECLARE_DYNAMIC(NestScheduleDlg)

public:
	NestScheduleDlg(CWnd* pParent = NULL);
	virtual ~NestScheduleDlg();

	enum { IDD = IDD_clNestUI_NestSchedule };

protected: // inherit from the super class.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected: // message mapping.
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );

	afx_msg void OnSelectCamParam();
	afx_msg void OnEditParam();

	afx_msg void OnAddPartsToNesting();
	afx_msg void OnSetNestInfo();
	afx_msg void OnRemoveFromNestList();
	afx_msg void OnNestPartListchanged(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnAddMat();
	afx_msg void OnDeleteMat();
	afx_msg void OnNestMatListchanged(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnExecuteNest();

	DECLARE_MESSAGE_MAP()

public:
	// client will call this to get the nest job.
	NestJobPtr GetNestJob() const { return m_pNestJob; }

	FolderNodePtr GetSaveFolder(CString& strJobName) const;
	BOOL IsOpenSheet() const { return m_bOpenSheet; }

	// 把文件对话框中选的dxfdwg文件加入排版。
	void AddDxfdwgToNest(const std::vector<CString>& dxfdwgFullPaths, const CString& strDataCenterPath);

private:
	/************************************************************************/
	// 零件相关。

	// 初始化“零件表格”的样式。
	void InitPartGridStyle();

	// 预览列表中选中的零件。
	void PreviewNestPart();

	// 检查零件是不是有和“参数配置”配套的工艺数据。
	// 注：
	//   1) 要是没有，创建一套新的工艺数据。
	//   2) 返回工艺数据ID。
	LONGLONG CheckPart(LONGLONG iParamConfigID, LONGLONG iPartID);

	// 释放“零件表格”中的数据。
	void RlsPartGridData();

	// 是否添加了这个零件。
	BOOL PartAdded(LONGLONG iPartID);

	// 从列表得到排版零件信息。
	NestPartListPtr GetNestPartList();

	// 有关标准零件。
	void CreateStdPart();
	void ImportRectPart();

	// 向列表中添加一行。
	void AddNestPartToGrid(const PartItem* pPartItem, const NestPriority& iNestPriority, int iCount, PartRotateType iRotAng);

	// 从数据库添加零件
	void AddPartFromDB();

	// 从dxf/dwg载入零件。
	void AddPartsFromDxfDwg();
	/************************************************************************/


	/************************************************************************/
	// 材料相关。

	// 初始化“材料表格”的样式。
	void InitMatGridStyle();

	// 预览列表中选中的板材。
	void PreviewNestMat();

	// 释放“材料表格”中的数据”。
	void RlsMatGridData();

	// 是否添加了这个零件
	BOOL MatAdded(LONGLONG iMaterialSizeID);

	// 从列表得到排版材料信息。
	NestMaterialListPtr GetNestMatList();

	// 添加默认的材料。
	void AddDflMat();

	// 向列表中添加一行。
	void AddNestMat(IMaterialSizePtr pMaterialSize);
	/************************************************************************/


private: // for controls
	// “参数配置”分组。
	SkinMFCButton m_btnSelectCamParam;
	SkinMFCButton m_btnEditCamParam;

	// “排版零件”分组。
	SkinGridListCtrlGroups m_nestPartList;
	CStatic m_partPreviewWnd;
	SkinMFCMenuButton m_addPartBtn;
	CMenu m_btnMenu;
	SkinMFCButton m_ctrlNestInfo;
	SkinMFCButton m_btnDelPrt;

	// “排版材料”分组。
	SkinGridListCtrlGroups m_nestMatList;
	LineArc2DStaticControl m_matPreviewWnd;
	SkinMFCButton m_btnAddMat;
	SkinMFCButton m_btnDelMat;

	// 排版按钮。
	SkinMFCButton m_ctrlExecuteNest;
	SkinMFCButton m_btnCancel;

private: // data member
	// about selected scheme info.
	LONGLONG m_iExpLibItemID;
	LONGLONG m_iParamConfigID;

	// if we did a nest, we should generate such a job.
	NestJobPtr m_pNestJob;

	// “待排版零件”列表是否初始化完。
	BOOL m_bFinishAddNestPart;

	BOOL m_bOpenSheet;
};

END_CUTLEADER_NAMESPACE()
