/* ͷ�ļ� */
#include <windows.h>
#include <commctrl.h>
#include <Winuser.h>
#include <shellapi.h>
#include "resource.h"
#include "Window.h"
#pragma comment (lib, "comctl32.lib")
/* �ⲿ���� */
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
* ����	������ڵ㣬����InitApplication��InitInstance
*			��Ϣѭ��
* ����	hinstance��Ӧ�ó��򱾴�����ʵ��
*			hPrevInstance��Ӧ�ó���֮ǰ��ʵ����ʼ��ΪNULL
*			lpCmdLine�������в���
*			nCmdShow��������ڣ�SW_SHOW
* ����ֵ	ʧ�ܷ���FALSE
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
	//��ʼ��������Ϣ������Edit���ݵ�ѭ������
	if (FALSE == OninitLink() || FALSE == OninitCycleLink())
	{
		MessageBox(hwndMain, TEXT("�����ļ���������Ϣ.txt����Edit����.txt���������飡\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
	}
	// ��Ϣѭ��
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
* ����	ע����������
* ����	hinstance��Ӧ�ó��򱾴�����ʵ��
* ����ֵ	�Ƿ�ɹ�
**************************************/
BOOL InitApplication(HINSTANCE hinstance)
{
	// ʹ��RegisterClassEx
	WNDCLASSEX wcx;
	// ���ṹ
	wcx.cbSize = sizeof(wcx); // WNDCLASSEX�ṹ�Ĵ�С
	wcx.style = CS_HREDRAW | CS_VREDRAW; // �����С�ı����ػ洰��
	wcx.lpfnWndProc = MainWndProc; // ������Ϣ������
	wcx.cbClsExtra = 0; // �޸��Ӵ������ڴ�
	wcx.cbWndExtra = 0; // �޸��Ӵ����ڴ�
	wcx.hInstance = hinstance; // Ӧ�ó���ʵ��
	wcx.hIcon = (HICON)LoadImage(hinstance,
		MAKEINTRESOURCE(IDI_ICON_MAIN),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);	// ͼ��
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW); // ���ָ��
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ������ˢ
	wcx.lpszMenuName = NULL; // �˵���Դ
	wcx.lpszClassName = "MainWClass"; // ��������
	wcx.hIconSm = (HICON)LoadImage(hinstance, // Сͼ��
		MAKEINTRESOURCE(IDI_ICON_MAIN),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);
	// ע�ᴰ���࣬����
	return RegisterClassEx(&wcx);
}

/* ************************************
* BOOL InitInstance(HINSTANCE hinstance, int nCmdShow)
* ����	���������ںͿؼ�
* ����	hinstance��Ӧ�ó��򱾴�����ʵ��
*			nCmdShow�������ʾ
* ����ֵ	�Ƿ�ɹ�
**************************************/
BOOL InitInstance(HINSTANCE hinstance, int nCmdShow)
{
	HWND hwnd;
	RECT rect;
	// ����Ӧ�ó���ʵ�����
	hinst = hinstance;
	hwnd = CreateWindow(
		"MainWClass", // ����������ʹ��֮ǰע�����������
		"ͼ���˶���ս������ϵͳ", // ����������ʾ�ڴ��ڱ������ϵ��ַ���
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
		WS_POPUPWINDOW, 	// ������ʽ
		CW_USEDEFAULT, // ˮƽλ�ã�Ĭ�ϣ�
		CW_USEDEFAULT, // ��ֱλ�ã�Ĭ�ϣ�
		800, // ��
		600, // ��
		(HWND)NULL, // �޸�����
		NULL, // �˵�
		hinstance, //Ӧ�ó���ʵ��
		(LPVOID)NULL); // �޴��ڴ�������
					   // �����Ƿ񴴽��ɹ�
	if (!hwnd)
		return FALSE;
	// �������ھ��
	hwndMain = hwnd;
	// ��֤ͨ�ÿؼ���̬���ӿ��Ѿ�����
	InitCommonControls();
	// �������ֿؼ����Ӵ���
	hwndListView = CreateListView(hwndMain, "processing");
	hwndEdit = CreateEdit(hwndMain, "texta");
	CreateButton(hwndMain);
	// ��ȡ�����ڿͻ�����RECT�����η�����ĸ��߽�㣩
	GetClientRect(hwndMain, &rect);
	// �����Ӵ��ڵĴ�С��λ��
	SetWindows(&rect);
	// ����EDIT�ؼ�������
	SetEditFont();
	// ��ʾ�����ڣ�ʹ��WinMain�������õĴ�����ʾ��ʽ
	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);
	return TRUE;
}
/* ************************************
* MainWndProc
* ����	��������Ϣ������
**************************************/
LRESULT CALLBACK MainWndProc(
	HWND hwnd, // ���ھ��
	UINT uMsg, // ��Ϣ��ʶ��
	WPARAM wParam, // ��Ϣ�ĵ�һ������
	LPARAM lParam)// ��Ϣ�ĵڶ�������
{

	switch (uMsg)
	{
	case WM_CREATE:	// ���ڴ���ʱ�յ�����Ϣ
		IsSystemDirectory();
		OninitItemNature();
		SetTimer(hwndMain, ID_TIMER, 5000, TimerProc);
		break;

	case WM_PAINT:		// ���ڱ�����ʱ�յ�����Ϣ
		break;

	case WM_SIZE:		// ���ڴ�С�ı�ʱ
		OnWindowResize();
		break;
		// NOTIFY��ͨ���ɿؼ����͸������ڣ�˵���ؼ����ڽ���ĳ���
	case WM_NOTIFY:
		break;
		// �������٣�������ϽǵĹرհ�ť�ᴥ������Ϣ
	case WM_DESTROY:
		DeleteLinkMemory();
		DeleteCycleLinkMemory();
		KillTimer(hwndMain, ID_TIMER);
		PostQuitMessage(0);
		break;
		// �������룬�˵��ѡ�У���ť�����
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			//���ͼƬ���汨����Ϣ
		case IDB_PICTURE:
		{
			TCHAR path[256];
			// ���ڱ���·��
			TCHAR szRoot[MAX_PATH];
			TCHAR szChoose[MAX_PATH];
			TCHAR szDisplayName[MAX_PATH];
			// ��ر���
			LPITEMIDLIST pidlRoot = NULL;
			LPITEMIDLIST pidlSelected = NULL;
			BROWSEINFO bi = { 0 };
			LPMALLOC pMalloc = NULL;

			// ������ļ��С��ĸ�·����������Ա�ɸ������ѡ�񣬱���ֻ������ҵ��ĵ�����
			SHGetFolderLocation(NULL, CSIDL_DESKTOP, NULL, 0, &pidlRoot);
			SHGetPathFromIDList(pidlRoot, szRoot);
			// ��� BROWSEINFO �ṹ
			bi.hwndOwner = hwndMain;
			bi.pidlRoot = pidlRoot;
			bi.pszDisplayName = szDisplayName;
			bi.lpszTitle = "��ѡ����ϵͳ�����ļ������λ��";
			bi.ulFlags = 0;
			bi.lpfn = NULL;
			bi.lParam = 0;
			// �����Ի���
			pidlSelected = SHBrowseForFolder(&bi);
			// ѡ����ļ���
			SHGetPathFromIDList(pidlSelected, szChoose);
			if (0 == strcmp("", szChoose))
			{
				MessageBox(hwndMain, "��ȡ���˲����������汨����Ϣ��", "Tips:", MB_OK);
			}
			else
			{
				SYSTEMTIME st;
				GetLocalTime(&st);
				TCHAR Today[30], str[8][30];
				int i;
				wsprintf(Today, "%d-%d-%d", st.wYear, st.wMonth, st.wDay);
				wsprintf(path, TEXT("..\\������Ϣ\\%s.txt"),Today);
				FILE *fp1 = fopen(path, "r");
				if (NULL == fp1)
				{
					MessageBox(hwndMain, "����ʧ�ܣ�������δ���˱���������Ϣ�ļ��ѱ��ƶ���", "Tips:", MB_OK);
					return 0;
				}
				wsprintf(path, "%s\\������Ϣ.txt", szChoose);
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
				MessageBox(hwndMain, "����ɹ���", "Tips:", MB_OK);

			}
			// �ͷ�
			ILFree(pidlRoot);
			return 0;
		}
			// �����ť���״β��롱
		case IDB_FIRSTJOIN:
		{
			DialogBox(
				hinst, (LPCTSTR)IDD_FIRSTJOIN, hwnd, (DLGPROC)FirstJoin
			);
			return 0;
		}
		// �����ť���ٴβ��롱
		case IDB_AGAINJOIN:
		{
			DialogBox(
				hinst, (LPCTSTR)IDD_AGAINJOIN, hwnd, (DLGPROC)AgainJoin
			);
			return 0;
		}
		// �����ť����¼�ɼ���
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
	// �����ڴ˴���������Ϣ
	default:
		break;
	}
	// �кܶ���Ϣδ��������Ҫ��Ĭ�ϴ�����Ϣ������������
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
/* ************************************
* BOOL OnWindowResize()
* ����	�����ڴ�С�ı���Ϣ
*			�ı��Ӵ��ڴ�С��λ��
**************************************/
BOOL OnWindowResize()
{
	RECT rcClient;
	GetClientRect(hwndMain, &rcClient);
	SetWindows(&rcClient);
	ResetListViewColumns(hwndListView, &rcClient);

	//���ÿؼ�����Ĵ�С������
	HFONT hFont = CreateFont
	(
		30, 0,    //�߶�20, ��ȡ0��ʾ��ϵͳѡ�����ֵ
		0, 0,    //�ı���б����������б��Ϊ0
		FW_HEAVY,    //����
		0, 0, 0,        //��б�壬���»��ߣ����л���
		GB2312_CHARSET,    //�ַ���
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,        //һϵ�е�Ĭ��ֵ
		DEFAULT_PITCH | FF_DONTCARE,
		(TCHAR*)"����"    //��������
	);
	SendMessage(hwndListView, WM_SETFONT, (WPARAM)hFont, 1);
	//�������ֵı���ɫ
	SendMessage(hwndListView, LVM_SETTEXTBKCOLOR, 0, RGB(219, 219, 219));
	return TRUE;
}



/* ************************************
* FirstJoin
* ����	���״β��롱�Ի�����Ϣ������
**************************************/
LRESULT CALLBACK FirstJoin(
	HWND hDlg,		// �Ի��򴰿ھ��
	UINT message,		// ��Ϣ
	WPARAM wParam,	// ��Ϣ����
	LPARAM lParam)		// ��Ϣ����
{
	switch (message)
	{
	case WM_INITDIALOG:	
		{
			//��ʼ��Dialog�Ի����еĿؼ�ComboBox�е�����
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
		// ���OK�������Ի���
		if (LOWORD(wParam) == IDOK)
		{
			if (FALSE == FirstSingUp(hDlg))
			{
				return TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// ������˳�������������
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
* ����	���ٴβ��롱�Ի�����Ϣ������
**************************************/
LRESULT CALLBACK AgainJoin(
	HWND hDlg,		// �Ի��򴰿ھ��
	UINT message,		// ��Ϣ
	WPARAM wParam,	// ��Ϣ����
	LPARAM lParam)		// ��Ϣ����
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		//��ʼ��Dialog�Ի����еĿؼ�ComboBox�е�����
		HWND hwnd = GetDlgItem(hDlg, IDC_PROJECT2);
		for (int i = 0; i < projectNum; i++)
		{
			ComboBox_InsertString(hwnd, -1, project[i].name);
		}
	}
		return TRUE;

	case WM_COMMAND:
		// ���OK�������Ի���
		if (LOWORD(wParam) == IDOK)
		{
			if (-1 == AgainSingUp(hDlg))
			{
				return TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// ������˳�������������
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
* ����	����¼�ɼ����Ի�����Ϣ������
**************************************/
LRESULT CALLBACK Record(
	HWND hDlg,		// �Ի��򴰿ھ��
	UINT message,		// ��Ϣ
	WPARAM wParam,	// ��Ϣ����
	LPARAM lParam)		// ��Ϣ����
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		// ���OK�������Ի���
		if (LOWORD(wParam) == IDOK)
		{
			if (FALSE == RecordScore(hDlg))
			{
				return TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// ������˳�������������
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
* ����	�������д����������ؼ����������Ӵ���
*			�����������Ӵ��ڵ�λ�úʹ�С
* ����	lpRect��ָ���ʾ�����ڿͻ�����RECT
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
* ����	�������� List View �� ����
* ����	hWndListView��List View �ؼ�����
* ����ֵ	�Ƿ�ɹ�
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
* ����	��ʱ����Ϣ������
**************************************/
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	UpdateEditText();
}

/* ************************************
* void IsSystemDirectory()
* ����	�ж��Ƿ���ϵͳĿ¼��
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
		MessageBox(hwndMain, TEXT("���Թ���Ա������д˳���"), "Tips", MB_OK);
		exit(0);
	}
}