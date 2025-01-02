#pragma once

#include "clGeometryFeatureUICommon.h"
#include "ActionBar.h"
#include "taoresource.h"
#include "SkinManager.h"
#include "SkinNumEdit.h"
#include "SubjectBase.h"

DECLARE_CUTLEADER_CLASS(TextParam)

BEGIN_CUTLEADER_NAMESPACE()

// 这个事件表示文字参数变化了。
#define ID_EVENT_TextParamChanged 15983

// 文字参数action工具条。
class ClGeometryFeatureUI_Export TextParamActionBar : public ActionBar, public SubjectBase_2
{
	DECLARE_DYNCREATE(TextParamActionBar)

public:
	TextParamActionBar(CWnd* pParent = NULL);
	~TextParamActionBar(void);

	enum { IDD = IDD_clGeometryFeatureUI_TextParamActionBar };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

protected:
	afx_msg void OnPaint(); // 这个消息会代替基类的OnPaint消息。
	afx_msg LRESULT OnCharEventOfEditBoxFinished(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeFontName();

	DECLARE_MESSAGE_MAP()

public:
	void InitTextParam(TextParamPtr pTextParam); // 初始化文字参数并刷新界面。
	TextParamPtr GetTextParam_FW() const { return m_pTextParam; }
	const TextParam* GetTextParam() const { return m_pTextParam.get(); }

	void Show_TextParamActionBar(bool bExistActionBarBetyondMe, const CRect& existActionBarRect);

private:
	SkinEdit m_ctrlTextContent;
	CComboBox m_ctrlFontComboBox;
	SkinNumEdit m_ctrlTextHeight;
	SkinNumEdit m_ctrlTextWidth;
	SkinNumEdit m_ctrlTextTiltAngle;

	// 文字参数。
	TextParamPtr m_pTextParam;

	// 在我上方是不是有一个action bar，如果有那么我就显示在它的下方，底图下选中文字时就是这种情况。
	bool m_bExistActionBarBetyondMe;
	CRect m_existActionBarRect;
};

END_CUTLEADER_NAMESPACE()
