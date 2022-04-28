#include "UTL_MessageBox.h"
#include "Monitors.h"

thread_local UTL_MessageBox::InformationAboutPositionOfMessageBox UTL_MessageBox::MessageBoxPosition;

bool UTL_MessageBox::GetMonitorInfoId(UINT _id, RECT& _monitor)
{
	Monitors monitors;

	if (_id > monitors.iMonitors.size() - 1)
		return false;

	_monitor = monitors.rcMonitors[_id];
	return true;
}

bool UTL_MessageBox::GetMonitorInfoPrimary(RECT& _monitor)
{
	MONITORINFO monitorInfo = { 0 };
	monitorInfo.cbSize = sizeof(monitorInfo);
	HMONITOR hMonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTONULL);

	if (!hMonitor)
		return false;

	if (!GetMonitorInfo(hMonitor, &monitorInfo))
		return false;

	_monitor = monitorInfo.rcMonitor;
	return true;
}

bool UTL_MessageBox::GetMonitorInfoMouse(RECT& _monitor)
{
	POINT mouse;
	::GetCursorPos(&mouse);

	MONITORINFO monitorInfo = { 0 };
	monitorInfo.cbSize = sizeof(monitorInfo);
	HMONITOR hMonitor = MonitorFromPoint(mouse, MONITOR_DEFAULTTONULL);

	if (!hMonitor)
		return false;

	if (!GetMonitorInfo(hMonitor, &monitorInfo))
		return false;

	_monitor = monitorInfo.rcMonitor;
	return true;
}

long UTL_MessageBox::GetDiameterX(RECT _rect)
{
	return _rect.left + (_rect.right - _rect.left) / 2;
}

long UTL_MessageBox::GetDiameterY(RECT _rect)
{
	return _rect.top + (_rect.bottom - _rect.top) / 2;
}

LRESULT CALLBACK UTL_MessageBox::SetPosition(int _code, WPARAM _wParam, LPARAM _lParam)
{
	switch (UTL_MessageBox::MessageBoxPosition.type) {
	case _RIGHT_CENTER: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = UTL_MessageBox::MessageBoxPosition.position.x - s->lpcs->cx;
				s->lpcs->y = UTL_MessageBox::MessageBoxPosition.position.y - s->lpcs->cy / 2;
			}
		}
		break;
	}
	case _LEFT_CENTER: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = UTL_MessageBox::MessageBoxPosition.position.x;
				s->lpcs->y = UTL_MessageBox::MessageBoxPosition.position.y - s->lpcs->cy / 2;
			}
		}
		break;
	}
	case _TOP_CENTER: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = UTL_MessageBox::MessageBoxPosition.position.x - s->lpcs->cx / 2;
				s->lpcs->y = UTL_MessageBox::MessageBoxPosition.position.y;
			}
		}
		break;
	}
	case _BOTTOM_CENTER: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = UTL_MessageBox::MessageBoxPosition.position.x - s->lpcs->cx / 2;
				s->lpcs->y = UTL_MessageBox::MessageBoxPosition.position.y - s->lpcs->cy;
			}
		}
		break;
	}
	case _POINTER:
	case _CENTER: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = UTL_MessageBox::MessageBoxPosition.position.x - s->lpcs->cx / 2;
				s->lpcs->y = UTL_MessageBox::MessageBoxPosition.position.y - s->lpcs->cy / 2;
			}
		}
		break;
	}
	case _XY: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = UTL_MessageBox::MessageBoxPosition.position.x;
				s->lpcs->y = UTL_MessageBox::MessageBoxPosition.position.y;
			}
		}
		break;
	}
	default: {
		_tprintf(wstring(_T("Error - unknown position: ") + to_wstring(UTL_MessageBox::MessageBoxPosition.type) + _T("\n")).c_str());
		break;
	}
	}

	return CallNextHookEx(nullptr, _code, _wParam, _lParam);
}

int UTL_MessageBox::MessageBoxPos(HWND _hWnd, LPCTSTR _text, LPCTSTR _caption, UINT _type, struct UTL_MessageBox::InformationAboutPositionOfMessageBox _position)
{
	UTL_MessageBox::MessageBoxPosition = _position;

	bool monitor = true;
	RECT monitorSize = { 0 };

	switch (UTL_MessageBox::MessageBoxPosition.monitor) {
	case _PRIMARY:
		monitor = GetMonitorInfoPrimary(monitorSize);
		break;
	case _MOUSE:
		monitor = GetMonitorInfoMouse(monitorSize);
		break;
	case _MOUSE_POINTER:
		monitor = GetMonitorInfoMouse(monitorSize);
		UTL_MessageBox::MessageBoxPosition.type = _POINTER;
		break;
	case _ID:
		monitor = GetMonitorInfoId(UTL_MessageBox::MessageBoxPosition.id, monitorSize);
		break;
	}

	if (UTL_MessageBox::MessageBoxPosition.type == _POINTER) {
		RECT mouseMonitorSize = { 0 };
		monitor = GetMonitorInfoMouse(mouseMonitorSize);
		if (EqualRect(&mouseMonitorSize, &monitorSize) == false) {
			UTL_MessageBox::MessageBoxPosition.type = _CENTER;
		}
	}


	if (monitor) {
		switch (UTL_MessageBox::MessageBoxPosition.type) {
		case _CENTER: {
			UTL_MessageBox::MessageBoxPosition.position.x = GetDiameterX(monitorSize);
			UTL_MessageBox::MessageBoxPosition.position.y = GetDiameterY(monitorSize);
			break;
		}
		case _XY: {
			UTL_MessageBox::MessageBoxPosition.position.x = monitorSize.left;
			UTL_MessageBox::MessageBoxPosition.position.y = monitorSize.top;
			break;
		}
		case _TOP_CENTER: {
			UTL_MessageBox::MessageBoxPosition.position.x = GetDiameterX(monitorSize);
			UTL_MessageBox::MessageBoxPosition.position.y = monitorSize.top;
			break;
		}
		case _BOTTOM_CENTER: {
			UTL_MessageBox::MessageBoxPosition.position.x = GetDiameterX(monitorSize);
			UTL_MessageBox::MessageBoxPosition.position.y = monitorSize.bottom;
			break;
		}
		case _LEFT_CENTER: {
			UTL_MessageBox::MessageBoxPosition.position.x = monitorSize.left;
			UTL_MessageBox::MessageBoxPosition.position.y = GetDiameterY(monitorSize);
			break;
		}
		case _RIGHT_CENTER: {
			UTL_MessageBox::MessageBoxPosition.position.x = monitorSize.right;
			UTL_MessageBox::MessageBoxPosition.position.y = GetDiameterY(monitorSize);
			break;
		}
		case _POINTER: {
			POINT mouse;
			::GetCursorPos(&mouse);

			UTL_MessageBox::MessageBoxPosition.position.x = mouse.x;
			UTL_MessageBox::MessageBoxPosition.position.y = mouse.y;

			break;
		}
		default:
			_tprintf(wstring(_T("Error - unknown position: ") + to_wstring(UTL_MessageBox::MessageBoxPosition.type) + _T("\n")).c_str());
			UTL_MessageBox::MessageBoxPosition.type = _CENTER;
			break;
		}
	}
	else {
		_tprintf(wstring(_T("Error - problem loading information from the monitor")).c_str());
		UTL_MessageBox::MessageBoxPosition.monitor = _PRIMARY;
		UTL_MessageBox::MessageBoxPosition.type = _CENTER;
	}

	UTL_MessageBox::MessageBoxPosition.position.x += MessageBoxPosition.delta.x;
	UTL_MessageBox::MessageBoxPosition.position.y += MessageBoxPosition.delta.y;

	HHOOK hHook = SetWindowsHookEx(WH_CBT, &SetPosition, nullptr, GetCurrentThreadId());
	int result = MessageBox(_hWnd, _text, _caption, _type);
	if (hHook) UnhookWindowsHookEx(hHook);
	return result;
}
