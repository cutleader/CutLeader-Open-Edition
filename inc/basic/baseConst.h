#pragma once

/************************************************************************/
// the basic const.

// if values will be compared definitely, use this value.
#define SYSTEM_TOLERANCE					0.000000001

// the string length
#define MAXLENGTH_STR						256
#define MAXLENGTH_STR2						512
#define MAXLENGTH_STR4						1024
#define MAXLENGTH_STR_1M					(1024*1024)

// use this value to indicate invalid value, so we can skip that value. e.g. in database, use this to indicate an invalid field.
#define INVALID_VALUE						999999

#define INFINITE_VAL						0xFFFFFFFF

#define INVALID_ID							-1
#define INVALID_IDX							-1

// 小数点后面位数。
#define DIGITAL_NUM							4

// 数值格式化。
#define INT_FORMAT							_T("%d")
#define INT64_FORMAT						_T("%I64d")
#define FLOAT_FORMAT						_T("%f")
#define STRING_FORMAT						_T("%s")
#define EMPTY_STRING						_T("")
#define FLOAT_FORMAT_1						_T("%.1f")
#define FLOAT_FORMAT_2						_T("%.2f")
#define FLOAT_FORMAT_4						_T("%.4f")
#define FLOAT_FORMAT_1_1					_T("%.1fx%.1f")
#define FLOAT_FORMAT_2_2					_T("%.2fx%.2f")
#define FLOAT_FORMAT_3_3					_T("%.3fx%.3f")
/************************************************************************/


/************************************************************************/
// const used for Angle.

// angles
#define PI									3.14159265358979323846
#define TWOPI								2*3.14159265358979323846
#define PID2								3.14159265358979323846/2
#define PID4								3.14159265358979323846/4
#define ZEROPI								0

// convert the radius to degree
#define R2D     							57.29577951308
// convert the degree to radius
#define D2R     							0.01745329251994
/************************************************************************/


/************************************************************************/
// about the geometry

// if use this value to compare the position of the geometry, do not change it.
#define GEOM_TOLERANCE						0.0001

// this factor decides how to cut the arc to lines.
#define CUTARC_FACTOR						0.0052

// this factor indicate how to optimize the polygon.
#define OPTPOLY_FACTOR						0.000028

// snap the object within this pixel distance.
#define SNAP_DISTANCE						5
/************************************************************************/


/************************************************************************/
// 一些自定义消息。

// if we clicked the button in the prop bar, we will send such message to ***Bar class, 
// and the prop name(IDS_PROP_STARTPOINT_X) will be sent.
#define WM_PROP_BUTTON_CLICK				WM_USER + 0x7FF8

// if we clicked the check box in the prop bar, we will
// send such message to ***Bar class, and the prop(CheckBoxProp) will be sent.
#define WM_PROP_CHECKBOX_CLICK 				WM_USER + 0x7FF6

// in some case, we need to send this msg, e.g. if some data are changed in some page, the "Apply" button will be enabled by receiving this msg.
#define WM_DATA_MODIFIED					WM_USER + 0x7FF9

// data db click message.
#define WM_DATA_ITEM_DB_CLICK				WM_USER + 0x4E36

// “高亮按钮”的消息。
#define WM_HIGHLIGHT_RIBBON_BTN				WM_USER + 0x126C
/************************************************************************/


// the website for the product.
#define PRODUCT_WEBSITE						_T("http://www.cutleader.com")

// 界面的背景色。
#define UI_COLOR							RGB(239, 247, 253)

// 右侧停靠面板的宽度。
#define RIGHT_DOCK_PANE_WIDTH				230
// 底部停靠面板的高度。
#define BOTTOM_DOCK_PANE_HEIGHT				232

// 预览图片的像素尺寸和后缀。
#define PREVIEW_IMG_WIDTH					180
#define PREVIEW_IMG_HEIGHT					180
#define PREVIEW_IMG_EXT						_T("bmp")

// const used for unit.
#define INCH2MM								25.4
#define MM2INCH								0.03937007874015748031
