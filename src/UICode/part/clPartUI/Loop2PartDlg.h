#pragma once

#include "clPartUICommon.h"
#include "CGridListCtrlGroups.h"
#include "taoresource.h"
#include "LineArc2DStaticControl.h"
#include "CLDialog.h"
#include "SkinManager.h"

DECLARE_CUTLEADER_CLASS(PatternLoopList)
DECLARE_CUTLEADER_CLASS(PartCadData)
DECLARE_CUTLEADER_CLASS(PartsFromAsmDxf)
DECLARE_CUTLEADER_CLASS(LoopTopologyItemList)
DECLARE_CUTLEADER_CLASS(TextStructList)

BEGIN_CUTLEADER_NAMESPACE()

// 这个对话框从一些来自dxf/dwg的回路中识别出零件的拓扑关系，并且创建出这些零件，还会考虑合并相同零件（几何数据相同）等逻辑。
// 注：
//  1）这个类不保证返回的零件都有封闭的外边框，比如作为一个零件整体导入（不拆分）时。
class ClPartUI_Export Loop2PartDlg : public CLDialog
{
	DECLARE_DYNCREATE(Loop2PartDlg)

public:
	Loop2PartDlg();
	~Loop2PartDlg(void);

	enum { IDD = IDD_clPartUI_Loop2Part };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectAll();
	afx_msg void OnRecognizeSamePart();
	afx_msg void OnImportAsSinglePart();

	afx_msg void OnAccept();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	void Init(PartCadDataPtr pPartCadData, CString strDxfDwgPath, LONGLONG iParamConfigID);
	PartsFromAsmDxfPtr Get_PartsFromAsmDxf() const { return m_pPartsFromAsmDxf; }

private:
	// 初始化表格。
	void InitGrid();

private: // controls
	CString m_strMsg_1;

	// 识别出的零件列表。
	CGridListCtrlGroups m_partGrid;

	BOOL m_bSelectAll; // 是否全选。
	BOOL m_bRecognizeSamePart; // 是否识别相同的零件
	BOOL m_bImportAsSinglePart; // 是否按照一个整体导入

	// 预览窗口。
	LineArc2DStaticControl m_previewWnd;

	// use this brush to paint the background.
	CBrush m_brush;

	SkinMFCButton m_btnOK;
	SkinMFCButton m_btnCancel;

private: // 数据。
	// 调用方传入的数据。
	PartCadDataPtr m_pPartCadData;
	CString m_strDxfDwgPath;
	LONGLONG m_iParamConfigID;

	// 拆分出的零件，对话框初始化时会设置这个变量，它是由传入的cad数据进行深度克隆后生成的。
	std::vector<std::tuple<CString, PatternLoopListPtr, LoopTopologyItemListPtr, TextStructListPtr>> m_partList;

	// 初始化的时候禁止一些消息。
	BOOL m_bEnableEvent;

	// 这个变量会返回给调用方，点了确定后才更新这个变量。
	PartsFromAsmDxfPtr m_pPartsFromAsmDxf;
};

END_CUTLEADER_NAMESPACE()
