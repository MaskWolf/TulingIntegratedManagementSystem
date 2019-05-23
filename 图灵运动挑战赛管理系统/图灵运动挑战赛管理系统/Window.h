#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <shlobj.h>
#include<stdio.h>
#include<stdlib.h>
//结构体声明
typedef struct Project
{
	TCHAR x1[15];
	TCHAR x2[15];
	TCHAR x3[15];
	TCHAR x4[15];
	TCHAR x5[15];
	TCHAR x6[15];
	TCHAR x7[15];
	TCHAR x8[15];
	TCHAR x9[15];
	TCHAR x10[15];
	TCHAR x11[15];
	TCHAR x12[15];

	int y1;
	int y2;
	int y3;
	int y4;
	int y5;
	int y6;
	int y7;
	int y8;
	int y9;
	int y10;
	int y11;
	int y12;
}*pPro,Pro;

typedef struct Person
{
	TCHAR name[30];
	TCHAR sex[30];
	TCHAR classes[30];
	TCHAR id[30];
	TCHAR phone[30];
	int total;
	Project project;
	Person *next;
}Per,*pPer;

typedef struct ItemNature
{
	TCHAR name[30];
	int recordway;		//“0”表示记录方式为“数字”，“1”表示记录方式为“时间”
	int score;			//该项目的分数、积分
	TCHAR min[20];			//正常情况下数值最低成绩
	TCHAR max[20];			//正常情况下数值最高成绩
	int times;
}Ite,*pIte;

typedef struct SignUpMsg
{
	TCHAR time[30];
	TCHAR name[30];
	TCHAR sex[30];
	TCHAR classes[30];
	TCHAR id[30];
	TCHAR phone[30];
	TCHAR project[30];
	TCHAR score[20];
}Sig, *pSig;

typedef struct ClassLink
{
	TCHAR name[30];
	int score;
	ClassLink *next;
}*pCLink, CLink;
// 函数声明
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow);
BOOL InitApplication(HINSTANCE hinstance);
BOOL InitInstance(HINSTANCE hinstance, int nCmdShow);
DWORD SetWindows(LPRECT lpRect);

BOOL Fun_Change(HWND hDlg);
BOOL Fun_AddProject(HWND hDlg);
BOOL Fun_Inquire(HWND hDlg);
void Fun_Import();
void Fun_Statistic();
void Fun_Export();
BOOL Fun_Rank(HWND hDlg);
LRESULT CALLBACK More(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Rank(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Inquire(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Change(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AddProject(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ResetSystem(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Login(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

BOOL OnWindowResize();

HWND CreateEdit(HWND hwndP, LPSTR szTitle);
DWORD ShowTextOnEdit(LPSTR szTextToShow);

BOOL CreateButton(HWND hwndParent);

BOOL OninitPersonLink();
void SavePersonLink();
void DeletePersonLinkMemory();

void OninitItemNature();
void SaveItemNature();

void OninitClassesLink();
void SaveClassesLink();
void DeleteClassesLinkMemory();

BOOL IsTimeFormat(TCHAR str[]);
int ShiftScoreToInt(TCHAR score[]);
TCHAR *ShiftScoreToChar(int score);
void SetAllProjectScore(int num, TCHAR str[]);

void Edit_AddString(HWND hwndCtl, TCHAR str[]);
TCHAR *GetProjectScore(int num, pPer p);
void SetProjectScore(int num, pPer p, TCHAR str[]);

void MsgProcess(Sig Msg);
BOOL IsTodayFirst(int num, pPer p);

void OninitTimes();
void SaveTimes();
BOOL is_jisuanji(TCHAR classes[]);
void SwapNodeData(pPer p, pPer pr);
void AddClassesScore(TCHAR classes[], int score);

void GenerateSubsystemFile();

void ChangeFlagTime(TCHAR time[]);

void IsSystemDirectory();

void CreatNewNode(Sig Msg, int x);