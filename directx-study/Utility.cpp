#include "Utility.h"
#include <iostream>
#include "atlbase.h"
#include "atlstr.h"
#include "comutil.h"

char* Utility::WCharToChar(const wchar_t* str)
{
	return nullptr;
}

wchar_t* Utility::CharToWChar(const char* str)
{
	size_t convertedChars = 0;
	size_t newSize = strlen(str) + 1;
	wchar_t* wcstring = new wchar_t[newSize];

	mbstowcs_s(&convertedChars, wcstring, newSize, str, _TRUNCATE);
	return wcstring;
}
