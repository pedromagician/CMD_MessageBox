﻿#include "UTL_conversion.h"

#define WHITE_SPACE_CHARACTERS _T(" \t\n\v\f\r ")

int UTL_Conversion::ToInt(const wstring& _buff)
{
	return _tcstol(_buff.c_str(), nullptr, 10);
}

wstring UTL_Conversion::LeftTrimString(wstring _str, const wstring& _val)
{
	wstring::size_type pos = _str.find_first_not_of(_val);
	if (pos != wstring::npos)
		_str.erase(0, pos);
	else
		_str.erase(_str.begin(), _str.end());

	return _str;
}

wstring UTL_Conversion::RightTrimString(wstring _str, const wstring& _val)
{
	wstring::size_type pos = _str.find_last_not_of(_val);
	if (pos != wstring::npos)
		_str.erase(pos + 1);
	else
		_str.erase(_str.begin(), _str.end());

	return _str;
}

wstring UTL_Conversion::TrimString(wstring _str, const wstring& _val)
{
	wstring::size_type pos = _str.find_last_not_of(_val);
	if (pos != wstring::npos)
	{
		_str.erase(pos + 1);
		pos = _str.find_first_not_of(_val);
		if (pos != wstring::npos)
			_str.erase(0, pos);
	}
	else
		_str.erase(_str.begin(), _str.end());

	return _str;
}

wstring UTL_Conversion::TrimWhiteChar(const wstring& _val)
{
	return TrimString(_val, WHITE_SPACE_CHARACTERS);
}

wstring UTL_Conversion::ToLower(wstring _val)
{
	transform(_val.begin(), _val.end(), _val.begin(), [](wchar_t c) { return towlower(c); });
	return _val;
}

void UTL_Conversion::StringReplaceAll(wstring& _mess, const wstring& _oldStr, const wstring& _newStr)
{
	const size_t oldLen = _oldStr.length();
	const size_t newLen = _newStr.length();
	size_t position = 0;
	while ((position = _mess.find(_oldStr, position)) != string::npos)
	{
		_mess.replace(position, oldLen, _newStr);
		position += newLen;
	}
}
