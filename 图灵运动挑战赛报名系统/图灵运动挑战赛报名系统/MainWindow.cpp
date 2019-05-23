/* 头文件 */
#include <windows.h>
#include <commctrl.h>
#include <Winuser.h>
#include <shellapi.h>
#include "resource.h"
#include "Window.h"
#pragma comment (lib, "comctl32.lib")
/* 外部变量 */
extern HINSTANCE hinst;
extern HWND hwndMain;
extern HWND hwndListView;
extern HWND hwndEdit;
extern int index;
extern pPer head;
extern pTex CycleHead;
extern Ite project[12];
extern int projectNum;
/* ************************************
* WinMain
* 功能	程序入口点，调用InitApplication，InitInstance
*			消息循环
* 参数	hinstance，应用程序本次运行实例
*			hPrevInstance，应用程序之前的实例，始终为NULL
*			lpCmdLine，命令行参数
*			nCmdShow，如果窗口，SW_SHOW
* 返回值	失败返回FALSE
**************************************/
int WINAPI WinMain(
	HINSTANCE hinstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	BOOL fGotMessage;
	//	 InitApplication
	if (!InitApplication(hinstance))
	{
		return FALSE;
	}
	//	 InitInstance
	if (!InitInstance(hinstance, nCmdShow))
	{
		return FALSE;
	}
	//初始化个人信息链表、含Edit内容的循环链表
	if (FALSE == OninitLink() || FALSE == OninitCycleLink())
	{
		MessageBox(hwndMain, TEXT("配置文件“个人信息.txt”或“Edit内容.txt”出错，请检查！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
	}
	// 消息循环
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0 && fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
	UNREFERENCED_PARAMETER(lpCmdLine);
}
/* ************************************
* BOOL InitApplication(HINSTANCE hinstance)
* 功能	注册主窗口类
* 参数	hinstance，应用程序本次运行实例
* 返回值	是否成功
**************************************/
BOOL InitApplication(HINSTANCE hinstance)
{
	// 使用RegisterClassEx
	WNDCLASSEX wcx;
	// 填充结构
	wcx.cbSize = sizeof(wcx); // WNDCLASSEX结构的大小
	wcx.style = CS_HREDRAW | CS_VREDRAW; // 如果大小改变了重绘窗口
	wcx.lpfnWndProc = MainWndProc; // 窗口消息处理函数
	wcx.cbClsExtra = 0; // 无附加窗口类内存
	wcx.cbWndExtra = 0; // 无附加窗口内存
	wcx.hInstance = hinstance; // 应用程序实例
	wcx.hIcon = (HICON)LoadImage(hinstance,
		MAKEINTRESOURCE(IDI_ICON_MAIN),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);	// 图标
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW); // 鼠标指针
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 背景画刷
	wcx.lpszMenuName = NULL; // 菜单资源
	wcx.lpszClassName = "MainWClass"; // 窗口类名
	wcx.hIconSm = (HICON)LoadImage(hinstance, // 小图标
		MAKEINTRESOURCE(IDI_ICON_MAIN),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);
	// 注册窗口类，返回
	return RegisterClassEx(&wcx);
}

/* ************************************
* BOOL InitInstance(HINSTANCE hinstance, int nCmdShow)
* 功能	创建主窗口和控件
* 参数	hinstance，应用程序本次运行实例
*			nCmdShow，如何显示
* 返回值	是否成功
**************************************/
BOOL InitInstance(HINSTANCE hinstance, int nCmdShow)
{
	HWND hwnd;
	RECT rect;
	// 保存应用程序实例句柄
	hinst = hinstance;
	hwnd = CreateWindow(
		"MainWClass", // 窗口类名，使用之前注册的主窗口类
		"图灵运动挑战赛报名系统", // 窗口名，显示在窗口标题栏上的字符串
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
		WS_POPUPWINDOW, 	// 窗口样式
		CW_USEDEFAULT, // 水平位置（默认）
		CW_USEDEFAULT, // 垂直位置（默认）
		800, // 宽
		600, // 高
		(HWND)NULL, // 无父窗口
		NULL, // 菜单
		hinstance, //应用程序实例
		(LPVOID)NULL); // 无窗口创建数据
					   // 窗口是否创建成功
	if (!hwnd)
		return FALSE;
	// 保留窗口句柄
	hwndMain = hwnd;
	// 保证通用控件动态链接库已经加载
	InitCommonControls();
	// 创建两种控件，子窗口
	hwndListView = CreateListView(hwndMain, "processing");
	hwndEdit = CreateEdit(hwndMain, "texta");
	CreateButton(hwndMain);
	// 获取本窗口客户区的RECT（矩形方框的四个边界点）
	GetClientRect(hwndMain, &rect);
	// 设置子窗口的大小的位置
	SetWindows(&rect);
	// 设置EDIT控件的字体
	SetEditFont();
	// 显示主窗口，使用WinMain函数设置的窗口显示方式
	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);
	return TRUE;
}
/* ************************************
* MainWndProc
* 功能	主窗口消息处理函数
**************************************/
LRESULT CALLBACK MainWndProc(
	HWND hwnd, // 窗口句柄
	UINT uMsg, // 消息标识符
	WPARAM wParam, // 消息的第一个参数
	LPARAM lParam)// 消息的第二个参数
{

	switch (uMsg)
	{
	case WM_CREATE:	// 窗口创建时收到此消息
		IsSystemDirectory();
		OninitItemNature();
		SetTimer(hwndMain, ID_TIMER, 5000, TimerProc);
		break;

	case WM_PAINT:		// 窗口被绘制时收到此消息
		break;

	case WM_SIZE:		// 窗口大小改变时
		OnWindowResize();
		break;
		// NOTIFY，通常由控件发送给父窗口，说明控件正在进行某项窗口
	case WM_NOTIFY:
		break;
		// 窗口销毁，点击右上角的关闭按钮会触发此消息
	case WM_DESTROY:
		DeleteLinkMemory();
		DeleteCycleLinkMemory();
		KillTimer(hwndMain, ID_TIMER);
		PostQuitMessage(0);
		break;
		// 命令输入，菜单项被选中，或按钮被点击
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			//点击图片保存报名信息
		case IDB_PICTURE:
		{
			TCHAR path[256];
			// 用于保存路径
			TCHAR szRoot[MAX_PATH];
			TCHAR szChoose[MAX_PATH];
			TCHAR szDisplayName[MAX_PATH];
			// 相关变量
			LPITEMIDLIST pidlRoot = NULL;
			LPITEMIDLIST pidlSelected = NULL;
			BROWSEINFO bi = { 0 };
			LPMALLOC pMalloc = NULL;

			// “浏览文件夹”的根路径，开发人员可根据情况选择，比如只浏览“我的文档”。
			SHGetFolderLocation(NULL, CSIDL_DESKTOP, NULL, 0, &pidlRoot);
			SHGetPathFromIDList(pidlRoot, szRoot);
			// 填充 BROWSEINFO 结构
			bi.hwndOwner = hwndMain;
			bi.pidlRoot = pidlRoot;
			bi.pszDisplayName = szDisplayName;
			bi.lpszTitle = "请选择子系统配置文件保存的位置";
			bi.ulFlags = 0;
			bi.lpfn = NULL;
			bi.lParam = 0;
			// 弹出对话框
			pidlSelected = SHBrowseForFolder(&bi);
			// 选择的文件夹
			SHGetPathFromIDList(pidlSelected, szChoose);
			if (0 == strcmp("", szChoose))
			{
				MessageBox(hwndMain, "您取消了操作——保存报名信息！", "Tips:", MB_OK);
			}
			else
			{
				SYSTEMTIME st;
				GetLocalTime(&st);
				TCHAR Today[30], str[8][30];
				int i;
				wsprintf(Today, "%d-%d-%d", st.wYear, st.wMonth, st.wDay);
				wsprintf(path, TEXT("..\\报名信息\\%s.txt"),Today);
				FILE *fp1 = fopen(path, "r");
				if (NULL == fp1)
				{
					MessageBox(hwndMain, "保存失败！今日尚未有人报名或报名信息文件已被移动！", "Tips:", MB_OK);
					return 0;
				}
				wsprintf(path, "%s\\报名信息.txt", szChoose);
				FILE *fp = fopen(path, "w");
				while (0 == feof(fp1))
				{
					for (i = 0; i < 8; i++)
					{
						fscanf(fp1, "%s\n", str[i]);
					}
					for (i = 0; i < 8; i++)
					{
						fprintf(fp, "%s\t", str[i]);
					}
					fprintf(fp, "\n");
				}
				fclose(fp1);
				fclose(fp);
				MessageBox(hwndMain, "保存成功！", "Tips:", MB_OK);

			}
			// 释放
			ILFree(pidlRoot);
			return 0;
		}
			// 点击按钮“首次参与”
		case IDB_FIRSTJOIN:
		{
			DialogBox(
				hinst, (LPCTSTR)IDD_FIRSTJOIN, hwnd, (DLGPROC)FirstJoin
			);
			return 0;
		}
		// 点击按钮“再次参与”
		case IDB_AGAINJOIN:
		{
			DialogBox(
				hinst, (LPCTSTR)IDD_AGAINJOIN, hwnd, (DLGPROC)AgainJoin
			);
			return 0;
		}
		// 点击按钮“记录成绩”
		case IDB_RECORD:
		{
			DialogBox(
				hinst, (LPCTSTR)IDD_RECORD, hwnd, (DLGPROC)Record
			);
			return 0;
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	// 可以在此处理其他消息
	default:
		break;
	}
	// 有很多消息未做处理，需要由默认窗口消息处理函数来处理
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
/* ************************************
* BOOL OnWindowResize()
* 功能	处理窗口大小改变消息
*			改变子窗口大小和位置
**************************************/
BOOL OnWindowResize()
{
	RECT rcClient;
	GetClientRect(hwndMain, &rcClient);
	SetWindows(&rcClient);
	ResetListViewColumns(hwndListView, &rcClient);

	//设置控件字体的大小，类型
	HFONT hFont = CreateFont
	(
		30, 0,    //高度20, 宽取0表示由系统选择最佳值
		0, 0,    //文本倾斜，与字体倾斜都为0
		FW_HEAVY,    //粗体
		0, 0, 0,        //非斜体，无下划线，无中划线
		GB2312_CHARSET,    //字符集
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,        //一系列的默认值
		DEFAULT_PITCH | FF_DONTCARE,
		(TCHAR*)"楷体"    //字体名称
	);
	SendMessage(hwndListView, WM_SETFONT, (WPARAM)hFont, 1);
	//设置文字的背景色
	SendMessage(hwndListView, LVM_SETTEXTBKCOLOR, 0, RGB(219, 219, 219));
	return TRUE;
}



/* ************************************
* FirstJoin
* 功能	“首次参与”对话框消息处理函数
**************************************/
LRESULT CALLBACK FirstJoin(
	HWND hDlg,		// 对话框窗口句柄
	UINT message,		// 消息
	WPARAM wParam,	// 消息参数
	LPARAM lParam)		// 消息参数
{
	switch (message)
	{
	case WM_INITDIALOG:	
		{
			//初始化Dialog对话框中的控价ComboBox中的内容
			HWND hwnd = GetDlgItem(hDlg, IDC_PROJECT);
			for (int i = 0; i < projectNum; i++)
			{
				ComboBox_InsertString(hwnd, -1, project[i].name);
			}
		}
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_MAN)
		{
			HWND hwnd = GetDlgItem(hDlg, IDC_WOMAN);
			Button_SetCheck(hwnd, BST_UNCHECKED);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_WOMAN)
		{
			HWND hwnd = GetDlgItem(hDlg, IDC_MAN);
			Button_SetCheck(hwnd, BST_UNCHECKED);
			return TRUE;
		}
		// 点击OK，结束对话框
		if (LOWORD(wParam) == IDOK)
		{
			if (FALSE == FirstSingUp(hDlg))
			{
				return TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// 点击“退出”，结束进程
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

/* ************************************
* AgainJoin
* 功能	“再次参与”对话框消息处理函数
**************************************/
LRESULT CALLBACK AgainJoin(
	HWND hDlg,		// 对话框窗口句柄
	UINT message,		// 消息
	WPARAM wParam,	// 消息参数
	LPARAM lParam)		// 消息参数
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		//初始化Dialog对话框中的控价ComboBox中的内容
		HWND hwnd = GetDlgItem(hDlg, IDC_PROJECT2);
		for (int i = 0; i < projectNum; i++)
		{
			ComboBox_InsertString(hwnd, -1, project[i].name);
		}
	}
		return TRUE;

	case WM_COMMAND:
		// 点击OK，结束对话框
		if (LOWORD(wParam) == IDOK)
		{
			if (-1 == AgainSingUp(hDlg))
			{
				return TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// 点击“退出”，结束进程
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

/* ************************************
* Record
* 功能	“记录成绩”对话框消息处理函数
**************************************/
LRESULT CALLBACK Record(
	HWND hDlg,		// 对话框窗口句柄
	UINT message,		// 消息
	WPARAM wParam,	// 消息参数
	LPARAM lParam)		// 消息参数
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		// 点击OK，结束对话框
		if (LOWORD(wParam) == IDOK)
		{
			if (FALSE == RecordScore(hDlg))
			{
				return TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// 点击“退出”，结束进程
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

/* ************************************
* DWORD SetWindows(LPRECT lpRect)
* 功能	本程序中创建了三个控件，有三个子窗口
*			本函数设置子窗口的位置和大小
* 参数	lpRect，指向表示父窗口客户区的RECT
**************************************/
DWORD SetWindows(LPRECT lpRect)
{
	//Button
	HWND HwndButton = GetDlgItem(hwndMain, IDB_PICTURE);
	SetWindowPos(HwndButton, HWND_TOP,
		lpRect->right* 0.835, lpRect->bottom * 0.03,
		lpRect->right * 0.15, lpRect->right * 0.15,
		SWP_SHOWWINDOW);
	HwndButton = GetDlgItem(hwndMain, IDB_FIRSTJOIN);
	SetWindowPos(HwndButton, HWND_TOP,
		lpRect->right* 0.835, lpRect->bottom * 0.35,
		lpRect->right * 0.15, lpRect->bottom * 0.15,
		SWP_SHOWWINDOW);
	HwndButton = GetDlgItem(hwndMain, IDB_AGAINJOIN);
	SetWindowPos(HwndButton, HWND_TOP,
		lpRect->right* 0.835, lpRect->bottom * 0.55,
		lpRect->right * 0.15, lpRect->bottom * 0.15,
		SWP_SHOWWINDOW);
	HwndButton = GetDlgItem(hwndMain, IDB_RECORD);
	SetWindowPos(HwndButton, HWND_TOP,
		lpRect->right* 0.835, lpRect->bottom * 0.75,
		lpRect->right * 0.15, lpRect->bottom * 0.15,
		SWP_SHOWWINDOW);
	// List View
	SetWindowPos(hwndListView, HWND_TOP,
		lpRect->right*0.01, lpRect->bottom * 0.24,
		lpRect->right * 0.8, lpRect->bottom * 0.74,
		SWP_SHOWWINDOW);
	// Edit
	SetWindowPos(hwndEdit, HWND_TOP,
		lpRect->right*0.01, lpRect->bottom * 0.03,
		lpRect->right * 0.8, lpRect->bottom * 0.21,
		SWP_SHOWWINDOW);
	return 0;
}

/* ************************************
* BOOL ResetListViewColumns(HWND hWndListView, LPRECT lpRect)
* 功能	重新设置 List View 的 分栏
* 参数	hWndListView，List View 控件窗口
* 返回值	是否成功
**************************************/
BOOL ResetListViewColumns(HWND hWndListView, LPRECT lpRect)
{
	float width[8] = { 0.052, 0.08,0.06,0.11,0.13 ,0.14 ,0.13,0.1 };
	for (int i = 0; i < 8; i++)
	{
		ListView_SetColumnWidth(hWndListView, i, (lpRect->right - lpRect->left) * width[i]);
	}
	return TRUE;
}

/* ************************************
* TimerProc
* 功能	定时器消息处理函数
**************************************/
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	UpdateEditText();
}

/* ************************************
* void IsSystemDirectory()
* 功能	判断是否在系统目录下
**************************************/
void IsSystemDirectory()
{
	FILE *fp = fopen("test.txt", "w");

	TCHAR path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	TCHAR *p = path;
	p++;
	*p = '\0';

	if (0 == strcmp(path, "C") && NULL == fp)
	{
		MessageBox(hwndMain, TEXT("请以管理员身份运行此程序！"), "Tips", MB_OK);
		exit(0);
	}
}