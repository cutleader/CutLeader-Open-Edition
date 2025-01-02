#pragma once

#include "drawWrapperCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

// 内存设备上下文。
class ClDrawWrapper_Export MemDC
{
public:
	MemDC(void);
	~MemDC(void);

public:
	HDC GetMemDC() { return m_hMemDC; }
	HBITMAP GetMemBMP() { return m_hMemBitmap; }

	BOOL CreateMemoryDC(HDC hDC, int cx, int cy);
	BOOL ReSize(int cx, int cy);
	BOOL ReleaseRes();

public:
	// mem dc handle
	HDC m_hMemDC;

protected:
	// old mem dc bitmap.
	HBITMAP m_hOldBitmap;

	// mem dc bitmap.
	HBITMAP m_hMemBitmap;

	HDC m_hDC;
};

END_CUTLEADER_NAMESPACE()
