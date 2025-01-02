#include "clUILibResource.h"

///////////////////////////////////////////////////////////////////////////////
// ID range is (12000~13000)
///////////////////////////////////////////////////////////////////////////////

#define ClGeometryFeature_Resource_StartID							12000

/************************************************************************/
/*                the string ID                                         */

#define IDS_GEOMETRY_CATEGORY							ClGeometryFeature_Resource_StartID+1

// basic
#define IDS_GEOMETRY									ClGeometryFeature_Resource_StartID+5
#define IDS_GEOMETRY_LIST								ClGeometryFeature_Resource_StartID+6
#define IDS_GEOM_TYPE									ClGeometryFeature_Resource_StartID+7

// the names for line side
#define IDS_LINESIDE_LEFT								ClGeometryFeature_Resource_StartID+10
#define IDS_LINESIDE_CENTER								ClGeometryFeature_Resource_StartID+11
#define IDS_LINESIDE_RIGHT								ClGeometryFeature_Resource_StartID+12
//

// the names for arc side
#define IDS_ARCSIDE_OUTSIDE								ClGeometryFeature_Resource_StartID+15
#define IDS_ARCSIDE_ON									ClGeometryFeature_Resource_StartID+16
#define IDS_ARCSIDE_INSIDE								ClGeometryFeature_Resource_StartID+17
//

// pattern type.
#define IDS_GEOMETRY_STD								ClGeometryFeature_Resource_StartID+21
#define IDS_GEOMETRY_POLYGON							ClGeometryFeature_Resource_StartID+22
#define IDS_GEOMETRY_ARRAY								ClGeometryFeature_Resource_StartID+23
//

// the type names of pattern
#define IDS_GEOMETRY_LINE								ClGeometryFeature_Resource_StartID+24
#define IDS_GEOMETRY_ARC								ClGeometryFeature_Resource_StartID+25
#define IDS_GEOMETRY_RECTHOLE							ClGeometryFeature_Resource_StartID+26
#define IDS_GEOMETRY_TRHOLE								ClGeometryFeature_Resource_StartID+27
#define IDS_GEOMETRY_ROUNDHOLE							ClGeometryFeature_Resource_StartID+28
#define IDS_GEOMETRY_OBROUND							ClGeometryFeature_Resource_StartID+29
#define IDS_GEOMETRY_ELLIPSE							ClGeometryFeature_Resource_StartID+30
#define IDS_GEOMETRY_SINGLED							ClGeometryFeature_Resource_StartID+31
#define IDS_GEOMETRY_DOUBLED							ClGeometryFeature_Resource_StartID+32
#define IDS_GEOMETRY_RCHOLE								ClGeometryFeature_Resource_StartID+33
#define IDS_GEOMETRY_RADSLOT							ClGeometryFeature_Resource_StartID+34
#define IDS_GEOMETRY_PATLINE							ClGeometryFeature_Resource_StartID+35
#define IDS_GEOMETRY_PATARC								ClGeometryFeature_Resource_StartID+36
#define IDS_GEOMETRY_PATCIRCLE							ClGeometryFeature_Resource_StartID+37
#define IDS_GEOMETRY_PATGRID							ClGeometryFeature_Resource_StartID+38
//

// the names of the loop.
#define IDS_LOOP										ClGeometryFeature_Resource_StartID+40
#define IDS_LOOP_LIST									ClGeometryFeature_Resource_StartID+41
#define IDS_LOOP_POLY									ClGeometryFeature_Resource_StartID+42
#define IDS_LOOP_SINGLE									ClGeometryFeature_Resource_StartID+43
#define IDS_LOOP_ARRAY									ClGeometryFeature_Resource_StartID+44
//

// 对齐轮廓的类型
#define IDS_AlignLoop_Left								ClGeometryFeature_Resource_StartID+50
#define IDS_AlignLoop_Right								ClGeometryFeature_Resource_StartID+51
#define IDS_AlignLoop_Top								ClGeometryFeature_Resource_StartID+52
#define IDS_AlignLoop_Bottom							ClGeometryFeature_Resource_StartID+53
#define IDS_AlignLoop_HorizontalCenter					ClGeometryFeature_Resource_StartID+54
#define IDS_AlignLoop_VerticalCenter					ClGeometryFeature_Resource_StartID+55
#define IDS_AlignLoop_Center							ClGeometryFeature_Resource_StartID+56
//

// 捕获点的名称。
#define IDS_SnapPt_EndPt								ClGeometryFeature_Resource_StartID+60
#define IDS_SnapPt_MiddlePt								ClGeometryFeature_Resource_StartID+61
#define IDS_SnapPt_CenterPt								ClGeometryFeature_Resource_StartID+62
#define IDS_SnapPt_ClosestPt							ClGeometryFeature_Resource_StartID+63
#define IDS_SnapPt_FootPt								ClGeometryFeature_Resource_StartID+64
#define IDS_SnapPt_TangencyPt							ClGeometryFeature_Resource_StartID+65
#define IDS_SnapPt_ControlPt							ClGeometryFeature_Resource_StartID+66
#define IDS_SnapPt_CrossPt								ClGeometryFeature_Resource_StartID+67
//

// 几何特征属性用到的字符串。
#define IDS_CATEGORY_GEOMETRY							ClGeometryFeature_Resource_StartID+99
#define IDS_PROPBAR_PATPROP								ClGeometryFeature_Resource_StartID+100
//
#define IDS_PROP_STARTPOINT_X							ClGeometryFeature_Resource_StartID+101
#define IDS_PROP_STARTPOINT_Y							ClGeometryFeature_Resource_StartID+102
#define IDS_PROP_ENDPOINT_X								ClGeometryFeature_Resource_StartID+103
#define IDS_PROP_ENDPOINT_Y								ClGeometryFeature_Resource_StartID+104
#define IDS_PROP_LENGTH									ClGeometryFeature_Resource_StartID+105
#define IDS_PROP_RADIUS									ClGeometryFeature_Resource_StartID+106
#define IDS_PROP_START_ANGLE							ClGeometryFeature_Resource_StartID+107
#define IDS_PROP_MOVE_ANGLE								ClGeometryFeature_Resource_StartID+108
#define IDS_PROP_CENTERPOINT_X							ClGeometryFeature_Resource_StartID+109
#define IDS_PROP_CENTERPOINT_Y							ClGeometryFeature_Resource_StartID+110
//
#define IDS_PROP_ANGLE									ClGeometryFeature_Resource_StartID+117
#define IDS_PROP_WIDTH									ClGeometryFeature_Resource_StartID+118
//
#define IDS_PROP_SPACE_X								ClGeometryFeature_Resource_StartID+119
#define IDS_PROP_SPACE_Y								ClGeometryFeature_Resource_StartID+120
#define IDS_PROP_SPACE									ClGeometryFeature_Resource_StartID+121
#define IDS_PROP_REPEAT_NUM								ClGeometryFeature_Resource_StartID+122
#define IDS_PROP_HEIGHT									ClGeometryFeature_Resource_StartID+123
#define IDS_PROP_SPACE_ANGLE							ClGeometryFeature_Resource_StartID+124
#define IDS_PROP_REPEAT_NUM_X							ClGeometryFeature_Resource_StartID+125
#define IDS_PROP_REPEAT_NUM_Y							ClGeometryFeature_Resource_StartID+126
#define IDS_PROP_DEPTH									ClGeometryFeature_Resource_StartID+127
#define IDS_PROP_CORNER_ANGLE							ClGeometryFeature_Resource_StartID+128
//
#define IDS_PROP_LOCATION_X								ClGeometryFeature_Resource_StartID+129
#define IDS_PROP_LOCATION_Y								ClGeometryFeature_Resource_StartID+130
//
#define IDS_PROP_CORNER_RADIUS							ClGeometryFeature_Resource_StartID+131
#define IDS_PROP_SEGMENT_COUNT							ClGeometryFeature_Resource_StartID+132
//

// used for "draw basic geometry" panel.
#define IDS_DRAW_BASIC_GEOM								ClGeometryFeature_Resource_StartID+190
#define IDS_GEOM_DRAW_RCLINEARC							ClGeometryFeature_Resource_StartID+191
#define IDS_GEOM_DRAW_3PtArc							ClGeometryFeature_Resource_StartID+192
//

// used for "draw standard geometry" panel.
#define IDS_DRAW_STD_GEOM								ClGeometryFeature_Resource_StartID+200
//

// used for "draw array geometry" panel.
#define IDS_DRAW_ARRAY_GEOM								ClGeometryFeature_Resource_StartID+210
//

// used for "edit" panel.
#define IDS_SELECT_GEOM									ClGeometryFeature_Resource_StartID+220
#define IDS_GEOM_OPTIMIZATION							ClGeometryFeature_Resource_StartID+221
#define IDS_GEOM_OFFSET_LOOP							ClGeometryFeature_Resource_StartID+222
//

// used for "view" panel
#define IDS_GEOM_VIEW_ZOOMALL							ClGeometryFeature_Resource_StartID+230
#define IDS_GEOM_VIEW_ZOOMIN							ClGeometryFeature_Resource_StartID+231
#define IDS_GEOM_VIEW_ZOOMOUT							ClGeometryFeature_Resource_StartID+232
//

// names of commands.
#define IDS_COMMAND_MOVE_PAT							ClGeometryFeature_Resource_StartID+250
#define IDS_COMMAND_ROTATE_PAT							ClGeometryFeature_Resource_StartID+251
#define IDS_COMMAND_ADD_PAT								ClGeometryFeature_Resource_StartID+252
#define IDS_COMMAND_COPY_PAT							ClGeometryFeature_Resource_StartID+253
#define IDS_COMMAND_DEL_PAT								ClGeometryFeature_Resource_StartID+254
#define IDS_COMMAND_GRIPEDIT_PAT						ClGeometryFeature_Resource_StartID+255
#define IDS_COMMAND_EDITPROP_PAT						ClGeometryFeature_Resource_StartID+256
#define IDS_COMMAND_EXPLODE_STD_PAT						ClGeometryFeature_Resource_StartID+257
#define IDS_COMMAND_EXPLODE_ARRAY_PAT					ClGeometryFeature_Resource_StartID+258
#define IDS_COMMAND_ARRAY_PAT							ClGeometryFeature_Resource_StartID+259
#define IDS_COMMAND_DEL_PATLOOP							ClGeometryFeature_Resource_StartID+260
#define IDS_COMMAND_AlignLoop							ClGeometryFeature_Resource_StartID+261
//

// the message IDs that will be displayed, allocate 10 ID for one section.
// draw/grip-edit line
#define IDS_OUTPUT_DRAWLINE_STARTPT						ClGeometryFeature_Resource_StartID+301
#define IDS_OUTPUT_DRAWLINE_ENDPT						ClGeometryFeature_Resource_StartID+302
// draw/grip-edit rect lines
#define IDS_OUTPUT_RECTLINE_STARTCORNER					ClGeometryFeature_Resource_StartID+310
#define IDS_OUTPUT_RECTLINE_ENDCORNER					ClGeometryFeature_Resource_StartID+311
// 绘制和编辑直线阵列。
#define IDS_OUTPUT_PATLINE_PAT							ClGeometryFeature_Resource_StartID+320
#define IDS_OUTPUT_PATLINE_XYSPACE						ClGeometryFeature_Resource_StartID+321
#define IDS_OUTPUT_PATLINE_NUM							ClGeometryFeature_Resource_StartID+322
// 绘制和编辑圆弧阵列。
#define IDS_OUTPUT_PATARC_PAT							ClGeometryFeature_Resource_StartID+330
#define IDS_OUTPUT_PATARC_RAD_ANGLE						ClGeometryFeature_Resource_StartID+331
#define IDS_OUTPUT_PATARC_REPEAT_ANGLE					ClGeometryFeature_Resource_StartID+332
#define IDS_OUTPUT_PATARC_NUM							ClGeometryFeature_Resource_StartID+333
// draw/grip-edit 3 point arc
#define IDS_OUTPUT_PATARC3PT_FIRSTPT					ClGeometryFeature_Resource_StartID+340
#define IDS_OUTPUT_PATARC3PT_SECONDPT					ClGeometryFeature_Resource_StartID+341
#define IDS_OUTPUT_PATARC3PT_THIRDPT					ClGeometryFeature_Resource_StartID+342
// draw/grip-edit rc line/arc
#define IDS_OUTPUT_RCLINE_CENTERPT						ClGeometryFeature_Resource_StartID+350
#define IDS_OUTPUT_RCLINE_LENGTH_WIDTH					ClGeometryFeature_Resource_StartID+351
#define IDS_OUTPUT_RCLINE_RADIUS						ClGeometryFeature_Resource_StartID+352
// draw/grip-edit double d
#define IDS_OUTPUT_PATDOUBLED_CENTERPT					ClGeometryFeature_Resource_StartID+360
#define IDS_OUTPUT_PATDOUBLED_RADIUS					ClGeometryFeature_Resource_StartID+361
#define IDS_OUTPUT_PATDOUBLED_WIDTH_ANGLE				ClGeometryFeature_Resource_StartID+362
#define IDS_OUTPUT_PATDOUBLED_WIDTH						ClGeometryFeature_Resource_StartID+363
// draw/grip-edit ellipse
#define IDS_OUTPUT_PATELLIPSE_CENTERPT					ClGeometryFeature_Resource_StartID+370
#define IDS_OUTPUT_PATELLIPSE_LENGTH_ANGLE				ClGeometryFeature_Resource_StartID+371
#define IDS_OUTPUT_PATELLIPSE_WIDTH						ClGeometryFeature_Resource_StartID+372
#define IDS_OUTPUT_PATELLIPSE_LENGTH					ClGeometryFeature_Resource_StartID+373
// draw/grip-edit obround
#define IDS_OUTPUT_OBROUND_CENTERPT						ClGeometryFeature_Resource_StartID+380
#define IDS_OUTPUT_OBROUND_ANGLE_LENGTH					ClGeometryFeature_Resource_StartID+381
#define IDS_OUTPUT_OBROUND_WIDTH						ClGeometryFeature_Resource_StartID+382
#define IDS_OUTPUT_OBROUND_LENGTH						ClGeometryFeature_Resource_StartID+383
// draw/grip-edit radius slot
#define IDS_OUTPUT_RADSLOT_CENTERPT						ClGeometryFeature_Resource_StartID+390
#define IDS_OUTPUT_RADSLOT_RAD_ANGLE					ClGeometryFeature_Resource_StartID+391
#define IDS_OUTPUT_RADSLOT_STARTANGLE					ClGeometryFeature_Resource_StartID+392
#define IDS_OUTPUT_RADSLOT_ENDANGLE						ClGeometryFeature_Resource_StartID+393
#define IDS_OUTPUT_RADSLOT_WIDTH						ClGeometryFeature_Resource_StartID+394
#define IDS_OUTPUT_RADSLOT_RADIUS						ClGeometryFeature_Resource_StartID+395
// draw/grip-edit rc hole
#define IDS_OUTPUT_RCHOLE_CENTERPT						ClGeometryFeature_Resource_StartID+400
#define IDS_OUTPUT_RCHOLE_LENGTH_WIDTH					ClGeometryFeature_Resource_StartID+401
#define IDS_OUTPUT_RCHOLE_LENGTH						ClGeometryFeature_Resource_StartID+402
#define IDS_OUTPUT_RCHOLE_WIDTH							ClGeometryFeature_Resource_StartID+403
#define IDS_OUTPUT_RCHOLE_RADIUS						ClGeometryFeature_Resource_StartID+404
// draw/grip-edit rect hole
#define IDS_OUTPUT_RECTHOLE_CENTERPT					ClGeometryFeature_Resource_StartID+410
#define IDS_OUTPUT_RECTHOLE_LENGTH_WIDTH				ClGeometryFeature_Resource_StartID+411
// draw/grip-edit triangle hole
#define IDS_OUTPUT_TRIHOLE_BASEPT						ClGeometryFeature_Resource_StartID+420
#define IDS_OUTPUT_TRIHOLE_ANGLE_WIDTH					ClGeometryFeature_Resource_StartID+421
#define IDS_OUTPUT_TRIHOLE_WIDTH						ClGeometryFeature_Resource_StartID+422
#define IDS_OUTPUT_TRIHOLE_HEIGHT						ClGeometryFeature_Resource_StartID+423
// draw/grip-edit single d
#define IDS_OUTPUT_PATSINGLED_CENTERPT					ClGeometryFeature_Resource_StartID+430
#define IDS_OUTPUT_PATSINGLED_RADIUS					ClGeometryFeature_Resource_StartID+431
#define IDS_OUTPUT_PATSINGLED_WIDTH_ANGLE				ClGeometryFeature_Resource_StartID+432
#define IDS_OUTPUT_PATSINGLED_WIDTH						ClGeometryFeature_Resource_StartID+433
// array
#define IDS_OUTPUT_SELECT_STDPAT						ClGeometryFeature_Resource_StartID+440
#define IDS_OUTPUT_REPEAT_NUM_SMALL						ClGeometryFeature_Resource_StartID+441
#define IDS_OUTPUT_REPEAT_NUM_SMALL_EX					ClGeometryFeature_Resource_StartID+442
// draw/grip-edit grid array
#define IDS_OUTPUT_PATGRID_PAT							ClGeometryFeature_Resource_StartID+450
#define IDS_OUTPUT_PATGRID_XYSPACE						ClGeometryFeature_Resource_StartID+451
#define IDS_OUTPUT_PATGRID_NUM							ClGeometryFeature_Resource_StartID+452
#define IDS_OUTPUT_PATGRID_XNUM							ClGeometryFeature_Resource_StartID+453
#define IDS_OUTPUT_PATGRID_YNUM							ClGeometryFeature_Resource_StartID+454
// draw/grip-edit circle array
#define IDS_OUTPUT_PATCIRCLE_PAT						ClGeometryFeature_Resource_StartID+460
#define IDS_OUTPUT_PATCIRCLE_RAD_ANGLE					ClGeometryFeature_Resource_StartID+461
#define IDS_OUTPUT_PATCIRCLE_NUM						ClGeometryFeature_Resource_StartID+462
#define IDS_OUTPUT_PATCIRCLE_CENTERPT					ClGeometryFeature_Resource_StartID+463
#define IDS_OUTPUT_PATCIRCLE_RADIUS						ClGeometryFeature_Resource_StartID+464
// 绘制和编辑圆弧。
#define IDS_OUTPUT_PATARC_CENTERPT						ClGeometryFeature_Resource_StartID+470
#define IDS_OUTPUT_PATARC_RAD_STARTANGLE				ClGeometryFeature_Resource_StartID+471
#define IDS_OUTPUT_PATARC_MOVEANGLE						ClGeometryFeature_Resource_StartID+472
#define IDS_OUTPUT_PATARC_STARTANGLE					ClGeometryFeature_Resource_StartID+473
#define IDS_OUTPUT_PATARC_CENTERPT_RAD					ClGeometryFeature_Resource_StartID+474
// select
#define IDS_OUTPUT_SELECTPATTERN						ClGeometryFeature_Resource_StartID+480
#define IDS_OUTPUT_SELECTPAT_NEXTPT						ClGeometryFeature_Resource_StartID+481
#define IDS_OUTPUT_NOPAT_SELECT							ClGeometryFeature_Resource_StartID+482
#define IDS_OUTPUT_RCLICK_SELECT						ClGeometryFeature_Resource_StartID+483
#define IDS_OUTPUT_PAT_SELECT							ClGeometryFeature_Resource_StartID+484
#define IDS_NO_PATLOOP_SELECTED							ClGeometryFeature_Resource_StartID+485
// copy edit
#define IDS_OUTPUT_COPYPAT_BASEPT						ClGeometryFeature_Resource_StartID+490
#define IDS_OUTPUT_COPYPAT_NEWLOCATION					ClGeometryFeature_Resource_StartID+491
// 有关轮廓对齐。
#define IDS_OUTPUT_SelectMultiLoop2Align				ClGeometryFeature_Resource_StartID+495
// explode edit
#define IDS_OUTPUT_SELECT_STD_ARRAY_PAT					ClGeometryFeature_Resource_StartID+500
#define IDS_OUTPUT_RIGHT_CLICK_EXPLODE					ClGeometryFeature_Resource_StartID+501
// move edit
#define IDS_OUTPUT_MOVEPAT_BASEPT						ClGeometryFeature_Resource_StartID+510
#define IDS_OUTPUT_MOVEPAT_NEWLOCATION					ClGeometryFeature_Resource_StartID+511
// rotate edit
#define IDS_OUTPUT_ROTATEPAT_BASEPT						ClGeometryFeature_Resource_StartID+520
#define IDS_OUTPUT_ROTATEPAT_ANGLE						ClGeometryFeature_Resource_StartID+521
#define IDS_OUTPUT_ROTATEPAT_ARRAY						ClGeometryFeature_Resource_StartID+522
// offset loop.
#define IDS_OUTPUT_OFFSETLOOP_SELECTLOOP				ClGeometryFeature_Resource_StartID+525
#define IDS_OUTPUT_OFFSETLOOP_OPEN_LOOP					ClGeometryFeature_Resource_StartID+526
#define IDS_OUTPUT_OFFSETLOOP_PROP_VAL					ClGeometryFeature_Resource_StartID+527

// for execute task.
#define IDS_MSG_OPT_TASK								ClGeometryFeature_Resource_StartID+530
#define IDS_MSG_OPT_LOOP								ClGeometryFeature_Resource_StartID+531
#define IDS_MSG_OPT_ALL_LOOPS							ClGeometryFeature_Resource_StartID+532
#define IDS_MSG_OPT_TASK_OK								ClGeometryFeature_Resource_StartID+533
#define IDS_MSG_OPT_TASK_CANCEL							ClGeometryFeature_Resource_StartID+534
//

// 按钮提示信息。
// “绘制基本图形”分组
#define IDS_GEOMETRY_LINE_TIP							ClGeometryFeature_Resource_StartID+600
#define IDS_GEOMETRY_MULTILINE_TIP						ClGeometryFeature_Resource_StartID+601
#define IDS_GEOMETRY_ARC_TIP							ClGeometryFeature_Resource_StartID+602
#define IDS_GEOM_DRAW_RCLINEARC_TIP						ClGeometryFeature_Resource_StartID+603
#define IDS_GEOM_DRAW_RectLine_TIP						ClGeometryFeature_Resource_StartID+604
// “绘制标准图形”分组
#define IDS_GEOMETRY_ROUNDHOLE_TIP						ClGeometryFeature_Resource_StartID+610
#define IDS_GEOMETRY_RECTHOLE_TIP						ClGeometryFeature_Resource_StartID+611
#define IDS_GEOMETRY_SINGLED_TIP						ClGeometryFeature_Resource_StartID+612
#define IDS_GEOMETRY_TRHOLE_TIP							ClGeometryFeature_Resource_StartID+613
#define IDS_GEOMETRY_RADSLOT_TIP						ClGeometryFeature_Resource_StartID+614
// “绘制阵列图形”分组
#define IDS_GEOMETRY_PATLINE_TIP						ClGeometryFeature_Resource_StartID+620
#define IDS_GEOMETRY_PATARC_TIP							ClGeometryFeature_Resource_StartID+621
#define IDS_GEOMETRY_PATCIRCLE_TIP						ClGeometryFeature_Resource_StartID+622
#define IDS_GEOMETRY_PATGRID_TIP						ClGeometryFeature_Resource_StartID+623
// “编辑”分组
#define IDS_SELECT_GEOM_TIP								ClGeometryFeature_Resource_StartID+630
#define IDS_DELETE_GEOM_TIP								ClGeometryFeature_Resource_StartID+631
#define IDS_EXPLODE_GEOM_TIP							ClGeometryFeature_Resource_StartID+632
#define IDS_COPY_GEOM_TIP								ClGeometryFeature_Resource_StartID+633
#define IDS_MOVE_GEOM_TIP								ClGeometryFeature_Resource_StartID+634
#define IDS_ROTATE_GEOM_TIP								ClGeometryFeature_Resource_StartID+635
#define IDS_OPTIMIZE_GEOM_TIP							ClGeometryFeature_Resource_StartID+636
#define IDS_OFFSET_LOOP_TIP								ClGeometryFeature_Resource_StartID+637
// “查看”分组
#define IDS_GEOM_VIEW_ZOOMALL_TIP						ClGeometryFeature_Resource_StartID+640
#define IDS_GEOM_VIEW_ZOOMIN_TIP						ClGeometryFeature_Resource_StartID+641
#define IDS_GEOM_VIEW_ZOOMOUT_TIP						ClGeometryFeature_Resource_StartID+642
// “窗口”分组
#define IDS_VIEWBAR_PATTERN_TIP							ClGeometryFeature_Resource_StartID+650
#define IDS_VIEWBAR_PATLOOP_TIP							ClGeometryFeature_Resource_StartID+651
//
/************************************************************************/


/************************************************************************/
/*                the control created by code                           */

// used for "draw" panel.
// line/arc
#define IDC_GEOM_DRAW_RCLINEARC							ClGeometryFeature_Resource_StartID+705
// std
#define IDC_GEOM_DRAW_TRIHOLE							ClGeometryFeature_Resource_StartID+711
#define IDC_GEOM_DRAW_RADSLOT							ClGeometryFeature_Resource_StartID+718
// group
#define IDC_GEOM_DRAW_PATCIRCLE							ClGeometryFeature_Resource_StartID+720
#define IDC_GEOM_DRAW_PATARC							ClGeometryFeature_Resource_StartID+721
#define IDC_GEOM_DRAW_PATGRID							ClGeometryFeature_Resource_StartID+722
#define IDC_GEOM_DRAW_PATLINE							ClGeometryFeature_Resource_StartID+723
// 文字。
#define IDC_DrawText									ClGeometryFeature_Resource_StartID+724
//

// used for "view" panel
#define IDC_GEOM_VIEW_ZOOMALL							ClGeometryFeature_Resource_StartID+725
#define IDC_GEOM_VIEW_ZOOMIN							ClGeometryFeature_Resource_StartID+726
#define IDC_GEOM_VIEW_ZOOMOUT							ClGeometryFeature_Resource_StartID+727
//

// used for "edit" panel
#define IDC_GEOM_EDIT_SELECTPAT							ClGeometryFeature_Resource_StartID+730
#define IDC_GEOM_EDIT_COPY								ClGeometryFeature_Resource_StartID+731
#define IDC_GEOM_EDIT_DELETE							ClGeometryFeature_Resource_StartID+732
#define IDC_GEOM_EDIT_EXPLODE							ClGeometryFeature_Resource_StartID+733
#define IDC_GEOM_EDIT_ROTATE							ClGeometryFeature_Resource_StartID+734
#define IDC_GEOM_EDIT_MOVE								ClGeometryFeature_Resource_StartID+735
#define IDC_GEOM_EDIT_OPTIMIZE							ClGeometryFeature_Resource_StartID+736
#define IDC_GEOM_OFFSET_LOOP							ClGeometryFeature_Resource_StartID+737
//

// for prop bar.
#define IDC_PROPBAR_PATPROP								ClGeometryFeature_Resource_StartID+750
#define IDC_VIEWBAR_PATTERN								ClGeometryFeature_Resource_StartID+751
#define IDC_VIEWBAR_PATLOOP								ClGeometryFeature_Resource_StartID+752
#define IDC_TREEVIEW_PATTERN							ClGeometryFeature_Resource_StartID+753
#define IDC_TREEVIEW_PATLOOP							ClGeometryFeature_Resource_StartID+754
/************************************************************************/
