#include "UTL_Cmd.h"
#include "UTL_Conversion.h"

bool UTL_Cmd::IsHelp(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-?") || comm == _T("-help") || comm == _T("-h")) return true;
	return false;
}

bool UTL_Cmd::IsTitle(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-title")) return true;
	return false;
}

bool UTL_Cmd::IsMessage(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-message")) return true;
	return false;
}

bool UTL_Cmd::IsButton(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-button")) return true;
	return false;
}

bool UTL_Cmd::IsDefault(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-default")) return true;
	return false;
}

bool UTL_Cmd::IsIcon(wstring comm)
{
	comm = UTL_Conversion::ToLower(comm);
	if (comm == _T("-icon")) return true;
	return false;
}

void UTL_Cmd::Help()
{
	_tprintf(_T("MessageBox 0.1\n"));
	_tprintf(_T("	MessageBox for command line. Amiga Rulez!\n"));
	_tprintf(_T("\nUsage:\n"));
	_tprintf(_T("	MessageBox [OPTIONS]\n"));
	_tprintf(_T("\nOptions:\n"));
	_tprintf(_T("	-title TITLE\n"));
	_tprintf(_T("		The TITLE argument specifies the title of the message box.\n"));
	_tprintf(_T("	-message MESSAGE\n"));
	_tprintf(_T("		The MESSAGE argument specifies the text of the message box. Newline can be embedded using \\n.\n"));
	_tprintf(_T("	-icon ITYPE\n"));
	_tprintf(_T("		The ITYPE argument specifies the text of the message box. Allowed options: NoIcon|Information|Question|Warning|Error.\n"));
	_tprintf(_T("	-button BTYPE\n"));
	_tprintf(_T("		The BTYPE argument specifies the type of the message box. Allowed options: Ok|OkCancel|YesNo|YesNoCancel.\n"));
	_tprintf(_T("	-default N\n"));
	_tprintf(_T("		The N argument specifies the default button. Allowed options: 1|2|3.\n"));
}

int UTL_Cmd::ParseCommandLinbe(int argc, _TCHAR* argv[], int& iCorrectParameters, bool& bHelp, wstring& title, wstring& message, Type& type, Icon& icon, DefaultButton& def)
{
	iCorrectParameters = 0;

	for (int i = 0; i < argc; i++) {
		wstring tmp = argv[i];
		tmp = UTL_Conversion::TrimWhiteChar(tmp);

		if (UTL_Cmd::IsHelp(tmp)) {
			bHelp = true;
			iCorrectParameters++;
			continue;
		}

		if (UTL_Cmd::IsTitle(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				title = tmp;
				iCorrectParameters++;
			}
			continue;
		}

		if (UTL_Cmd::IsMessage(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				message = tmp;
				iCorrectParameters++;
			}
			continue;
		}

		if (UTL_Cmd::IsButton(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				tmp = UTL_Conversion::ToLower(tmp);
				if (tmp == _T("ok")) type = Ok;
				else if (tmp == _T("okcancel")) type = OkCancel;
				else if (tmp == _T("yesno")) type = YesNo;
				else if (tmp == _T("yesnocancel")) type = YesNoCancel;
				else {
					_tprintf(_T("Error - unknown button\n"));
					return 1;
				}
				iCorrectParameters++;
			}
			else {
				_tprintf(_T("Error - unknown button\n"));
				return 1;
			}
			continue;
		}

		if (UTL_Cmd::IsIcon(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				tmp = UTL_Conversion::ToLower(tmp);
				if (tmp == _T("noicon")) icon = NoIcon;
				else if (tmp == _T("information")) icon = Information;
				else if (tmp == _T("question")) icon = Question;
				else if (tmp == _T("warning")) icon = Warning;
				else if (tmp == _T("error")) icon = Error;
				else {
					_tprintf(_T("Error - unknown icon\n"));
					return 1;
				}
				iCorrectParameters++;
			}
			else {
				_tprintf(_T("Error - unknown icon\n"));
				return 1;
			}
			continue;
		}

		if (UTL_Cmd::IsDefault(tmp)) {
			i++;
			if (i < argc) {
				tmp = argv[i];
				tmp = UTL_Conversion::TrimWhiteChar(tmp);
				if (tmp == _T("1")) def = Def1;
				else if (tmp == _T("2")) def = Def2;
				else if (tmp == _T("3")) def = Def3;
				else {
					_tprintf(_T("Error - unknown default button\n"));
					return 1;
				}
				iCorrectParameters++;
			}
			else {
				_tprintf(_T("Error - unknown default button\n"));
				return 1;
			}
			continue;
		}
	}

	return 0;
}
