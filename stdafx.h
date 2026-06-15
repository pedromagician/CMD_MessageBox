#pragma once

#include <windows.h>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <io.h>
#include <iostream>
#include <fcntl.h>
#include <atlbase.h>

#include "Debug.h"
#ifdef _DEBUG
#	define new DEBUG_NEW
#endif

using namespace std;

enum class Type {
	Ok				= MB_OK,
	OkCancel		= MB_OKCANCEL,
	YesNo			= MB_YESNO,
	YesNoCancel		= MB_YESNOCANCEL,
};

enum class Icon {
	NoIcon			= 0x00000000L,
	Information		= MB_ICONINFORMATION,
	Question		= MB_ICONQUESTION,
	Warning			= MB_ICONWARNING,
	Error			= MB_ICONERROR,
};

enum class DefaultButton {
	Def1 = MB_DEFBUTTON1,
	Def2 = MB_DEFBUTTON2,
	Def3 = MB_DEFBUTTON3,
};
