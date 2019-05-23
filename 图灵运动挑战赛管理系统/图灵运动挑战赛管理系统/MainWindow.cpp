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
	CreateButton(hwndMain);
	hwndEdit = CreateEdit(hwndMain, "texta");
	// ��ȡ�����ڿͻ�����RECT�����η�����ĸ��߽�㣩
	GetClientRect(hwndMain, &rect);
	// �����Ӵ��ڵĴ�С��λ��
	SetWindows(&rect);
	// ��EDIT�ؼ�����ʾ�ı�
	ShowTextOnEdit(lpszLatin);
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
		OninitTimes();
		OninitClassesLink();
		OninitItemNature();
		OninitPersonLink();
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
		GenerateSubsystemFile();
		SaveTimes();
		SaveClassesLink();
		SavePersonLink();
		SaveItemNature();
		DeleteClassesLinkMemory();
		DeletePersonLinkMemory();
		PostQuitMessage(0);
		break;
		// �������룬�˵��ѡ�У���ť�����
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
		//�����ť�����ࡱ�����Ի���
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
	return TRUE;
}

/* ************************************
* void Fun_Import()
* ����	��ť���������ݡ���Ϣ��Ϣ������
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
	ofn.lpstrFilter = TEXT("������Ϣ\0������Ϣ*.TXT\0ȫ���ļ�\0*.*\0");
	if (GetOpenFileName(&ofn))
	{
		//�ѵ�һ���ļ���ǰ�ĸ��Ƶ�szPath,��:
		//���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'\'
		//���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�
		lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);
		//��ֻѡ��һ���ļ�ʱ,�������NULL�ַ��Ǳ����.
		//���ﲻ����Դ�ѡ��һ���Ͷ���ļ������
		szPath[ofn.nFileOffset] = '\0';
		nLen = lstrlen(szPath);

		if (szPath[nLen - 1] != '\\')   //���ѡ�˶���ļ�,��������'\\'
		{
			lstrcat(szPath, TEXT("\\"));
		}

		p = szOpenFileNames + ofn.nFileOffset; //��ָ���Ƶ���һ���ļ�

		FILE *fp = NULL;
		Sig msg;
		pPer pr = NULL;
		FILE *fpbk = fopen(TEXT("..\\�����ļ�\\�ѵ��뱨����Ϣ.txt"), "a");
		if (NULL == fpbk)
		{
			fpbk = fopen(TEXT("..\\�����ļ�\\�ѵ��뱨����Ϣ.txt"), "w");
			if (NULL == fpbk)
			{
				return;
			}
		}

		while (*p)
		{
			strcpy(path, "");
			lstrcat(path, szPath);	//���ļ�������·��  
			lstrcat(path, p);		//�����ļ���
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
			p += lstrlen(p) + 1;     //������һ���ļ�
		}
		fclose(fpbk);
		MessageBox(hwndMain, TEXT("���뱨����Ϣ�ɹ�������"), TEXT("Tips"), MB_OK);
	}
}


/* ************************************
* More
* ����	����"����"�Ի�����Ϣ������
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
			Edit_AddString(hwndEdit, "����������������������������������������������������������������������������\r\n");
			Edit_AddString(hwndEdit, "˵����\r\n\r\n\r\n");
			Edit_AddString(hwndEdit, "1��������Ϣ\r\n");
			Edit_AddString(hwndEdit, "(1)���������뾡�����յ��롣\r\n");
			Edit_AddString(hwndEdit, "(2)ÿһ����������ݶ��ᱸ�ݵ���..\\�����ļ�\\�ѵ��뱨����Ϣ.txt���С�\r\n\r\n");
			Edit_AddString(hwndEdit, "2�����а�\r\n");
			Edit_AddString(hwndEdit, "(1)�����༶�����ˡ���Ŀ���а�\r\n");
			Edit_AddString(hwndEdit, "(2)�鿴���а�ʱ���������ѡ�������а�Ů���а�ȫ�����а�(ͬʱѡ����Ů)��\r\n");
			Edit_AddString(hwndEdit, "(3)���а���༶���а���Բ�ѡ�������Χֱ�ӵó����а�����\r\n\r\n");
			Edit_AddString(hwndEdit, "3������ͳ��\r\n");
			Edit_AddString(hwndEdit, "��Ҫͳ��������ͼ���˶���ս���ܲ����˴Ρ����ղ����˴Ρ������ѧԺ�����˴Ρ�����Ŀ�����˴Ρ�\r\n\r\n");
			Edit_AddString(hwndEdit, "4����ѯ\r\n");
			Edit_AddString(hwndEdit, "ͨ��ѧ�Ų鿴�ò����߸�����Ϣ�͸���Ŀ�ɼ���\r\n\r\n");
			Edit_AddString(hwndEdit, "5�����ĸ�����Ϣ\r\n");
			Edit_AddString(hwndEdit, "ͨ��ѧ�ſ���ѡ���Եĸ��ĸò����߸�����Ϣ��\r\n\r\n");
			Edit_AddString(hwndEdit, "6���������Ŀ\r\n");
			Edit_AddString(hwndEdit, "(1)���Զ������ơ�����֣�����ʱ�����µĻ��Ŀ��\r\n");
			Edit_AddString(hwndEdit, "(2)�����ɼ�������ָ����һ����ֵ�ϵ���Сֵ�����ֵ���ɼ���¼��ʽΪ�����֡�����Ŀ��ʹ����Сֵ��ʼ�����˳ɼ���ʹ�����ֵ�ڱ���ϵͳ��¼����ʱ���в��Ϸ����ݼ�⣻�ɼ���¼��ʽΪ��ʱ�䡿����Ŀ��ʹ�����ֵ��ʼ�����˳ɼ���ʹ����Сֵ�ڱ���ϵͳ��¼����ʱ���в��Ϸ����ݼ�⡣���磺1000���������¼��2m20s���ң�ʹ��2m20s��Ϊ��Сֵ�����ڱ���ϵͳ��¼�ɼ�������Сֵϵͳ������ʾ����\r\n\r\n");
			Edit_AddString(hwndEdit, "7������ϵͳ\r\n");
			Edit_AddString(hwndEdit, "(1)����Ա����Ϊ��������������ȫƴ(Сд��ĸ)��\r\n");
			Edit_AddString(hwndEdit, "(2)�����ô˲�����һ�����ã��޷��˻���ǰ�����ݡ�\r\n\r\n");
			Edit_AddString(hwndEdit, "8���������а�\r\n");
			Edit_AddString(hwndEdit, "���뵼��̶���ʽ������Ϣ���ݵ�TXT�ļ�������ܲ�������ʹ�á�\r\n\r\n");
			Edit_AddString(hwndEdit, "�̶���ʽ���£�\r\n");
			Edit_AddString(hwndEdit, "����1������\r\n");
			Edit_AddString(hwndEdit, "���ų�Ա1��ѧ��\r\n");
			Edit_AddString(hwndEdit, "���ų�Ա2��ѧ��\r\n");
			Edit_AddString(hwndEdit, "����\r\n");
			Edit_AddString(hwndEdit, "���ų�ԱN��ѧ��\r\n");
			Edit_AddString(hwndEdit, "end\r\n\r\n");
			Edit_AddString(hwndEdit, "����2������\r\n");
			Edit_AddString(hwndEdit, "���ų�Ա1��ѧ��\r\n");
			Edit_AddString(hwndEdit, "���ų�Ա2��ѧ��\r\n");
			Edit_AddString(hwndEdit, "����\r\n");
			Edit_AddString(hwndEdit, "���ų�ԱN��ѧ��\r\n");
			Edit_AddString(hwndEdit, "end\r\n");
			Edit_AddString(hwndEdit, "\r\n");
			Edit_AddString(hwndEdit, "����\r\n");
			Edit_AddString(hwndEdit, "\r\n\r\n");
			Edit_AddString(hwndEdit, "8���й�����ϵͳ��ͼ���˶���ս������ϵͳ��\r\n");
			Edit_AddString(hwndEdit, "(1)����ϵͳ�����ʼϵͳ���ļ��д����ڡ�..\\�����ļ�������������ϵͳ��Ϣ���轫���ļ��и��Ƶ���Ӧλ��(����ϵͳ���е���һ��Ŀ¼)���ɡ�\r\n");
			Edit_AddString(hwndEdit, "(2)��������ϵͳ�Ϸ��Ĺ�����ʾ���������ݣ��������ڡ������ļ�\\Edit����.txt�����������ݼ��ɡ���Edit����.txt����������ʱ��һ����Ϊһ����Ϣ��һ��5������ʾ����ϵͳ�ı����ϣ�ʹ��=+���滻�з���ʹ��#����\\t��\r\n");
			Edit_AddString(hwndEdit, "(3)��ϵͳ�����ļ���ͨ����ť�����������ļ�������������Ŀ¼��\r\n\r\n");
			Edit_AddString(hwndEdit, "����������������������������������������������������������������������������\r\n");
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
				Edit_AddString(hwndEdit, "��������\t\t������\t�ܳɼ�\r\n");
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
		// ������˳����������Ի���
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
* ����	����"���а�"�Ի�����Ϣ������
**************************************/
LRESULT CALLBACK Rank(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND hwndComboBox = GetDlgItem(hDlg, IDC_TYPE);
		ComboBox_InsertString(hwndComboBox, -1, "�������а�");
		ComboBox_InsertString(hwndComboBox, -1, "�༶���а�");
		for (int i = 0; i < projectNum; i++)
		{
			ComboBox_InsertString(hwndComboBox, -1, project[i].name);
		}
	}
	return TRUE;

	case WM_COMMAND:
		// ���OK�������Ի���
		if (LOWORD(wParam) == IDOK)
		{
			if(FALSE == Fun_Rank(hDlg))
			{
				return TRUE;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// ������˳����������Ի���
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
* ����	����"��ѯ"�Ի�����Ϣ������
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
		// ������˳����������Ի���
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
* ����	����"���ĸ�����Ϣ"�Ի�����Ϣ������
**************************************/
LRESULT CALLBACK Change(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND hwnd = GetDlgItem(hDlg, IDC_CHANGETYPE);
		ComboBox_InsertString(hwnd, -1, "����");
		ComboBox_InsertString(hwnd, -1, "�Ա�");
		ComboBox_InsertString(hwnd, -1, "�༶");
		ComboBox_InsertString(hwnd, -1, "��ϵ��ʽ");
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
		// ������˳����������Ի���
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
* ����	����"�������Ŀ"�Ի�����Ϣ������
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
		// ������˳����������Ի���
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
* ����	����"����ϵͳ"�Ի�����Ϣ������
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
		//���������˸���Ŀ�ɼ�Ϊ��ʼֵ
		if (LOWORD(wParam) == IDC_BUTTON1)
		{
			int x= MessageBox(hDlg, TEXT("��ȷ����Ҫ���������˸���Ŀ�ɼ�Ϊ��ʼֵ������\n"), TEXT("Tips"), MB_OKCANCEL | MB_ICONWARNING);
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
			MessageBox(hDlg, TEXT("���������˸���Ŀ�ɼ�Ϊ��ʼֵ�ɹ���\n"), TEXT("Tips"), MB_OK);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		//���������˸��˻���Ϊ��
		if (LOWORD(wParam) == IDC_BUTTON2)
		{
			int x = MessageBox(hDlg, TEXT("��ȷ����Ҫ���������˸��˻���Ϊ�㣡����\n"), TEXT("Tips"), MB_OKCANCEL | MB_ICONWARNING);
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
			MessageBox(hDlg, TEXT("���������˸��˻���Ϊ��ɹ���\n"), TEXT("Tips"), MB_OK);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		//���ø���༶����Ϊ��
		if (LOWORD(wParam) == IDC_BUTTON3)
		{
			int x = MessageBox(hDlg, TEXT("��ȷ����Ҫ���ø���༶����Ϊ�㣡����\n"), TEXT("Tips"), MB_OKCANCEL | MB_ICONWARNING);
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
			MessageBox(hDlg, TEXT("���ø���༶����Ϊ��ɹ���\n"), TEXT("Tips"), MB_OK);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		//����ϵͳ�����ʼ״̬
		if (LOWORD(wParam) == IDC_BUTTON4)
		{
			int x = MessageBox(hDlg, TEXT("��ȷ����Ҫ����ϵͳ�����ʼ״̬������\n"), TEXT("Tips"), MB_OKCANCEL | MB_ICONWARNING);
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
				strcpy(head->sex, "��");
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
			MessageBox(hDlg, TEXT("����ϵͳ�����ʼ״̬�ɹ���\n"), TEXT("Tips"), MB_OK);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		// ������˳����������Ի���
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
* ����	����"��½"�Ի�����Ϣ������
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
				MessageBox(hDlg, TEXT("���������룡����\n��ʾ��������ȫƴ����\n"), TEXT("Tips"), MB_OK | MB_ICONWARNING);
			}
			if (0 == strcmp(password, TEXT("liaojincan")))
			{
				EndDialog(hDlg, LOWORD(wParam));
				DialogBox(hinst, (LPCTSTR)IDD_RESETSYSTEM, hwndMain, (DLGPROC)ResetSystem);
				return TRUE;
			}
			else
			{
				MessageBox(hDlg, TEXT("������󣡣���\n"), TEXT("Tips"), MB_OK | MB_ICONERROR);
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			return TRUE;
		}
		// ������˳����������Ի���
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
* ����	��Ӧ��ť�����������ļ�������ϵͳ
		�����ļ����浽�û�ָ����Ŀ¼��
**************************************/
void Fun_Export()
{
	TCHAR path[256], szpath[256];
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
		MessageBox(hwndMain, "��ȡ���˲�������������ϵͳ�����ļ���", "Tips:", MB_OK);
	}
	else
	{
		wsprintf(szpath, "%s\\�����ļ�", szChoose);
		if (!CreateDirectory(szpath, NULL))
		{
			MessageBox(hwndMain, "����ʧ�ܣ�", "Tips:", MB_OK);
		}
		else
		{
			wsprintf(path, "%s\\���Ŀ.txt", szpath);
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

			wsprintf(path, "%s\\������Ϣ.txt", szpath);
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
			wsprintf(path, "%s\\Edit����.txt", szpath);
			fp = fopen(path, "w");
			if (NULL == fp)
			{
				return;
			}
			fprintf(fp, "#�������ͼ���˶���ս������ϵͳ=+=+#���ߣ������ѧԺ-2017211960-�ν��\n\n");
			fprintf(fp, "��ӭ�μ�ͼ���˶���ս����ͼ���˶���ս���Ǽ������ѧ�뼼��ѧԺ����֧ѧ����Χ�ơ����ߡ�Ϊ����߻��Ļ��ΪӪ������ѧУ������������Χ��ּ��ͨ����ս���ķ�ʽ���ﵽ��ѧ���߳����ң��������磬����ٳ���Ŀ�ģ�\n\n");
			fprintf(fp, "��ȡͼ���˶���ս��������Ϣ�������QQȺ��529192272��ɨ���Ҳ��ά�뼴�ɼ��룩��ÿ�ն�ʱ�������а���Ϣ��\n\n");
			fprintf(fp, "������ǽ�ְ����У����Ա����ѧ�ſ��Բ���������ϵ������Ա��������Ա��Ϊ�����䡰�������롱��֮���ٴβ������ͨ������ġ��������롱���в��룡\n\n");
			fprintf(fp, "���и�����Ϣ��������ϵ������Ա���еǼǣ�=+=+#PS��ͼ���˶���ս���ɼ������Ŀ�������죡\n\n");
			fprintf(fp, "#�������ͼ���˶���ս������ϵͳ=+=+#���ߣ������ѧԺ-2017211960-�ν��\n\n");


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
			fprintf(fp, "�༶���а�ʾ=+��һ����%s��#�ɼ�Ϊ%d=+", q->name, q->score);
			qr = q->next;
			if (NULL != qr)
			{
				fprintf(fp, "�ڶ�����%s��#�ɼ�Ϊ%d=+", qr->name, qr->score);
				y = qr->next;
				if (NULL != y)
				{
					fprintf(fp, "��������%s��#�ɼ�Ϊ%d\n\n", y->name, y->score);
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
			fprintf(fp, "�������а�ʾ=+��һ����%s��ѧ��Ϊ%s��%s���ɼ�Ϊ%d=+", p->classes, p->id, p->name, p->total);
			pr = p->next;
			if (pr != NULL)
			{
				fprintf(fp, "�ڶ�����%s��ѧ��Ϊ%s��%s���ɼ�Ϊ%d=+", pr->classes, pr->id, pr->name, pr->total);
				x = pr->next;
				if (NULL != x)
				{
					fprintf(fp, "��������%s��ѧ��Ϊ%s��%s���ɼ�Ϊ%d\n\n", x->classes, x->id, x->name, x->total);
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
				fprintf(fp, "%s���а�ʾ=+��һ����%s��ѧ��Ϊ%s��%s���ɼ�Ϊ%s=+", project[i].name, p->classes, p->id, p->name, GetProjectScore(i + 1, p));
				pr = p->next;
				if (pr == NULL)
				{
					fprintf(fp, "\n\n");
					continue;
				}
				fprintf(fp, "�ڶ�����%s��ѧ��Ϊ%s��%s���ɼ�Ϊ%s=+", pr->classes, pr->id, pr->name, GetProjectScore(i + 1, pr));
				x = pr->next;
				if (x == NULL)
				{
					fprintf(fp, "\n\n");
					continue;
				}
				fprintf(fp, "��������%s��ѧ��Ϊ%s��%s���ɼ�Ϊ%s\n\n", x->classes, x->id, x->name, GetProjectScore(i + 1, x));
			}
			fclose(fp);
			MessageBox(hwndMain, "����ɹ���", "Tips:", MB_OK);
		}
	}
	// �ͷ�
	ILFree(pidlRoot);
}

/* ************************************
* DWORD SetWindows(LPRECT lpRect)
* ����	�������д�����һ���ؼ����������Ӵ���
*			�����������Ӵ��ڵ�λ�úʹ�С
* ����	lpRect��ָ���ʾ�����ڿͻ�����RECT
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

