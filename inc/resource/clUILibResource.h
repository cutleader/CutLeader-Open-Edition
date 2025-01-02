
///////////////////////////////////////////////////////////////////////////////
// ID range is (10000~10500)
///////////////////////////////////////////////////////////////////////////////


#define ClUILib_Resource_StartID							10000


/************************************************************************/
/*                the string ID                                         */

// the message displayed in InteractBar.
#define IDS_VALUEINPUT									ClUILib_Resource_StartID+1
//

// the message which will be written into log file.
#define IDS_TASK_MESSAGE								ClUILib_Resource_StartID+10
#define IDS_MESSAGE_TYPE								ClUILib_Resource_StartID+11
#define IDS_TASK_NAME									ClUILib_Resource_StartID+12
#define IDS_MESSAGE_CONTENT								ClUILib_Resource_StartID+13
//

// the message IDs that will be displayed in InteractBar.
#define IDS_OUTPUT_NEWPOSITION							ClUILib_Resource_StartID+20
#define IDS_OUTPUT_ZOOM_STARTCORNER						ClUILib_Resource_StartID+21
#define IDS_OUTPUT_ZOOM_ENDCORNER						ClUILib_Resource_StartID+22
#define IDS_OUTPUT_WELCOME								ClUILib_Resource_StartID+23
#define IDS_OUTPUT_SELECTACTION							ClUILib_Resource_StartID+24
//

// used for popup menu.
#define IDS_ACTION_CANCEL_ACTION						ClUILib_Resource_StartID+30
//

// 属性相关的名称。
#define IDS_CATEGORY_GENERAL							ClUILib_Resource_StartID+41
#define IDS_NEW_EXT_PROP								ClUILib_Resource_StartID+42
#define IDS_EXT_PROP_SAME_NAME							ClUILib_Resource_StartID+43
//
/************************************************************************/


/************************************************************************/
// 自定义消息

// 编辑框控件发送这个消息给父窗口，表示自己刚刚处理完OnChar事件。
#define WM_OnCharEventFinished							ClUILib_Resource_StartID+150

// 编辑框控件发送这个消息给父窗口，表示自己刚刚处理完OnKillFocus事件。
#define WM_OnKillFocusEventFinished						ClUILib_Resource_StartID+151
/************************************************************************/