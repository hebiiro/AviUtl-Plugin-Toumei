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
	m_cwpHook = 0;
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

void CToumeiApp::load(int* track_def)
{
	// ini ファイルから設定を読み込む。
	TCHAR path[MAX_PATH] = {};
	::GetModuleFileName(m_instance, path, MAX_PATH);
	::PathRenameExtension(path, _T(".ini"));
	MY_TRACE_TSTR(path);

	m_exeditTimelineWindow.load(path, _T("Settings"), _T("exeditTimelineWindow"));
	m_exeditObjectDialog.load(path, _T("Settings"), _T("exeditObjectDialog"));

	track_def[0] = m_exeditTimelineWindow.getAlphaActive();
	track_def[1] = m_exeditTimelineWindow.getAlphaInactive();
	track_def[2] = m_exeditTimelineWindow.getAlphaTransparent();
	track_def[3] = m_exeditObjectDialog.getAlphaActive();
	track_def[4] = m_exeditObjectDialog.getAlphaInactive();
	track_def[5] = m_exeditObjectDialog.getAlphaTransparent();
}

BOOL CToumeiApp::func_init(FILTER *fp)
{
	MY_TRACE(_T("CToumeiApp::func_init()\n"));

	m_filterWindow = fp->hwnd;
	MY_TRACE_HEX(m_filterWindow);

	m_cwpHook = SetWindowsHookEx(WH_CALLWNDPROC, _cwpHookProc, 0, ::GetCurrentThreadId());

	fp->exfunc->add_menu_item(fp, (LPSTR)"マウス操作の背景通知ON/OFF", fp->hwnd, 1, 0, 0);

	return TRUE;
}

BOOL CToumeiApp::func_exit(FILTER *fp)
{
	MY_TRACE(_T("CToumeiApp::func_exit()\n"));

	::UnhookWindowsHookEx(m_cwpHook), m_cwpHook = 0;

	return TRUE;
}

BOOL CToumeiApp::func_proc(FILTER *fp, FILTER_PROC_INFO *fpip)
{
	MY_TRACE(_T("CToumeiApp::func_proc()\n"));

	return FALSE;
}

BOOL CToumeiApp::func_update(FILTER *fp, int status)
{
	m_exeditTimelineWindow.setAlphaActive(fp->track[0]);
	m_exeditTimelineWindow.setAlphaInactive(fp->track[1]);
	m_exeditTimelineWindow.setAlphaTransparent(fp->track[2]);
	m_exeditObjectDialog.setAlphaActive(fp->track[3]);
	m_exeditObjectDialog.setAlphaInactive(fp->track[4]);
	m_exeditObjectDialog.setAlphaTransparent(fp->track[5]);

	if (fp->check[0])
	{
		m_exeditTimelineWindow.applyAlphaTransparent();
		m_exeditObjectDialog.applyAlphaTransparent();
	}
	else
	{
		m_exeditTimelineWindow.applyAlpha();
		m_exeditObjectDialog.applyAlpha();
	}

	return TRUE;
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
	case WM_FILTER_COMMAND:
		{
			if (wParam == 1)
			{
				fp->check[0] = !fp->check[0];
				func_update(fp, FILTER_UPDATE_STATUS_CHECK+0);
				fp->exfunc->filter_window_update(fp);
			}

			break;
		}
	}

	return FALSE;
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
