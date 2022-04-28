#pragma once

#include "stdafx.h"

class UTL_MessageBox {
public:
	enum MONITOR {
		_PRIMARY,
		_MOUSE,
		_MOUSE_POINTER,
		_ID,
	};

	enum POSITION {
		_CENTER,
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
	};

	static thread_local InformationAboutPositionOfMessageBox MessageBoxPosition;

	static int MessageBoxPos(HWND _hWnd, LPCTSTR _text, LPCTSTR _caption, UINT _type, struct UTL_MessageBox::InformationAboutPositionOfMessageBox _position);
private:
	static LRESULT CALLBACK SetPosition(int _code, WPARAM _wParam, LPARAM _lParam);

	static bool GetMonitorInfoPrimary(RECT& _monitor);
	static bool GetMonitorInfoMouse(RECT& _monitor);
	static bool GetMonitorInfoId(UINT _id, RECT& _monitor);
	static long GetDiameterX(RECT _rect);
	static long GetDiameterY(RECT _rect);
};
