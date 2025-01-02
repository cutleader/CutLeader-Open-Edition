#pragma once

#include "clUtilityCommon.h"

DECLARE_CUTLEADER_CLASS(FontParam)

BEGIN_CUTLEADER_NAMESPACE()

// ���������
class ClUtility_Export FontParam
{
public:
	FontParam(void);
	~FontParam(void);

public:
	/************************************************************************/
	// relations about logic font.

	// �õ��߼����塣
	LOGFONT GetLogicFont();

	// update this object from logic font.
	void UpdateFromLogicFont(LOGFONT logicFont, COLORREF fontColor);

	// whether this object fit the "logicFont".
	BOOL FitLogicFont(LOGFONT logicFont, COLORREF fontColor);
	/************************************************************************/

	// update this object from "pFontParam";
	void UpdateFrom(FontParamPtr pFontParam);

public:
	virtual CString GetFontName() const { return m_strFontName; }
	virtual void SetFontName(CString strFontName) { m_strFontName = strFontName; }

	virtual int GetFontSize() const { return m_iFontSize; }
	virtual void SetFontSize(int iFontSize) { m_iFontSize = iFontSize; }

	virtual BOOL IsFontBold() const { return m_bFontBold; }
	virtual void IsFontBold(BOOL bBord) { m_bFontBold = bBord; }

	virtual BOOL IsFontItalic() const { return m_bFontItalic; }
	virtual void IsFontItalic(BOOL bItalic) { m_bFontItalic = bItalic; }

	virtual BOOL IsFontUnderline() const { return m_bFontUnderline; }
	virtual void IsFontUnderline(BOOL bUnderline) { m_bFontUnderline = bUnderline; }

	virtual BOOL IsFontStrikeout() const { return m_bFontStrikeout; }
	virtual void IsFontStrikeout(BOOL bStrikeout) { m_bFontStrikeout = bStrikeout; }

	virtual COLORREF GetFontColor() const { return m_fontColor; }
	virtual void SetFontColor(COLORREF iColor) { m_fontColor = iColor; }

	virtual int	GetCharset() const { return m_iCharset; }
	virtual void SetCharset(int iCharset) { m_iCharset = iCharset; }

private:
	// the font name.
	CString m_strFontName;

	// the font size.
	// ע��
	//  1) ���ֵ������ѡ��Ի�������õ��ģ���λ�ǵ㣻�������ֻ�����ʾ����Ի���ǰ��Ҫ����ת��Ϊ���ص�λ��
	int m_iFontSize;

	// whether the font is bold.
	BOOL m_bFontBold;

	// whether the font is italic.
	BOOL m_bFontItalic;

	// whether the font is underline.
	BOOL m_bFontUnderline;

	// whether the font is strike out.
	BOOL m_bFontStrikeout;

	// the font color.
	COLORREF m_fontColor;

	// the used charset.
	int m_iCharset;
};

END_CUTLEADER_NAMESPACE()
