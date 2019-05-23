/* 头文件 */
#include <windows.h>
#include <commctrl.h>
#include <Winuser.h>
#include <shellapi.h>
#include "resource.h"
#include "Window.h"
/* 全局变量 */
HINSTANCE hinst;
HWND hwndMain;
HWND hwndListView;
HWND hwndEdit;
int index = 0;
pPer head = NULL;
pTex CycleHead = NULL;
pTex CycleNode = NULL;
Ite project[12];
int projectNum = 0;

/* ************************************
* HWND CreateListView (HWND hwndParent, LPSTR szWindowName)
* 功能	创建 List View
* 参数	hwndParent，父窗口
*			szWindowName，窗口标题
* 返回值	窗口句柄
**************************************/
HWND CreateListView(HWND hwndParent, LPSTR szWindowName)
{
	HWND hWndListView;
	// 创建List View
	hWndListView = CreateWindow(
		WC_LISTVIEW, // List View窗口类
					 // 窗口标题，由于未指定 WS_TILED ，这是一个无标题的窗口
		szWindowName,
		// 窗口样式，可视，子窗口，以及List View样式
		WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_EDITLABELS,
		// 位置和大小，创建完成后使用SetWindows设置
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndParent,		// 父窗口
		(HMENU)ID_LISTVIEW,	//窗口ID
		hinst, NULL);	// 无菜单，无图标
	if (hWndListView == NULL)
	{
		return NULL;
	}

	// 初始化Image List、初始化列
	if (InitListViewImageLists(hWndListView) &&
		InitListViewColumns(hWndListView))
	{
		return hWndListView;
	}
	DestroyWindow(hWndListView);
	return FALSE;
}

/* ************************************
* BOOL InitListViewImageLists(HWND hWndListView)
* 功能	设置 List View 的 ImageList
* 参数	hWndListView，List View 控件窗口
**************************************/
BOOL InitListViewImageLists(HWND hWndListView)
{
	HIMAGELIST himl;
	// 调用 GetImageList 获得ImageList
	GetImageList(&himl);
	// 设置 List View的的 ImageList
	ListView_SetImageList(hWndListView, himl, LVSIL_SMALL);
	return TRUE;
}

/* ************************************
* BOOL InitListViewColumns(HWND hWndListView)
* 功能	设置 List View 的 分栏
* 参数	hWndListView，List View 控件窗口
* 返回值	是否成功
**************************************/
BOOL InitListViewColumns(HWND hWndListView)
{
	char szText[256];
	LVCOLUMN lvc;
	DWORD i;
	float width[8] = {0.052, 0.08,0.06,0.11,0.13 ,0.14 ,0.13,0.1};

	RECT rect;
	GetClientRect(hwndMain, &rect);
	//给list控件添加列标题，设置网格线风格 
	ListView_SetExtendedListViewStyle(hWndListView, LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	// 分栏标题
	LPSTR ColNames[] = { "序号", "姓名", "性别","班级", "学号", "联系方式" ,"参与项目", "成绩" };
	// LVCOLUMN中有效的成员
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	// 填充LVCOLUMN结构
	lvc.pszText = szText;
	lvc.iImage = 1;
	lvc.fmt = LVCFMT_CENTER;	// 居中
	SendMessage(hWndListView, LVM_SETBKCOLOR, 0, RGB(219, 219, 219));	//设置控件的背景色
	SendMessage(hwndListView, LVM_SETTEXTBKCOLOR, 0, RGB(219, 219, 219));	//设置文字的背景色
	
	for (i = 0; i<8; i++)
	{
		//设置栏的长度
		lvc.cx = (rect.right - rect.left) * width[i];
		//	设置栏的文本
		lvc.pszText = ColNames[i];
		lvc.iSubItem = i;
		// 调用ListView_InsertColumn插入分栏
		if (ListView_InsertColumn(hWndListView, i, &lvc) == -1)
		{
			return FALSE;
		}
	}

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

	return TRUE;
}

/* ************************************
* BOOL AddListViewItem(HWND hwndListView, SIGNUPMSG content)
* 功能	为List View增加项
* 参数	hwndListView，父窗口
		index,行数
		content,添加的内容
* 返回值	布尔值
**************************************/
BOOL AddListViewItem(HWND hwndListView, SIGNUPMSG content)
{
	LVITEM lvI;
	TCHAR num[30];
	wsprintf(num, "No.%d", index + 1);

	ZeroMemory(&lvI, sizeof(lvI));
	// 有效的项
	lvI.mask = LVIF_TEXT;

	lvI.iItem = index;		//行
	lvI.iImage = 0;
	lvI.iSubItem = 0;	//列
	// 项的文本和长度
	lvI.pszText = num;
	ListView_InsertItem(hwndListView, &lvI);//插入行的第一个格子
	ListView_SetItemText(hwndListView, index, 1, content.name); //插入行的指定格子
	ListView_SetItemText(hwndListView, index, 2, content.sex); //插入行的指定格子
	ListView_SetItemText(hwndListView, index, 3, content.classes); //插入行的指定格子
	ListView_SetItemText(hwndListView, index, 4, content.id); //插入行的指定格子
	ListView_SetItemText(hwndListView, index, 5, content.phone); //插入行的指定格子
	ListView_SetItemText(hwndListView, index, 6, content.project); //插入行的指定格子

	WriteToTemp(content);

	index++;
	return TRUE;
}

/* ************************************
* BOOL GetImageList(HIMAGELIST * pHiml)
* 功能	TODO 增加ImageList项
* 参数	pHiml，Image List
**************************************/
BOOL GetImageList(HIMAGELIST * pHiml)
{
	return TRUE;
}

/* ************************************
* HWND CreateEdit(HWND hwndP, LPSTR szTitle)
* 功能	创建文本编辑控件
* 参数	hwndP，父窗口句柄
*			szTitle，窗口标题
* 返回值	窗口句柄，失败返回 NULL
**************************************/
HWND CreateEdit(HWND hwndP, LPSTR szTitle)
{
	HWND hwnd;

	hwnd = CreateWindow(
		"EDIT", // 文本编译控件
		NULL, // 没有标题
			  // 窗口样式
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER |
		// 文本框样式
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
		// 位置和大小
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndP, // 父窗口
		(HMENU)NULL, // 无菜单
		hinst, // 实例句柄
		(LPVOID)NULL); // 无图标

	if (!hwnd)
	{
		return NULL;
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	return hwnd;
}

/* ************************************
* DWORD SetEditFont(HWND hwndP, LPSTR szTitle)
* 功能	设置Edit的字体族
* 参数	无
**************************************/
DWORD SetEditFont()
{
	//设置控件字体的大小，类型
	HFONT hFont = CreateFont
	(
		40, 0,    //高度20, 宽取0表示由系统选择最佳值
		0, 0,    //文本倾斜，与字体倾斜都为0
		FW_HEAVY,    //粗体
		0, 0, 0,        //非斜体，无下划线，无中划线
		GB2312_CHARSET,    //字符集
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,        //一系列的默认值
		DEFAULT_PITCH | FF_DONTCARE,
		(TCHAR*)"华文新魏"    //字体名称
	);
	SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, 1);
	return 0;
}

/*************************************
* HFONT ChooseNewFont()
* 功能 选择字体
*
* 返回值 返回字体句柄
**************************************/
HFONT ChooseNewFont()
{
	CHOOSEFONT cf;
	LOGFONT lf;
	HFONT hfont;

	// CHOOSEFONT 结构 
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = (HWND)NULL;
	cf.hDC = (HDC)NULL;
	cf.lpLogFont = &lf;
	cf.iPointSize = 0;
	cf.Flags = CF_SCREENFONTS;
	cf.rgbColors = RGB(0, 0, 0);
	cf.lCustData = 0L;
	cf.lpfnHook = (LPCFHOOKPROC)NULL;
	cf.lpTemplateName = (LPSTR)NULL;
	cf.hInstance = (HINSTANCE)NULL;
	cf.lpszStyle = (LPSTR)NULL;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.nSizeMin = 0;
	cf.nSizeMax = 0;

	// 选择字体对话框 
	ChooseFont(&cf);
	// 得到HFONT 返回
	hfont = CreateFontIndirect(cf.lpLogFont);
	return (hfont);
}

/* ************************************
* HWND CreateButton (HWND hwndParent, LPSTR szWindowName)
* 功能	创建 Button
* 参数	hwndParent，父窗口
*			szWindowName，窗口标题
* 返回值	窗口句柄
**************************************/
BOOL CreateButton(HWND hwndParent)
{
	HINSTANCE button = 0;
	HWND HwndButton;
	//设置控件字体的大小，类型
	HFONT hFont = CreateFont
	(
		40, 0,    //高度20, 宽取0表示由系统选择最佳值
		0, 0,    //文本倾斜，与字体倾斜都为0
		FW_HEAVY,    //粗体
		0, 0, 0,        //非斜体，无下划线，无中划线
		GB2312_CHARSET,    //字符集
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,        //一系列的默认值
		DEFAULT_PITCH | FF_DONTCARE,
		(TCHAR*)"华文行楷"    //字体名称
	);
	HwndButton = CreateWindow("Button",
		"",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndParent,
		(HMENU)IDB_PICTURE,
		button,
		NULL);
	// 位图句柄
	HBITMAP Hbmp1;
	//加载一幅位图
	Hbmp1 = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_BITMAP));
	//主窗口发送一个设置位图消息并发送位图句柄
	SendMessage(HwndButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Hbmp1);
	HwndButton = CreateWindow("Button",
		"首次参加",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndParent,
		(HMENU)IDB_FIRSTJOIN,
		button,
		NULL);
	SendMessage(HwndButton, WM_SETFONT, (WPARAM)hFont, 1);
	HwndButton = CreateWindow("Button",
		"再次参加",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		hwndParent, 
		(HMENU)IDB_AGAINJOIN,
		button,
		NULL);
	SendMessage(HwndButton, WM_SETFONT, (WPARAM)hFont, 1);
	HwndButton = CreateWindow("Button",
		"记录成绩",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndParent, 
		(HMENU)IDB_RECORD, 
		button,
		NULL);
	SendMessage(HwndButton, WM_SETFONT, (WPARAM)hFont, 1);
	return TRUE;
}


/* ************************************
* BOOL FirstSingUp(HWND hDlg)
* 功能	将初次报名信息显示到ListView控件上
* 参数	hDlg，父窗口
* 返回值	成功报名返回TRUE
			失败返回FALSE
**************************************/
BOOL FirstSingUp(HWND hDlg)
{
	SIGNUPMSG x;
	HWND hwnd = GetDlgItem(hDlg, IDC_MAN);
	GetDlgItemText(hDlg, IDC_NAME, x.name, sizeof(x.name));
	if (BST_CHECKED == Button_GetCheck(hwnd))
	{
		strcpy(x.sex, "男");
	}
	hwnd = GetDlgItem(hDlg, IDC_WOMAN);
	if (BST_CHECKED == Button_GetCheck(hwnd))
	{
		strcpy(x.sex, "女");
	}
	GetDlgItemText(hDlg, IDC_CLASS, x.classes, sizeof(x.classes));
	GetDlgItemText(hDlg, IDC_ID, x.id, sizeof(x.id));
	GetDlgItemText(hDlg, IDC_PHONE, x.phone, sizeof(x.phone));
	GetDlgItemText(hDlg, IDC_PROJECT, x.project, sizeof(x.project));
	if (TRUE == BlankTest(hDlg, x))
	{
		return FALSE;
	}
	AddListViewItem(hwndListView, x);
	AddLinkNode(x);
	return TRUE;
}

/* ************************************
* int AgainSingUp(HWND hDlg)
* 功能	将再次报名信息显示到ListView控件上
* 参数	hDlg，父窗口
* 返回值	成功报名返回TRUE
			失败返回FALSE
			信息未填完返回-1
**************************************/
int AgainSingUp(HWND hDlg)
{
	SIGNUPMSG x;
	GetDlgItemText(hDlg, IDC_ID2, x.id, sizeof(x.id));
	GetDlgItemText(hDlg, IDC_PROJECT2, x.project, sizeof(x.project));
	if (0 == strcmp(x.id, ""))
	{
		MessageBox(hDlg, TEXT("请输入你的学号！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return -1;
	}
	if (0 == strcmp(x.project, ""))
	{
		MessageBox(hDlg, TEXT("请选择你想参与的运动项目！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return -1;
	}
	if (FALSE == AutoCompleteContent(hDlg, &x))
	{
		MessageBox(hDlg, TEXT("再次参与失败！\n可能原因：\n1、您首次参与图灵运动挑战赛，请点击“首次参加”按钮报名。\n2、您第一次参与填写信息有误，请点击“首次参加”按钮重新参与。\n3、您今日第一次参与，参与信息未在其他电脑同步，请在首次报名的电脑使用“再次参加”按钮。（参与信息每日同步一次）"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	AddListViewItem(hwndListView, x);
	return TRUE;
}

/* ************************************
* BOOL AutoCompleteContent(HWND hDlg, SIGNUPMSG *px)
* 功能	自动补全再次参加活动人员的其他信息
* 参数	hDlg，父窗口
		px，保存人员信息结构体的地址
* 返回值	成功返回TRUE
			失败返回FALSE
**************************************/
BOOL AutoCompleteContent(HWND hDlg, SIGNUPMSG *px)
{	
	pPer p = head;
	while (p != NULL)
	{
		if (0 == strcmp(px->id, p->id))
		{
			strcpy(px->name, p->name);
			strcpy(px->sex, p->sex);
			strcpy(px->classes, p->classes);
			strcpy(px->phone, p->phone);
			return TRUE;
		}
		p = p->next;
	}
	return FALSE;
}

/* ************************************
* BOOL BlankTest(HWND hDlg, SIGNUPMSG content)
* 功能	确定是否有空白内容
* 参数	content，控件填写的内容
* 返回值	无空白内容返回FALSE
			反之返回TRUE
**************************************/
BOOL BlankTest(HWND hDlg, SIGNUPMSG content)
{
	TCHAR temp[30] = "";
	if (0 == strcmp(content.name, temp))
	{
		MessageBox(hDlg, TEXT("请输入你的名字！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return TRUE;
	}
	if (0 != strcmp(content.sex, "男") && 0 != strcmp(content.sex, "女"))
	{
		MessageBox(hDlg, TEXT("请选择你的性别！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return TRUE;
	}
	if (0 == strcmp(content.classes, temp))
	{
		MessageBox(hDlg, TEXT("请输入你的班级！！！\n如果你没有所属班级，请在班级一栏输入【0】。\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return TRUE;
	}
	if (0 == strcmp(content.id, temp))
	{
		MessageBox(hDlg, TEXT("请输入你的学号！！！\n如果你没有所属学号，请联系工作人员，工作人员将为你分配学号。\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return TRUE;
	}
	if (0 == strcmp(content.phone, temp))
	{
		MessageBox(hDlg, TEXT("请输入你的联系手机号！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return TRUE;
	}
	if (0 == strcmp(content.project, temp))
	{
		MessageBox(hDlg, TEXT("请选择你想参与的运动项目！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* ************************************
* BOOL OninitLink()
* 功能	初始化个人信息的链表
* 参数	无
* 返回值	失败返回FALSE
			反之返回TRUE
**************************************/
BOOL OninitLink()
{
	TCHAR path[256] = TEXT("..\\配置文件\\个人信息.txt");
	FILE *fp = fopen(path, "r");
	if (NULL == fp)
	{
		return FALSE;
	}
	pPer p = NULL, pr = NULL;
	if (head == NULL)
	{
		p = (pPer)malloc(sizeof(Per));
		if (p == NULL)
		{
			return FALSE;
		}
		head = p;

		fscanf(fp, "%s", p->name);
		fscanf(fp, "%s", p->sex);
		fscanf(fp, "%s", p->classes);
		fscanf(fp, "%s", p->id);
		fscanf(fp, "%s", p->phone);
		p->next = NULL;
	}
	while (0 == feof(fp))
	{
		pr = p;
		p = (pPer)malloc(sizeof(Per));
		if (p == NULL)
		{
			return FALSE;
		}

		fscanf(fp, "%s", p->name);
		fscanf(fp, "%s", p->sex);
		fscanf(fp, "%s", p->classes);
		fscanf(fp, "%s", p->id);
		fscanf(fp, "%s", p->phone);
		pr->next = p;
	}
	p->next = NULL;
	fclose(fp);
	return TRUE;
}

/* ************************************
* void WriteToTemp(SIGNUPMSG content)
* 功能		将参与人员信息写入临时文件
* 参数		content，参与人员的信息
* 返回值	无
**************************************/
void WriteToTemp(SIGNUPMSG content)
{
	TCHAR num[30];
	wsprintf(num, "No.%d", index + 1);

	TCHAR path[256] = TEXT("..\\配置文件\\temp.txt");
	FILE *fp = fopen(path, "a+");
	if (NULL == fp)
	{
		fp = fopen(path, "w+");
	}

	fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", num, content.name, content.sex, content.classes, content.id, content.phone, content.project);
	fclose(fp);
}

/* ************************************
* void AddLinkNode(SIGNUPMSG content)
* 功能	将第一次参与人员信息写入链表
* 参数	content，参与人员的信息
* 返回值	无
**************************************/
void AddLinkNode(SIGNUPMSG content)
{
	pPer p = head, pr = NULL;
	while (p->next != NULL)
	{
		p = p->next;
	}
	pr = p;
	p = (pPer)malloc(sizeof(Per));
	if (NULL == p)
	{
		return;
	}
	strcpy(p->name, content.name);
	strcpy(p->sex, content.sex);
	strcpy(p->classes, content.classes);
	strcpy(p->id, content.id);
	strcpy(p->phone, content.phone);
	p->next = NULL;
	pr->next = p;
}

/* ************************************
* BOOL RecordScore(HWND hDlg)
* 功能	记录成绩并将成绩和参赛者信息写入文件
* 参数	hDlg，父窗口句柄
* 返回值	布尔值
**************************************/
BOOL RecordScore(HWND hDlg)
{
	//记录成绩并将其显示在ListView
	int nSelectIndex = ListView_GetSelectionMark(hwndListView);
	if (-1 == nSelectIndex)
	{
		MessageBox(hDlg, TEXT("请选择你你要记录成绩的一项！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return TRUE;
	}
	TCHAR score[30], project[30];
	GetDlgItemText(hDlg, IDC_SCORE, score, sizeof(score));
	if (0 == strcmp(score, ""))
	{
		MessageBox(hDlg, TEXT("请输入成绩！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	ListView_GetItemText(hwndListView, nSelectIndex, 6, project, sizeof(project));
	if (FALSE == FormatControl(project, score))
	{
		return FALSE;
	}
	ListView_SetItemText(hwndListView, nSelectIndex, 7, score); //插入行的指定格子

	//通过一个for循环 能够获取第 nSelectIndex item的所有内容 
	TCHAR wstrText[8][128] = { 0 };
	for (int i = 0; i < 8; i++)
	{
		ListView_GetItemText(hwndListView, nSelectIndex, i, wstrText[i], sizeof(wstrText[i]));
	}

	SYSTEMTIME st;
	GetLocalTime(&st);
	TCHAR Today[30];
	wsprintf(Today, "%d-%d-%d", st.wYear, st.wMonth, st.wDay);

	//将参赛者个人信息与成绩写入文件
	TCHAR path[256] = TEXT("..\\报名信息\\");
	wsprintf(path, "%s%s.txt", path, Today);
	FILE *fp = fopen(path, "a+");
	if (NULL == fp)
	{
		fp = fopen(path, "w+");
		if (fp == NULL)
		{
			MessageBox(hwndMain, TEXT("记录成绩失败！"), "Tips", MB_OK);
		}
	}
	fp = fopen(path, "a+");
	fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", Today, wstrText[1], wstrText[2], wstrText[3], wstrText[4], wstrText[5], wstrText[6], wstrText[7]);
	fclose(fp);
	return TRUE;
}

/* ************************************
* BOOL OninitCycleLink()
* 功能	初始化Edit内容的循环链表
* 参数	无
* 返回值	失败返回FALSE
			反之返回TRUE
**************************************/
BOOL OninitCycleLink()
{
	TCHAR path[256] = TEXT("..\\配置文件\\Edit内容.txt");
	FILE *fp = fopen(path, "r");
	if (NULL == fp)
	{
		return FALSE;
	}
	pTex pr = NULL, p = NULL;
	if (CycleHead == NULL)
	{
		p = (pTex)malloc(sizeof(Tex));
		if (p == NULL)
		{
			return FALSE;
		}
		CycleHead = p;

		fscanf(fp, "%s\n", p->text);
		p->next = NULL;
	}
	while (0 == feof(fp))
	{
		pr = p;
		p = (pTex)malloc(sizeof(Tex));
		if (p == NULL)
		{
			return FALSE;
		}

		fscanf(fp, "%s\n", p->text);
		pr->next = p;
	}
	p->next = NULL;
	for (pTex q = CycleHead; q != NULL; q = q->next)
	{
		ChangToNewline(q->text);
	}
	p->next = CycleHead;
	fclose(fp);
	return TRUE;
}

/* ************************************
* void UpdateEditText()
* 功能	更新Edit中的内容信息
**************************************/
void UpdateEditText()
{
	if (CycleNode == NULL)
	{
		CycleNode = CycleHead;
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)CycleHead->text);
	}
	else
	{
		CycleNode = CycleNode->next;
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)CycleNode->text);
	}
}

/* ************************************
* void DeleteLinkMemory()
* 功能	释放链表占用的系统内存
**************************************/
void DeleteLinkMemory()
{
	pPer p = head, pr = NULL;
	while (p->next != NULL)
	{
		pr = p;
		p = p->next;
		free(pr);
	}
}

/* ************************************
* void DeleteCycleLinkMemory()
* 功能	释放循环链表占用的系统内存
**************************************/
void DeleteCycleLinkMemory()
{
	pTex p = CycleHead, pr = NULL;
	while (p->next != NULL && p->next != CycleHead)
	{
		pr = p;
		p = p->next;
		free(pr);
	}
}

/* ************************************
* void OninitItemNature()
* 功能	初始化项目信息属性
**************************************/
void OninitItemNature()
{
	TCHAR path[256] = TEXT("..\\配置文件\\活动项目.txt");
	FILE *fp = fopen(path, "r");
	if (NULL == fp)
	{
		fp = fopen(path, "w+");
	}
	if (feof(fp) == 0)
	{
		fscanf(fp, "%d\n\n", &projectNum);
	}
	for (int i = 0; i < projectNum; i++)
	{
		fscanf(fp, "%s", project[i].name);
		fscanf(fp, "%d", &project[i].recordway);
		fscanf(fp, "%d", &project[i].score);
		fscanf(fp, "%s", project[i].min);
		fscanf(fp, "%s", project[i].max);
		fscanf(fp, "%d\n\n", &project[i].times);
	}
	fclose(fp);
}


/* ************************************
* BOOL FormatControl(TCHAR str[], TCHAR score[])
* 功能	控制输入的成绩的格式
**************************************/
BOOL FormatControl(TCHAR str[], TCHAR score[])
{
	int i;
	for (i = 0; i < projectNum; i++)
	{
		if (0 == strcmp(str, project[i].name))
		{
			break;
		}
	}
	if (project[i].recordway == 0)
	{
		if (ShiftScoreToInt(score) > ShiftScoreToInt(project[i].max))
		{
			MessageBox(hwndMain, TEXT("您输入的成绩不是一个合理的值！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
			return FALSE;
		}
	}
	if (project[i].recordway == 1)
	{
		if (FALSE == IsTimeFormat(score))
		{
			MessageBox(hwndMain, TEXT("请输入正确格式的成绩！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
			return FALSE;
		}
		if (ShiftScoreToInt(score) < ShiftScoreToInt(project[i].min))
		{
			MessageBox(hwndMain, TEXT("您输入的成绩不是一个合理的值！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
			return FALSE;
		}
	}
	return TRUE;
}

/* ************************************
* BOOL IsTimeFormat(TCHAR str[])
* 功能	判断是否为正确的时间格式
**************************************/
BOOL IsTimeFormat(TCHAR str[])
{
	TCHAR *p = str;
	int m = 0, s = 0;
	for (; *p != '\0'; p++)
	{
		if (*p == 'm')
		{
			m++;
		}
		if (*p == 's')
		{
			s++;
		}
	}
	if (m == 1 && s == 1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/* ************************************
* int ShiftScoreToInt(TCHAR score[])
* 功能	将字符中存储的成绩转化为整型数据并返回
**************************************/
int ShiftScoreToInt(TCHAR score[])
{
	int s = 0;
	if (TRUE == IsTimeFormat(score))
	{
		TCHAR *p, *q, str[5];
		q = str;
		for (p = score; *p != 'm'; p++)
		{
			*q = *p;
			q++;
		}
		*q = '\0';
		s += 60 * atoi(str);
		p++;
		for (q = str; *p != 's'; p++)
		{
			*q = *p;
			q++;
		}
		*q = '\0';
		s += atoi(str);
	}
	else
	{
		s = atoi(score);
	}
	return s;
}


/* ************************************
* void ChangToNewline(TCHAR str[])
* 功能	将字符串中存储的+变成\n
**************************************/
void ChangToNewline(TCHAR str[])
{
	TCHAR *q = str;
	for (; *q != '\0'; q++)
	{
		if (*q == '+')
		{
			*q = '\n';
		}
		if (*q == '=')
		{
			*q = '\r';
		}
		if (*q == '#')
		{
			*q = '\t';
		}
	}
}