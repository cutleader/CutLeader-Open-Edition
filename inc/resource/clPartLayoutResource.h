
///////////////////////////////////////////////////////////////////////////////
// ID range is (14500~15000)
///////////////////////////////////////////////////////////////////////////////

#define ClPartLayout_Resource_StartID				14500


/************************************************************************/
// the string ID.

#define IDS_PART_LAYOUT						ClPartLayout_Resource_StartID+1

// names for data
#define IDS_PARTPMT							ClPartLayout_Resource_StartID+3
#define IDS_PARTPMT_LIST					ClPartLayout_Resource_StartID+4
#define IDS_PARTPMT_TYPE					ClPartLayout_Resource_StartID+5
#define IDS_PARTPMT_SINGLE					ClPartLayout_Resource_StartID+6
#define IDS_PARTPMT_GRID					ClPartLayout_Resource_StartID+7
#define IDS_PARTINST						ClPartLayout_Resource_StartID+8
//

// used for "view" panel
#define IDS_PARTPMT_VIEW_ZOOMALL			ClPartLayout_Resource_StartID+15
#define IDS_PARTPMT_VIEW_ZOOMIN				ClPartLayout_Resource_StartID+16
#define IDS_PARTPMT_VIEW_ZOOMOUT			ClPartLayout_Resource_StartID+17
#define IDS_PARTPMT_VIEW_OFFSET				ClPartLayout_Resource_StartID+18
//

// “窗口”分组。
#define IDS_PARTPMT_VIEW_ALL_PART			ClPartLayout_Resource_StartID+20
//

// used for "PrtPmtAddPane"
#define IDS_PROPBAR_AddPRTPMT				ClPartLayout_Resource_StartID+25
//

// used for "PartPmtPropBar"
#define IDS_PROPBAR_EDITPRTPMT				ClPartLayout_Resource_StartID+30
#define IDS_CATEGORY_PARTPMT_DATA			ClPartLayout_Resource_StartID+31
#define IDS_CATEGORY_PARTPMT_GRID			ClPartLayout_Resource_StartID+32
#define IDS_PROP_PARTNAME					ClPartLayout_Resource_StartID+33
#define IDS_SET_GRID_GAP					ClPartLayout_Resource_StartID+34
//

// used for "PartPmtViewBar"
#define IDS_VIEWBAR_PARTPMT					ClPartLayout_Resource_StartID+40
#define IDS_VIEWBAR_PARTTOP					ClPartLayout_Resource_StartID+41
//

// names for commands
#define IDS_COMMAND_MOVE_PARTPMT			ClPartLayout_Resource_StartID+50
#define IDS_COMMAND_ADD_PARTPMT				ClPartLayout_Resource_StartID+51
#define IDS_COMMAND_DEL_PARTPMT				ClPartLayout_Resource_StartID+52
#define IDS_COMMAND_EDITPROP_PRTPMT			ClPartLayout_Resource_StartID+53
#define IDS_COMMAND_GRID_PRTPMT				ClPartLayout_Resource_StartID+54
#define IDS_COMMAND_COPY_PARTPMT			ClPartLayout_Resource_StartID+55
#define IDS_COMMAND_ROTATE_PARTPMT			ClPartLayout_Resource_StartID+56
#define IDS_COMMAND_CHG_GRID_SPACE			ClPartLayout_Resource_StartID+57
#define IDS_COMMAND_EXPLODE_GRID			ClPartLayout_Resource_StartID+58
#define IDS_COMMAND_CreatePartGroup			ClPartLayout_Resource_StartID+59
#define IDS_COMMAND_ExplodePartGroup		ClPartLayout_Resource_StartID+60
//

// the messages.
// select
#define IDS_OUTPUT_SELECTPRTPMT				ClPartLayout_Resource_StartID+80
#define IDS_OUTPUT_SELECTPRTPMT_EDIT		ClPartLayout_Resource_StartID+81
#define IDS_OUTPUT_SELECTPRTPMT_NEXTPT		ClPartLayout_Resource_StartID+82
#define IDS_OUTPUT_NOPRTPMT_SELECT			ClPartLayout_Resource_StartID+83
#define IDS_OUTPUT_SELECT_GRID_PART			ClPartLayout_Resource_StartID+84
#define IDS_OUTPUT_SelectLoopInstance		ClPartLayout_Resource_StartID+85
// add part pmt
#define IDS_OUTPUT_PARTPMT_BASEPT			ClPartLayout_Resource_StartID+90
#define IDS_MSG_PART_DB_CLICK_ADD			ClPartLayout_Resource_StartID+91
// grid
#define IDS_OUTPUT_GRID_SEL_PART			ClPartLayout_Resource_StartID+100
#define IDS_OUTPUT_GRID_PARTPMT				ClPartLayout_Resource_StartID+101
#define IDS_OUTPUT_GRID_XY_NUM				ClPartLayout_Resource_StartID+102
#define IDS_MSG_PARTGRID_GAP				ClPartLayout_Resource_StartID+103
#define IDS_MSG_EXPLODE_PARTGRID			ClPartLayout_Resource_StartID+104
#define IDS_OUTPUT_GRID_PART_SELECTED		ClPartLayout_Resource_StartID+105
// copy
#define IDS_OUTPUT_COPYPARTPMT_NEWLOCATION	ClPartLayout_Resource_StartID+110
#define IDS_OUTPUT_SELECT_PRTPMT_COPY		ClPartLayout_Resource_StartID+111
// rotate
#define IDS_OUTPUT_ROTATEPARTPMT_BASEPT		ClPartLayout_Resource_StartID+120
#define IDS_OUTPUT_ROTATEPARTPMT_ANGLE		ClPartLayout_Resource_StartID+121
#define IDS_OUTPUT_SELECT_PRTPMT_ROTATE		ClPartLayout_Resource_StartID+122
// move
#define IDS_OUTPUT_MOVEPARTPMT_NEWLOCATION	ClPartLayout_Resource_StartID+130
#define IDS_OUTPUT_SELECT_PRTPMT_MOVE		ClPartLayout_Resource_StartID+131
// 删除
#define IDS_OUTPUT_SELECT_PRTPMT_DEL		ClPartLayout_Resource_StartID+140
// start/end cut
#define IDS_OUTPUT_SELECT_STARTCUT			ClPartLayout_Resource_StartID+150
#define IDS_OUTPUT_SELECT_ENDCUT			ClPartLayout_Resource_StartID+151
// 测量距离。
#define IDS_MEASURE_DIS_FST_LOOP			ClPartLayout_Resource_StartID+160
#define IDS_MEASURE_DIS_FST_PT				ClPartLayout_Resource_StartID+161
#define IDS_MEASURE_DIS_SND_LOOP			ClPartLayout_Resource_StartID+162
#define IDS_MEASURE_DIS_SND_PT				ClPartLayout_Resource_StartID+163
#define IDS_MEASURE_DIS_RESULT_1			ClPartLayout_Resource_StartID+164
#define IDS_MEASURE_DIS_RESULT_2			ClPartLayout_Resource_StartID+165
#define IDS_MEASURE_DIS_RESULT_3			ClPartLayout_Resource_StartID+166
// group
#define IDS_MSG_PartGroup_1			        ClPartLayout_Resource_StartID+170
#define IDS_MSG_PartGroup_2			        ClPartLayout_Resource_StartID+171
//

// used for popup menu.
#define IDS_ACTION_EDIT_PART_ACTION			ClPartLayout_Resource_StartID+180
//

// 各个分组。
// “增加零件”分组
#define IDS_PARTPMT_ADDPART					ClPartLayout_Resource_StartID+200
#define IDS_PARTPMT_FROM_DB					ClPartLayout_Resource_StartID+201
#define IDS_PARTPMT_FROM_DXF				ClPartLayout_Resource_StartID+202
#define IDS_AddPartsByNest					ClPartLayout_Resource_StartID+203
// "编辑零件布局"分组。
#define IDS_PARTPMT_SELECT					ClPartLayout_Resource_StartID+210
#define IDS_EDIT_LAYOUT						ClPartLayout_Resource_StartID+211
#define IDS_PARTPMT_OPEN_PART				ClPartLayout_Resource_StartID+212
#define IDS_PARTPMT_CREATE_GRID				ClPartLayout_Resource_StartID+213
#define IDS_PARTPMT_EXPLODE_GRID			ClPartLayout_Resource_StartID+214
#define IDS_PARTPMT_GRID_GAP				ClPartLayout_Resource_StartID+215
#define IDS_SelectLoopInstance				ClPartLayout_Resource_StartID+216
// "编辑切割特征"分组
#define IDS_EDIT_FEATURE					ClPartLayout_Resource_StartID+220
// 生成余料
#define IDS_GEN_SKELE_REMANT				ClPartLayout_Resource_StartID+225
// 测量距离
#define IDS_MEASURE_DISTANCE				ClPartLayout_Resource_StartID+230
//

// 按钮提示信息。
// “创建零件阵列”对话框
#define IDS_PART_GRID_PREVIEW				ClPartLayout_Resource_StartID+240
#define IDS_PART_GRID_OK					ClPartLayout_Resource_StartID+241
#define IDS_PART_GRID_CANCEL				ClPartLayout_Resource_StartID+242
// “增加零件”分组
#define IDS_PARTPMT_FROM_DB_TIP				ClPartLayout_Resource_StartID+250
#define IDS_PARTPMT_FROM_DXF_TIP			ClPartLayout_Resource_StartID+251
#define IDS_NestTaskParts_TIP				ClPartLayout_Resource_StartID+252
// "编辑零件布局"分组。
#define IDS_PARTPMT_SELECT_TIP				ClPartLayout_Resource_StartID+260
#define IDS_PARTPMT_OPEN_PART_TIP			ClPartLayout_Resource_StartID+261
#define IDS_PARTPMT_CREATE_GRID_TIP			ClPartLayout_Resource_StartID+262
#define IDS_PARTPMT_EXPLODE_GRID_TIP		ClPartLayout_Resource_StartID+263
#define IDS_PARTPMT_DELETE_TIP				ClPartLayout_Resource_StartID+264
#define IDS_PARTPMT_COPY_TIP				ClPartLayout_Resource_StartID+265
#define IDS_PARTPMT_MOVE_TIP				ClPartLayout_Resource_StartID+266
#define IDS_PARTPMT_ROTATE_TIP				ClPartLayout_Resource_StartID+267
// 其他
#define IDS_PARTPMT_VIEW_OFFSET_TIP			ClPartLayout_Resource_StartID+270
#define IDS_SwitchSelectObject			    ClPartLayout_Resource_StartID+271
#define IDS_QuitCurrentAction			    ClPartLayout_Resource_StartID+272
#define IDS_VIEWBAR_PARTPMT_TIP				ClPartLayout_Resource_StartID+275
#define IDS_GEN_SKELE_REMANT_TIP			ClPartLayout_Resource_StartID+280
#define IDS_MEASURE_DISTANCE_TIP			ClPartLayout_Resource_StartID+285
//

// for execute task.
#define IDS_MSG_INTF_CHECK_TASK_EX			ClPartLayout_Resource_StartID+290
#define IDS_MSG_INTF_CHECK_PART				ClPartLayout_Resource_StartID+291
//
/************************************************************************/


/************************************************************************/
/*                the control created by code                           */

#define IDC_PARTPMT_FROM_DB					ClPartLayout_Resource_StartID+300
#define IDC_PARTPMT_FROM_DXF				ClPartLayout_Resource_StartID+301
#define IDC_PARTPMT_SELECT					ClPartLayout_Resource_StartID+302
#define IDC_NestTaskParts					ClPartLayout_Resource_StartID+303

// used for "view" panel
#define IDC_PARTPMT_VIEW_ZOOMALL			ClPartLayout_Resource_StartID+310
#define IDC_PARTPMT_VIEW_ZOOMIN				ClPartLayout_Resource_StartID+311
#define IDC_PARTPMT_VIEW_ZOOMOUT			ClPartLayout_Resource_StartID+312
#define IDC_PARTPMT_VIEW_OFFSET				ClPartLayout_Resource_StartID+313
//

// used for "edit" panel
#define IDC_PARTPMT_EDIT_COPY				ClPartLayout_Resource_StartID+321
#define IDC_PARTPMT_EDIT_DELETE				ClPartLayout_Resource_StartID+322
#define IDC_PARTPMT_EDIT_MOVE				ClPartLayout_Resource_StartID+323
#define IDC_PARTPMT_EDIT_ROTATE				ClPartLayout_Resource_StartID+324
#define IDC_PARTPMT_EDIT_GRID				ClPartLayout_Resource_StartID+325
#define IDC_PARTPMT_EXPLODE_GRID			ClPartLayout_Resource_StartID+326
#define IDC_PARTPMT_CreatePartGroup			ClPartLayout_Resource_StartID+327
#define IDC_PARTPMT_ExplodePartGroup		ClPartLayout_Resource_StartID+328
//

// 余料
#define IDC_GEN_SKELE_REMNANT				ClPartLayout_Resource_StartID+330
//

// 测量距离
#define IDC_MEASURE_DISTANCE				ClPartLayout_Resource_StartID+335
//

// used for "windows" panel
#define IDC_PARTPMT_VIEWBAR_PARTPMT			ClPartLayout_Resource_StartID+350
#define IDC_PARTPMT_VIEWBAR_PARTTOP			ClPartLayout_Resource_StartID+351
//

// used for "PrtPmtAddPane".
#define IDC_PROPBAR_AddPRTPMT				ClPartLayout_Resource_StartID+400
//

// used for "PartPmtPropBar".
#define IDC_PROPBAR_EDITPRTPMT				ClPartLayout_Resource_StartID+420
#define IDC_SET_GRID_GAP					ClPartLayout_Resource_StartID+421

// used for "PartPmtViewBar".
#define IDC_VIEWBAR_PARTPMT					ClPartLayout_Resource_StartID+430
#define IDC_TREEVIEW_PARTPMT				ClPartLayout_Resource_StartID+431

// use for "PartTopBar"
#define IDC_VIEWBAR_PARTTOP					ClPartLayout_Resource_StartID+440
#define IDC_TREEVIEW_PARTTOP				ClPartLayout_Resource_StartID+441
/************************************************************************/


/************************************************************************/
// 自定义消息

// 发送这个消息给任务视图，告诉任务视图刚刚在板材上添加了零件。任务视图会根据任务零件中有无这些零件，用适当的方式更新任务对象和零件管理器面板。
#define WM_PartAddedOnSheet					ClPartLayout_Resource_StartID+480

// 由于零件布局修改，发这个消息给板材视图要求更新板材的缩略图。
#define WM_updateSheetPreviewData	        ClPartLayout_Resource_StartID+490

// 由于零件布局修改，发这个消息给任务视图重新显示板材管理器面板上的某个板材的缩略图。
#define WM_redrawSheetThumbnailOnSheetManagementPanel	ClPartLayout_Resource_StartID+491

// 由于板材上零件布局变化，发这个消息给任务视图要求重新显示每个“任务零件豆腐块”上的零件已排数量。
#define WM_ReDisplayNestedCountOfEachPart	ClPartLayout_Resource_StartID+493

// “排版任务中零件”功能，由于任务视图不能直接接受工具条命令，所以需要板材视图发消息给任务视图。
#define WM_NestTaskParts	                ClPartLayout_Resource_StartID+494
#define WM_NestTaskPartsOnCcdMaterial       ClPartLayout_Resource_StartID+495

// 发送这个消息给板材视图，要求显示或隐藏切割路径，消息值为1表示显示，0表示隐藏。
#define WM_HideOrShowCutPath	            ClPartLayout_Resource_StartID+496

// 发送这个消息给板材视图，要求进入选择零件或者选择轮廓动作。
#define WM_EnterSelectPartPlacementOrLoopInstanceAction	    ClPartLayout_Resource_StartID+497 // 消息参数：1选择轮廓实例，2选择零件放置，3编辑器参考上一次选择的对象进行选择，这个消息不会预先选择对象。
#define WM_EnterSelectPartPlacementAction	                ClPartLayout_Resource_StartID+498 // 参数中携带要预先选中的零件放置ID集合的指针，消息接收者负责释放。
#define WM_EnterSelectLoopInstanceAction	                ClPartLayout_Resource_StartID+499 // 参数中携带要预先选中的轮廓实例集合(vector<pair<零件实例id,几何轮廓id>>)的指针，消息接收者负责释放。
/************************************************************************/
