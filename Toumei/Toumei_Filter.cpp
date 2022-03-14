#include "pch.h"
#include "Toumei.h"

//---------------------------------------------------------------------

LPCSTR track_name[] = { "TLアクティブ", "TL非アクティブ", "TL透過", "ODアクティブ", "OD非アクティブ", "OD透過" };
int   track_def[]  = { 224, 192,  64, 224, 192,  64 };
int   track_min[]  = {   0,   0,   0,   0,   0,   0 };
int   track_max[]  = { 255, 255, 255, 255, 255, 255 };

LPCSTR check_name[] = { "マウス操作を背景に通知" };
int   check_def[]  = { 0 };

//---------------------------------------------------------------------
//		フィルタ構造体のポインタを渡す関数
//---------------------------------------------------------------------
EXTERN_C FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable(void)
{
	theApp.load(track_def);

	static TCHAR filterName[] = TEXT("拡張編集透明化");
	static TCHAR filterInformation[] = TEXT("拡張編集透明化 version 2.0.0 by 蛇色");

	static FILTER_DLL filter =
	{
//		FILTER_FLAG_NO_CONFIG | // このフラグを指定するとウィンドウが作成されなくなってしまう。
		FILTER_FLAG_ALWAYS_ACTIVE | // このフラグがないと「フィルタ」に ON/OFF を切り替えるための項目が追加されてしまう。
		FILTER_FLAG_DISP_FILTER | // このフラグがないと「設定」の方にウィンドウを表示するための項目が追加されてしまう。
//		FILTER_FLAG_WINDOW_THICKFRAME |
//		FILTER_FLAG_WINDOW_SIZE |
		FILTER_FLAG_EX_INFORMATION,
		0, 0,
		filterName,
		sizeof(track_name) / sizeof(*track_name), (TCHAR**)track_name, track_def, track_min, track_max,
		sizeof(check_name) / sizeof(*check_name), (TCHAR**)check_name, check_def,
		NULL,//func_proc,
		func_init,
		func_exit,
		func_update,
		func_WndProc,
		NULL, NULL,
		NULL,
		NULL,
		filterInformation,
		NULL, NULL,
		NULL, NULL, NULL, NULL,
		NULL,
	};

	return &filter;
}

//---------------------------------------------------------------------
//		初期化
//---------------------------------------------------------------------

BOOL func_init(FILTER *fp)
{
	return theApp.func_init(fp);
}

//---------------------------------------------------------------------
//		終了
//---------------------------------------------------------------------
BOOL func_exit(FILTER *fp)
{
	return theApp.func_exit(fp);
}

//---------------------------------------------------------------------
//		フィルタ関数
//---------------------------------------------------------------------
BOOL func_proc(FILTER *fp, FILTER_PROC_INFO *fpip)
{
	return theApp.func_proc(fp, fpip);
}

//---------------------------------------------------------------------
//		更新
//---------------------------------------------------------------------

BOOL func_update(FILTER *fp, int status)
{
	return theApp.func_update(fp, status);
}

//---------------------------------------------------------------------
//		WndProc
//---------------------------------------------------------------------
BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, void *editp, FILTER *fp)
{
	return theApp.func_WndProc(hwnd, message, wParam, lParam, editp, fp);
}

//---------------------------------------------------------------------
//		DllMain
//---------------------------------------------------------------------
BOOL APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	return theApp.DllMain(instance, reason, reserved);
}

//---------------------------------------------------------------------
