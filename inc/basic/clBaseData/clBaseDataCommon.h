#pragma once

#include "cutLeaderCommon.h"

#if defined(ClBaseData_Home)
#    define ClBaseData_Export _declspec(dllexport)
#else
#    define ClBaseData_Export _declspec(dllimport)
#endif

// dxf/dwg版本的定义。
enum DXF_VER_TYPE
{
	DXF_VER_R12 = 16,
	DXF_VER_2004 = 25,
	DXF_VER_2010 = 29
};