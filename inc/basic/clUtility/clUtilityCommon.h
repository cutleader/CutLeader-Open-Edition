#pragma once

#include "cutLeaderCommon.h"

#if defined(ClUtility_Home)
#    define ClUtility_Export _declspec(dllexport)
#else
#    define ClUtility_Export _declspec(dllimport)
#endif

// Ãı–Œ¬Î±‡¬Î°£
enum BARCODE_TYPE
{
	BARCODE_CODE_128		= 0,
	BARCODE_CODE_39			= 1,
	BARCODE_CODE_93			= 2,
	BARCODE_EAN_13			= 3,
};

// the type of basic data.
typedef enum tagBasicDataType
{	
	BASIC_DATA_STRING		= 0,
	BASIC_DATA_INT			= 1,
	BASIC_DATA_BOOL			= 2,
	BASIC_DATA_FLOAT		= 3
} BASIC_DATA_TYPE;