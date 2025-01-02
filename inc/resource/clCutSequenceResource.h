
///////////////////////////////////////////////////////////////////////////////
// ID range is (15500~16500)
///////////////////////////////////////////////////////////////////////////////

#define ClCutSequence_Resource_StartID				15500

/************************************************************************/
/*                the string ID                                         */
#define IDS_SEQ								ClCutSequence_Resource_StartID+1
#define IDS_SEQNC							ClCutSequence_Resource_StartID+2

// for execute task.
#define IDS_GEN_CUTNODE_TASK				ClCutSequence_Resource_StartID+50
#define IDS_MSG_OFFSET_PART					ClCutSequence_Resource_StartID+51
#define IDS_MSG_CHECK_INTF					ClCutSequence_Resource_StartID+52
#define IDS_MSG_GEN_CUTPATH					ClCutSequence_Resource_StartID+53
#define IDS_MSG_GEN_CUTPATH_OK				ClCutSequence_Resource_StartID+54
//

// names for data
#define IDS_SEQUNIT							ClCutSequence_Resource_StartID+100
#define IDS_CUTSEQ_LIST						ClCutSequence_Resource_StartID+101
#define IDS_LOOP_SEQUNIT					ClCutSequence_Resource_StartID+102
#define IDS_COMCUT_SEQUNIT					ClCutSequence_Resource_StartID+103
#define IDS_SUBRT_SEQUNIT					ClCutSequence_Resource_StartID+104
#define IDS_SUBRT_GRID_SEQUNIT				ClCutSequence_Resource_StartID+105
#define IDS_LOOP_GRID_SEQUNIT				ClCutSequence_Resource_StartID+106
#define IDS_BDGE_SEQUNIT					ClCutSequence_Resource_StartID+107
#define IDS_CHAIN_SEQUNIT					ClCutSequence_Resource_StartID+108
#define IDS_GRID_SEQUNIT					ClCutSequence_Resource_StartID+109
#define IDS_FLYCUT_SEQUNIT					ClCutSequence_Resource_StartID+110
#define IDS_MOVE_PT							ClCutSequence_Resource_StartID+111
#define IDS_REMNANT_CUTLINE					ClCutSequence_Resource_StartID+112

#define IDS_CUTNODE							ClCutSequence_Resource_StartID+140
#define IDS_LINE_CUTNODE					ClCutSequence_Resource_StartID+141
#define IDS_ARC_CUTNODE						ClCutSequence_Resource_StartID+142
#define IDS_SELECTTOOL_CUTNODE				ClCutSequence_Resource_StartID+143
#define IDS_TRACE_CUTNODE					ClCutSequence_Resource_StartID+144
#define IDS_DOPIERCE_CUTNODE				ClCutSequence_Resource_StartID+145
#define IDS_UNPIERCE_CUTNODE				ClCutSequence_Resource_StartID+146
#define IDS_LOOPINFO_CUTNODE				ClCutSequence_Resource_StartID+147
#define IDS_CoolingPoint_CutNode			ClCutSequence_Resource_StartID+148
//

// used for first panel
#define IDS_SEQ_AUTOSEQUENCE				ClCutSequence_Resource_StartID+203
#define IDS_SEQ_CLEARALLSEQUENCE			ClCutSequence_Resource_StartID+204
#define IDS_SEQ_ResetSequence				ClCutSequence_Resource_StartID+205
//

// “编辑工序”分组。
#define IDS_SEQ_EDIT						ClCutSequence_Resource_StartID+210
#define IDS_SEQ_ADD_LOOP_SEQ				ClCutSequence_Resource_StartID+211
#define IDS_SEQ_INSERT_LOOP_SEQ				ClCutSequence_Resource_StartID+212
#define IDS_SEQ_REPLACE_HOLE_SEQ			ClCutSequence_Resource_StartID+213
#define IDS_SEQ_REMOVESEQUENCE				ClCutSequence_Resource_StartID+214
#define IDS_SEQ_SELECTSEQUENCE				ClCutSequence_Resource_StartID+215
#define IDS_SEQ_ADD_BDGE_SEQ				ClCutSequence_Resource_StartID+216
#define IDS_SEQ_BREAK_BDGE_SEQ				ClCutSequence_Resource_StartID+217
#define IDS_SEQ_BDGE_START_LOOP				ClCutSequence_Resource_StartID+218
#define IDS_SEQ_SORTSEQUENCE				ClCutSequence_Resource_StartID+219
#define IDS_SEQ_ADD_MOV_PT					ClCutSequence_Resource_StartID+220
#define IDS_SEQ_DEL_MOV_PT					ClCutSequence_Resource_StartID+221
#define IDS_SEQ_ADD_CHAIN_SEQ				ClCutSequence_Resource_StartID+222
#define IDS_SEQ_INSERT_CHAIN_NODE			ClCutSequence_Resource_StartID+223
#define IDS_SEQ_DEL_CHAIN_NODE				ClCutSequence_Resource_StartID+224
#define IDS_SEQ_ADD_GRID_SEQ				ClCutSequence_Resource_StartID+225
#define IDS_SEQ_AddFlyCut					ClCutSequence_Resource_StartID+226
#define IDS_SEQ_AUTO_REM_CUTLINE			ClCutSequence_Resource_StartID+227
#define IDS_SEQ_CREATE_REM_CUTLINE			ClCutSequence_Resource_StartID+228
#define IDS_SEQ_DELETE_REM_CUTLINE			ClCutSequence_Resource_StartID+229
#define IDS_SEQ_INSERT_CHAIN_RESET			ClCutSequence_Resource_StartID+230
#define IDS_SEQ_DEL_CHAIN_RESET				ClCutSequence_Resource_StartID+231
//

// for "View" panel
#define IDS_SEQ_VIEW						ClCutSequence_Resource_StartID+240
#define IDS_SEQ_VIEWSEQUENCE				ClCutSequence_Resource_StartID+241
//

// “窗口”分组。
#define IDS_SEQ_LIST_SEQ					ClCutSequence_Resource_StartID+245
//

// names for commands
#define IDS_SEQ_DELETE_SEQ					ClCutSequence_Resource_StartID+251
#define IDS_SEQ_INSERT_SEQ					ClCutSequence_Resource_StartID+252
#define IDS_SEQ_ADD_BDGE_NODE				ClCutSequence_Resource_StartID+253
#define IDS_SEQ_ADD_BDGESEQ					ClCutSequence_Resource_StartID+254
#define IDS_SEQ_INSERT_BDGE_NODE			ClCutSequence_Resource_StartID+255
#define IDS_SEQ_LINK_BDGE_SEQ				ClCutSequence_Resource_StartID+256
#define IDS_SEQ_BREAK_BDGESEQ				ClCutSequence_Resource_StartID+257
#define IDS_SEQ_CHANGE_BDGE_START			ClCutSequence_Resource_StartID+258
#define IDS_SEQ_CHANGE_SEQ_POS				ClCutSequence_Resource_StartID+259
#define IDS_SEQ_ADD_CHAIN_NODE_CMD			ClCutSequence_Resource_StartID+260
#define IDS_SEQ_ADD_CHAIN_SEQ_CMD			ClCutSequence_Resource_StartID+261
#define IDS_SEQ_DEL_CHAIN_NODE_CMD			ClCutSequence_Resource_StartID+262
#define IDS_SEQ_ADD_GRIDSEQ					ClCutSequence_Resource_StartID+263
#define IDS_SEQ_ADD_FLYCUT_SEQ_CMD			ClCutSequence_Resource_StartID+264
//

// 余料切割线相关
#define IDS_GET_REM_FROM_TOP				ClCutSequence_Resource_StartID+280
#define IDS_GET_REM_FROM_BOTTOM				ClCutSequence_Resource_StartID+281
#define IDS_GET_REM_FROM_LEFT				ClCutSequence_Resource_StartID+282
#define IDS_GET_REM_FROM_RIGHT				ClCutSequence_Resource_StartID+283

// used for SeqUnitPane
#define IDS_SEQ_VIEW_SEQUNIT				ClCutSequence_Resource_StartID+360
#define IDS_SEQ_VIEW_BY_ORDER				ClCutSequence_Resource_StartID+361
#define IDS_SEQ_VIEW_BY_TYPE				ClCutSequence_Resource_StartID+362
//

// used for SubRtTplPane
#define IDS_VIEWBAR_SUBRT_TPL				ClCutSequence_Resource_StartID+370
//

// used for "CutNodeViewBar"
#define IDS_SEQ_VIEW_CUTNODE				ClCutSequence_Resource_StartID+375
//

// the message IDs that will be displayed to prompt user.
// auto.
#define IDS_SEQ_CLEAR_ALL					ClCutSequence_Resource_StartID+390
#define IDS_SEQ_NO_CUTSEQ_EXIST				ClCutSequence_Resource_StartID+391
// 关于“编辑工序”
#define IDS_SEQ_SELECT_SEQ					ClCutSequence_Resource_StartID+400
#define IDS_SEQ_NO_SEQ_SELECT				ClCutSequence_Resource_StartID+401
#define IDS_SEQ_ACTIVE_IS_LOOP_SEQ			ClCutSequence_Resource_StartID+402
#define IDS_SEQ_ACTIVE_IS_COMCUT_SEQ		ClCutSequence_Resource_StartID+403
#define IDS_SEQ_ACTIVE_IS_BDGE_SEQ			ClCutSequence_Resource_StartID+404
#define IDS_SEQ_ACTIVE_IS_GridCut_SEQ		ClCutSequence_Resource_StartID+405
#define IDS_SEQ_DELETE_SEQUENCE				ClCutSequence_Resource_StartID+406
#define IDS_SEQ_SELECT_SEQ_1				ClCutSequence_Resource_StartID+407
#define IDS_SEQ_SELECT_SEQ_2				ClCutSequence_Resource_StartID+408
#define IDS_SEQ_SELECT_SEQ_3				ClCutSequence_Resource_StartID+409
#define IDS_SEQ_REPLACE_HOLESEQ_SELECT_PART	ClCutSequence_Resource_StartID+410
#define IDS_SEQ_REPLACE_HOLESEQ_1			ClCutSequence_Resource_StartID+411
#define IDS_SEQ_REPLACE_HOLESEQ_2			ClCutSequence_Resource_StartID+412
#define IDS_SEQ_REPLACE_HOLESEQ_3			ClCutSequence_Resource_StartID+413
#define IDS_SEQ_REPLACE_HOLESEQ_4			ClCutSequence_Resource_StartID+414
#define IDS_SEQ_REPLACE_HOLESEQ_5			ClCutSequence_Resource_StartID+415
#define IDS_SEQ_PartNoCutting				ClCutSequence_Resource_StartID+416
#define IDS_SEQ_LoopNoCutting				ClCutSequence_Resource_StartID+417
#define IDS_SEQ_ACTIVE_IS_FlyCut_SEQ		ClCutSequence_Resource_StartID+418
#define IDS_SEQ_ACTIVE_IS_ChainCut_SEQ		ClCutSequence_Resource_StartID+419
// 关于“回路工序”
#define IDS_SEQ_SELECT_LOOPINST				ClCutSequence_Resource_StartID+420
#define IDS_SEQ_ADD_LOOPSEQ					ClCutSequence_Resource_StartID+421
#define IDS_SEQ_LOOP_SEQUENCED				ClCutSequence_Resource_StartID+422
#define IDS_SEQ_INSERT_BASE_SEQ				ClCutSequence_Resource_StartID+423
#define IDS_SEQ_INSERT_NEWLOOP				ClCutSequence_Resource_StartID+424
#define IDS_SEQ_SELECT_PARTINST				ClCutSequence_Resource_StartID+425
#define IDS_SEQ_PART_SEQUENCED				ClCutSequence_Resource_StartID+426
// 关于“查看工序”
#define IDS_SEQ_VIEW_SPEED1					ClCutSequence_Resource_StartID+440
#define IDS_SEQ_VIEW_SPEED2					ClCutSequence_Resource_StartID+441
#define IDS_SEQ_VIEW_SPEED3					ClCutSequence_Resource_StartID+442
#define IDS_SEQ_VIEW_SPEED4					ClCutSequence_Resource_StartID+443
#define IDS_SEQ_VIEW_SPEED5					ClCutSequence_Resource_StartID+444
#define IDS_SEQ_VIEW_SPEED6					ClCutSequence_Resource_StartID+445
#define IDS_SEQ_VIEW_SPEED7					ClCutSequence_Resource_StartID+446
#define IDS_SEQ_VIEW_SPEED8					ClCutSequence_Resource_StartID+447
#define IDS_SEQ_VIEW_PAUSE					ClCutSequence_Resource_StartID+448
#define IDS_SEQ_VIEW_FINISH					ClCutSequence_Resource_StartID+449
#define IDS_SEQ_NO_SEQ_SIM					ClCutSequence_Resource_StartID+450
// 关于“快速移动点”
#define IDS_SEQ_DELETE_MOV_PT				ClCutSequence_Resource_StartID+460
#define IDS_SEQ_DEL_MOVPT_NO_SEL			ClCutSequence_Resource_StartID+461
#define IDS_SEQ_ADD_MOVPT_BASE				ClCutSequence_Resource_StartID+462
#define IDS_SEQ_MOVPT_NO_BASE_SEL			ClCutSequence_Resource_StartID+463
#define IDS_SEQ_ADD_MOVPT_PT				ClCutSequence_Resource_StartID+464
// 关于“桥接工序”。
#define IDS_SEQ_PT_NOTALLOW_BDGE			ClCutSequence_Resource_StartID+470
#define IDS_SEQ_SEL_BDGE_PT					ClCutSequence_Resource_StartID+471
#define IDS_SEQ_LOOP_NOTALLOW_BDGE			ClCutSequence_Resource_StartID+472
#define IDS_SEQ_BREAK_BDGE					ClCutSequence_Resource_StartID+473
#define IDS_SEQ_BREAK_BDGE_NO_SEL			ClCutSequence_Resource_StartID+474
#define IDS_SEQ_BDGE_START_SELECT			ClCutSequence_Resource_StartID+475
#define IDS_SEQ_LOOP_NOT_BDGE_1				ClCutSequence_Resource_StartID+476
#define IDS_SEQ_LOOP_NOT_BDGE_2				ClCutSequence_Resource_StartID+477
#define IDS_SEQ_BDGE_SAME_LOOP				ClCutSequence_Resource_StartID+478
#define IDS_SEQ_BDGE_LOOPTOOL_WARN			ClCutSequence_Resource_StartID+479
// 关于“连割”。
#define IDS_SEQ_SEL_PT_LOOP_4_CHAIN			ClCutSequence_Resource_StartID+480
#define IDS_SEQ_LOOP_NOT_CHAIN_1			ClCutSequence_Resource_StartID+481
#define IDS_SEQ_LOOP_NOT_CHAIN_2			ClCutSequence_Resource_StartID+482
#define IDS_SEQ_LOOP_NOT_CHAIN_3			ClCutSequence_Resource_StartID+483
#define IDS_SEQ_CHAIN_SEL_BASE_NODE			ClCutSequence_Resource_StartID+484
#define IDS_SEQ_CHAIN_NO_BASE_NODE_SEL		ClCutSequence_Resource_StartID+485
#define IDS_SEQ_CHAIN_DEL_NODE				ClCutSequence_Resource_StartID+486
#define IDS_SEQ_CHAIN_DEL_LAST_LOOP			ClCutSequence_Resource_StartID+487
#define IDS_SEQ_CHAIN_DEL_NO_NODE_SEL		ClCutSequence_Resource_StartID+488
#define IDS_SEQ_CHAIN_LOOPTOOL_WARN			ClCutSequence_Resource_StartID+489
// “田字工序”分组。
#define IDS_GRID_SEQ_NO_PART_SEL		    ClCutSequence_Resource_StartID+490
#define IDS_GRID_SEQ_ONE_PART_SEL_TIP		ClCutSequence_Resource_StartID+491
#define IDS_GRID_SEQ_SEL_GRID_TIP			ClCutSequence_Resource_StartID+492
#define IDS_GRID_SEQ_PART_GAP_TIP			ClCutSequence_Resource_StartID+493
#define IDS_GRID_SEQ_EXIST_SEQ_TIP			ClCutSequence_Resource_StartID+494
#define IDS_GRID_SEQ_LOOPTOOL_WARN			ClCutSequence_Resource_StartID+495
#define IDS_GRID_SEQ_RECT_BOUNDARY			ClCutSequence_Resource_StartID+496
#define IDS_GRID_Warning_1			        ClCutSequence_Resource_StartID+497
// "飞行切割"分组。
#define IDS_FLYCUT_SEQ_NO_HOLE_SEL_TIP		ClCutSequence_Resource_StartID+500
#define IDS_SEQ_ADD_FLYCUT_GRID_TIP			ClCutSequence_Resource_StartID+501
#define IDS_SEQ_FLYCUT_SEQ_EXIST_TIP		ClCutSequence_Resource_StartID+502
#define IDS_SEQ_FLYCUT_APPLY_SAMEPARTS_TIP	ClCutSequence_Resource_StartID+503
#define IDS_FlyCut_LoopsMustHaveSameCutOffset	ClCutSequence_Resource_StartID+504
#define IDS_FlyCut_LoopsMustBelongOnePartInst	ClCutSequence_Resource_StartID+505
#define IDS_FlyCut_LoopsMustBeRectOrCircle	ClCutSequence_Resource_StartID+506
// 余料切割线
#define IDS_SEQ_AUTO_REM_CUTLINE_ACTION		ClCutSequence_Resource_StartID+510
#define IDS_SEQ_ADD_REM_CUTLINE_ACTION		ClCutSequence_Resource_StartID+511
#define IDS_SEQ_DEL_REM_CUTLINE_ACTION		ClCutSequence_Resource_StartID+512
#define IDS_SEQ_AUTO_REM_CUTLINE_ERROR		ClCutSequence_Resource_StartID+513
#define IDS_SEQ_AUTO_REM_CUTLINE_OK			ClCutSequence_Resource_StartID+514
#define IDS_SEQ_NO_PART_EXIST				ClCutSequence_Resource_StartID+515
#define IDS_SEQ_REM_MAT_ERROR				ClCutSequence_Resource_StartID+516
#define IDS_SEQ_REM_MAT_OK					ClCutSequence_Resource_StartID+517
//

// 按钮提示
// “自动”分组
#define IDS_SEQ_AUTOSEQUENCE_TIP			ClCutSequence_Resource_StartID+540
#define IDS_SEQ_CLEARALLSEQUENCE_TIP		ClCutSequence_Resource_StartID+541
// “编辑”分组
#define IDS_SEQ_SELECTSEQUENCE_TIP			ClCutSequence_Resource_StartID+545
#define IDS_SEQ_REMOVESEQUENCE_TIP			ClCutSequence_Resource_StartID+546
#define IDS_SEQ_ORDER_SEQUENCE_TIP			ClCutSequence_Resource_StartID+547
#define IDS_SEQ_REPLACE_HOLE_SEQ_TIP		ClCutSequence_Resource_StartID+548
// “轮廓工序”分组。
#define IDS_SEQ_ADDSEQUENCE_TIP				ClCutSequence_Resource_StartID+550
#define IDS_SEQ_INSERTSEQUENCE_TIP			ClCutSequence_Resource_StartID+551
// “桥接工序”分组。
#define IDS_SEQ_ADD_BDGE_SEQ_TIP			ClCutSequence_Resource_StartID+555
#define IDS_SEQ_BREAK_BDGE_SEQ_TIP			ClCutSequence_Resource_StartID+556
#define IDS_SEQ_BDGE_SEQ_START_TIP			ClCutSequence_Resource_StartID+557
// “连割工序”分组。
#define IDS_SEQ_ADD_CHAIN_SEQ_TIP			ClCutSequence_Resource_StartID+560
#define IDS_SEQ_ADD_CHAIN_NODE_TIP			ClCutSequence_Resource_StartID+561
#define IDS_SEQ_DEL_CHAIN_NODE_TIP			ClCutSequence_Resource_StartID+562
// “田字工序”分组。
#define IDS_SEQ_ADD_GRID_SEQ_TIP			ClCutSequence_Resource_StartID+565
// "飞行切割"分组。
#define IDS_SEQ_AddFlyCut_TIP				ClCutSequence_Resource_StartID+570
// “运动点”分组。
#define IDS_SEQ_ADD_MOV_PT_TIP				ClCutSequence_Resource_StartID+575
#define IDS_SEQ_DEL_MOV_PT_TIP				ClCutSequence_Resource_StartID+576
// “余料切割线”分组
#define IDS_SEQ_AUTO_REM_CUTLINE_TIP		ClCutSequence_Resource_StartID+580
#define IDS_SEQ_ADD_REM_CUTLINE_TIP			ClCutSequence_Resource_StartID+581
#define IDS_SEQ_DEL_REM_CUTLINE_TIP			ClCutSequence_Resource_StartID+582
// “查看”分组
#define IDS_SEQ_VIEWSEQUENCE_TIP			ClCutSequence_Resource_StartID+585
// “窗口”分组
#define IDS_SEQ_VIEW_CUTSEQ_TIP				ClCutSequence_Resource_StartID+590
//
/************************************************************************/


/************************************************************************/
/*                the control created by code                           */

// used for "Auto" panel
#define IDC_SEQ_AUTOSEQUENCE				ClCutSequence_Resource_StartID+600
#define IDC_SEQ_CLEARALLSEQUENCE			ClCutSequence_Resource_StartID+601
//

// used for "Edit sequence" panel
#define IDC_SEQ_ADD_LOOPSEQ					ClCutSequence_Resource_StartID+610
#define IDC_SEQ_ADD_BDGESEQ					ClCutSequence_Resource_StartID+611
#define IDC_SEQ_INSERT_LOOPSEQ				ClCutSequence_Resource_StartID+612
#define IDC_SEQ_REPLACE_HOLE_SEQ			ClCutSequence_Resource_StartID+613
#define IDC_SEQ_REMOVESEQUENCE				ClCutSequence_Resource_StartID+614
#define IDC_SEQ_SELECTSEQUENCE				ClCutSequence_Resource_StartID+615
#define IDC_SEQ_BREAK_BDGESEQ				ClCutSequence_Resource_StartID+616
#define IDC_SEQ_BDGE_START_LOOP				ClCutSequence_Resource_StartID+617
#define IDC_SEQ_SORTSEQUENCE				ClCutSequence_Resource_StartID+618
#define IDC_SEQ_ADD_MOV_PT					ClCutSequence_Resource_StartID+619
#define IDC_SEQ_DEL_MOV_PT					ClCutSequence_Resource_StartID+620
#define IDC_SEQ_ADD_CHAINSEQ				ClCutSequence_Resource_StartID+621
#define IDC_SEQ_INSERT_CHAIN_NODE			ClCutSequence_Resource_StartID+622
#define IDC_SEQ_DEL_CHAIN_NODE				ClCutSequence_Resource_StartID+623
#define IDC_SEQ_ADD_GRIDSEQ					ClCutSequence_Resource_StartID+624
#define IDC_SEQ_ADD_FLYCUTSEQ				ClCutSequence_Resource_StartID+625
#define IDC_SEQ_AUTO_REM_CUTLINE			ClCutSequence_Resource_StartID+626
#define IDC_SEQ_CREATE_REM_CUTLINE			ClCutSequence_Resource_StartID+627
#define IDC_SEQ_DELETE_REM_CUTLINE			ClCutSequence_Resource_StartID+628
#define IDC_SEQ_INSERT_RESET_INSTRUCTION	ClCutSequence_Resource_StartID+629
#define IDC_SEQ_DEL_RESET_INSTRUCTION		ClCutSequence_Resource_StartID+630
//

// used for "View Sequence" panel
#define IDC_SEQ_VIEWSEQUENCE				ClCutSequence_Resource_StartID+635
//

// used for "windows" panel
#define IDC_SEQ_VIEWBAR_SEQUNIT				ClCutSequence_Resource_StartID+640
#define IDC_SEQ_VIEWBAR_SUBRT_TPL			ClCutSequence_Resource_StartID+641
//

// used for "SeqUnitPane"
#define IDC_SEQ_VIEW_SEQUNIT				ClCutSequence_Resource_StartID+700
#define IDC_CUTSEQ_PANE_COMBO				ClCutSequence_Resource_StartID+701
#define IDC_TREEVIEW_SEQUNIT				ClCutSequence_Resource_StartID+702
#define IDC_LISTCTRL_SEQUNIT				ClCutSequence_Resource_StartID+703
//

// used for "CutNodeViewBar"
#define IDC_SEQ_VIEW_CUTNODE				ClCutSequence_Resource_StartID+710
#define IDC_LISTCTRL_CUTNODE				ClCutSequence_Resource_StartID+711
//

// used for "SubRtTplPane"
#define IDC_SEQ_VIEW_SUBRT_TPL				ClCutSequence_Resource_StartID+720
#define IDC_TREEVIEW_SUBRT_TPL				ClCutSequence_Resource_StartID+721
//
/************************************************************************/
