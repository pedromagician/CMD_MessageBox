#include "stdafx.h"

#ifdef _DEBUG
Debug theDebug;
#endif

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
	wstring monitor		= _T("primary");
	bool windowsReturnCode	= false;
	bool quiet				= false;

	map<wstring, int> string2icon{
		{_T("noicon"),		(int)Icon::NoIcon},
		{_T("information"),	(int)Icon::Information},
		{_T("question"),	(int)Icon::Question},
		{_T("warning"),		(int)Icon::Warning},
		{_T("error"),		(int)Icon::Error}
	};
	int icon = (int)Icon::NoIcon;

	map<wstring, int> string2button{
		{_T("ok"),			(int)Type::Ok},
		{_T("okcancel"),	(int)Type::OkCancel},
		{_T("yesno"),		(int)Type::YesNo},
		{_T("yesnocancel"),	(int)Type::YesNoCancel},
		{_T("1"),			(int)Type::Ok},
		{_T("2"),			(int)Type::YesNo},
		{_T("3"),			(int)Type::YesNoCancel},
	};
	int type = (int)Type::Ok;

	map<wstring, int> string2default{
		{_T("1"),	(int)DefaultButton::Def1},
		{_T("2"),	(int)DefaultButton::Def2},
		{_T("3"),	(int)DefaultButton::Def3}
	};
	int def = (int)DefaultButton::Def1;

	map<wstring, int> string2positionType{
		{_T("center"),	(int)MessageBoxPos::_CENTER},
		{_T("top"),		(int)MessageBoxPos::_TOP_CENTER},
		{_T("bottom"),	(int)MessageBoxPos::_BOTTOM_CENTER},
		{_T("left"),	(int)MessageBoxPos::_LEFT_CENTER},
		{_T("right"),	(int)MessageBoxPos::_RIGHT_CENTER},
		{_T("pointer"),	(int)MessageBoxPos::_POINTER},
		{_T("xy"),		(int)MessageBoxPos::_XY},
	};
	int posType = (int)MessageBoxPos::_CENTER;

	MessageBoxPos::MessageBoxPosition().monitor	= MessageBoxPos::_PRIMARY;
	MessageBoxPos::MessageBoxPosition().id		= 0;
	MessageBoxPos::MessageBoxPosition().type		= MessageBoxPos::_CENTER;
	MessageBoxPos::MessageBoxPosition().delta		= { 0, 0 };
	MessageBoxPos::MessageBoxPosition().position	= { 0 };
	MessageBoxPos::MessageBoxPosition().button1.second	= MessageBoxPos::MessageBoxPosition().button2.second	= MessageBoxPos::MessageBoxPosition().button3.second	= _T("");
	MessageBoxPos::MessageBoxPosition().button1.first	= MessageBoxPos::MessageBoxPosition().button2.first		= MessageBoxPos::MessageBoxPosition().button3.first		= 0;

	CommandLine cmd;
	{
		cmd.AddString({ _T("title"), _T("t") },
			_T("Specifies the title text displayed in the message box window."),
			title);

		cmd.AddString({ _T("message"), _T("m") },
			_T("Specifies the message text displayed in the dialog. Use \\n for line breaks; Unicode supported via \\u{XXXX} (e.g., \\u{1F44D})."),
			message);

		cmd.AddBool({ _T("help"), _T("h"), _T("?") },
			_T("Displays this help information."),
			help);

		cmd.AddEnum({ _T("icon"), _T("i") },
			_T("Specifies the icon displayed in the message box. Allowed options: NoIcon|Information|Question|Warning|Error."),
			string2icon, icon);

		cmd.AddEnum({ _T("button"), _T("b") },
			_T("Specifies the button configuration. Allowed options: Ok|OkCancel|YesNo|YesNoCancel|1|2|3."),
			string2button, type);

		cmd.AddString({ _T("b1") },
			_T("Specifies the label text for button 1."),
			MessageBoxPos::MessageBoxPosition().button1.second);

		cmd.AddString({ _T("b2") },
			_T("Specifies the label text for button 2."),
			MessageBoxPos::MessageBoxPosition().button2.second);

		cmd.AddString({ _T("b3") },
			_T("Specifies the label text for button 3."),
			MessageBoxPos::MessageBoxPosition().button3.second);

		cmd.AddEnum({ _T("default"), _T("d") },
			_T("Specifies which button is selected by default. Allowed options: 1|2|3."),
			string2default, def);

		cmd.AddString({ _T("monitor"), _T("mon") },
			_T("Specifies the target monitor for display: Primary|Mouse|MousePointer|0|1|2|n (where n is monitor index)."),
			monitor);

		cmd.AddEnum({ _T("position"), _T("pos") },
			_T("Specifies the dialog placement: Center|Top|Bottom|Left|Right|Pointer|xy (explicit coordinates)."),
			string2positionType, posType);

		cmd.AddInt({ _T("x") },
			_T("Specifies the X-axis position offset in pixels."),
			MessageBoxPos::MessageBoxPosition().delta.x);

		cmd.AddInt({ _T("y") },
			_T("Specifies the Y-axis position offset in pixels."),
			MessageBoxPos::MessageBoxPosition().delta.y);

		cmd.AddBool({ _T("windowsReturnCode"), _T("wrc") },
			_T("Enables Windows-style return codes instead of custom button IDs."),
			windowsReturnCode);

		cmd.AddBool({ _T("quiet") },
			_T("Disables output logging to the command line."),
			quiet);
	}

	if (!cmd.ParseCommandLine(_argc, _pArgv, correctParameters) || correctParameters == 0 || help) {
		cmd.Help();
		return 0;
	}

	MessageBoxPos::MessageBoxPosition().type = (MessageBoxPos::POSITION) posType;

	if (message.empty()) {
		wcout << _T("Error - message is empty") << endl;
		return 0;
	}

	monitor = Conversion::ToLower(Conversion::TrimWhiteChar(monitor));
	if (monitor == _T("primary"))			MessageBoxPos::MessageBoxPosition().monitor	= MessageBoxPos::_PRIMARY;
	else if (monitor == _T("mouse"))		MessageBoxPos::MessageBoxPosition().monitor	= MessageBoxPos::_MOUSE;
	else if (monitor == _T("mousepointer"))	MessageBoxPos::MessageBoxPosition().monitor	= MessageBoxPos::_MOUSE_POINTER;
	else {
		MessageBoxPos::MessageBoxPosition().monitor	= MessageBoxPos::_ID;
		MessageBoxPos::MessageBoxPosition().id		= (UINT)Conversion::ToInt(monitor);
	}

	if (type == (int)Type::Ok		|| type == (int)Type::OkCancel)		MessageBoxPos::MessageBoxPosition().button1.first = IDOK;
	if (type == (int)Type::YesNo	|| type == (int)Type::YesNoCancel)	MessageBoxPos::MessageBoxPosition().button1.first = IDYES;

	if (type == (int)Type::OkCancel)									MessageBoxPos::MessageBoxPosition().button2.first = IDCANCEL;
	if (type == (int)Type::YesNo	|| type == (int)Type::YesNoCancel)	MessageBoxPos::MessageBoxPosition().button2.first = IDNO;

	if (type == (int)Type::YesNoCancel)									MessageBoxPos::MessageBoxPosition().button3.first = IDCANCEL;

	Conversion::UnicodeCodeConverter(title);
	Conversion::UnicodeCodeConverter(message);

	UINT flags = static_cast<UINT>(type)
		| static_cast<UINT>(icon)
		| static_cast<UINT>(def)
		| MB_TOPMOST;
	int resultButton = MessageBoxPos::MessageBox(nullptr, (LPCWSTR)message.c_str(), title.c_str(), flags);

	if (resultButton != 0) {
		if (type == (int)Type::YesNo) {
			if (resultButton == IDYES) resultButton = 1;
			else resultButton = 2;
		}
		if (type == (int)Type::YesNoCancel) {
			if (resultButton == IDYES) resultButton = 1;
			else if (resultButton == IDNO) resultButton = 2;
			else resultButton = 3;
		}

		if (windowsReturnCode) {
			switch ((Type)type) {
			case Type::Ok: {
				resultButton = IDOK;
				break;
			}
			case Type::OkCancel: {
				if (resultButton == 1) resultButton = IDOK;
				else if (resultButton == 2) resultButton = IDCANCEL;
				break;
			}
			case Type::YesNo: {
				if (resultButton == 1) resultButton = IDYES;
				else if (resultButton == 2) resultButton = IDNO;
				break;
			}
			case Type::YesNoCancel: {
				if (resultButton == 1) resultButton = IDYES;
				else if (resultButton == 2) resultButton = IDNO;
				else if (resultButton == 3) resultButton = IDCANCEL;
				break;
			}
			}
		}
	}

	if (quiet == false)
		wcout << to_wstring(resultButton);

	return resultButton;
}
