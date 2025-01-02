#pragma once

BEGIN_CUTLEADER_NAMESPACE()

class _declspec(dllexport) Utility
{
private:
	Utility( void );

public:
	static Utility* GetInstance();

public:
	CString UTF8toUnicode( const char* utf8Str );

private:
	static Utility* pUtility;

};

END_CUTLEADER_NAMESPACE()
