﻿#pragma once

//---------------------------------------------------------------------

inline void AddExStyle(HWND hwnd, DWORD exstyle)
{
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | exstyle);
}

inline void ClearExStyle(HWND hwnd, DWORD exstyle)
{
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & ~exstyle);
}

//---------------------------------------------------------------------

class AlphaManager
{
private:
	HWND m_hwnd;
	BYTE m_alphaActive;
	BYTE m_alphaInactive;
	BYTE m_alphaTransparent;
public:

	AlphaManager()
	{
		m_hwnd = 0;
		m_alphaActive = 224;
		m_alphaInactive = 192;
		m_alphaTransparent = 64;
	}

	void load(LPCTSTR path, LPCTSTR section, LPCTSTR name)
	{
		m_alphaActive = ::GetPrivateProfileInt(section, yulib::format(_T("%s%s"), name, _T("AlphaActive")), m_alphaActive, path);
		m_alphaInactive = ::GetPrivateProfileInt(section, yulib::format(_T("%s%s"), name, _T("AlphaInactive")), m_alphaInactive, path);
		m_alphaTransparent = ::GetPrivateProfileInt(section, yulib::format(_T("%s%s"), name, _T("AlphaTransparent")), m_alphaTransparent, path);
	}

	void init(HWND hwnd)
	{
		m_hwnd = hwnd;
		AddExStyle(m_hwnd, WS_EX_LAYERED);
		applyAlpha();
	}

	void applyAlpha()
	{
		applyAlpha(m_hwnd == ::GetActiveWindow());
	}

	void applyAlpha(BOOL active)
	{
		ClearExStyle(m_hwnd, WS_EX_TRANSPARENT);
		if (active)
			::SetLayeredWindowAttributes(m_hwnd, 0, m_alphaActive, LWA_ALPHA);
		else
			::SetLayeredWindowAttributes(m_hwnd, 0, m_alphaInactive, LWA_ALPHA);
	}

	void applyAlphaTransparent()
	{
		AddExStyle(m_hwnd, WS_EX_TRANSPARENT);
		::SetLayeredWindowAttributes(m_hwnd, 0, m_alphaTransparent, LWA_ALPHA);
	}

	operator HWND() const { return m_hwnd; }

	BOOL isTransparent() const
	{
		DWORD exStyle = GetWindowExStyle(m_hwnd);
		return !!(exStyle & WS_EX_TRANSPARENT);
	}

};

class CToumeiApp
{
public:

	HINSTANCE m_instance;
	HWND m_filterWindow;
	HHOOK m_keyboardHook;
	HHOOK m_cwpHook;

	AlphaManager m_exeditTimelineWindow;
	AlphaManager m_exeditObjectDialog;

	BYTE m_vkCode;
	BYTE m_vkModifier;

public:

	CToumeiApp();
	~CToumeiApp();

	BOOL DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved);
	BOOL func_init(FILTER *fp);
	BOOL func_exit(FILTER *fp);
	BOOL func_proc(FILTER *fp, FILTER_PROC_INFO *fpip);
	BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void *editp, FILTER *fp);

	LRESULT CALLBACK keyboardHookProc(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK _keyboardHookProc(int code, WPARAM wParam, LPARAM lParam);

	LRESULT CALLBACK cwpHookProc(int code, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK _cwpHookProc(int code, WPARAM wParam, LPARAM lParam);
};

extern CToumeiApp theApp;

//---------------------------------------------------------------------
