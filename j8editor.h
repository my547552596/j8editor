#include "Atwo.h"
#include "jet.h"

#define	FILE_READ					0
#define	FILE_WRITE					1
#define	TEXT_SEARCH					SW_HIDE
#define	TEXT_REPLACE				SW_SHOW

#define	IDI_MAINFRAME				128
#define	IDS_FILE_FILTER				1001
#define	IDS_FILE_NEW				1002
#define IDS_ABOUT_TITLE				"����"
#define IDS_ABOUT_TEXT				1976

#define MAKE_STRING(N)				#N
#define MAKE_VERSION(A, B, C, D)	MAKE_STRING(A.B.C.D)
#define VERSION_INFO				MAKE_VERSION(VI_MAJOR, VI_MINOR, VI_REVISION, VI_BUILD)
#define VI_MAJOR					1
#define VI_MINOR					0
#define VI_REVISION					0
#define VI_BUILD					13
#define VI_PROGRAM_NAME_CN			"����ӱ�"
#define VI_PROGRAM_NAME_EN			"j8editor"

HICON hIcon;
HWND hDialog, hEditor, hFrame;
NOTIFYICONDATA nid;
static const TCHAR cFrameName[9] = VI_PROGRAM_NAME_EN;
static TCHAR cFilePath[MAX_PATH], cLoadString[MAX_PATH], cSearchString[50], cReplaceString[50];

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void toClickMenuItem(HWND hWnd, WORD wId);
BOOL toDrawText(BOOL bCalc, HDC hDC, TCHAR* cBuffer);
void toHelp(TCHAR *cTitle, TCHAR *cText);
TCHAR *toLoadString(int iDS);
void toSelectMenuItem();

/* Layout */
void toCreateEditor(HWND hParent);
void toCreateFrame(HINSTANCE hInstance);
void toCreateNotifyIconData(HWND hWnd);
void toResizeEdit();
void toSetFrameTitle();

/* File */
BOOL toConfirmFileExist();
void toCreateFile();
void toDropFiles(HWND hWnd, HDROP hDropInfo);
void toGetFileReadonly();
void toOpenOrSaveAsFile(BOOL iOS);
void toPrintFile();
void toReadFile();
void toSaveFile();
void toSetFileReadonly();
void toWriteFile();

/* Edit */
void toClear();
void toCopyCutPasteSelectallUndo(UINT uMsg);

/* Index & Tool */
void toGetTextAndLineCount();

LRESULT CALLBACK indexProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void toCreateDialog(HWND hParent);
void toReplaceText();
BOOL toSearchOrCancel(HWND hWnd, WORD wId);
int toSearchText();
void toShowDialog(int iSOR);