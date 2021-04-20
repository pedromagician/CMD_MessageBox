#pragma once

#include "stdafx.h"

class UTL_Cmd
{
public:
	enum Type {
		Ok = MB_OK,
		OkCancel = MB_OKCANCEL,
		YesNo = MB_YESNO,
		YesNoCancel = MB_YESNOCANCEL,
	};

	enum Icon {
		NoIcon = 0x00000000L,
		Information = MB_ICONINFORMATION,
		Question = MB_ICONQUESTION,
		Warning = MB_ICONWARNING,
		Error = MB_ICONERROR,
	};

	enum DefaultButton {
		Def1 = MB_DEFBUTTON1,
		Def2 = MB_DEFBUTTON2,
		Def3 = MB_DEFBUTTON3,
	};

	static int ParseCommandLinbe(int argc, _TCHAR* argv[], int& iCorrectParameters, bool& bHelp, wstring& title, wstring& message, Type& type, Icon& icon, DefaultButton& def);

	static bool IsHelp(wstring comm);
	static bool IsTitle(wstring comm);
	static bool IsMessage(wstring comm);
	static bool IsButton(wstring comm);
	static bool IsDefault(wstring comm);
	static bool IsIcon(wstring comm);

	static void Help();
};
