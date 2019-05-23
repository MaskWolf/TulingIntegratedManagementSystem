#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include<stdlib.h>
#include<stdio.h>
#include <shlobj.h>
#include<string.h>
//�ṹ������
typedef struct SIGNUPMSG
{
	TCHAR name[30];
	TCHAR sex[30];
	TCHAR classes[60];
	TCHAR id[60];
	TCHAR phone[60];
	TCHAR project[60];
}SIGNUPMSG;

typedef struct Person
{
	TCHAR name[30];
	TCHAR sex[30];
	TCHAR classes[60];
	TCHAR id[60];
	TCHAR phone[60];
	Person *next;
}Per,*pPer;

typedef struct Text
{
	TCHAR text[1024];
	Text *next;
}Tex,*pTex;

typedef struct ItemNature
{
	TCHAR name[30];
	int recordway;		//��0����ʾ��¼��ʽΪ�����֡�����1����ʾ��¼��ʽΪ��ʱ�䡱
	int score;			//����Ŀ�ķ���������
	TCHAR min[20];			//�����������ֵ��ͳɼ�
	TCHAR max[20];			//�����������ֵ��߳ɼ�
	int times;
}Ite, *pIte;
// ��������
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow);
BOOL InitApplication(HINSTANCE hinstance);
BOOL InitInstance(HINSTANCE hinstance, int nCmdShow);
DWORD SetWindows(LPRECT lpRect);

LRESULT CALLBACK FirstJoin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AgainJoin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Record(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);

BOOL OnWindowResize();


HWND CreateListView(HWND hwndParent, LPSTR szWindowName);
BOOL InitListViewColumns(HWND hWndListView);
BOOL GetImageList(HIMAGELIST * pHiml);
BOOL InitListViewImageLists(HWND hWndListView);
BOOL AddListViewItem(HWND hwndListView, SIGNUPMSG content);
BOOL ResetListViewColumns(HWND hWndListView, LPRECT lpRect);

HWND CreateEdit(HWND hwndP, LPSTR szTitle);
DWORD SetEditFont();
void UpdateEditText();

HFONT ChooseNewFont();

BOOL CreateButton(HWND hwndParent);

BOOL FirstSingUp(HWND hDlg);
int AgainSingUp(HWND hDlg);
BOOL RecordScore(HWND hDlg);
BOOL AutoCompleteContent(HWND hDlg, SIGNUPMSG *px);
BOOL BlankTest(HWND hDlg, SIGNUPMSG content);
void WriteToTemp(SIGNUPMSG content);

BOOL OninitLink();
void DeleteLinkMemory();
void AddLinkNode(SIGNUPMSG content);

BOOL OninitCycleLink();
void DeleteCycleLinkMemory();

void OninitItemNature();
BOOL FormatControl(TCHAR str[], TCHAR score[]);
BOOL IsTimeFormat(TCHAR str[]);
int ShiftScoreToInt(TCHAR score[]);
void ChangToNewline(TCHAR str[]);

void IsSystemDirectory();

#define IDB_FIRST