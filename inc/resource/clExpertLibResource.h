
///////////////////////////////////////////////////////////////////////////////
// ID range is (15000~15500)
///////////////////////////////////////////////////////////////////////////////

#define ClExpertLib_Resource_StartID				15000

/************************************************************************/
/*                the string ID                                         */

#define IDS_EXPERTLIBRARY					ClExpertLib_Resource_StartID+1
#define IDS_EXPLIB_ITEM						ClExpertLib_Resource_StartID+2
#define IDS_CAM_PARAM						ClExpertLib_Resource_StartID+3

// the task type.
#define IDS_TASK_SAVE_EXPLIB				ClExpertLib_Resource_StartID+10
//

// 参数配置。
#define IDS_PARAM_CONFIG					ClExpertLib_Resource_StartID+50
#define IDS_NEW_PARAM_CONFIG				ClExpertLib_Resource_StartID+51
//

// 参数配置项。
#define IDS_LEAD_CONFIG						ClExpertLib_Resource_StartID+60
#define IDS_PIERCE_CONFIG					ClExpertLib_Resource_StartID+61
#define IDS_MJ_CONFIG						ClExpertLib_Resource_StartID+62
#define IDS_CORNER_CONFIG					ClExpertLib_Resource_StartID+63
#define IDS_SEQUENCE_CONFIG					ClExpertLib_Resource_StartID+64
#define IDS_NC_CONFIG						ClExpertLib_Resource_StartID+65
#define IDS_NEST_PARAM						ClExpertLib_Resource_StartID+66
#define IDS_TOOL_ASSIGN						ClExpertLib_Resource_StartID+67
//

// used for Micro Joint config.
#define IDS_RANGEONE						ClExpertLib_Resource_StartID+71
#define IDS_RANGETWO						ClExpertLib_Resource_StartID+72
#define IDS_RANGETHREE						ClExpertLib_Resource_StartID+73
//

// used for Lead In/Out config.
#define IDS_INTERNAL_GEOMETRY				ClExpertLib_Resource_StartID+80
#define IDS_EXTERNAL_GEOMETRY				ClExpertLib_Resource_StartID+81

#define IDS_LEADINTYPE_LINE					ClExpertLib_Resource_StartID+82
#define IDS_LEADINTYPE_TWOLINES				ClExpertLib_Resource_StartID+83
#define IDS_LEADINTYPE_LINEANDARC			ClExpertLib_Resource_StartID+84
#define IDS_LEADINTYPE_HOOK					ClExpertLib_Resource_StartID+85
#define IDS_LEADINTYPE_NONE					ClExpertLib_Resource_StartID+86

#define IDS_LEADOUTTYPE_LINE				ClExpertLib_Resource_StartID+88
#define IDS_LEADOUTTYPE_ARC					ClExpertLib_Resource_StartID+89
#define IDS_LEADOUTTYPE_LINEANDARC			ClExpertLib_Resource_StartID+90
#define IDS_LEADOUTTYPE_NONE				ClExpertLib_Resource_StartID+91
//

// used for corner assign.
#define IDS_CORNERTYPE_NONE					ClExpertLib_Resource_StartID+92
#define IDS_CORNERTYPE_CROSS				ClExpertLib_Resource_StartID+93
#define IDS_CORNERTYPE_RADIUS				ClExpertLib_Resource_StartID+94
#define IDS_CORNERTYPE_TAB					ClExpertLib_Resource_StartID+95
#define IDS_CORNERTYPE_LOOP					ClExpertLib_Resource_StartID+96
#define IDS_CORNERTYPE_CoolingPoint 		ClExpertLib_Resource_StartID+97
//

// 用于排版
#define IDS_NESTALG_REMNANT_RECT			ClExpertLib_Resource_StartID+100
#define IDS_NESTALG_TRUE_SHAPE				ClExpertLib_Resource_StartID+101
//

// used for condition assign.
// for DefaultCondDlg and RangeCondDlg.
#define IDS_MIN_WIDTH						ClExpertLib_Resource_StartID+110
#define IDS_MAX_WIDTH						ClExpertLib_Resource_StartID+111
#define IDS_ZONE_FROM						ClExpertLib_Resource_StartID+112
#define IDS_ZONE_TO							ClExpertLib_Resource_StartID+113
#define IDS_LENGTH_FROM						ClExpertLib_Resource_StartID+114
#define IDS_LENGTH_TO						ClExpertLib_Resource_StartID+115
#define IDS_RADIUS_FROM						ClExpertLib_Resource_StartID+116
#define IDS_RADIUS_TO						ClExpertLib_Resource_StartID+117
// for CondAssignPage.
#define IDS_TAB_DEFAULT						ClExpertLib_Resource_StartID+120
#define IDS_TAB_STD_SHAPE					ClExpertLib_Resource_StartID+121
#define IDS_TAB_POLYGON						ClExpertLib_Resource_StartID+122
#define IDS_TAB_LINE						ClExpertLib_Resource_StartID+123
#define IDS_TAB_ARC							ClExpertLib_Resource_StartID+124
//

// 刀具分配
#define IDS_NEW_TOOL                        ClExpertLib_Resource_StartID+140
#define IDS_LOOP_WIDTH_SMALLER_THAN         ClExpertLib_Resource_StartID+141
#define IDS_LOOP_HEIGHT_SMALLER_THAN        ClExpertLib_Resource_StartID+142
#define IDS_TOOL                            ClExpertLib_Resource_StartID+143
#define IDS_TOOL_CUTSIDE                    ClExpertLib_Resource_StartID+144
#define IDS_TOOL_CUTSIDE_CUTON              ClExpertLib_Resource_StartID+145
#define IDS_TOOL_CUTSIDE_AUTO               ClExpertLib_Resource_StartID+146
#define IDS_TOOL_MAP_LAYERNAME              ClExpertLib_Resource_StartID+147
//

// 碰撞避免
#define IDS_ADJUST_STARTPT_INNERLOOP		ClExpertLib_Resource_StartID+160
#define IDS_ADJUST_STARTPT_ALLLOOP			ClExpertLib_Resource_StartID+161
//

// the prompt message
// 专家库项。
#define IDS_MSG_SAVE_EXPLIB_ITEM			ClExpertLib_Resource_StartID+230
#define IDS_MSG_EXPLIB_ITEM_NOT_CHANGE		ClExpertLib_Resource_StartID+231
#define IDS_MSG_EXPLIB_ITEM_SAVED			ClExpertLib_Resource_StartID+232
#define IDS_MSG_DEL_EXPLIB_ITEM				ClExpertLib_Resource_StartID+233
#define IDS_MSG_DEL_RESERVED_EXPLIBITEM		ClExpertLib_Resource_StartID+234
#define IDS_MSG_DEL_DFL_EXPLIBITEM			ClExpertLib_Resource_StartID+235
#define IDS_MSG_DEL_DFL_SCHEME				ClExpertLib_Resource_StartID+236
// 参数配置。
#define IDS_MSG_PARAM_CONFIG_EXIST			ClExpertLib_Resource_StartID+240
#define IDS_MSG_DEL_PARAM_CONFIG			ClExpertLib_Resource_StartID+241
#define IDS_MSG_SelectParamConfigSubNode	ClExpertLib_Resource_StartID+242
// nest param
#define IDS_MSG_SAVE_NESTPARAM				ClExpertLib_Resource_StartID+255
#define IDS_MSG_NESTPARAM_NOT_CHANGE		ClExpertLib_Resource_StartID+256
#define IDS_MSG_NESTPARAM_SAVED				ClExpertLib_Resource_StartID+257
// lead rules
#define IDS_MSG_SAVE_LEAD					ClExpertLib_Resource_StartID+260
#define IDS_MSG_LEAD_NOT_CHANGE				ClExpertLib_Resource_StartID+261
#define IDS_MSG_LEAD_SAVED					ClExpertLib_Resource_StartID+262
// pierce rules
#define IDS_MSG_SAVE_PIERCE					ClExpertLib_Resource_StartID+265
#define IDS_MSG_PIERCE_NOT_CHANGE			ClExpertLib_Resource_StartID+266
#define IDS_MSG_PIERCE_SAVED				ClExpertLib_Resource_StartID+267
#define IDS_MSG_CHANGE_PIERCE_PT			ClExpertLib_Resource_StartID+268
// bmj rules
#define IDS_MSG_SAVE_MJ_ASSIGN				ClExpertLib_Resource_StartID+270
#define IDS_MSG_MJ_ASSIGN_NOT_CHANGE		ClExpertLib_Resource_StartID+271
#define IDS_MSG_MJ_ASSIGN_SAVED			    ClExpertLib_Resource_StartID+272
// corner rules
#define IDS_MSG_SAVE_CORNER_ASSIGN			ClExpertLib_Resource_StartID+275
#define IDS_MSG_CORNER_NOT_CHANGE			ClExpertLib_Resource_StartID+276
#define IDS_MSG_CORNER_SAVED				ClExpertLib_Resource_StartID+277
#define IDS_MSG_CORNER_PREVIEW				ClExpertLib_Resource_StartID+278
#define IDS_MSG_CornerAssignRule_2			ClExpertLib_Resource_StartID+279
// condition rules
#define IDS_MSG_SAVE_COND_ASSIGN			ClExpertLib_Resource_StartID+280
#define IDS_MSG_COND_ASSIGN_NOT_CHANGE		ClExpertLib_Resource_StartID+281
#define IDS_MSG_COND_ASSIGN_SAVED			ClExpertLib_Resource_StartID+282
#define IDS_MSG_CHANGE_COND_RULE			ClExpertLib_Resource_StartID+283
// sequence rules
#define IDS_MSG_SAVE_SEQ_CONFIG				ClExpertLib_Resource_StartID+285
#define IDS_MSG_SEQ_CONFIG_NOT_CHANGE		ClExpertLib_Resource_StartID+286
#define IDS_MSG_SEQ_CONFIG_SAVED			ClExpertLib_Resource_StartID+287
#define IDS_MSG_SUBRT_AVOID					ClExpertLib_Resource_StartID+288
// nc rules
#define IDS_MSG_SAVE_NC_CONFIG				ClExpertLib_Resource_StartID+290
#define IDS_MSG_NC_CONFIG_NOT_CHANGE		ClExpertLib_Resource_StartID+291
#define IDS_MSG_NC_CONFIG_SAVED				ClExpertLib_Resource_StartID+292
// used for MacSelectDlg.
#define IDS_MSG_MAC_CONFIGED				ClExpertLib_Resource_StartID+310
#define IDS_MSG_MAC_SELECT					ClExpertLib_Resource_StartID+311
#define IDS_MSG_MAC_HAS_CONFIGED			ClExpertLib_Resource_StartID+312
// 刀具分配
#define IDS_MSG_DELETE_TOOL                 ClExpertLib_Resource_StartID+320
#define IDS_MSG_DELETE_DEFAULT_TOOL         ClExpertLib_Resource_StartID+321
#define IDS_MSG_DELETE_TOOL_ASSIGN_RULE     ClExpertLib_Resource_StartID+322
#define IDS_MSG_SAVE_TOOL_CONFIG            ClExpertLib_Resource_StartID+323
#define IDS_MSG_TOOL_CONFIG_NOT_CHANGE      ClExpertLib_Resource_StartID+324
#define IDS_MSG_TOOL_CONFIG_SAVED           ClExpertLib_Resource_StartID+325
#define IDS_MSG_TOOL_RENAME                 ClExpertLib_Resource_StartID+326
#define IDS_MSG_DELETE_USED_TOOL			ClExpertLib_Resource_StartID+327
//
/************************************************************************/


/************************************************************************/
/*                the control created by code                           */

// used in "Configuration" ribbon bar.
#define IDC_CONFIGURATION_MAC_PARAM			ClExpertLib_Resource_StartID+401
#define IDC_CONFIGURATION_MAC_CONDITION		ClExpertLib_Resource_StartID+402
#define IDC_CONFIGURATION_LEAD_CONFIG		ClExpertLib_Resource_StartID+403
#define IDC_CONFIGURATION_PIERCE_CONFIG		ClExpertLib_Resource_StartID+404
#define IDC_CONFIGURATION_MJ_CONFIG			ClExpertLib_Resource_StartID+405
#define IDC_CONFIGURATION_CONDITION_CONFIG	ClExpertLib_Resource_StartID+406
#define IDC_CONFIGURATION_CORNER_CONFIG		ClExpertLib_Resource_StartID+407
#define IDC_CONFIGURATION_SEQUENCE_CONFIG	ClExpertLib_Resource_StartID+408
#define IDC_CONFIGURATION_NEST_PARAM		ClExpertLib_Resource_StartID+409
#define IDC_CONFIGURATION_NC_CONFIG			ClExpertLib_Resource_StartID+410
//

// for popup
#define IDC_CREATE_PARAM_CONFIG				ClExpertLib_Resource_StartID+420
#define IDC_RENAME_PARAM_CONFIG				ClExpertLib_Resource_StartID+421
#define IDC_DELETE_PARAM_CONFIG				ClExpertLib_Resource_StartID+422
#define IDC_COPY_PARAM_CONFIG				ClExpertLib_Resource_StartID+423

#define IDC_NEW_EXPLIBITEM					ClExpertLib_Resource_StartID+430
#define IDC_DEL_EXPLIB_ITEM					ClExpertLib_Resource_StartID+431
#define IDC_IMPORT_EXPLIBITEM				ClExpertLib_Resource_StartID+432
#define IDC_EXPORT_EXPLIBITEM				ClExpertLib_Resource_StartID+433
//
/************************************************************************/
