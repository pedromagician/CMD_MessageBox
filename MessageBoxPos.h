#pragma once

#include "stdafx.h"

class MessageBoxPos {
public:
	enum MONITOR : unsigned int
	{
		_PRIMARY = 1,
		_MOUSE,
		_MOUSE_POINTER,
		_ID,
	};

	enum POSITION : unsigned int
	{
		_CENTER = 0,
		_TOP_CENTER = 1,
		_BOTTOM_CENTER = 2,
		_LEFT_CENTER = 3,
		_RIGHT_CENTER = 4,
		_XY = 5,
		_POINTER = 6,
	};

	typedef struct iPOINT
	{
		INT x;
		INT y;
	} iPOINT;

	struct InformationAboutPositionOfMessageBox {
		MONITOR		monitor;
		UINT		id;
		POSITION	type;
		iPOINT		delta;
		POINT		position;
		pair<int, wstring> button1;
		pair<int, wstring> button2;
		pair<int, wstring> button3;
	};

	static InformationAboutPositionOfMessageBox& MessageBoxPosition() { static thread_local InformationAboutPositionOfMessageBox mMessageBoxPosition; return mMessageBoxPosition; };

	static int MessageBox(HWND _hWnd, LPCTSTR _text, LPCTSTR _caption, UINT _type);
private:
	static LRESULT CALLBACK SetPosition(int _code, WPARAM _wParam, LPARAM _lParam);

	static long GetDiameterX(RECT _rect);
	static long GetDiameterY(RECT _rect);
};
