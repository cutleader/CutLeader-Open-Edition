#pragma once

#include "clBaseDataCommon.h"

DECLARE_CUTLEADER_CLASS(ImgData)

BEGIN_CUTLEADER_NAMESPACE()

// bmp光栅图对象。
class ClBaseData_Export ImgData
{
public:
	ImgData(void);
	~ImgData(void);

public: // get/set functions.
	void SetData(LPVOID pData) { m_pData = pData; }
	LPVOID GetData() const { return m_pData; }

	void SetSize(int iSize) { m_iSize = iSize; }
	int GetSize() const { return m_iSize; }

	void SetExt(const CString& strImgExt) { m_strImgExt = strImgExt; }
	const CString& GetExt() const { return m_strImgExt; }

public:
	// 在窗口上画出图片。
	// 注：
	// 1) 图像充满窗口。
	// 2) dwNewBackgroundColor是bmp图片绘制时的新背景色，注意，这里假设了创建bmp图片时的背景色都是白色。
	static void DrawImgData(const ImgData* pImgData, CWnd* pWnd, COLORREF dwNewBackgroundColor);

private:
	// the data in the image.
	LPVOID m_pData;

	// the image size.
	int m_iSize;

	// the image ext.
	// notes:
	//   1) do not have "."
	CString m_strImgExt;
};

END_CUTLEADER_NAMESPACE()
