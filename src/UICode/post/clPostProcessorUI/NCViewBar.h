#pragma once

#include "clPostProcessorUICommon.h"
#include "CGridListCtrlEx.h"
#include "XBarBase.h"
#include <vector>
#include <set>

DECLARE_CUTLEADER_CLASS(SequenceSimulateShapeList)
DECLARE_CUTLEADER_CLASS(NCCodeData)
DECLARE_CUTLEADER_CLASS(MacRouterEditor)
DECLARE_CUTLEADER_CLASS(LoopInstanceList)

BEGIN_CUTLEADER_NAMESPACE()

typedef boost::shared_ptr< std::vector<int> > IntListPtr;

// 定义一个不支持拷贝的list
class CGridListCtrlEx_cannotCopy : public CGridListCtrlEx
{
public:
	CGridListCtrlEx_cannotCopy() {}
	virtual ~CGridListCtrlEx_cannotCopy() {}

public:
	virtual void OnCopyToClipboard() {}
};

// use this pane to display the NC code.
class ClPostProcessorUI_Export NCViewBar : public XBarBase
{
private:
	NCViewBar();

public:
	virtual ~NCViewBar();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGrid_beginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnGrid_endLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnSaveNCCode();
	afx_msg void OnUpdateClickNCBtn(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

public:
	static NCViewBar* GetInstance(CWnd* pParent = NULL);

	void OnSaveNCCodeTo( CString strFilePath, CString strNCExt, BOOL bMustOverride);
	void InitBar(CString strSheetName, SequenceSimulateShapeListPtr pSeqSimShapeList, NCCodeDataPtr pNCCodeData, MacRouterEditorPtr pPostEditor, CString strNCPostfix,
        LoopInstanceListPtr pAllLoopInstances, const std::set<LONGLONG>& patternLoops_failedToOffset);

	const CGridListCtrlEx_cannotCopy& GetNcListCtrl() const { return m_ncListGrid; }

private:
	void AdjustLayout();

	BOOL CreateNCGrid();
	BOOL CreateNCBtn();
	void InitNCGrid();

	IntListPtr GetSelectedNCRow();

private: // for controls
	// the singleton object
	static NCViewBar *m_pNCViewBar;

	// the list which will list the NC code.
	CGridListCtrlEx_cannotCopy m_ncListGrid;

	// the button to save NC code into file.
	CButton m_saveNCBtn;

private: // data members.
	CString m_strSheetName;

	// “模拟形状”集合
	SequenceSimulateShapeListPtr m_pSequenceSimulateShapeList;

	// all cut nodes, include which can not be "drawn".
	NCCodeDataPtr m_pNCCodeData;

	// the post editor.
	MacRouterEditorPtr m_pMacRouterEditor;

	// the postfix of NC code.
	CString m_strNCPostfix;

	// when filling grid, disable the event.
	BOOL m_bFillingGrid;

    // 在编辑完成之前记录下单元格的内容，用于编辑好之后比较。
    CString m_strCellText_beforeEdit;

    // 用于在NCSimulateAction中显示偏移不对的轮廓。
    LoopInstanceListPtr m_pAllLoopInstances;
    std::set<LONGLONG> m_patternLoops_failedToOffset;
};

END_CUTLEADER_NAMESPACE()
