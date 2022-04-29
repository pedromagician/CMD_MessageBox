#pragma once

#include "stdafx.h"

class MessageBoxPos {
public:
	enum MONITOR {
		_PRIMARY = 1,
		_MOUSE,
		_MOUSE_POINTER,
		_ID,
	};

	enum POSITION {
		_CENTER = 1,
		_TOP_CENTER,
		_BOTTOM_CENTER,
		_LEFT_CENTER,
		_RIGHT_CENTER,
		_XY,
		_POINTER,
	};

	struct InformationAboutPositionOfMessageBox {
		MONITOR		monitor;
		UINT		id;
		POSITION	type;
		POINT		delta;
		POINT		position;
		pair<int, wstring> button1;
		pair<int, wstring> button2;
		pair<int, wstring> button3;
	};

	static thread_local InformationAboutPositionOfMessageBox MessageBoxPosition;

	static int MessageBox(HWND _hWnd, LPCTSTR _text, LPCTSTR _caption, UINT _type);
private:
	static LRESULT CALLBACK SetPosition(int _code, WPARAM _wParam, LPARAM _lParam);

	static long GetDiameterX(RECT _rect);
	static long GetDiameterY(RECT _rect);
};
