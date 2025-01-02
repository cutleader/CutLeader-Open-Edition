#include "clUILibResource.h"

///////////////////////////////////////////////////////////////////////////////
// ID range is (13000~13500)
///////////////////////////////////////////////////////////////////////////////

#define ClCutFeature_Resource_StartID				13000


/************************************************************************/
/*                the string ID                                         */

#define IDS_CUTFEATURE							ClCutFeature_Resource_StartID+10

// task names
#define IDS_TASK_ASSIGN_MicroJoint				ClCutFeature_Resource_StartID+20
//

// the names of panels
#define IDS_AUTO_PROCESS						ClCutFeature_Resource_StartID+80
#define IDS_STARTEND_CUT						ClCutFeature_Resource_StartID+81
#define IDS_MICROJOINT							ClCutFeature_Resource_StartID+82
#define IDS_CORNER_FEATURE						ClCutFeature_Resource_StartID+83
#define IDS_COND_FEATURE						ClCutFeature_Resource_StartID+84
#define IDS_LOOPTOOL_FEATURE					ClCutFeature_Resource_StartID+85
//

// the names of button on panel.
#define IDS_SetAsCutOutside						ClCutFeature_Resource_StartID+90
#define IDS_SetAsCutInside						ClCutFeature_Resource_StartID+91
#define IDS_SetAsCutOn							ClCutFeature_Resource_StartID+92
#define IDS_SetLoopGap								ClCutFeature_Resource_StartID+93
#define IDS_SwitchCutDirection					ClCutFeature_Resource_StartID+94
#define IDS_CcwCutDirection						ClCutFeature_Resource_StartID+95
#define IDS_CwCutDirection						ClCutFeature_Resource_StartID+96
#define IDS_ASSIGN_CUTFEATURE					ClCutFeature_Resource_StartID+100
#define IDS_RESET_CUTFEATURE					ClCutFeature_Resource_StartID+101
#define IDS_CHECK_INTERFERE						ClCutFeature_Resource_StartID+102
#define IDS_EDIT_LOOP_LEAD						ClCutFeature_Resource_StartID+103
#define IDS_EDIT_LOOP_START_PT					ClCutFeature_Resource_StartID+104
#define IDS_EDIT_LEADOUT						ClCutFeature_Resource_StartID+105
#define IDS_EDIT_COND							ClCutFeature_Resource_StartID+106
#define IDS_SWITCH_CUTSIDE						ClCutFeature_Resource_StartID+107
#define IDS_CHANGE_CUTDIR						ClCutFeature_Resource_StartID+108
#define IDS_ResetLeadLine						ClCutFeature_Resource_StartID+109
#define IDS_ResetStartCutPt						ClCutFeature_Resource_StartID+110
#define IDS_ResetMicroJoint						ClCutFeature_Resource_StartID+111
#define IDS_ResetCorner							ClCutFeature_Resource_StartID+112
#define IDS_AddMicroJoint						ClCutFeature_Resource_StartID+113
#define IDS_EditMicroJoint						ClCutFeature_Resource_StartID+114
#define IDS_DeleteMicroJoint					ClCutFeature_Resource_StartID+115
#define IDS_AddCornerFeature					ClCutFeature_Resource_StartID+116
#define IDS_EditCornerFeature					ClCutFeature_Resource_StartID+117
#define IDS_DeleteCornerFeature					ClCutFeature_Resource_StartID+118
//

// used for Corner Property
#define IDS_PROPBAR_CORNERPROP					ClCutFeature_Resource_StartID+120
#define IDS_CUTFEATURE_CORNERINFO				ClCutFeature_Resource_StartID+121
#define IDS_PROP_CORNER_TYPE					ClCutFeature_Resource_StartID+122
#define IDS_PROP_CORNER_VALUE					ClCutFeature_Resource_StartID+123
//

// used for StartCutPropBar/EndCutPropBar.
#define IDS_PROPBAR_STARTCUTPROP				ClCutFeature_Resource_StartID+140
#define IDS_PROPBAR_ENDCUTPROP					ClCutFeature_Resource_StartID+141
// some groups.
#define IDS_CATEGORY_STARTPOINT					ClCutFeature_Resource_StartID+142
#define IDS_CATEGORY_LEADINDATA					ClCutFeature_Resource_StartID+143
#define IDS_CATEGORY_LEADOUTDATA				ClCutFeature_Resource_StartID+144
#define IDS_CATEGORY_LEADINCOND					ClCutFeature_Resource_StartID+145
#define IDS_CATEGORY_LEADOUTCOND				ClCutFeature_Resource_StartID+146
#define IDS_CATEGORY_DOPIERCECOND				ClCutFeature_Resource_StartID+147
#define IDS_CATEGORY_UNPIERCECOND				ClCutFeature_Resource_StartID+148
// lead data group
#define IDS_PROP_CORNER_LEAD_ENABLE				ClCutFeature_Resource_StartID+150
#define IDS_PROP_LEAD_TYPE						ClCutFeature_Resource_StartID+151
#define IDS_PROP_LEAD_LENGTH					ClCutFeature_Resource_StartID+152
#define IDS_PROP_LEAD_ANGLE						ClCutFeature_Resource_StartID+153
#define IDS_PROP_LEAD_ARCRAD					ClCutFeature_Resource_StartID+154
#define IDS_PROP_LEAD_ARC_ANGLE					ClCutFeature_Resource_StartID+155
#define IDS_PROP_LEAD_LINESPLIT					ClCutFeature_Resource_StartID+156
#define IDS_PROP_LEAD_OVERCUT					ClCutFeature_Resource_StartID+157
// condition group
#define IDS_PROP_LEAD_ENABLECOND				ClCutFeature_Resource_StartID+160
#define IDS_PROP_LEAD_CONDVAL					ClCutFeature_Resource_StartID+161
#define IDS_PROP_PIERCE_ENABLECOND				ClCutFeature_Resource_StartID+162
#define IDS_PROP_PIERCE_CONDVAL					ClCutFeature_Resource_StartID+163
// button
#define IDS_SET_STARTCUT_POINT					ClCutFeature_Resource_StartID+165
//

// used for MicroJointPropBar.
#define IDS_PROPBAR_MicroJointPROP				ClCutFeature_Resource_StartID+175
// 分组
#define IDS_CATEGORY_MJ_POS_WIDTH				ClCutFeature_Resource_StartID+176
#define IDS_CATEGORY_MJ_LEADIN					ClCutFeature_Resource_StartID+177
#define IDS_CATEGORY_MJ_LEADOUT					ClCutFeature_Resource_StartID+178
// 属性项
#define IDS_PROP_MicroJointWIDTH				ClCutFeature_Resource_StartID+180
#define IDS_PROP_LEADIN_ENABLE					ClCutFeature_Resource_StartID+181
#define IDS_PROP_LEADIN_LENGTH					ClCutFeature_Resource_StartID+182
#define IDS_PROP_LEADIN_ANGLE					ClCutFeature_Resource_StartID+183
#define IDS_PROP_LEADOUT_ENABLE					ClCutFeature_Resource_StartID+184
#define IDS_PROP_LEADOUT_LENGTH					ClCutFeature_Resource_StartID+185
#define IDS_PROP_LEADOUT_ANGLE					ClCutFeature_Resource_StartID+186
#define IDS_PROP_MJ_LEADIN_COND					ClCutFeature_Resource_StartID+187
#define IDS_PROP_MJ_LEADOUT_COND				ClCutFeature_Resource_StartID+188
// 按钮
#define IDS_SET_MJ_POINT						ClCutFeature_Resource_StartID+189
//

// used for LoopToolEditPane
#define IDS_PROPBAR_LOOPTOOL					ClCutFeature_Resource_StartID+200
//

// 用于命令。
// 回路起切特征。
#define IDS_COMMAMD_MOD_LEAD					ClCutFeature_Resource_StartID+210
#define IDS_COMMAMD_MOD_STARTPT					ClCutFeature_Resource_StartID+211
#define IDS_COMMAMD_MOD_ENDPT					ClCutFeature_Resource_StartID+212
#define IDS_COMMAMD_CHANGE_CUTSIDE				ClCutFeature_Resource_StartID+213
#define IDS_COMMAMD_CHANGE_DIR					ClCutFeature_Resource_StartID+214
#define IDS_COMMAMD_SetOverCut					ClCutFeature_Resource_StartID+215
#define IDS_COMMAMD_SetLoopGap					ClCutFeature_Resource_StartID+216
// 微连接。
#define IDS_COMMAND_ADD_MicroJoint				ClCutFeature_Resource_StartID+220
#define IDS_COMMAMD_DEL_MicroJoint				ClCutFeature_Resource_StartID+221
#define IDS_COMMAMD_MOD_MJ_WIDTH				ClCutFeature_Resource_StartID+222
#define IDS_COMMAMD_MOD_MJ_LEAD					ClCutFeature_Resource_StartID+223
#define IDS_COMMAMD_MOD_MJ_PT					ClCutFeature_Resource_StartID+224
// 角特征。
#define IDS_COMMAMD_ADD_CORNER					ClCutFeature_Resource_StartID+230
#define IDS_COMMAMD_DEL_CORNER					ClCutFeature_Resource_StartID+231
#define IDS_COMMAMD_MOD_CORNER					ClCutFeature_Resource_StartID+232
// 轮廓刀具
#define IDS_COMMAMD_MOD_COND					ClCutFeature_Resource_StartID+240
#define IDS_COMMAMD_ADD_COND					ClCutFeature_Resource_StartID+241
#define IDS_COMMAMD_DEL_COND					ClCutFeature_Resource_StartID+242
#define IDS_COMMAMD_MOD_TOOLSIZE				ClCutFeature_Resource_StartID+243
#define IDS_COMMAMD_CHANGE_TOOL					ClCutFeature_Resource_StartID+244
#define IDS_COMMAMD_MOD_TOOLCOLOR				ClCutFeature_Resource_StartID+245
#define IDS_COMMAMD_NoCutLoop					ClCutFeature_Resource_StartID+246
//

// the names of the feature
#define IDS_CUTFEATURE_STARTCUT					ClCutFeature_Resource_StartID+250
#define IDS_CUTFEATURE_ENDCUT					ClCutFeature_Resource_StartID+251
#define IDS_CUTFEATURE_MicroJoint				ClCutFeature_Resource_StartID+252
#define IDS_CUTFEATURE_CORNER					ClCutFeature_Resource_StartID+253
//

// the prompt message.
// MicroJoint
#define IDS_CUTFEATURE_DELETE_MicroJoint		ClCutFeature_Resource_StartID+260
#define IDS_CUTFEATURE_SELECT_MicroJoint		ClCutFeature_Resource_StartID+261
#define IDS_CUTFEATURE_GEOM_NOT_HOLD_MicroJoint		ClCutFeature_Resource_StartID+262
#define IDS_MSG_AddMicroJoint					ClCutFeature_Resource_StartID+263
#define IDS_CUTFEATURE_NO_MJ_SELECT				ClCutFeature_Resource_StartID+264
#define IDS_CUTFEATURE_MJ_SELECTED				ClCutFeature_Resource_StartID+265
// CORNER
#define IDS_CUTFEATURE_DELETE_CORNER			ClCutFeature_Resource_StartID+270
#define IDS_CUTFEATURE_EDIT_CORNER				ClCutFeature_Resource_StartID+271
#define IDS_CUTFEATURE_SELECT_CORNER			ClCutFeature_Resource_StartID+272
#define IDS_CUTFEATURE_ASSIGN_SMOOTH_CORNER     ClCutFeature_Resource_StartID+273
#define IDS_CUTFEATURE_CORNER_EXIST				ClCutFeature_Resource_StartID+274
#define IDS_CUTFEATURE_SELECT_CORNER_PT			ClCutFeature_Resource_StartID+275
#define IDS_CUTFEATURE_CORNER_OPEN_LOOP			ClCutFeature_Resource_StartID+276
#define IDS_CUTFEATURE_CORNER_WRONG_VAL			ClCutFeature_Resource_StartID+277
#define IDS_CUTFEATURE_NO_CORNER_SELECT			ClCutFeature_Resource_StartID+278
#define IDS_CUTFEATURE_CORNER_SELECTED			ClCutFeature_Resource_StartID+279
// start/end cut
#define IDS_CUTFEATURE_SELECT_STARTCUT			ClCutFeature_Resource_StartID+280
#define IDS_CHANGE_CUTSIDE					    ClCutFeature_Resource_StartID+281
#define IDS_CHANGE_CUTSIDE_OPENLOOP			    ClCutFeature_Resource_StartID+282
#define IDS_CHANGE_DIRECTION					ClCutFeature_Resource_StartID+283
#define IDS_CUTFEATURE_NO_STARTPT_SELECT		ClCutFeature_Resource_StartID+284
#define IDS_CUTFEATURE_STARTPT_SELECTED			ClCutFeature_Resource_StartID+285
#define IDS_MSG_AdjustStartCutPt				ClCutFeature_Resource_StartID+286
// cam feature
#define IDS_MSG_ASSIGN_CUTFEATURE				ClCutFeature_Resource_StartID+290
#define IDS_MSG_RESET_CUTFEATURE				ClCutFeature_Resource_StartID+291
// 轮廓刀具
#define IDS_MSG_EDIT_LOOP_TOOL			        ClCutFeature_Resource_StartID+300
#define IDS_MSG_NoCutLoop			            ClCutFeature_Resource_StartID+301
#define IDS_MSG_LoopHaveWrongToolSize			ClCutFeature_Resource_StartID+302
//

// 按钮提示信息。
// “自动处理”分组。
#define IDS_ASSIGN_CUTFEATURE_TIP				ClCutFeature_Resource_StartID+320
#define IDS_RESET_CUTFEATURE_TIP				ClCutFeature_Resource_StartID+321
#define IDS_CHECK_INTERFERE_TIP					ClCutFeature_Resource_StartID+322
// “开始/结束切割”分组。
#define IDS_EDIT_LEAD_TIP						ClCutFeature_Resource_StartID+330
#define IDS_EDIT_START_PT_TIP					ClCutFeature_Resource_StartID+331
#define IDS_CHANGE_CUTDIR_TIP					ClCutFeature_Resource_StartID+332
#define IDS_SWITCH_CUTSIDE_TIP					ClCutFeature_Resource_StartID+333
// “微连接”分组。
#define IDS_ADD_MJ_TIP							ClCutFeature_Resource_StartID+340
#define IDS_EDIT_MJ_TIP							ClCutFeature_Resource_StartID+341
#define IDS_DELETE_MJ_TIP						ClCutFeature_Resource_StartID+342
// “角特征”分组。
#define IDS_ADD_CORNER_TIP						ClCutFeature_Resource_StartID+350
#define IDS_EDIT_CORNER_TIP						ClCutFeature_Resource_StartID+351
#define IDS_DELETE_CORNER_TIP					ClCutFeature_Resource_StartID+352
// “轮廓刀具”分组。
#define IDS_EDIT_LOOPTOOL_TIP					ClCutFeature_Resource_StartID+361
//

// for execute task.
#define IDS_MSG_INTF_CHECK_TASK					ClCutFeature_Resource_StartID+380
#define IDS_MSG_INTF_CHECK_LOOP					ClCutFeature_Resource_StartID+381
#define IDS_MSG_INTF_CHECK_ALL_LOOPS			ClCutFeature_Resource_StartID+382
#define IDS_MSG_INTF_CHECK_TASK_OK				ClCutFeature_Resource_StartID+383
#define IDS_MSG_INTF_CHECK_TASK_CANCEL			ClCutFeature_Resource_StartID+384
//
/************************************************************************/


/************************************************************************/
/*                the control created by code                           */

// buttons on panel
#define IDC_CUTFEATURE_ASSIGN					ClCutFeature_Resource_StartID+400
#define IDC_CUTFEATURE_RESET					ClCutFeature_Resource_StartID+401
#define IDC_CHECK_INTERFERE						ClCutFeature_Resource_StartID+402
// MicroJoint
#define IDC_CUTFEATURE_MJ_ADD					ClCutFeature_Resource_StartID+411
#define IDC_CUTFEATURE_MJ_EDIT					ClCutFeature_Resource_StartID+412
#define IDC_CUTFEATURE_MJ_DELETE				ClCutFeature_Resource_StartID+413
// start/end cut
#define IDC_CUTFEATURE_LOOP_LEAD_EDIT			ClCutFeature_Resource_StartID+414
#define IDC_CUTFEATURE_LOOP_START_PT			ClCutFeature_Resource_StartID+415
#define IDC_SWITCH_CUTSIDE						ClCutFeature_Resource_StartID+416
// corner
#define IDC_CUTFEATURE_CORNER_ADD				ClCutFeature_Resource_StartID+418
#define IDC_CUTFEATURE_CORNER_EDIT				ClCutFeature_Resource_StartID+419
#define IDC_CUTFEATURE_CORNER_DELETE			ClCutFeature_Resource_StartID+420
// 轮廓刀具
#define IDC_CUTFEATURE_LOOPTOOL_EDIT			ClCutFeature_Resource_StartID+425
#define IDC_CUTFEATURE_LoopNoCut			    ClCutFeature_Resource_StartID+426

// used for "*PropBar"
#define IDC_PROPBAR_CORNERPROP					ClCutFeature_Resource_StartID+440
#define IDC_PROPBAR_MicroJointPROP				ClCutFeature_Resource_StartID+441
#define IDC_PROPBAR_STARTCUTPROP				ClCutFeature_Resource_StartID+442
#define IDC_PROPBAR_ENDCUTPROP					ClCutFeature_Resource_StartID+443
#define IDC_SET_STARTCUT_POINT					ClCutFeature_Resource_StartID+445
#define IDC_SET_MJ_POINT						ClCutFeature_Resource_StartID+446
#define IDC_PROPBAR_LOOPLEAD					ClCutFeature_Resource_StartID+447
#define IDC_PROPBAR_LOOPTOOL					ClCutFeature_Resource_StartID+448
//

// used for "CutFeatureViewBar"
#define IDC_VIEWBAR_CUTFEATURE					ClCutFeature_Resource_StartID+450
#define IDC_TREEVIEW_CUTFEATURE					ClCutFeature_Resource_StartID+460
#define IDC_CUTFEATURE_DELETE					ClCutFeature_Resource_StartID+461
//

/************************************************************************/


/************************************************************************/
// 自定义消息

// 发送这个消息给板材视图。当把一个轮廓设为“不加工”时，需要查询该轮廓所在工序（比如连割工序）中的其他轮廓，那些轮廓也要设置为不加工。
// 那就需要把轮廓刀具ID送给板材视图，板材视图通过lParam返回其他的轮廓刀具。
#define WM_QueryOtherLoopTools	                ClCutFeature_Resource_StartID+480
/************************************************************************/