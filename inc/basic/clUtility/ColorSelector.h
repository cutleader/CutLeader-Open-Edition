#pragma once

#include "clUtilityCommon.h"

BEGIN_CUTLEADER_NAMESPACE()

typedef enum ColorType
{
	COLOR_RED				= 0,
	COLOR_LIGHT_RED			= 1,

	COLOR_YELLOW			= 2,
	COLOR_LIGHT_YELLOW		= 3,

	COLOR_GREEN				= 4,
	COLOR_BLUE				= 5,
	COLOR_ORANGE			= 6,
	COLOR_BLACK				= 7,
	COLOR_WHITE				= 8,
} COLOR_TYPE;

static COLORREF m_colorList[] = 
{
	RGB(255, 0, 0),			// COLOR_RED
	RGB(255, 128, 128),		// COLOR_LIGHT_RED
	RGB(255, 255, 0),		// COLOR_YELLOW
	RGB(255, 255, 128),		// COLOR_LIGHT_YELLOW
	RGB(128, 255, 0),		// COLOR_GREEN
	RGB(0, 128, 192),		// COLOR_BLUE
	RGB(255, 128, 0),		// COLOR_ORANGE
	RGB(0, 0, 0),			// COLOR_BLACK
	RGB(255, 255, 255)		// COLOR_WHITE
};

// use this class to select color rapidly.
class ClUtility_Export ColorSelector
{
public:
	static COLORREF GetColor(COLOR_TYPE eColorType);
};

END_CUTLEADER_NAMESPACE()
