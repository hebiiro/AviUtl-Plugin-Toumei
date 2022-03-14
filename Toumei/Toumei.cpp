#include "pch.h"
#include "Toumei.h"

//---------------------------------------------------------------------

// オブジェクト設定ダイアログを取得
HWND GetObjectDialog(void)
{
	struct local
	{
		static BOOL CALLBACK proc(HWND hwnd, LPARAM lp)
		{
			if (::GetWindow(hwnd, GW_OWNER) != theApp.m_exeditTimelineWindow)
				return TRUE;

			TCHAR className[MAX_PATH] = {};
			::GetClassName(hwnd, className, MAX_PATH);
			if (::lstrcmpi(className, _T("ExtendedFilterClass")) != 0)
				return TRUE;

			*(HWND*)lp = hwnd;
			return FALSE;
		}
	};

	HWND hwnd = NULL;
	::EnumWindows(local::proc, (LPARAM)&hwnd);
	return hwnd;
}

//---------------------------------------------------------------------

CToumeiApp theApp;

// デバッグ用コールバック関数。デバッグメッセージを出力する。
void ___outputLog(LPCTSTR text, LPCTSTR output)
{
	::OutputDebugString(output);
}

CToumeiApp::CToumeiApp()
{
	m_instance = 0;
	m_filterWindow = 0;
	m_keyboardHook = 0;
	m_cwpHook = 0;

	m_vkCode = 'Q';
	m_vkModifier = VK_MENU;
}

CToumeiApp::~CToumeiApp()
{
}

BOOL CToumeiApp::DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		{
			MY_TRACE(_T("DLL_PROCESS_ATTACH\n"));

			// DLL インスタンスハンドルを m_instance に格納する。
			m_instance = instance;
			MY_TRACE_HEX(m_instance);

			break;
		}
	case DLL_PROCESS_DETACH:
		{
			MY_TRACE(_T("DLL_PROCESS_DETACH\n"));

			break;
		}
	}

	return TRUE;
}

BOOL CToumeiApp::func_init(FILTER *fp)
{
	MY_TRACE(_T("CToumeiApp::func_init()\n"));

	m_filterWindow = fp->hwnd;
	MY_TRACE_HEX(m_filterWindow);

	{
		// ini ファイルから設定を読み込む。
		TCHAR path[MAX_PATH];
		::GetModuleFileName(m_instance, path, MAX_PATH);
		::PathRenameExtension(path, _T(".ini"));
		MY_TRACE_TSTR(path);

		m_exeditTimelineWindow.load(path, _T("Settings"), _T("exeditTimelineWindow"));
		m_exeditObjectDialog.load(path, _T("Settings"), _T("exeditObjectDialog"));

		m_vkCode = ::GetPrivateProfileInt(_T("Settings"), _T("vkCode"), m_vkCode, path);
		m_vkModifier = ::GetPrivateProfileInt(_T("Settings"), _T("vkModifier"), m_vkModifier, path);
	}

	m_keyboardHook = SetWindowsHookEx(WH_KEYBOARD, _keyboardHookProc, 0, ::GetCurrentThreadId());
	m_cwpHook = SetWindowsHookEx(WH_CALLWNDPROC, _cwpHookProc, 0, ::GetCurrentThreadId());

	return TRUE;
}

BOOL CToumeiApp::func_exit(FILTER *fp)
{
	MY_TRACE(_T("CToumeiApp::func_exit()\n"));

	::UnhookWindowsHookEx(m_keyboardHook), m_keyboardHook = 0;
	::UnhookWindowsHookEx(m_cwpHook), m_cwpHook = 0;

	return TRUE;
}

BOOL CToumeiApp::func_proc(FILTER *fp, FILTER_PROC_INFO *fpip)
{
	MY_TRACE(_T("CToumeiApp::func_proc()\n"));

	return FALSE;
}

BOOL CToumeiApp::func_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void *editp, FILTER *fp)
{
	switch (message)
	{
	case WM_FILTER_CHANGE_WINDOW:
		{
			MY_TRACE(_T("CToumeiApp::func_WndProc(WM_FILTER_CHANGE_WINDOW)\n"));

			if (!m_exeditObjectDialog)
			{
				// 拡張編集のタイムラインウィンドウを取得する。
				m_exeditTimelineWindow.init(auls::Exedit_GetWindow(fp));
				MY_TRACE_HEX(m_exeditTimelineWindow);

				// 拡張編集のオブジェクトダイアログを取得する。
				m_exeditObjectDialog.init(GetObjectDialog());
				MY_TRACE_HEX(m_exeditObjectDialog);
			}
			break;
		}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			::SendMessage(::GetWindow(hwnd, GW_OWNER), message, wParam, lParam);
			break;
		}
	}

	return FALSE;
}

LRESULT CALLBACK CToumeiApp::keyboardHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
		return ::CallNextHookEx(m_keyboardHook, code, wParam, lParam);

	if (wParam == m_vkCode && LOWORD(lParam) == 1 && lParam > 0)
	{
		if (!m_vkModifier || ::GetKeyState(m_vkModifier) < 0)
		{
			if (m_exeditTimelineWindow.isTransparent())
			{
				MY_TRACE(_T("マウスの透過を解除します\n"));

				m_exeditTimelineWindow.applyAlpha();
				m_exeditObjectDialog.applyAlpha();
			}
			else
			{
				MY_TRACE(_T("マウスを透過させます\n"));

				m_exeditTimelineWindow.applyAlphaTransparent();
				m_exeditObjectDialog.applyAlphaTransparent();
			}

		    return TRUE;
		}
	}

	return ::CallNextHookEx(m_keyboardHook, code, wParam, lParam);
}

LRESULT CALLBACK CToumeiApp::_keyboardHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	return theApp.keyboardHookProc(code, wParam, lParam);
}

LRESULT CALLBACK CToumeiApp::cwpHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
		return ::CallNextHookEx(m_cwpHook, code, wParam, lParam);

	CWPSTRUCT* cwp = (CWPSTRUCT*)lParam;

	switch (cwp->message)
	{
	case WM_ACTIVATE:
		{
			BOOL active = LOWORD(cwp->wParam);

			if (cwp->hwnd == m_exeditTimelineWindow) m_exeditTimelineWindow.applyAlpha(active);
			if (cwp->hwnd == m_exeditObjectDialog) m_exeditObjectDialog.applyAlpha(active);

			break;
		}
	}

	return ::CallNextHookEx(m_cwpHook, code, wParam, lParam);
}

LRESULT CALLBACK CToumeiApp::_cwpHookProc(int code, WPARAM wParam, LPARAM lParam)
{
	return theApp.cwpHookProc(code, wParam, lParam);
}

//---------------------------------------------------------------------
