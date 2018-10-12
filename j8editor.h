#include "Atwo.h"
#include "jet.h"

#define MAKE_STRING(N)				#N
#define MAKE_VERSION(A, B, C, D)	MAKE_STRING(A.B.C.D)
#define VERSION_INFO				MAKE_VERSION(VI_MAJOR, VI_MINOR, VI_REVISION, VI_BUILD)
#define VI_MAJOR					1
#define VI_MINOR					0
#define VI_REVISION					0
#define VI_BUILD					22

#define	IDI_MAINFRAME				128
#define IDS_ABOUT					999
#define IDS_DEFAULT_TITLE			1000
#define	IDS_FILE_FILTER				1001
#define	IDS_FILE_NEW				1002
#define	IDS_INDEX					1101
#define	IDS_PROGRAM_NAME			1999

#define	CHAR_ANSI					2001
#define	CHAR_USC2BE					2116
#define	CHAR_USC2LE					2016
#define	CHAR_UTF8					2008
#define	CHAR_UTF8BOM				2018

#define	FILE_READ					0
#define	FILE_WRITE					1
#define	EDIT_GOTO					-1
#define	EDIT_REPLACE				0
#define	EDIT_SEARCH					1
#define	TRIM_ALL					0
#define	TRIM_LEFT					1
#define	TRIM_RIGHT					2
#define	WORD_LOWER					0
#define	WORD_UPPER					1

#define WM_TASKBAR_TRAY				(WM_USER + 100)
#define WM_TASKBAR_CREATED			RegisterWindowMessage(TEXT("TaskbarCreated"))

HICON hIcon;
HWND hDialog, hEditor, hFrame;
NOTIFYICONDATA nid;
static const TCHAR cFrameName[9] = "j8editor\0";
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
BOOL toBeFileExist(char *cPath);
void toConvertAnsiFromUnicodeBe(wchar_t *wcUnicode, char *cAnsi);
void toConvertAnsiFromUnicodeLe(wchar_t *wcUnicode);
void toConvertAnsiFromUtf8(char *cUtf8);
int toConvertUtf8FromAnsi(char *cAnsi, char *cUtf8);
void toCreateFile();
void toDropFiles(HWND hWnd, HDROP hDropInfo);
DWORD toGetCharacterCode(char *cAuu);
void toGetFileReadonly();
void toOpenOrSaveAsFile(BOOL bOS);
void toPrintFile();
void toReadFile();
void toSaveFile();
void toSetFileReadonly();
void toWriteFile();

/* Edit & Index & Tool */
void toCopyCutPasteSelectallUndo(UINT uMsg);
int toCountCharacter(char *cString);
void toGetTextAndLineCount();
void toShiftSel(BOOL bUL);
void toTrimString(int iFlag);

LRESULT CALLBACK indexProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void toCreateDialog(HWND hParent);
BOOL toDoIndex(HWND hWnd, WORD wId);
void toGetLine(int iNumber);
void toReplaceText();
int toSearchText();
void toShowDialog(int iGRS);