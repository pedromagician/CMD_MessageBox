#pragma once

#include "stdafx.h"
#include "CommandLine.h"
#include "Conversion.h"
#include "MessageBoxPos.h"

int _tmain(int _argc, _TCHAR* _pArgv[])
{
	int correctParameters	= 0;
	bool help		= false;
	wstring title	= _T("");
	wstring message	= _T("");
	CommandLine::Type type			= CommandLine::Type::Ok;
	CommandLine::Icon icon			= CommandLine::Icon::NoIcon;
	CommandLine::DefaultButton def	= CommandLine::DefaultButton::Def1;
	wstring monitor		= _T("primary");
	wstring position	= _T("center");
	int deltaX = 0;
	int deltaY = 0;

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

	CommandLine cmd;
	cmd.Add(CommandLine::_STRING,	2, _T("-title"),	_T("-t"),				_T("The 'xxx' argument specifies the title of the message box."),																&title);
	cmd.Add(CommandLine::_STRING,	2, _T("-message"),	_T("-m"),				_T("The 'xxx' argument specifies the text of the message box. Newline can be embedded using \\n."),								&message);
	cmd.Add(CommandLine::_TRUE,		3, _T("-help"),		_T("-h"),	_T("-?"),	_T("To view help."),																											&help);
	cmd.Add(CommandLine::_ENUM,		2, _T("-icon"),		_T("-i"),				_T("The 'xxx' argument specifies the text of the message box. Allowed options: NoIcon|Information|Question|Warning|Error."),	&icon,		&string2icon);
	cmd.Add(CommandLine::_ENUM,		2, _T("-button"),	_T("-b"),				_T("The 'xxx' argument specifies the type of the message box. Allowed options: Ok|OkCancel|YesNo|YesNoCancel."),				&type,		&string2button);
	cmd.Add(CommandLine::_ENUM,		2, _T("-default"),	_T("-d"),				_T("The 'xxx' argument specifies the default button. Allowed options: 1|2|3."),													&def,		&string2default);
	cmd.Add(CommandLine::_STRING,	2, _T("-monitor"),	_T("-mon"),				_T("The 'xxx' argument specifies the default monitor. Allowed options: Primary|Mouse|MousePointer|0|1|2|n."),					&monitor);
	cmd.Add(CommandLine::_STRING,	2, _T("-position"), _T("-pos"),				_T("The 'xxx' argument specifies the default position. Allowed options: Center|Top|Bottom|Left|Right|Pointer|xy."),				&position);
	cmd.Add(CommandLine::_INT,		1, _T("-x"),								_T("The 'xxx' argument specifies the position offset along the X coordinate."),													&deltaX);
	cmd.Add(CommandLine::_INT,		1, _T("-y"),								_T("The 'xxx' argument specifies the position offset along the Y coordinate."),													&deltaY);

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

	struct MessageBoxPos::InformationAboutPositionOfMessageBox messageBoxPosition;
	messageBoxPosition.monitor	= MessageBoxPos::_PRIMARY;
	messageBoxPosition.id		= 0;
	messageBoxPosition.type		= MessageBoxPos::_CENTER;
	messageBoxPosition.delta	= { 0, 0 };
	messageBoxPosition.position	= { 0 };

	monitor = Conversion::ToLower(Conversion::TrimWhiteChar(monitor));
	if (monitor == _T("primary"))			messageBoxPosition.monitor	= MessageBoxPos::_PRIMARY;
	else if (monitor == _T("mouse"))		messageBoxPosition.monitor	= MessageBoxPos::_MOUSE;
	else if (monitor == _T("mousepointer"))	messageBoxPosition.monitor	= MessageBoxPos::_MOUSE_POINTER;
	else {
		messageBoxPosition.monitor	= MessageBoxPos::_ID;
		messageBoxPosition.id		= (UINT)Conversion::ToInt(monitor);
	}

	position = Conversion::ToLower(Conversion::TrimWhiteChar(position));
	if (position == _T("center"))		messageBoxPosition.type = MessageBoxPos::_CENTER;
	else if (position == _T("top"))		messageBoxPosition.type = MessageBoxPos::_TOP_CENTER;
	else if (position == _T("bottom"))	messageBoxPosition.type = MessageBoxPos::_BOTTOM_CENTER;
	else if (position == _T("left"))	messageBoxPosition.type = MessageBoxPos::_LEFT_CENTER;
	else if (position == _T("right"))	messageBoxPosition.type = MessageBoxPos::_RIGHT_CENTER;
	else if (position == _T("pointer"))	messageBoxPosition.type = MessageBoxPos::_POINTER;
	else if (position == _T("xy"))		messageBoxPosition.type = MessageBoxPos::_XY;

	messageBoxPosition.delta = { deltaX, deltaY };

	int resultButton = MessageBoxPos::MessageBox(nullptr, (LPCWSTR)message.c_str(), title.c_str(), type | icon | def, messageBoxPosition);

	if (type == CommandLine::Type::YesNo) resultButton -= 5;
	if (type == CommandLine::Type::YesNoCancel) {
		if (resultButton > 5) resultButton -= 5;
		else resultButton = 3;
	}

	_tprintf(to_wstring(resultButton).c_str());
	return 0;
}
