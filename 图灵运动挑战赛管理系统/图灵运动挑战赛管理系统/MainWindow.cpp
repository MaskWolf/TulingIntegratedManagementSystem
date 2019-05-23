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
extern HWND hwndEdit;
extern HMENU hMenuMain;
extern HMENU hPopup;
extern LPSTR lpszLatin;
extern pPer head;
extern Ite project[12];
extern int projectNum;
extern pCLink ClassLink;
extern int TotalTimes;
extern int TodayTimes;
extern int JisuanjiTimes;
extern TCHAR FlagTime[30];
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
		"图灵运动挑战赛管理系统", // 窗口名，显示在窗口标题栏上的字符串
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
	CreateButton(hwndMain);
	hwndEdit = CreateEdit(hwndMain, "texta");
	// 获取本窗口客户区的RECT（矩形方框的四个边界点）
	GetClientRect(hwndMain, &rect);
	// 设置子窗口的大小的位置
	SetWindows(&rect);
	// 在EDIT控件中显示文本
	ShowTextOnEdit(lpszLatin);
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
		OninitTimes();
		OninitClassesLink();
		OninitItemNature();
		OninitPersonLink();
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
		GenerateSubsystemFile();
		SaveTimes();
		SaveClassesLink();
		SavePersonLink();
		SaveItemNature();
		DeleteClassesLinkMemory();
		DeletePersonLinkMemory();
		PostQuitMessage(0);
		break;
		// 命令输入，菜单项被选中，或按钮被点击
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDB_IMPORT:
		{
			Fun_Import();
			return 0;
		}
		case IDB_RANK:
		{
			DialogBox(
				hinst, (LPCTSTR)IDD_RANK, hwnd, (DLGPROC)Rank
			);
			return 0;
		}
		case IDB_STATISTIC:
		{
			Fun_Statistic();
			return 0;
		}
		case IDB_EXPORT:
		{
			Fun_Export();
			return 0;
		}
		//点击按钮“更多”弹出对话框
		case IDB_MORE:
		{
			DialogBox(
				hinst, (LPCTSTR)IDD_MORE, hwndMain, (DLGPROC)More
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
	return TRUE;
}

/* ************************************
* void Fun_Import()
* 功能	按钮“导入数据”消息消息处理函数
**************************************/
void Fun_Import()
{
	OPENFILENAME ofn;
	TCHAR szOpenFileNames[80 * MAX_PATH];
	TCHAR szPath[MAX_PATH];
	TCHAR path[MAX_PATH];
	TCHAR* p;
	int nLen = 0;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	ofn.hwndOwner = hwndMain;
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szOpenFileNames;
	ofn.nMaxFile = sizeof(szOpenFileNames);
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrFilter = TEXT("报名信息\0报名信息*.TXT\0全部文件\0*.*\0");
	if (GetOpenFileName(&ofn))
	{
		//把第一个文件名前的复制到szPath,即:
		//如果只选了一个文件,就复制到最后一个'\'
		//如果选了多个文件,就复制到第一个NULL字符
		lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);
		//当只选了一个文件时,下面这个NULL字符是必需的.
		//这里不区别对待选了一个和多个文件的情况
		szPath[ofn.nFileOffset] = '\0';
		nLen = lstrlen(szPath);

		if (szPath[nLen - 1] != '\\')   //如果选了多个文件,则必须加上'\\'
		{
			lstrcat(szPath, TEXT("\\"));
		}

		p = szOpenFileNames + ofn.nFileOffset; //把指针移到第一个文件

		FILE *fp = NULL;
		Sig msg;
		pPer pr = NULL;
		FILE *fpbk = fopen(TEXT("..\\加载文件\\已导入报名信息.txt"), "a");
		if (NULL == fpbk)
		{
			fpbk = fopen(TEXT("..\\加载文件\\已导入报名信息.txt"), "w");
			if (NULL == fpbk)
			{
				return;
			}
		}

		while (*p)
		{
			strcpy(path, "");
			lstrcat(path, szPath);	//给文件名加上路径  
			lstrcat(path, p);		//加上文件名
			fp = fopen(path, "r");
			if (NULL == fp)
			{
				return;
			}
			while (0 == feof(fp))
			{
				fscanf(fp, "%s", msg.time);
				fscanf(fp, "%s", msg.name);
				fscanf(fp, "%s", msg.sex);
				fscanf(fp, "%s", msg.classes);
				fscanf(fp, "%s", msg.id);
				fscanf(fp, "%s", msg.phone);
				fscanf(fp, "%s", msg.project);
				fscanf(fp, "%s\n", msg.score);
				fprintf(fpbk, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", msg.time, msg.name, msg.sex, msg.classes, msg.id, msg.phone, msg.project, msg.score);
				ChangeFlagTime(msg.time);
				MsgProcess(msg);
			}
			fclose(fp);
			p += lstrlen(p) + 1;     //移至下一个文件
		}
		fclose(fpbk);
		MessageBox(hwndMain, TEXT("导入报名信息成功！！！"), TEXT("Tips"), MB_OK);
	}
}


/* ************************************
* More
* 功能	关于"更多"对话框消息处理函数
**************************************/
LRESULT CALLBACK More(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_EXPLAIN)
		{
			EndDialog(hDlg, LOWORD(wParam));
			Edit_AddString(hwndEdit, "——————————————————————————————————————\r\n");
			Edit_AddString(hwndEdit, "说明书\r\n\r\n\r\n");
			Edit_AddString(hwndEdit, "1、导入信息\r\n");
			Edit_AddString(hwndEdit, "(1)当日数据请尽量当日导入。\r\n");
			Edit_AddString(hwndEdit, "(2)每一条导入的数据都会备份到“..\\加载文件\\已导入报名信息.txt”中。\r\n\r\n");
			Edit_AddString(hwndEdit, "2、排行榜\r\n");
			Edit_AddString(hwndEdit, "(1)含括班级、个人、项目排行榜。\r\n");
			Edit_AddString(hwndEdit, "(2)查看排行榜时请根据需求选择男排行榜，女排行榜，全部排行榜(同时选择男女)。\r\n");
			Edit_AddString(hwndEdit, "(3)排行榜仅班级排行榜可以不选择输出范围直接得出排行榜结果。\r\n\r\n");
			Edit_AddString(hwndEdit, "3、数据统计\r\n");
			Edit_AddString(hwndEdit, "主要统计数据有图灵运动挑战赛总参与人次、当日参与人次、计算机学院参与人次、各项目参与人次。\r\n\r\n");
			Edit_AddString(hwndEdit, "4、查询\r\n");
			Edit_AddString(hwndEdit, "通过学号查看该参赛者个人信息和各项目成绩。\r\n\r\n");
			Edit_AddString(hwndEdit, "5、更改个人信息\r\n");
			Edit_AddString(hwndEdit, "通过学号可以选择性的更改该参赛者个人信息。\r\n\r\n");
			Edit_AddString(hwndEdit, "6、新增活动项目\r\n");
			Edit_AddString(hwndEdit, "(1)可自定义活动名称、活动积分，可随时增加新的活动项目。\r\n");
			Edit_AddString(hwndEdit, "(2)正常成绩区间是指输入一个数值上的最小值和最大值。成绩记录方式为【数字】的项目将使用最小值初始化个人成绩，使用最大值在报名系统记录数据时进行不合法数据检测；成绩记录方式为【时间】的项目将使用最大值初始化个人成绩，使用最小值在报名系统记录数据时进行不合法数据检测。例如：1000米跑世界记录在2m20s左右，使用2m20s作为最小值，若在报名系统记录成绩低于最小值系统将会提示错误。\r\n\r\n");
			Edit_AddString(hwndEdit, "7、重置系统\r\n");
			Edit_AddString(hwndEdit, "(1)管理员密码为本程序作者名的全拼(小写字母)。\r\n");
			Edit_AddString(hwndEdit, "(2)请慎用此操作，一旦重置，无法退回以前的数据。\r\n\r\n");
			Edit_AddString(hwndEdit, "8、部门排行榜\r\n");
			Edit_AddString(hwndEdit, "必须导入固定格式部门信息内容的TXT文件，此项功能才能正常使用。\r\n\r\n");
			Edit_AddString(hwndEdit, "固定格式如下：\r\n");
			Edit_AddString(hwndEdit, "部门1的名字\r\n");
			Edit_AddString(hwndEdit, "部门成员1的学号\r\n");
			Edit_AddString(hwndEdit, "部门成员2的学号\r\n");
			Edit_AddString(hwndEdit, "……\r\n");
			Edit_AddString(hwndEdit, "部门成员N的学号\r\n");
			Edit_AddString(hwndEdit, "end\r\n\r\n");
			Edit_AddString(hwndEdit, "部门2的名字\r\n");
			Edit_AddString(hwndEdit, "部门成员1的学号\r\n");
			Edit_AddString(hwndEdit, "部门成员2的学号\r\n");
			Edit_AddString(hwndEdit, "……\r\n");
			Edit_AddString(hwndEdit, "部门成员N的学号\r\n");
			Edit_AddString(hwndEdit, "end\r\n");
			Edit_AddString(hwndEdit, "\r\n");
			Edit_AddString(hwndEdit, "……\r\n");
			Edit_AddString(hwndEdit, "\r\n\r\n");
			Edit_AddString(hwndEdit, "8、有关配套系统【图灵运动挑战赛报名系统】\r\n");
			Edit_AddString(hwndEdit, "(1)配套系统所需初始系统的文件夹存在于“..\\配置文件”，更新配套系统信息仅需将此文件夹复制到相应位置(配套系统运行的上一级目录)即可。\r\n");
			Edit_AddString(hwndEdit, "(2)欲在配套系统上方的滚动显示框增加内容，仅需在在“配置文件\\Edit内容.txt”中增添内容即可。在Edit内容.txt中增添文字时，一段作为一条消息在一个5秒内显示到子系统文本框上，使用=+代替换行符，使用#代替\\t。\r\n");
			Edit_AddString(hwndEdit, "(3)子系统配置文件可通过按钮“导出配置文件”导出到任意目录。\r\n\r\n");
			Edit_AddString(hwndEdit, "——————————————————————————————————————\r\n");
			Edit_AddString(hwndEdit, "\r\n\r\n");
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_DEPARTMENT)
		{
			EndDialog(hDlg, LOWORD(wParam));
			OPENFILENAME ofn;
			char szFile[MAX_PATH];
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = TEXT('\0');
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = TEXT("Text\0*.TXT\0ALL\0*.*\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.hwndOwner = hwndMain;
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
			if (GetOpenFileName(&ofn))
			{
				TCHAR name[50], id[30], str[256];
				int score = 0, flag = 0, i = 0;
				pPer p = NULL;
				FILE *fp = fopen(szFile, "r");
				if (NULL == fp)
				{
					return TRUE;
				}
				Edit_AddString(hwndEdit, "部门名称\t\t总人数\t总成绩\r\n");
				while (0 == feof(fp))
				{
					i = 0;
					flag = 0;
					score = 0;
					fscanf(fp, "%s\n", name);
					do
					{
						fscanf(fp, "%s\n", id);
						if (0 == strcmp(id, "end"))
						{
							flag = 1;
							wsprintf(str, "%s\t%d\t%d\r\n", name, i, score);
							Edit_AddString(hwndEdit, str);
						}
						else
						{
							i++;
							for (p = head; p != NULL; p = p->next)
							{
								if (0 == strcmp(id, p->id))
								{
									score += p->total;
								}
							}
						}
					} while (flag == 0);
				}
				fclose(fp);
				Edit_AddString(hwndEdit, "\r\n\r\n");
			}
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_RESETSYSTEM)
		{
			EndDialog(hDlg, LOWORD(wParam));
			DialogBox(hinst, (LPCTSTR)IDD_LOGIN, hwndMain, (DLGPROC)Login);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_ADDPROJECT)
		{
			EndDialog(hDlg, LOWORD(wParam));
			DialogBox(hinst, (LPCTSTR)IDD_ADDPROJECT, hwndMain, (DLGPROC)AddProject);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_INQUIRE)
		{
			EndDialog(hDlg, LOWORD(wParam));
			DialogBox(hinst, (LPCTSTR)IDD_INQUIRE, hwndMain, (DLGPROC)Inquire);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_CHANGE)
		{
			EndDialog(hDlg, LOWORD(wParam));
			DialogBox(hinst, (LPCTSTR)IDD_CHANGE, hwndMain, (DLGPROC)Change);
			return TRUE;
		}
		// 点击“退出”，结束对话框
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
		}
		break;
	}
	return FALSE;
}


/* ************************************
* Rank
* 功能	关于"排行榜"对话框消息处理函数
**************************************/
LRESULT CALLBACK Rank(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND hwndComboBox = GetDlgItem(hDlg, IDC_TYPE);
		ComboBox_InsertString(hwndComboBox, -1, "个人排行榜");
		ComboBox_InsertString(hwndComboBox, -1, "班级排行榜");
		for (int i = 0; i < projectNum; i++)
		{
			ComboBox_InsertString(hwndComboBox, -1, project[i].name);
		}
	}
	return TRUE;

	case WM_COMMAND:
		// 点击OK，结束对话框
		if (LOWORD(wParam) == IDOK)
		{
			if(FALSE == Fun_Rank(hDlg))
			{
				return TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// 点击“退出”，结束对话框
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
		}
		break;
	}
	return FALSE;
}


/* ************************************
* Inquire
* 功能	关于"查询"对话框消息处理函数
**************************************/
LRESULT CALLBACK Inquire(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			if (FALSE == Fun_Inquire(hDlg))
			{
				return TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// 点击“退出”，结束对话框
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
		}
		break;
	}
	return FALSE;
}

/* ************************************
* Change
* 功能	关于"更改个人信息"对话框消息处理函数
**************************************/
LRESULT CALLBACK Change(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND hwnd = GetDlgItem(hDlg, IDC_CHANGETYPE);
		ComboBox_InsertString(hwnd, -1, "姓名");
		ComboBox_InsertString(hwnd, -1, "性别");
		ComboBox_InsertString(hwnd, -1, "班级");
		ComboBox_InsertString(hwnd, -1, "联系方式");
	}
	return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			if (FALSE == Fun_Change(hDlg))
			{
				return TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// 点击“退出”，结束对话框
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
		}
		break;
	}
	return FALSE;
}

/* ************************************
* AddProject
* 功能	关于"新增活动项目"对话框消息处理函数
**************************************/
LRESULT CALLBACK AddProject(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_TIME)
		{
			HWND hwnd = GetDlgItem(hDlg, IDC_DIGITAL);
			Button_SetCheck(hwnd, BST_UNCHECKED);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_DIGITAL)
		{
			HWND hwnd = GetDlgItem(hDlg, IDC_TIME);
			Button_SetCheck(hwnd, BST_UNCHECKED);
			return TRUE;
		}
		if (LOWORD(wParam) == IDOK)
		{
			if (FALSE == Fun_AddProject(hDlg))
			{
				return TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// 点击“退出”，结束对话框
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
		}
		break;
	}
	return FALSE;
}

/* ************************************
* ResetSystem
* 功能	关于"重置系统"对话框消息处理函数
**************************************/
LRESULT CALLBACK ResetSystem(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		//重置所有人各项目成绩为初始值
		if (LOWORD(wParam) == IDC_BUTTON1)
		{
			int x= MessageBox(hDlg, TEXT("请确定你要重置所有人各项目成绩为初始值！！！\n"), TEXT("Tips"), MB_OKCANCEL | MB_ICONWARNING);
			if (IDCANCEL == x)
			{
				return TRUE;
			}
			else
			{
				for (int i = 0; i < projectNum; i++)
				{
					if (project[i].recordway == 0)
					{
						SetAllProjectScore(i + 1, "0");
					}
					if (project[i].recordway == 1)
					{
						SetAllProjectScore(i + 1, project[i].max);
					}
				}
			}
			MessageBox(hDlg, TEXT("重置所有人各项目成绩为初始值成功！\n"), TEXT("Tips"), MB_OK);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		//重置所有人个人积分为零
		if (LOWORD(wParam) == IDC_BUTTON2)
		{
			int x = MessageBox(hDlg, TEXT("请确定你要重置所有人个人积分为零！！！\n"), TEXT("Tips"), MB_OKCANCEL | MB_ICONWARNING);
			if (IDCANCEL == x)
			{
				return TRUE;
			}
			else
			{
				pPer p = head;
				while (p != NULL)
				{
					p->total = 0;
					p = p->next;
				}
			}
			MessageBox(hDlg, TEXT("重置所有人个人积分为零成功！\n"), TEXT("Tips"), MB_OK);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		//重置各班班级积分为零
		if (LOWORD(wParam) == IDC_BUTTON3)
		{
			int x = MessageBox(hDlg, TEXT("请确定你要重置各班班级积分为零！！！\n"), TEXT("Tips"), MB_OKCANCEL | MB_ICONWARNING);
			if (IDCANCEL == x)
			{
				return TRUE;
			}
			else
			{
				pCLink p = ClassLink;
				while (p != NULL)
				{
					p->score = 0;
					p = p->next;
				}
			}
			MessageBox(hDlg, TEXT("重置各班班级积分为零成功！\n"), TEXT("Tips"), MB_OK);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		//重置系统至最初始状态
		if (LOWORD(wParam) == IDC_BUTTON4)
		{
			int x = MessageBox(hDlg, TEXT("请确定你要重置系统至最初始状态！！！\n"), TEXT("Tips"), MB_OKCANCEL | MB_ICONWARNING);
			if (IDCANCEL == x)
			{
				return TRUE;
			}
			else
			{
				DeleteClassesLinkMemory();
				ClassLink = (pCLink)malloc(sizeof(CLink));
				strcpy(ClassLink->name, "0");
				ClassLink->score = 0;
				ClassLink->next = NULL;

				DeletePersonLinkMemory();
				head = (pPer)malloc(sizeof(Per));
				strcpy(head->name, "0");
				strcpy(head->sex, "男");
				strcpy(head->classes, "0");
				strcpy(head->id, "0");
				strcpy(head->phone, "0");
				head->total = 0;
				for (int i = 0; i < 12; i++)
				{
					SetProjectScore(i + 1, head, "s");
				}
				head->next = NULL;

				projectNum = 0;

				TotalTimes = 0;
				TodayTimes = 0;
				JisuanjiTimes = 0;
			}
			MessageBox(hDlg, TEXT("重置系统至最初始状态成功！\n"), TEXT("Tips"), MB_OK);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// 点击“退出”，结束对话框
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
		}
		break;
	}
	return FALSE;
}


/* ************************************
* Login
* 功能	关于"登陆"对话框消息处理函数
**************************************/
LRESULT CALLBACK Login(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			TCHAR password[60];
			GetDlgItemText(hDlg, IDC_PASSWORD, password, sizeof(password));
			if (0 == strcmp(password, ""))
			{
				MessageBox(hDlg, TEXT("请输如密码！！！\n提示：作者名全拼音。\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
			}
			if (0 == strcmp(password, TEXT("liaojincan")))
			{
				EndDialog(hDlg, LOWORD(wParam));
				DialogBox(hinst, (LPCTSTR)IDD_RESETSYSTEM, hwndMain, (DLGPROC)ResetSystem);
				return TRUE;
			}
			else
			{
				MessageBox(hDlg, TEXT("密码错误！！！\n"), TEXT("Tips"), MB_OK | MB_ICONERROR);
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			return TRUE;
		}
		// 点击“退出”，结束对话框
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
		}
		break;
	}
	return FALSE;
}

/* ************************************
* void Fun_Export()
* 功能	响应按钮“导出配置文件”将子系统
		配置文件保存到用户指定的目录下
**************************************/
void Fun_Export()
{
	TCHAR path[256], szpath[256];
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
		MessageBox(hwndMain, "您取消了操作——保存子系统配置文件！", "Tips:", MB_OK);
	}
	else
	{
		wsprintf(szpath, "%s\\配置文件", szChoose);
		if (!CreateDirectory(szpath, NULL))
		{
			MessageBox(hwndMain, "保存失败！", "Tips:", MB_OK);
		}
		else
		{
			wsprintf(path, "%s\\活动项目.txt", szpath);
			FILE *fp = fopen(path, "w");
			if (NULL == fp)
			{
				return;
			}
			fprintf(fp, "%d\n\n", projectNum);
			for (int i = 0; i < projectNum; i++)
			{
				fprintf(fp, "%s\t%d\t%d\t%s\t%s\t%d\n\n", project[i].name, project[i].recordway, project[i].score, project[i].min, project[i].max, project[i].times);
			}
			fclose(fp);

			wsprintf(path, "%s\\个人信息.txt", szpath);
			fp = fopen(path, "w");
			if (NULL == fp)
			{
				return;
			}
			pPer p = NULL;
			for (p = head; p != NULL; p = p->next)
			{
				fprintf(fp, "%s\t%s\t%s\t%s\t%s\n", p->name, p->sex, p->classes, p->id, p->phone);
			}
			fclose(fp);

			pPer pr = NULL, x = NULL;
			wsprintf(path, "%s\\Edit内容.txt", szpath);
			fp = fopen(path, "w");
			if (NULL == fp)
			{
				return;
			}
			fprintf(fp, "#软件名：图灵运动挑战赛报名系统=+=+#作者：计算机学院-2017211960-廖金灿\n\n");
			fprintf(fp, "欢迎参加图灵运动挑战赛！图灵运动挑战赛是计算机科学与技术学院团总支学生会围绕“三走”为主题策划的活动，为营造整个学校的体育竞技氛围，旨在通过挑战赛的方式，达到让学生走出寝室，走下网络，走向操场的目的！\n\n");
			fprintf(fp, "获取图灵运动挑战赛最新信息，请加入QQ群：529192272（扫描右侧二维码即可加入），每日定时推送排行榜信息！\n\n");
			fprintf(fp, "如过您是教职工或校外人员，无学号可以参与活动，请联系工作人员，工作人员将为您分配“参赛编码”，之后再次参与活动便可通过分配的“参赛编码”进行参与！\n\n");
			fprintf(fp, "如有个人信息错误，请联系工作人员进行登记！=+=+#PS：图灵运动挑战赛由计算机项目管理部主办！\n\n");
			fprintf(fp, "#软件名：图灵运动挑战赛报名系统=+=+#作者：计算机学院-2017211960-廖金灿\n\n");


			pCLink q = NULL, qr = NULL, y = NULL;
			CLink temp;
			for (q = ClassLink; q != NULL; q = q->next)
			{
				for (qr = q->next; qr != NULL; qr = qr->next)
				{
					if (qr->score > q->score)
					{
						strcpy(temp.name, q->name);
						strcpy(q->name, qr->name);
						strcpy(qr->name, temp.name);

						temp.score = q->score;
						q->score = qr->score;
						qr->score = temp.score;
					}
				}
			}
			q = ClassLink;
			fprintf(fp, "班级排行榜公示=+第一名：%s班#成绩为%d=+", q->name, q->score);
			qr = q->next;
			if (NULL != qr)
			{
				fprintf(fp, "第二名：%s班#成绩为%d=+", qr->name, qr->score);
				y = qr->next;
				if (NULL != y)
				{
					fprintf(fp, "第三名：%s班#成绩为%d\n\n", y->name, y->score);
				}
				else
				{
					fprintf(fp, "\n\n");
				}
			}
			else
			{
				fprintf(fp, "\n\n");
			}


			for (p = head; p != NULL; p = p->next)
			{
				for (pr = p->next; pr != NULL; pr = pr->next)
				{
					if (pr->total > p->total)
					{
						SwapNodeData(p, pr);
					}
				}
			}
			p = head;
			fprintf(fp, "个人排行榜公示=+第一名：%s班学号为%s的%s，成绩为%d=+", p->classes, p->id, p->name, p->total);
			pr = p->next;
			if (pr != NULL)
			{
				fprintf(fp, "第二名：%s班学号为%s的%s，成绩为%d=+", pr->classes, pr->id, pr->name, pr->total);
				x = pr->next;
				if (NULL != x)
				{
					fprintf(fp, "第三名：%s班学号为%s的%s，成绩为%d\n\n", x->classes, x->id, x->name, x->total);
				}
				else
				{
					fprintf(fp, "\n\n");
				}
			}
			else
			{
				fprintf(fp, "\n\n");
			}


			for (int i = 0; i < projectNum; i++)
			{
				if (project[i].recordway == 0)
				{
					for (p = head; p != NULL; p = p->next)
					{
						for (pr = p->next; pr != NULL; pr = pr->next)
						{
							if (ShiftScoreToInt(GetProjectScore(i + 1, pr)) > ShiftScoreToInt(GetProjectScore(i + 1, p)))
							{
								SwapNodeData(p, pr);
							}
						}
					}
				}
				if (project[i].recordway == 1)
				{
					for (p = head; p != NULL; p = p->next)
					{
						for (pr = p->next; pr != NULL; pr = pr->next)
						{
							if (ShiftScoreToInt(GetProjectScore(i + 1, pr)) < ShiftScoreToInt(GetProjectScore(i + 1, p)))
							{
								SwapNodeData(p, pr);
							}
						}
					}
				}
				p = head;
				fprintf(fp, "%s排行榜公示=+第一名：%s班学号为%s的%s，成绩为%s=+", project[i].name, p->classes, p->id, p->name, GetProjectScore(i + 1, p));
				pr = p->next;
				if (pr == NULL)
				{
					fprintf(fp, "\n\n");
					continue;
				}
				fprintf(fp, "第二名：%s班学号为%s的%s，成绩为%s=+", pr->classes, pr->id, pr->name, GetProjectScore(i + 1, pr));
				x = pr->next;
				if (x == NULL)
				{
					fprintf(fp, "\n\n");
					continue;
				}
				fprintf(fp, "第三名：%s班学号为%s的%s，成绩为%s\n\n", x->classes, x->id, x->name, GetProjectScore(i + 1, x));
			}
			fclose(fp);
			MessageBox(hwndMain, "保存成功！", "Tips:", MB_OK);
		}
	}
	// 释放
	ILFree(pidlRoot);
}

/* ************************************
* DWORD SetWindows(LPRECT lpRect)
* 功能	本程序中创建了一个控件，有三个子窗口
*			本函数设置子窗口的位置和大小
* 参数	lpRect，指向表示父窗口客户区的RECT
**************************************/
DWORD SetWindows(LPRECT lpRect)
{
	//Button
	HWND HwndButton = GetDlgItem(hwndMain, IDB_IMPORT);
	SetWindowPos(HwndButton, HWND_TOP,
		lpRect->right* 0.825, lpRect->bottom * 0.07,
		lpRect->right * 0.16, lpRect->bottom * 0.13,
		SWP_SHOWWINDOW);
	HwndButton = GetDlgItem(hwndMain, IDB_RANK);
	SetWindowPos(HwndButton, HWND_TOP,
		lpRect->right* 0.825, lpRect->bottom * 0.25,
		lpRect->right * 0.16, lpRect->bottom * 0.13,
		SWP_SHOWWINDOW);
	HwndButton = GetDlgItem(hwndMain, IDB_STATISTIC);
	SetWindowPos(HwndButton, HWND_TOP,
		lpRect->right* 0.825, lpRect->bottom * 0.43,
		lpRect->right * 0.16, lpRect->bottom * 0.13,
		SWP_SHOWWINDOW);
	HwndButton = GetDlgItem(hwndMain, IDB_EXPORT);
	SetWindowPos(HwndButton, HWND_TOP,
		lpRect->right* 0.825, lpRect->bottom * 0.61,
		lpRect->right * 0.16, lpRect->bottom * 0.13,
		SWP_SHOWWINDOW);
	HwndButton = GetDlgItem(hwndMain, IDB_MORE);
	SetWindowPos(HwndButton, HWND_TOP,
		lpRect->right* 0.825, lpRect->bottom * 0.79,
		lpRect->right * 0.16, lpRect->bottom * 0.13,
		SWP_SHOWWINDOW);

	// Edit
	SetWindowPos(hwndEdit, HWND_TOP,
		lpRect->right * 0.01, lpRect->bottom * 0.02,
		lpRect->right * 0.8, lpRect->bottom * 0.96,
		SWP_SHOWWINDOW);
	return 0;
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

