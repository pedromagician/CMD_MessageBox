#include "MessageBoxPos.h"
#include "Monitors.h"

thread_local MessageBoxPos::InformationAboutPositionOfMessageBox MessageBoxPos::MessageBoxPosition;

LRESULT CALLBACK MessageBoxPos::SetPosition(int _code, WPARAM _wParam, LPARAM _lParam)
{

	if (_code == HC_ACTION)	{
		CWPSTRUCT* pwp = (CWPSTRUCT*)_lParam;
		if (pwp->message == WM_INITDIALOG) {
			if (MessageBoxPos::MessageBoxPosition.button1.second.empty() == false)
				SetDlgItemText(pwp->hwnd, MessageBoxPos::MessageBoxPosition.button1.first, MessageBoxPos::MessageBoxPosition.button1.second.c_str());

			if (MessageBoxPos::MessageBoxPosition.button2.second.empty() == false)
				SetDlgItemText(pwp->hwnd, MessageBoxPos::MessageBoxPosition.button2.first, MessageBoxPos::MessageBoxPosition.button2.second.c_str());

			if (MessageBoxPos::MessageBoxPosition.button3.second.empty() == false)
				SetDlgItemText(pwp->hwnd, MessageBoxPos::MessageBoxPosition.button3.first, MessageBoxPos::MessageBoxPosition.button3.second.c_str());
		}
	}

	switch (MessageBoxPos::MessageBoxPosition.type) {
	case _RIGHT_CENTER: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = MessageBoxPos::MessageBoxPosition.position.x - s->lpcs->cx;
				s->lpcs->y = MessageBoxPos::MessageBoxPosition.position.y - s->lpcs->cy / 2;
			}
		}
		break;
	}
	case _LEFT_CENTER: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = MessageBoxPos::MessageBoxPosition.position.x;
				s->lpcs->y = MessageBoxPos::MessageBoxPosition.position.y - s->lpcs->cy / 2;
			}
		}
		break;
	}
	case _TOP_CENTER: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = MessageBoxPos::MessageBoxPosition.position.x - s->lpcs->cx / 2;
				s->lpcs->y = MessageBoxPos::MessageBoxPosition.position.y;
			}
		}
		break;
	}
	case _BOTTOM_CENTER: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = MessageBoxPos::MessageBoxPosition.position.x - s->lpcs->cx / 2;
				s->lpcs->y = MessageBoxPos::MessageBoxPosition.position.y - s->lpcs->cy;
			}
		}
		break;
	}
	case _POINTER:
	case _CENTER: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = MessageBoxPos::MessageBoxPosition.position.x - s->lpcs->cx / 2;
				s->lpcs->y = MessageBoxPos::MessageBoxPosition.position.y - s->lpcs->cy / 2;
			}
		}
		break;
	}
	case _XY: {
		if (_code == HCBT_CREATEWND) {
			CBT_CREATEWND* s = (CBT_CREATEWND*)_lParam;
			if (s->lpcs->hwndParent == nullptr) {
				s->lpcs->x = MessageBoxPos::MessageBoxPosition.position.x;
				s->lpcs->y = MessageBoxPos::MessageBoxPosition.position.y;
			}
		}
		break;
	}
	default: {
		_tprintf(wstring(_T("Error - unknown position: ") + to_wstring(MessageBoxPos::MessageBoxPosition.type) + _T("\n")).c_str());
		break;
	}
	}

	return CallNextHookEx(nullptr, _code, _wParam, _lParam);
}

int MessageBoxPos::MessageBox(HWND _hWnd, LPCTSTR _text, LPCTSTR _caption, UINT _type)
{
	bool monitor = true;
	RECT monitorSize = { 0 };

	switch (MessageBoxPos::MessageBoxPosition.monitor) {
	case _PRIMARY:
		monitor = Monitors::GetMonitorInfoPrimary(monitorSize);
		break;
	case _MOUSE:
		monitor = Monitors::GetMonitorInfoMouse(monitorSize);
		break;
	case _MOUSE_POINTER:
		monitor = Monitors::GetMonitorInfoMouse(monitorSize);
		MessageBoxPos::MessageBoxPosition.type = _POINTER;
		break;
	case _ID:
		monitor = Monitors::GetMonitorInfoId(MessageBoxPos::MessageBoxPosition.id, monitorSize);
		break;
	}

	if (MessageBoxPos::MessageBoxPosition.type == _POINTER) {
		RECT mouseMonitorSize = { 0 };
		monitor = Monitors::GetMonitorInfoMouse(mouseMonitorSize);
		if (EqualRect(&mouseMonitorSize, &monitorSize) == false) {
			MessageBoxPos::MessageBoxPosition.type = _CENTER;
		}
	}

	if (monitor) {
		switch (MessageBoxPos::MessageBoxPosition.type) {
		case _CENTER: {
			MessageBoxPos::MessageBoxPosition.position.x = GetDiameterX(monitorSize);
			MessageBoxPos::MessageBoxPosition.position.y = GetDiameterY(monitorSize);
			break;
		}
		case _XY: {
			MessageBoxPos::MessageBoxPosition.position.x = monitorSize.left;
			MessageBoxPos::MessageBoxPosition.position.y = monitorSize.top;
			break;
		}
		case _TOP_CENTER: {
			MessageBoxPos::MessageBoxPosition.position.x = GetDiameterX(monitorSize);
			MessageBoxPos::MessageBoxPosition.position.y = monitorSize.top;
			break;
		}
		case _BOTTOM_CENTER: {
			MessageBoxPos::MessageBoxPosition.position.x = GetDiameterX(monitorSize);
			MessageBoxPos::MessageBoxPosition.position.y = monitorSize.bottom;
			break;
		}
		case _LEFT_CENTER: {
			MessageBoxPos::MessageBoxPosition.position.x = monitorSize.left;
			MessageBoxPos::MessageBoxPosition.position.y = GetDiameterY(monitorSize);
			break;
		}
		case _RIGHT_CENTER: {
			MessageBoxPos::MessageBoxPosition.position.x = monitorSize.right;
			MessageBoxPos::MessageBoxPosition.position.y = GetDiameterY(monitorSize);
			break;
		}
		case _POINTER: {
			POINT mouse;
			::GetCursorPos(&mouse);

			MessageBoxPos::MessageBoxPosition.position.x = mouse.x;
			MessageBoxPos::MessageBoxPosition.position.y = mouse.y;

			break;
		}
		default:
			_tprintf(wstring(_T("Error - unknown position: ") + to_wstring(MessageBoxPos::MessageBoxPosition.type) + _T("\n")).c_str());
			monitor = Monitors::GetMonitorInfoPrimary(monitorSize);
			MessageBoxPos::MessageBoxPosition.position.x = GetDiameterX(monitorSize);
			MessageBoxPos::MessageBoxPosition.position.y = GetDiameterY(monitorSize);
			monitor = Monitors::GetMonitorInfoPrimary(monitorSize);
			break;
		}
	}
	else {
		_tprintf(wstring(_T("Error - problem loading information from the monitor")).c_str());
		MessageBoxPos::MessageBoxPosition.monitor = _PRIMARY;
		MessageBoxPos::MessageBoxPosition.type = _XY;
		MessageBoxPos::MessageBoxPosition.position.x = 0;
		MessageBoxPos::MessageBoxPosition.position.y = 0;
	}

	MessageBoxPos::MessageBoxPosition.position.x += MessageBoxPosition.delta.x;
	MessageBoxPos::MessageBoxPosition.position.y += MessageBoxPosition.delta.y;

	HHOOK hHook1 = SetWindowsHookEx(WH_CBT, &SetPosition, nullptr, GetCurrentThreadId());
	HHOOK hHook2 = SetWindowsHookEx(WH_CALLWNDPROC, &SetPosition, nullptr, GetCurrentThreadId());
		int result = ::MessageBox(_hWnd, _text, _caption, _type);
	if (hHook1) UnhookWindowsHookEx(hHook1);
	if (hHook2) UnhookWindowsHookEx(hHook2);

	return result;
}

long MessageBoxPos::GetDiameterX(RECT _rect)
{
	return _rect.left + (_rect.right - _rect.left) / 2;
}

long MessageBoxPos::GetDiameterY(RECT _rect)
{
	return _rect.top + (_rect.bottom - _rect.top) / 2;
}
