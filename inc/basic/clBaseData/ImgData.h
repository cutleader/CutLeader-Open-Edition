#pragma once

#include "clBaseDataCommon.h"

DECLARE_CUTLEADER_CLASS(ImgData)

BEGIN_CUTLEADER_NAMESPACE()

// bmp��դͼ����
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
	// �ڴ����ϻ���ͼƬ��
	// ע��
	// 1) ͼ��������ڡ�
	// 2) dwNewBackgroundColor��bmpͼƬ����ʱ���±���ɫ��ע�⣬��������˴���bmpͼƬʱ�ı���ɫ���ǰ�ɫ��
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
