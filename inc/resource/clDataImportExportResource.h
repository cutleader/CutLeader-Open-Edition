
///////////////////////////////////////////////////////////////////////////////
// ID range is (21000~21500)
///////////////////////////////////////////////////////////////////////////////

#define ClDataImportExport_Resource_StartID							21000


/************************************************************************/
/*                the string ID                                         */

// the import/export data type.
#define IDS_IMPEXP_DXF_EXT								ClDataImportExport_Resource_StartID+10
#define IDS_IMPEXP_DWG_EXT								ClDataImportExport_Resource_StartID+11
#define IDS_IMPEXP_TAOPRT_EXT							ClDataImportExport_Resource_StartID+12
#define IDS_IMPEXP_TAOSHT_EXT							ClDataImportExport_Resource_StartID+13
#define IDS_IMPEXP_EXPLIBITEM_EXT						ClDataImportExport_Resource_StartID+14
#define IDS_IMPEXP_MATLIB_EXT							ClDataImportExport_Resource_StartID+15
#define IDS_IMPEXP_BK_DB_EXT							ClDataImportExport_Resource_StartID+16
#define IDS_IMPEXP_TaskItem_Ext                         ClDataImportExport_Resource_StartID+17
#define IDS_ImportExportCutParamLib_Ext					ClDataImportExport_Resource_StartID+18
#define IDS_ImportExportMachineParam_Ext				ClDataImportExport_Resource_StartID+19
//

// used for "import/export" dialog
#define IDS_IMPEXP_IMPORTFILE							ClDataImportExport_Resource_StartID+21
#define IDS_IMPEXP_EXPORTFILE							ClDataImportExport_Resource_StartID+22
#define IDS_IMPORT_FILEEXT								ClDataImportExport_Resource_StartID+23
#define IDS_IMPORT_DXFDWG_FILEEXT						ClDataImportExport_Resource_StartID+24
#define IDS_EXP_PRT_DXFDWG_FILEEXT						ClDataImportExport_Resource_StartID+25
#define IDS_EXP_SHT_DXFDWG_FILEEXT						ClDataImportExport_Resource_StartID+26
#define IDS_IMPEXP_EXPLIBITEM_FILEEXT					ClDataImportExport_Resource_StartID+27
#define IDS_IMPEXP_MATLIB_FILEEXT						ClDataImportExport_Resource_StartID+28
#define IDS_IMPEXP_BK_DB_FILEEXT						ClDataImportExport_Resource_StartID+29
#define IDS_IMPORT_XLS_EXT								ClDataImportExport_Resource_StartID+30
#define IDS_IMPEXP_TaskItem_FileExt					    ClDataImportExport_Resource_StartID+31
#define IDS_EXP_DXFDWG_FILEEXT						    ClDataImportExport_Resource_StartID+32
#define IDS_ImportClPart_FileExt						ClDataImportExport_Resource_StartID+33
#define IDS_ImportExportCutParamLib_FileFilter			ClDataImportExport_Resource_StartID+34
#define IDS_ImportExportMachineParam_FileFilter			ClDataImportExport_Resource_StartID+35
#define IDS_IMPORT_DxfDwgInfo							ClDataImportExport_Resource_StartID+36
//

// the string for prompt.
  // for imp/exp data.
#define IDS_ImportMachineParam_Success					ClDataImportExport_Resource_StartID+41
#define IDS_ImportMachineParam_Fail						ClDataImportExport_Resource_StartID+42
#define IDS_ExportMachineParam_Success					ClDataImportExport_Resource_StartID+43
#define IDS_ExportMachineParam_Fail						ClDataImportExport_Resource_StartID+44
#define IDS_ImportCutParamLib_Success					ClDataImportExport_Resource_StartID+45
#define IDS_ImportCutParamLib_Fail						ClDataImportExport_Resource_StartID+46
#define IDS_ExportCutParamLib_Success					ClDataImportExport_Resource_StartID+47
#define IDS_ExportCutParamLib_Fail						ClDataImportExport_Resource_StartID+48
#define IDS_EXP_SHEET_Fail								ClDataImportExport_Resource_StartID+49
#define IDS_EXP_PRT_SUCCESS								ClDataImportExport_Resource_StartID+50
#define IDS_EXP_TASK_SUCCESS							ClDataImportExport_Resource_StartID+51
#define IDS_EXP_SHEET_SUCCESS							ClDataImportExport_Resource_StartID+52
#define IDS_IMP_NOT_SUPPORT_VERSION						ClDataImportExport_Resource_StartID+53
#define IDS_EXP_EXPLIB_SUCCESS							ClDataImportExport_Resource_StartID+54
#define IDS_EXP_MATLIB_SUCCESS							ClDataImportExport_Resource_StartID+55
#define IDS_EXP_RESERVED_EXPLIB							ClDataImportExport_Resource_StartID+56
#define IDS_IMP_NO_ENOUGH_MAT							ClDataImportExport_Resource_StartID+57
#define IDS_MSG_Sheet_EXPORT_FOLDER						ClDataImportExport_Resource_StartID+58
#define IDS_MSG_SelectSheet2Export						ClDataImportExport_Resource_StartID+59
  // for backup/recovery data.
#define IDS_BK_SEL_BK_FOLDER							ClDataImportExport_Resource_StartID+60
#define IDS_BK_FOLDER_NOT_FOUND							ClDataImportExport_Resource_StartID+61
#define IDS_SHUTDOWN_ALL_APP							ClDataImportExport_Resource_StartID+62
#define IDS_BACKUP_DB_AND_LOG							ClDataImportExport_Resource_StartID+63
#define IDS_BACKUP_LOG									ClDataImportExport_Resource_StartID+64
#define IDS_BACKUP_DB									ClDataImportExport_Resource_StartID+65
#define IDS_BK_IN_SAFE_LOC								ClDataImportExport_Resource_StartID+66
//

// for execute task.
  // backup task.
#define IDS_MSG_BACKUP_TASK								ClDataImportExport_Resource_StartID+100
#define IDS_MSG_COPY_DB_FILES							ClDataImportExport_Resource_StartID+101
#define IDS_MSG_ZIP_DB_FILES							ClDataImportExport_Resource_StartID+102
#define IDS_MSG_DEL_TMP_FOLDER							ClDataImportExport_Resource_StartID+103
#define IDS_MSG_BK_LOG_FILES							ClDataImportExport_Resource_StartID+104
#define IDS_MSG_EMPTY_OLD_LOG							ClDataImportExport_Resource_StartID+105
#define IDS_MSG_ALL_DATA_BK								ClDataImportExport_Resource_StartID+106
#define IDS_MSG_BK_TASK_OK								ClDataImportExport_Resource_StartID+107
#define IDS_MSG_BK_TASK_CANCEL							ClDataImportExport_Resource_StartID+108
  // recovery task.
#define IDS_MSG_RECOVERY_TASK							ClDataImportExport_Resource_StartID+120
#define IDS_MSG_DEL_CURRENT_DB							ClDataImportExport_Resource_StartID+121
#define IDS_MSG_UNZIP_BK_DB								ClDataImportExport_Resource_StartID+122
#define IDS_MSG_ALL_DATA_RC								ClDataImportExport_Resource_StartID+123
#define IDS_MSG_RC_TASK_OK								ClDataImportExport_Resource_StartID+124
#define IDS_MSG_RC_TASK_CANCEL							ClDataImportExport_Resource_StartID+125
  // import/export data
#define IDS_MSG_IMPORT_TASK_OK				            ClDataImportExport_Resource_StartID+140
#define IDS_MSG_ClTask_Imported			                ClDataImportExport_Resource_StartID+141
#define IDS_MSG_DxfDwg_Imported			                ClDataImportExport_Resource_StartID+142
#define IDS_MSG_ClPart_Imported			                ClDataImportExport_Resource_StartID+143
//
/************************************************************************/


/************************************************************************/
/*                the control created by code                           */

#define IDC_IMPEXP_IMPORTFILE							ClDataImportExport_Resource_StartID+200
#define IDC_IMPEXP_EXPORTFILE							ClDataImportExport_Resource_StartID+201
/************************************************************************/

