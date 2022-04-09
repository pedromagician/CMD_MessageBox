#pragma once

#include "stdafx.h"
#include "UTL_Cmd.h"
#include "UTL_Conversion.h"

int _tmain(int _argc, _TCHAR* _pArgv[])
{
	int correctParameters	= 0;
	bool help		= false;
	wstring title	= _T("");
	wstring message	= _T("");
	UTL_Cmd::Type type			= UTL_Cmd::Type::Ok;
	UTL_Cmd::Icon icon			= UTL_Cmd::Icon::NoIcon;
	UTL_Cmd::DefaultButton def	= UTL_Cmd::DefaultButton::Def1;

	map<wstring, UINT> string2icon{
		{_T("noicon"),		0x00000000L},
		{_T("information"),	MB_ICONINFORMATION},
		{_T("question"),	MB_ICONQUESTION},
		{_T("warning"),		MB_ICONWARNING},
		{_T("error"),		MB_ICONERROR}
	};

	map<wstring, UINT> string2button{
		{_T("ok"),			MB_OK},
		{_T("okcancel"),	MB_OKCANCEL},
		{_T("yesno"),		MB_YESNO},
		{_T("yesnocancel"),	MB_YESNOCANCEL}
	};

	map<wstring, UINT> string2default{
		{_T("1"),	MB_DEFBUTTON1},
		{_T("2"),	MB_DEFBUTTON2},
		{_T("3"),	MB_DEFBUTTON3}
	};

	UTL_Cmd cmd;
	cmd.Add(UTL_Cmd::_STRING,	2, _T("-title"),	_T("-t"),				_T("The 'xxx' argument specifies the title of the message box."),																&title);
	cmd.Add(UTL_Cmd::_STRING,	2, _T("-message"),	_T("-m"),				_T("The 'xxx' argument specifies the text of the message box. Newline can be embedded using \\n."),								&message);
	cmd.Add(UTL_Cmd::_TRUE,		3, _T("-help"),		_T("-h"),	_T("-?"),	_T("To view help."),																											&help);
	cmd.Add(UTL_Cmd::_ENUM,		2, _T("-icon"),		_T("-i"),				_T("The 'xxx' argument specifies the text of the message box. Allowed options: NoIcon|Information|Question|Warning|Error."),	&icon,		&string2icon);
	cmd.Add(UTL_Cmd::_ENUM,		2, _T("-button"),	_T("-b"),				_T("The 'xxx' argument specifies the type of the message box. Allowed options: Ok|OkCancel|YesNo|YesNoCancel."),				&type,		&string2button);
	cmd.Add(UTL_Cmd::_ENUM,		2, _T("-default"),	_T("-d"),				_T("The 'xxx' argument specifies the default button. Allowed options: 1|2|3."),													&def,		&string2default);

	if (cmd.ParseCommandLine(_argc, _pArgv, correctParameters) != 0) {
		cmd.Help();
		return 1;
	}

	if (help || correctParameters == 0) {
		cmd.Help();
		return 0;
	}

	if (message.empty()) {
		_tprintf(_T("Error - message is emnpty\n"));
		return 1;
	}

	int resultButton = MessageBox(nullptr, (LPCWSTR)message.c_str(), title.c_str(), type | icon | def);

	if (type == UTL_Cmd::Type::YesNo) resultButton -= 5;
	if (type == UTL_Cmd::Type::YesNoCancel) {
		if (resultButton > 5) resultButton -= 5;
		else resultButton = 3;
	}

	_tprintf(to_wstring(resultButton).c_str());
	return 0;
}
