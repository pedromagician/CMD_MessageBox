#pragma once

#include "stdafx.h"
#include "UTL_Cmd.h"
#include "UTL_Conversion.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int iCorrectParameters = 0;
	bool bHelp = false;
	wstring title = _T("");
	wstring message = _T("");
	UTL_Cmd::Type type = UTL_Cmd::Type::Ok;
	UTL_Cmd::Icon icon = UTL_Cmd::Icon::NoIcon;
	UTL_Cmd::DefaultButton def = UTL_Cmd::DefaultButton::Def1;

	if (UTL_Cmd::ParseCommandLinbe(argc, argv, iCorrectParameters, bHelp, title, message, type, icon, def) != 0)
		bHelp = true;

	if (bHelp || iCorrectParameters == 0) {
		UTL_Cmd::Help();
		return 0;
	}

	UTL_Conversion::StringReplaceAll(message, _T("\\n"), _T("\n"));

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
