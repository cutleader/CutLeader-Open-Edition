#include "StdAfx.h"
#include "Utility.h"
#include "StringUtil.h"

using namespace std;

BEGIN_CUTLEADER_NAMESPACE()

Utility* Utility::pUtility = NULL;

Utility::Utility(void)
{
}

Utility* Utility::GetInstance()
{
	if( pUtility == NULL )
		pUtility = new Utility();
	return pUtility;
}

CString Utility::UTF8toUnicode( const char* utf8Str )
{
	UINT theLength = strlen( utf8Str );
	return StringUtil::UTF8ToUnicode( utf8Str, theLength );
}

END_CUTLEADER_NAMESPACE()
