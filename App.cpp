#pragma once

#include "stdafx.h"
#include "CommandLine.h"
#include "Conversion.h"
#include "MessageBoxPos.h"

int _tmain(int _argc, _TCHAR* _pArgv[])
{
	_setmode(_fileno(stdout), _O_U8TEXT);

	int correctParameters	= 0;
	bool help		= false;
	wstring title	= _T("");
	wstring message	= _T("");
	CommandLine::Type type			= CommandLine::Type::Ok;
	CommandLine::Icon icon			= CommandLine::Icon::NoIcon;
	CommandLine::DefaultButton def	= CommandLine::DefaultButton::Def1;
	wstring monitor		= _T("primary");
	bool windowsReturnCode	= false;
	bool quiet				= false;

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
		{_T("yesnocancel"),	MB_YESNOCANCEL},
		{_T("1"),			MB_OK},
		{_T("2"),			MB_YESNO},
		{_T("3"),			MB_YESNOCANCEL},
	};

	map<wstring, UINT> string2default{
		{_T("1"),	MB_DEFBUTTON1},
		{_T("2"),	MB_DEFBUTTON2},
		{_T("3"),	MB_DEFBUTTON3}
	};

	map<wstring, UINT> string2positionType{
		{_T("center"),	MessageBoxPos::_CENTER},
		{_T("top"),		MessageBoxPos::_TOP_CENTER},
		{_T("bottom"),	MessageBoxPos::_BOTTOM_CENTER},
		{_T("left"),	MessageBoxPos::_LEFT_CENTER},
		{_T("right"),	MessageBoxPos::_RIGHT_CENTER},
		{_T("pointer"),	MessageBoxPos::_POINTER},
		{_T("xy"),		MessageBoxPos::_XY},
	};

	MessageBoxPos::MessageBoxPosition.monitor	= MessageBoxPos::_PRIMARY;
	MessageBoxPos::MessageBoxPosition.id		= 0;
	MessageBoxPos::MessageBoxPosition.type		= MessageBoxPos::_CENTER;
	MessageBoxPos::MessageBoxPosition.delta		= { 0, 0 };
	MessageBoxPos::MessageBoxPosition.position	= { 0 };
	MessageBoxPos::MessageBoxPosition.button1.second	= MessageBoxPos::MessageBoxPosition.button2.second	= MessageBoxPos::MessageBoxPosition.button3.second	= _T("");
	MessageBoxPos::MessageBoxPosition.button1.first		= MessageBoxPos::MessageBoxPosition.button2.first	= MessageBoxPos::MessageBoxPosition.button3.first	= 0;

	CommandLine cmd;
	cmd.Add(CommandLine::_STRING,	2, _T("-title"),	_T("-t"),				_T("The 'xxx' argument specifies the title of the message box."),																&title);
	cmd.Add(CommandLine::_STRING,	2, _T("-message"),	_T("-m"),				_T("The 'xxx' argument specifies the text of the message box. Newline can be embedded using \\n."),								&message);
	cmd.Add(CommandLine::_TRUE,		3, _T("-help"),		_T("-h"),	_T("-?"),	_T("To view help."),																											&help);
	cmd.Add(CommandLine::_ENUM,		2, _T("-icon"),		_T("-i"),				_T("The 'xxx' argument specifies the text of the message box. Allowed options: NoIcon|Information|Question|Warning|Error."),	&icon,		&string2icon);
	cmd.Add(CommandLine::_ENUM,		2, _T("-button"),	_T("-b"),				_T("The 'xxx' argument specifies the type of the message box. Allowed options: Ok|OkCancel|YesNo|YesNoCancel|1|2|3."),			&type,		&string2button);
	cmd.Add(CommandLine::_STRING,	1, _T("-b1"),								_T("The 'xxx' argument specifies the text of the button 1."),																	&MessageBoxPos::MessageBoxPosition.button1.second);
	cmd.Add(CommandLine::_STRING,	1, _T("-b2"),								_T("The 'xxx' argument specifies the text of the button 2."),																	&MessageBoxPos::MessageBoxPosition.button2.second);
	cmd.Add(CommandLine::_STRING,	1, _T("-b3"),								_T("The 'xxx' argument specifies the text of the button 3."),																	&MessageBoxPos::MessageBoxPosition.button3.second);
	cmd.Add(CommandLine::_ENUM,		2, _T("-default"),	_T("-d"),				_T("The 'xxx' argument specifies the default button. Allowed options: 1|2|3."),													&def,		&string2default);
	cmd.Add(CommandLine::_STRING,	2, _T("-monitor"),	_T("-mon"),				_T("The 'xxx' argument specifies the default monitor. Allowed options: Primary|Mouse|MousePointer|0|1|2|n."),					&monitor);
	cmd.Add(CommandLine::_ENUM,		2, _T("-position"), _T("-pos"),				_T("The 'xxx' argument specifies the default position. Allowed options: Center|Top|Bottom|Left|Right|Pointer|xy."),				&MessageBoxPos::MessageBoxPosition.type,	&string2positionType);
	cmd.Add(CommandLine::_INT,		1, _T("-x"),								_T("The 'xxx' argument specifies the position offset along the X coordinate."),													&MessageBoxPos::MessageBoxPosition.delta.x);
	cmd.Add(CommandLine::_INT,		1, _T("-y"),								_T("The 'xxx' argument specifies the position offset along the Y coordinate."),													&MessageBoxPos::MessageBoxPosition.delta.y);
	cmd.Add(CommandLine::_TRUE,		2, _T("-windowsReturnCode"),	_T("-wrc"),	_T("The argument enables the Windows return code."),																			&windowsReturnCode);
	cmd.Add(CommandLine::_TRUE,		1, _T("-quiet"),							_T("The argument disables output to the command line."),																		&quiet);

	if (cmd.ParseCommandLine(_argc, _pArgv, correctParameters) != 0) {
		cmd.Help();
		return 0;
	}

	if (help || correctParameters == 0) {
		cmd.Help();
		return 0;
	}

	if (message.empty()) {
		wcout << _T("Error - message is emnpty") << endl;
		return 0;
	}

	monitor = Conversion::ToLower(Conversion::TrimWhiteChar(monitor));
	if (monitor == _T("primary"))			MessageBoxPos::MessageBoxPosition.monitor	= MessageBoxPos::_PRIMARY;
	else if (monitor == _T("mouse"))		MessageBoxPos::MessageBoxPosition.monitor	= MessageBoxPos::_MOUSE;
	else if (monitor == _T("mousepointer"))	MessageBoxPos::MessageBoxPosition.monitor	= MessageBoxPos::_MOUSE_POINTER;
	else {
		MessageBoxPos::MessageBoxPosition.monitor	= MessageBoxPos::_ID;
		MessageBoxPos::MessageBoxPosition.id		= (UINT)Conversion::ToInt(monitor);
	}

	if (type == CommandLine::Type::Ok		|| type == CommandLine::Type::OkCancel)		MessageBoxPos::MessageBoxPosition.button1.first = IDOK;
	if (type == CommandLine::Type::YesNo	|| type == CommandLine::Type::YesNoCancel)	MessageBoxPos::MessageBoxPosition.button1.first = IDYES;

	if (type == CommandLine::Type::OkCancel)											MessageBoxPos::MessageBoxPosition.button2.first = IDCANCEL;
	if (type == CommandLine::Type::YesNo	|| type == CommandLine::Type::YesNoCancel)	MessageBoxPos::MessageBoxPosition.button2.first = IDNO;

	if (type == CommandLine::Type::YesNoCancel)											MessageBoxPos::MessageBoxPosition.button3.first = IDCANCEL;

	int resultButton = MessageBoxPos::MessageBox(nullptr, (LPCWSTR)message.c_str(), title.c_str(), type | icon | def | MB_TOPMOST);

	if (type == CommandLine::Type::YesNo) resultButton -= 5;
	if (type == CommandLine::Type::YesNoCancel) {
		if (resultButton > 5) resultButton -= 5;
		else resultButton = 3;
	}

	if (windowsReturnCode) {
		switch (type) {
		case CommandLine::Type::Ok: {
			resultButton = IDOK;
			break;
		}
		case CommandLine::Type::OkCancel: {
			if (resultButton == 1) resultButton = IDOK;
			else if (resultButton == 2) resultButton = IDCANCEL;
			break;
		}
		case CommandLine::Type::YesNo: {
			if (resultButton == 1) resultButton = IDYES;
			else if (resultButton == 2) resultButton = IDNO;
			break;
		}
		case CommandLine::Type::YesNoCancel: {
			if (resultButton == 1) resultButton = IDYES;
			else if (resultButton == 2) resultButton = IDNO;
			else if (resultButton == 3) resultButton = IDCANCEL;
			break;
		}
		}
	}

	if (quiet == false)
		wcout << to_wstring(resultButton);

	return resultButton;
}
