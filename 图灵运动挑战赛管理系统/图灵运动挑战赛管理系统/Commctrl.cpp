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
HWND hwndEdit;
pPer head = NULL;
pCLink ClassLink = NULL;
Ite project[12];
int projectNum = 0;
int TotalTimes = 0;
int TodayTimes = 0;
int JisuanjiTimes = 0;
TCHAR FlagTime[30];

// 在界面上显示的文本
LPSTR lpszLatin = "欢迎使用图灵运动挑战赛管理系统！\r\n如需了解更多与本系统有关信息及其操作，请点击【更多】【说明书】进行查看！\r\n\r\n";


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
		ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
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
* HWND ShowTextOnEdit(HWND hwndP, LPSTR szTitle)
* 功能	向文本框发送消息，设置文本
* 参数	szTextToShow，需要显示的文本
**************************************/
DWORD ShowTextOnEdit(LPSTR szTextToShow)
{
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
	SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, 1);
	// 向文本框发送WM_SETTEXT消息，设置窗口的文本
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)szTextToShow);
	return 0;
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
		"排行榜",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndParent,
		(HMENU)IDB_RANK,
		button,
		NULL);
	SendMessage(HwndButton, WM_SETFONT, (WPARAM)hFont, 1);
	HwndButton = CreateWindow("Button",
		"导入信息",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndParent,
		(HMENU)IDB_IMPORT,
		button,
		NULL);
	SendMessage(HwndButton, WM_SETFONT, (WPARAM)hFont, 1);
	HwndButton = CreateWindow("Button",
		"数据统计",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndParent,
		(HMENU)IDB_STATISTIC,
		button,
		NULL);
	SendMessage(HwndButton, WM_SETFONT, (WPARAM)hFont, 1);
	HwndButton = CreateWindow("Button",
		"导出配置文件",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndParent,
		(HMENU)IDB_EXPORT,
		button,
		NULL);
	SendMessage(HwndButton, WM_SETFONT, (WPARAM)hFont, 1);
	HwndButton = CreateWindow("Button",
		"更多",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		hwndParent,
		(HMENU)IDB_MORE,
		button,
		NULL);
	SendMessage(HwndButton, WM_SETFONT, (WPARAM)hFont, 1);
	return TRUE;
}


/* ************************************
* BOOL OninitPersonLink()
* 功能	从文件“..\\加载文件\\个人数据.txt”中初始化个人信息链表
* 参数	无
* 返回值	布尔值
**************************************/
BOOL OninitPersonLink()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	TCHAR Today[30], date[30];
	wsprintf(Today, "%d-%d-%d", st.wYear, st.wMonth, st.wDay);
	strcpy(FlagTime, Today);

	TCHAR path[256] = TEXT("..\\加载文件\\个人数据.txt");
	FILE *fp = fopen(path, "r");
	if (NULL == fp)
	{
		fp = fopen(path, "w");
	}
	fscanf(fp, "%s\n\n", date);
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
		fscanf(fp, "%d\n", &p->total);

		fscanf(fp, "%s", p->project.x1);
		fscanf(fp, "%s", p->project.x2);
		fscanf(fp, "%s", p->project.x3);
		fscanf(fp, "%s", p->project.x4);
		fscanf(fp, "%s", p->project.x5);
		fscanf(fp, "%s", p->project.x6);
		fscanf(fp, "%s", p->project.x7);
		fscanf(fp, "%s", p->project.x8);
		fscanf(fp, "%s", p->project.x9);
		fscanf(fp, "%s", p->project.x10);
		fscanf(fp, "%s", p->project.x11);
		fscanf(fp, "%s\n", p->project.x12);

		fscanf(fp, "%d", &p->project.y1);
		fscanf(fp, "%d", &p->project.y2);
		fscanf(fp, "%d", &p->project.y3);
		fscanf(fp, "%d", &p->project.y4);
		fscanf(fp, "%d", &p->project.y5);
		fscanf(fp, "%d", &p->project.y6);
		fscanf(fp, "%d", &p->project.y7);
		fscanf(fp, "%d", &p->project.y8);
		fscanf(fp, "%d", &p->project.y9);
		fscanf(fp, "%d", &p->project.y10);
		fscanf(fp, "%d", &p->project.y11);
		fscanf(fp, "%d\n", &p->project.y12);

		if (0 != strcmp(date, Today))
		{
			p->project.y1 = 1;
			p->project.y2 = 1;
			p->project.y3 = 1;
			p->project.y4 = 1;
			p->project.y5 = 1;
			p->project.y6 = 1;
			p->project.y7 = 1;
			p->project.y8 = 1;
			p->project.y9 = 1;
			p->project.y10 = 1;
			p->project.y11 = 1;
			p->project.y12 = 1;
		}

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
		fscanf(fp, "%d\n", &p->total);

		fscanf(fp, "%s", p->project.x1);
		fscanf(fp, "%s", p->project.x2);
		fscanf(fp, "%s", p->project.x3);
		fscanf(fp, "%s", p->project.x4);
		fscanf(fp, "%s", p->project.x5);
		fscanf(fp, "%s", p->project.x6);
		fscanf(fp, "%s", p->project.x7);
		fscanf(fp, "%s", p->project.x8);
		fscanf(fp, "%s", p->project.x9);
		fscanf(fp, "%s", p->project.x10);
		fscanf(fp, "%s", p->project.x11);
		fscanf(fp, "%s\n", p->project.x12);

		fscanf(fp, "%d", &p->project.y1);
		fscanf(fp, "%d", &p->project.y2);
		fscanf(fp, "%d", &p->project.y3);
		fscanf(fp, "%d", &p->project.y4);
		fscanf(fp, "%d", &p->project.y5);
		fscanf(fp, "%d", &p->project.y6);
		fscanf(fp, "%d", &p->project.y7);
		fscanf(fp, "%d", &p->project.y8);
		fscanf(fp, "%d", &p->project.y9);
		fscanf(fp, "%d", &p->project.y10);
		fscanf(fp, "%d", &p->project.y11);
		fscanf(fp, "%d\n", &p->project.y12);

		if (0 != strcmp(date, Today))
		{
			p->project.y1 = 1;
			p->project.y2 = 1;
			p->project.y3 = 1;
			p->project.y4 = 1;
			p->project.y5 = 1;
			p->project.y6 = 1;
			p->project.y7 = 1;
			p->project.y8 = 1;
			p->project.y9 = 1;
			p->project.y10 = 1;
			p->project.y11 = 1;
			p->project.y12 = 1;
		}
		pr->next = p;
	}
	p->next = NULL;
	fclose(fp);
	return TRUE;
}


/* ************************************
* void DeletePersonLinkMemory()
* 功能	释放个人信息链表占用的系统内存
**************************************/
void DeletePersonLinkMemory()
{
	pPer p = head, pr = NULL;
	while (p != NULL)
	{
		pr = p;
		p = p->next;
		free(pr);
	}
}


/* ************************************
* void SavePersonLink()
* 功能	保存个人信息链表到“..\\加载文件\\个人数据.txt”
**************************************/
void SavePersonLink()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	TCHAR Today[30];
	wsprintf(Today, "%d-%d-%d", st.wYear, st.wMonth, st.wDay);

	TCHAR path[256] = TEXT("..\\加载文件\\个人数据.txt");
	FILE *fp = fopen(path, "w");
	if (NULL == fp)
	{
		return;
	}
	fprintf(fp, "%s\n\n", Today);
	pPer p = head;
	while (p != NULL)
	{
		fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%d\n", p->name, p->sex, p->classes, p->id, p->phone, p->total);
		fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", p->project.x1, p->project.x2, p->project.x3, p->project.x4,
			p->project.x5, p->project.x6, p->project.x7, p->project.x8, p->project.x9, p->project.x10, p->project.x11, p->project.x12);
		fprintf(fp, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n", p->project.y1, p->project.y2, p->project.y3, p->project.y4,
			p->project.y5, p->project.y6, p->project.y7, p->project.y8, p->project.y9, p->project.y10, p->project.y11, p->project.y12);
		p = p->next;
	}
	fclose(fp);
}

/* ************************************
* void OninitItemNature()
* 功能	从“..\\加载文件\\活动项目信息.txt”初始化活动项目信息
**************************************/
void OninitItemNature()
{
	TCHAR path[256] = TEXT("..\\加载文件\\活动项目信息.txt");
	FILE *fp = fopen(path, "r");
	if (NULL == fp)
	{
		fp = fopen(path, "w");
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
* void SaveItemNature()
* 功能	保存活动项目信息到“..\\加载文件\\活动项目信息.txt”
**************************************/
void SaveItemNature()
{
	TCHAR path[256] = TEXT("..\\加载文件\\活动项目信息.txt");
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
}

/* ************************************
* BOOL Fun_AddProject(HWND hDlg)
* 功能	新增活动项目并将初始化信息
**************************************/
BOOL Fun_AddProject(HWND hDlg)
{
	//保证用户输入的信息符合系统所需条件
	TCHAR name[30], min[15], max[15], score[15], temp[15] = "";
	GetDlgItemText(hDlg, IDC_NAME, name, sizeof(name));
	GetDlgItemText(hDlg, IDC_MIN, min, sizeof(min));
	GetDlgItemText(hDlg, IDC_MAX, max, sizeof(max));
	GetDlgItemText(hDlg, IDC_SCORE, score, sizeof(score));
	if (0 == strcmp(name, temp))
	{
		MessageBox(hDlg, TEXT("请输入新增项目的名称！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	HWND hwnd1 = GetDlgItem(hDlg, IDC_DIGITAL);
	HWND hwnd2 = GetDlgItem(hDlg, IDC_TIME);
	if (BST_UNCHECKED == Button_GetCheck(hwnd1) && BST_UNCHECKED == Button_GetCheck(hwnd2))
	{
		MessageBox(hDlg, TEXT("请选择成绩记录方式！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if (0 == strcmp(min, temp))
	{
		MessageBox(hDlg, TEXT("请输入新增项目的正常成绩区间数值最小值！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if (0 == strcmp(max, temp))
	{
		MessageBox(hDlg, TEXT("请输入新增项目的正常成绩区间数值最大值！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if (0 == strcmp(score, temp))
	{
		MessageBox(hDlg, TEXT("请输入新增项目的项目积分！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if (FALSE == IsTimeFormat(min) && BST_CHECKED == Button_GetCheck(hwnd2))
	{
		MessageBox(hDlg, TEXT("请用正确的格式输入新增项目的正常成绩区间数值最小值！！！\n例如：10m20s、4m24s、40m5s等\nPS:如果成绩在1小时以上的，请将小时转化为分钟数输入!\n1h12m25s->->->->72m25s")
			, TEXT("错误"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	if (FALSE == IsTimeFormat(max) && BST_CHECKED == Button_GetCheck(hwnd2))
	{
		MessageBox(hDlg, TEXT("请用正确的格式输入新增项目的正常成绩区间数值最大值！！！\n例如：10m20s、4m24s、40m5s等\nPS:如果成绩在1小时以上的，请将小时转化为分钟数输入!\n1h12m25s->->->->72m25s")
			, TEXT("错误"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	//保证同时存在的活动项目数量不超出12个
	if (projectNum >= 12)
	{
		MessageBox(hDlg, TEXT("最多同时存在12个活动项目！！！\n"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return TRUE;
	}


	strcpy(project[projectNum].name, name);
	if (BST_CHECKED == Button_GetCheck(hwnd1))
	{
		SetAllProjectScore(projectNum + 1, min);
		project[projectNum].recordway = 0;
	}
	if (BST_CHECKED == Button_GetCheck(hwnd2))
	{
		SetAllProjectScore(projectNum + 1, max);
		project[projectNum].recordway = 1;
	}
	project[projectNum].score = atoi(score);
	strcpy(project[projectNum].min, min);
	strcpy(project[projectNum].max, max);
	project[projectNum].times = 0;
	projectNum++;

	TCHAR str[256];
	wsprintf(str, TEXT("您已经成功添加第%d个活动项目：%s\n"), projectNum, project[projectNum - 1].name);
	MessageBox(hDlg, str, TEXT("Tips"), MB_OK);

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
* TCHAR *ShiftScoreToChar(int score)
* 功能	将整型字符转换为时间格式的成绩字符串并返回
**************************************/
TCHAR *ShiftScoreToChar(int score)
{
	static TCHAR str[15];
	wsprintf(str, "%dm%ds", score / 60, score % 60);
	return str;
}

/* ************************************
* void SetAllProjectScore(int num, TCHAR str[])
* 功能	设置所有人员某一项目的成绩
**************************************/
void SetAllProjectScore(int num, TCHAR str[])
{
	pPer p = head;
	switch (num)
	{
	case 1:
	{
		while (NULL != p)
		{
			strcpy(p->project.x1, str);
			p = p->next;
		}
	}
	break;
	case 2:
	{
		while (NULL != p)
		{
			strcpy(p->project.x2, str);
			p = p->next;
		}
	}
	break;
	case 3:
	{
		while (NULL != p)
		{
			strcpy(p->project.x3, str);
			p = p->next;
		}
	}
	break;
	case 4:
	{
		while (NULL != p)
		{
			strcpy(p->project.x4, str);
			p = p->next;
		}
	}
	break;
	case 5:
	{
		while (NULL != p)
		{
			strcpy(p->project.x5, str);
			p = p->next;
		}
	}
	break;
	case 6:
	{
		while (NULL != p)
		{
			strcpy(p->project.x6, str);
			p = p->next;
		}
	}
	break;
	case 7:
	{
		while (NULL != p)
		{
			strcpy(p->project.x7, str);
			p = p->next;
		}
	}
	break;
	case 8:
	{
		while (NULL != p)
		{
			strcpy(p->project.x8, str);
			p = p->next;
		}
	}
	break;
	case 9:
	{
		while (NULL != p)
		{
			strcpy(p->project.x9, str);
			p = p->next;
		}
	}
	break;
	case 10:
	{
		while (NULL != p)
		{
			strcpy(p->project.x10, str);
			p = p->next;
		}
	}
	break;
	case 11:
	{
		while (NULL != p)
		{
			strcpy(p->project.x11, str);
			p = p->next;
		}
	}
	break;
	case 12:
	{
		while (NULL != p)
		{
			strcpy(p->project.x12, str);
			p = p->next;
		}
	}
	break;
	default:
		break;
	}
}


BOOL Fun_Inquire(HWND hDlg)
{
	TCHAR id[30], str[512];
	GetDlgItemText(hDlg, IDC_ID, id, sizeof(id));
	if (0 == strcmp(id, ""))
	{
		MessageBox(hDlg, TEXT("请输入需要查询的参赛人员的学号！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	pPer p = head;
	while (NULL != p)
	{
		if (0 == strcmp(id, p->id))
		{
			wsprintf(str, "姓名：%s\r\n性别：%s\r\n班级：%s\r\n学号：%s\r\n联系方式：%s\r\n总积分：%d\r\n"
				, p->name, p->sex, p->classes, p->id, p->phone, p->total);
			for (int i = 0; i < projectNum; i++)
			{
				wsprintf(str, "%s%s：%s\r\n", str, project[i].name, GetProjectScore(i + 1, p));
			}
			Edit_AddString(hwndEdit, str);
			Edit_AddString(hwndEdit, "\r\n\r\n");
			return TRUE;
		}
		p = p->next;
	}
	MessageBox(hDlg, TEXT("未查询到该学号的参赛人员！！！\n可能原因：\n1、数据未进行导入更新。\n2、该参赛人员参与活动时填写了错误的学号。\n3、你输入了错误的学号。\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
	return TRUE;
}

void Edit_AddString(HWND hwndCtl, TCHAR str[])
{
	int length = Edit_GetTextLength(hwndCtl);
	Edit_SetSel(hwndCtl, length, -1);
	Edit_ReplaceSel(hwndCtl, str);
}

TCHAR *GetProjectScore(int num, pPer p)
{
	switch (num)
	{
	case 1:
	{
		return p->project.x1;
	}
	break;
	case 2:
	{
		return p->project.x2;
	}
	break;
	case 3:
	{
		return p->project.x3;
	}
	break;
	case 4:
	{
		return p->project.x4;
	}
	break;
	case 5:
	{
		return p->project.x5;
	}
	break;
	case 6:
	{
		return p->project.x6;
	}
	break;
	case 7:
	{
		return p->project.x7;
	}
	break;
	case 8:
	{
		return p->project.x8;
	}
	break;
	case 9:
	{
		return p->project.x9;
	}
	break;
	case 10:
	{
		return p->project.x10;
	}
	break;
	case 11:
	{
		return p->project.x11;
	}
	break;
	default:
	{
		return p->project.x12;
	}
	break;
	}
}

void MsgProcess(Sig Msg)
{
	TotalTimes++;
	TodayTimes++;
	if (TRUE == is_jisuanji(Msg.classes))
	{
		JisuanjiTimes++;
	}

	int i, x;
	for (i = 0; i < projectNum; i++)
	{
		if (0 == strcmp(project[i].name, Msg.project))
		{
			x = i + 1;
			project[i].times++;
			break;
		}
	}

	if (project[i].recordway == 0)
	{
		if (ShiftScoreToInt(Msg.score) > ShiftScoreToInt(project[i].max))
		{
			return;
		}
	}
	if (project[i].recordway == 1)
	{
		if (ShiftScoreToInt(Msg.score) < ShiftScoreToInt(project[i].min))
		{
			return;
		}
	}

	pPer p = head;
	while (p != NULL)
	{
		if (0 == strcmp(p->id, Msg.id))
		{
			if (TRUE == IsTodayFirst(x, p))
			{
				AddClassesScore(Msg.classes, project[x - 1].score);
				p->total += project[x - 1].score;
			}
			if (ShiftScoreToInt(Msg.score) > ShiftScoreToInt(GetProjectScore(x, p)) && project[x - 1].recordway == 0)
			{
				SetProjectScore(x, p, Msg.score);
			}
			if (ShiftScoreToInt(Msg.score) < ShiftScoreToInt(GetProjectScore(x, p)) && project[x - 1].recordway == 1)
			{
				SetProjectScore(x, p, Msg.score);
			}
			return;
		}
		p = p->next;
	}

	CreatNewNode(Msg, x);
}

BOOL IsTodayFirst(int num, pPer p)
{
	switch (num)
	{
	case 1:
	{
		if (p->project.y1 == 1)
		{
			p->project.y1 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	case 2:
	{
		if (p->project.y2 == 1)
		{
			p->project.y2 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	case 3:
	{
		if (p->project.y3 == 1)
		{
			p->project.y3 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	case 4:
	{
		if (p->project.y4 == 1)
		{
			p->project.y4 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	case 5:
	{
		if (p->project.y5 == 1)
		{
			p->project.y5 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	case 6:
	{
		if (p->project.y6 == 1)
		{
			p->project.y6 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	case 7:
	{
		if (p->project.y7 == 1)
		{
			p->project.y7 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	case 8:
	{
		if (p->project.y8 == 1)
		{
			p->project.y8 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	case 9:
	{
		if (p->project.y9 == 1)
		{
			p->project.y9 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	case 10:
	{
		if (p->project.y10 == 1)
		{
			p->project.y10 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	case 11:
	{
		if (p->project.y11 == 1)
		{
			p->project.y11 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	default:
	{
		if (p->project.y12 == 1)
		{
			p->project.y12 = 0;
			return TRUE;
		}
		return FALSE;
	}
	break;
	}
}

void SetProjectScore(int num, pPer p, TCHAR str[])
{
	switch (num)
	{
	case 1:
	{
		strcpy(p->project.x1, str);
		p->project.y1 = 0;
	}
	break;
	case 2:
	{
		strcpy(p->project.x2, str);
		p->project.y2 = 0;
	}
	break;
	case 3:
	{
		strcpy(p->project.x3, str);
		p->project.y3 = 0;
	}
	break;
	case 4:
	{
		strcpy(p->project.x4, str);
		p->project.y4 = 0;
	}
	break;
	case 5:
	{
		strcpy(p->project.x5, str);
		p->project.y5 = 0;
	}
	break;
	case 6:
	{
		strcpy(p->project.x6, str);
		p->project.y6 = 0;
	}
	break;
	case 7:
	{
		strcpy(p->project.x7, str);
		p->project.y7 = 0;
	}
	break;
	case 8:
	{
		strcpy(p->project.x8, str);
		p->project.y8 = 0;
	}
	break;
	case 9:
	{
		strcpy(p->project.x9, str);
		p->project.y9 = 0;
	}
	break;
	case 10:
	{
		strcpy(p->project.x10, str);
		p->project.y10 = 0;
	}
	break;
	case 11:
	{
		strcpy(p->project.x11, str);
		p->project.y11 = 0;
	}
	break;
	default:
	{
		strcpy(p->project.x12, str);
		p->project.y12 = 0;
	}
	break;
	}
}

BOOL Fun_Change(HWND hDlg)
{
	TCHAR id[30], changContent[30], str[512];
	HWND hwnd = GetDlgItem(hDlg, IDC_CHANGETYPE);
	int x = ComboBox_GetCurSel(hwnd);
	GetDlgItemText(hDlg, IDC_ID2, id, sizeof(id));
	GetDlgItemText(hDlg, IDC_CHANGECONTENT, changContent, sizeof(changContent));
	if (0 == strcmp(id, ""))
	{
		MessageBox(hDlg, TEXT("请输入需要更改信息的参赛人员的学号！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if (-1 == x)
	{
		MessageBox(hDlg, TEXT("请选择需要更改信息的一项！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if (0 == strcmp(changContent, ""))
	{
		MessageBox(hDlg, TEXT("请输入需要更改为的信息内容！！！\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	pPer p = head;
	while (NULL != p)
	{
		if (0 == strcmp(id, p->id))
		{
			switch (x)
			{
			case 0:
				strcpy(p->name, changContent);
				break;
			case 1:
				strcpy(p->sex, changContent);
				break;
			case 2:
				strcpy(p->classes, changContent);
				break;
			case 3:
				strcpy(p->phone, changContent);
				break;
			default:
				break;
			}
			MessageBox(hDlg, TEXT("更改成功！！！\n"), TEXT("Tips"), MB_OK);
			wsprintf(str, "更改后信息如下\r\n姓名：%s\r\n性别：%s\r\n班级：%s\r\n学号：%s\r\n联系方式：%s\r\n"
				, p->name, p->sex, p->classes, p->id, p->phone);
			Edit_AddString(hwndEdit, str);
			Edit_AddString(hwndEdit, "\r\n\r\n");
			return TRUE;
		}
		p = p->next;
	}
	MessageBox(hDlg, TEXT("未查询到该学号的参赛人员！！！\n可能原因：\n1、数据未进行导入更新。\n2、该参赛人员参与活动时填写了错误的学号。\n3、你输入了错误的学号。\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
	return TRUE;
}

void OninitTimes()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	TCHAR Today[30], date[30];
	wsprintf(Today, "%d-%d-%d", st.wYear, st.wMonth, st.wDay);

	TCHAR path[256] = TEXT("..\\加载文件\\统计数据.txt");
	FILE *fp = fopen(path, "r");
	if (NULL == fp)
	{
		return;
	}
	fscanf(fp, "%s", date);
	fscanf(fp, "%d", &TotalTimes);
	fscanf(fp, "%d", &JisuanjiTimes);
	fscanf(fp, "%d", &TodayTimes);
	if (0 != strcmp(Today, date))
	{
		TodayTimes = 0;
	}
}

void SaveTimes()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	TCHAR Today[30];
	wsprintf(Today, "%d-%d-%d", st.wYear, st.wMonth, st.wDay);
	TCHAR path[256] = TEXT("..\\加载文件\\统计数据.txt");
	FILE *fp = fopen(path, "w");
	if (NULL == fp)
	{
		return;
	}
	fprintf(fp, "%s\n%d\n%d\n%d", Today, TotalTimes, JisuanjiTimes, TodayTimes);
}

void Fun_Statistic()
{
	TCHAR str[512];
	wsprintf(str, "————————————\r\n图灵运动挑战赛数据分析\r\n\r\n总参与人次：%d\r\n计算机学院参与人次：%d\r\n今日参与人次：%d\r\n\r\n各项目参与人次\r\n", TotalTimes, JisuanjiTimes, TodayTimes);
	for (int i = 0; i < projectNum; i++)
	{
		wsprintf(str, "%s%s：%d\r\n", str, project[i].name, project[i].times);
	}
	Edit_AddString(hwndEdit, str);
	Edit_AddString(hwndEdit, "————————————\r\n");
	Edit_AddString(hwndEdit, "\r\n\r\n");
}


void SwapNodeData(pPer p, pPer pr)
{
	Per temp;

	strcpy(temp.name, pr->name);
	strcpy(temp.sex, pr->sex);
	strcpy(temp.classes, pr->classes);
	strcpy(temp.id, pr->id);
	strcpy(temp.phone, pr->phone);
	temp.total = pr->total;
	temp.project = pr->project;

	strcpy(pr->name, p->name);
	strcpy(pr->sex, p->sex);
	strcpy(pr->classes, p->classes);
	strcpy(pr->id, p->id);
	strcpy(pr->phone, p->phone);
	pr->total = p->total;
	pr->project = p->project;

	strcpy(p->name, temp.name);
	strcpy(p->sex, temp.sex);
	strcpy(p->classes, temp.classes);
	strcpy(p->id, temp.id);
	strcpy(p->phone, temp.phone);
	p->total = temp.total;
	p->project = temp.project;
}

BOOL Fun_Rank(HWND hDlg)
{
	HWND hwnd = GetDlgItem(hDlg, IDC_TYPE);
	HWND hwndMan = GetDlgItem(hDlg, IDC_MAN);
	HWND hwndWoman = GetDlgItem(hDlg, IDC_WOMAN);
	int op = ComboBox_GetCurSel(hwnd);
	if (1 == op)
	{
		Button_SetCheck(hwndMan, BST_CHECKED);
		Button_SetCheck(hwndWoman, BST_CHECKED);
	}
	if (-1 == op)
	{
		MessageBox(hDlg, TEXT("请选择你需要查看的排行榜！！！"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if (Button_GetCheck(hwndMan) == BST_UNCHECKED && Button_GetCheck(hwndWoman) == BST_UNCHECKED)
	{
		MessageBox(hDlg, TEXT("请选择排行榜的范围！！！"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	pPer p = NULL, pr = NULL;
	int i = 0;
	TCHAR str[1024], name[30];
	pCLink q = NULL, qr = NULL;
	CLink temp;

	switch (op)
	{
		case 0:
		{
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
			wsprintf(str, "个人排行榜：\r\n");
			for (i = 0, p = head; p != NULL && i < 10; p = p->next)
			{
				if (0 == strcmp(p->id, "0"))
				{
					continue;
				}
				if (Button_GetCheck(hwndMan) == BST_CHECKED && Button_GetCheck(hwndWoman) == BST_CHECKED)
				{
					wsprintf(str, "%s%s\t%s\t%s\t%s\t%s\t%d\r\n", str, p->name, p->sex, p->classes, p->id, p->phone, p->total);
					i++;
				}
				if (Button_GetCheck(hwndMan) == BST_CHECKED && Button_GetCheck(hwndWoman) == BST_UNCHECKED)
				{
					if (0 == strcmp(p->sex, "男"))
					{
						wsprintf(str, "%s%s\t%s\t%s\t%s\t%s\t%d\r\n", str, p->name, p->sex, p->classes, p->id, p->phone, p->total);
						i++;
					}
				}
				if (Button_GetCheck(hwndMan) == BST_UNCHECKED && Button_GetCheck(hwndWoman) == BST_CHECKED)
				{
					if (0 == strcmp(p->sex, "女"))
					{
						wsprintf(str, "%s%s\t%s\t%s\t%s\t%s\t%d\r\n", str, p->name, p->sex, p->classes, p->id, p->phone, p->total);
						i++;
					}
				}
			}
			Edit_AddString(hwndEdit, str);
			Edit_AddString(hwndEdit, "\r\n\r\n");
		}
		break;
		case 1:
		{
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
			wsprintf(str, "班级排行榜：\r\n");
			for (i = 0, q = ClassLink; q != NULL && i < 10; q = q->next)
			{
				if (0 == strcmp(q->name, "0"))
				{
					continue;
				}
				wsprintf(str, "%s%s\t%d\r\n", str, q->name, q->score);
				i++;
			}
			Edit_AddString(hwndEdit, str);
			Edit_AddString(hwndEdit, "\r\n\r\n");
		}
		break;
		default:
		{
			int j = 0;
			GetDlgItemText(hDlg, IDC_TYPE, name, sizeof(name));
			for (i = 0; i < projectNum; i++)
			{
				if (0 == strcmp(project[i].name, name))
				{
					break;
				}
			}
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
			wsprintf(str, "%s排行榜：\r\n",project[i].name);
			for (j = 0, p = head; p != NULL && j < 10; p = p->next)
			{
				if (0 == strcmp(p->id, "0"))
				{
					continue;
				}
				if (Button_GetCheck(hwndMan) == BST_CHECKED && Button_GetCheck(hwndWoman) == BST_CHECKED)
				{
					wsprintf(str, "%s%s\t%s\t%s\t%s\t%s\t%s\r\n", str, p->name, p->sex, p->classes, p->id, p->phone, GetProjectScore(i + 1, p));
					j++;
				}
				if (Button_GetCheck(hwndMan) == BST_CHECKED && Button_GetCheck(hwndWoman) == BST_UNCHECKED)
				{
					if (0 == strcmp(p->sex, "男"))
					{
						wsprintf(str, "%s%s\t%s\t%s\t%s\t%s\t%s\r\n", str, p->name, p->sex, p->classes, p->id, p->phone, GetProjectScore(i + 1, p));
						j++;
					}
				}
				if (Button_GetCheck(hwndMan) == BST_UNCHECKED && Button_GetCheck(hwndWoman) == BST_CHECKED)
				{
					if (0 == strcmp(p->sex, "女"))
					{
						wsprintf(str, "%s%s\t%s\t%s\t%s\t%s\t%s\r\n", str, p->name, p->sex, p->classes, p->id, p->phone, GetProjectScore(i + 1, p));
						j++;
					}
				}
			}
			Edit_AddString(hwndEdit, str);
			Edit_AddString(hwndEdit, "\r\n\r\n");
		}
		break;
	}
	return TRUE;
}

BOOL is_jisuanji(TCHAR classes[])
{
	TCHAR str[20];
	strcpy(str, classes);
	TCHAR *p = str;
	p++;
	p++;
	*p = '\0';
	if (strcmp(str, "04") == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void OninitClassesLink()
{
	TCHAR path[256] = TEXT("..\\加载文件\\班级数据.txt");
	FILE *fp = fopen(path, "r");
	if (NULL == fp)
	{
		return;
	}
	pCLink p = NULL, pr = NULL;
	if (NULL == ClassLink)
	{
		p = (pCLink)malloc(sizeof(CLink));
		fscanf(fp, "%s", p->name);
		fscanf(fp, "%d\n", &p->score);
		ClassLink = p;
		p->next = NULL;
	}
	while (0 == feof(fp))
	{
		pr = p;
		p = (pCLink)malloc(sizeof(CLink));
		fscanf(fp, "%s", p->name);
		fscanf(fp, "%d\n", &p->score);
		pr->next = p;
	}
	p->next = NULL;
	fclose(fp);
}

void SaveClassesLink()
{
	TCHAR path[256] = TEXT("..\\加载文件\\班级数据.txt");
	FILE *fp = fopen(path, "w");
	if (NULL == fp)
	{
		return;
	}

	pCLink p = ClassLink;
	while (p != NULL)
	{
		fprintf(fp, "%s\t%d\n\n", p->name, p->score);
		p = p->next;
	}
	fclose(fp);
}

void DeleteClassesLinkMemory()
{
	pCLink p = ClassLink, pr = NULL;
	while (p != NULL)
	{
		pr = p;
		p = p->next;
		free(pr);
	}
}

void AddClassesScore(TCHAR classes[], int score)
{
	pCLink p = ClassLink;
	while (p != NULL)
	{
		if (0 == strcmp(classes, p->name))
		{
			p->score += score;
			return;
		}
		p = p->next;
	}

	pCLink pr = ClassLink;
	while (pr->next != NULL)
	{
		pr = pr->next;
	}
	p = (pCLink)malloc(sizeof(CLink));
	if (NULL == p)
	{
		MessageBox(hwndMain, TEXT("没有足够的内存空间！"), "Tips", MB_OK);
	}
	strcpy(p->name, classes);
	p->score = score;
	p->next = NULL;
	pr->next = p;
}

/***********************************
	生成子系统初始化所需文件
***********************************/
void GenerateSubsystemFile()
{
	TCHAR path[256] = TEXT("..\\配置文件\\活动项目.txt");
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

	strcpy(path, TEXT("..\\配置文件\\个人信息.txt"));
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
	strcpy(path, TEXT("..\\配置文件\\Edit内容.txt"));
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
	fprintf(fp, "班级排行榜公示=+第一名：%s班，成绩为%d=+", q->name, q->score);
	qr = q->next;
	if (NULL != qr)
	{
		fprintf(fp, "第二名：%s班，成绩为%d=+", qr->name, qr->score);
		y = qr->next;
		if (NULL != y)
		{
			fprintf(fp, "第三名：%s班，成绩为%d\n\n", y->name, y->score);
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
}

void ChangeFlagTime(TCHAR time[])
{
	if (0 != strcmp(FlagTime, time))
	{
		strcpy(FlagTime, time);
		for (pPer pr = head; pr != NULL; pr = pr->next)
		{
			pr->project.y1 = 1;
			pr->project.y2 = 1;
			pr->project.y3 = 1;
			pr->project.y4 = 1;
			pr->project.y5 = 1;
			pr->project.y6 = 1;
			pr->project.y7 = 1;
			pr->project.y8 = 1;
			pr->project.y9 = 1;
			pr->project.y10 = 1;
			pr->project.y11 = 1;
			pr->project.y12 = 1;
		}
	}
}

void CreatNewNode(Sig Msg, int x)
{
	int i;
	pPer pr = head, p = NULL;
	while (pr->next != NULL)
	{
		pr = pr->next;
	}
	p = (pPer)malloc(sizeof(Per));
	if (NULL == p)
	{
		MessageBox(hwndMain, TEXT("没有足够的内存！！！"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
		return;
	}
	strcpy(p->name, Msg.name);
	strcpy(p->sex, Msg.sex);
	strcpy(p->classes, Msg.classes);
	strcpy(p->id, Msg.id);
	strcpy(p->phone, Msg.phone);
	for (i = 0; i < projectNum; i++)
	{
		if (project[i].recordway == 0)
		{
			SetProjectScore(i + 1, p, project[i].min);
		}
		if (project[i].recordway == 1)
		{
			SetProjectScore(i + 1, p, project[i].max);
		}
	}
	for (; i < 12; i++)
	{
		SetProjectScore(i + 1, p, "s");
	}
	p->project.y1 = 1;
	p->project.y2 = 1;
	p->project.y3 = 1;
	p->project.y4 = 1;
	p->project.y5 = 1;
	p->project.y6 = 1;
	p->project.y7 = 1;
	p->project.y7 = 1;
	p->project.y8 = 1;
	p->project.y9 = 1;
	p->project.y10 = 1;
	p->project.y11 = 1;
	p->project.y12 = 1;
	SetProjectScore(x, p, Msg.score);
	p->total = project[x - 1].score;
	AddClassesScore(Msg.classes, project[x - 1].score);
	p->next = NULL;
	pr->next = p;
}