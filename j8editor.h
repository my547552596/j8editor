#include "Atwo.h"
#include "jet.h"

#define MAKE_STRING(N)				#N
#define MAKE_VERSION(A, B, C, D)	MAKE_STRING(A.B.C.D)
#define VERSION_INFO				MAKE_VERSION(VI_MAJOR, VI_MINOR, VI_REVISION, VI_BUILD)
#define VI_MAJOR					1
#define VI_MINOR					0
#define VI_REVISION					0
#define VI_BUILD					15
#define VI_PROGRAM_NAME_CN			"快码加编"
#define VI_PROGRAM_NAME_EN			"j8editor"

#define	IDI_MAINFRAME				128
#define	IDS_FILE_FILTER				1001
#define	IDS_FILE_NEW				1002
#define IDS_ABOUT_TITLE				"关于"
#define IDS_ABOUT_TEXT				1976

#define	FILE_READ					0
#define	FILE_WRITE					1
#define	TEXT_SEARCH					SW_HIDE
#define	TEXT_REPLACE				SW_SHOW
#define	WORD_LOWER					0
#define	WORD_UPPER					1

#define WM_TASKBAR_TRAY				(WM_USER + 100)
#define WM_TASKBAR_CREATED			RegisterWindowMessage(TEXT("TaskbarCreated"))

HICON hIcon;
HWND hDialog, hEditor, hFrame;
NOTIFYICONDATA nid;
static const TCHAR cFrameName[9] = VI_PROGRAM_NAME_EN;
static TCHAR cFilePath[MAX_PATH], cLoadString[MAX_PATH], cSearchString[50], cReplaceString[50];

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void toClickMenuItem(HWND hWnd, WPARAM wParam);
void toClickNotifyIconData(HWND hWnd, LPARAM lParam);
BOOL toDrawText(BOOL bCalc, HDC hDC, TCHAR* cBuffer);
void toHelp(TCHAR *cTitle, TCHAR *cText);
TCHAR *toLoadString(int iDS);
void toSelectMenuItem();

void toTest();

/* Layout */
void toCreateEditor(HWND hParent);
void toCreateFrame(HINSTANCE hInstance);
void toCreateNotifyIconData(HWND hWnd);
void toRecoverNotifyIconData(UINT message);
void toResizeEdit(WPARAM wParam);
void toSetFrameTitle();
void toSetNotifyIconData();

/* File */
BOOL toConfirmFileExist(char *cPath);
void toCreateFile();
void toDropFiles(HWND hWnd, HDROP hDropInfo);
void toGetFileReadonly();
void toOpenOrSaveAsFile(BOOL bOS);
void toPrintFile();
void toReadFile();
void toSaveFile();
void toSetFileReadonly();
void toWriteFile();

/* Edit & Index & Tool */
int toConvertAnsiFromUtf8(char *cUtf8, char *cAnsi);
void toCopyCutPasteSelectallUndo(UINT uMsg);
int toCountCharacter(char *cString);
void toGetTextAndLineCount();
void toShiftSel(BOOL bUL);

LRESULT CALLBACK indexProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void toCreateDialog(HWND hParent);
void toReplaceText();
BOOL toSearchOrCancel(HWND hWnd, WORD wId);
int toSearchText();
void toShowDialog(int iSOR);