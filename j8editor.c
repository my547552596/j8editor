#include "j8editor.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR sCmdLine, int iCmdShow) {
	hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINFRAME));
	toCreateFrame(hInstance);

	if(toBeFileExist(sCmdLine)) {
		toReadFile();
	} else {
		toCreateFile();
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(300));

	while(GetMessage(&msg, NULL, 0, 0) > 0) {
		if(!IsDialogMessage(hDialog, &msg)) {
			if(!TranslateAccelerator(hFrame, hAccel, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	UnregisterClass(cFrameName, hInstance);
	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message)	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_COMMAND:
		toClickMenuItem(hWnd, wParam);
		break;
	case WM_CREATE:
		toCreateDialog(hWnd);
		toCreateEditor(hWnd);
		toCreateNotifyIconData(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_DROPFILES:
		toDropFiles(hWnd, (HDROP)wParam);
		break;
	case WM_MENUSELECT:
		toSelectMenuItem();
		break;
	case WM_SETFOCUS:
		SetFocus(hEditor);
		break;
	case WM_SIZE:
		toResizeEdit(wParam);
		break;
	case WM_TASKBAR_TRAY:
		toClickNotifyIconData(hWnd, lParam);
		break;
	default:
		toRecoverNotifyIconData(message);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

BOOL toBeFileExist(char *cPath) {
	DWORD dAttr = GetFileAttributes(cPath);

	if(dAttr == INVALID_FILE_ATTRIBUTES) {
		return FALSE;
	}

	if(dAttr & FILE_ATTRIBUTE_DIRECTORY) {
		return FALSE;
	}

	strcpy(cFilePath, cPath);
	return TRUE;
}

void toClickMenuItem(HWND hWnd, WPARAM wParam) {
	switch(LOWORD(wParam)) {
	case 201:
	case 301:
		toCreateFile();
		break;
	case 202:
	case 302:
		toOpenOrSaveAsFile(FILE_READ);
		break;
	case 203:
	case 303:
		toSaveFile();
		break;
	case 204:
	case 304:
		toOpenOrSaveAsFile(FILE_WRITE);
		break;
	case 205:
		toSetFileReadonly();
		break;
	case 206:
	case 306:
		toPrintFile();
		break;
	case 210:
	case 310:
		toCopyCutPasteSelectallUndo(WM_UNDO);
		break;
	case 211:
	case 311:
		toCopyCutPasteSelectallUndo(WM_CUT);
		break;
	case 212:
	case 312:
		toCopyCutPasteSelectallUndo(WM_COPY);
		break;
	case 213:
	case 313:
		toCopyCutPasteSelectallUndo(WM_PASTE);
		break;
	case 214:
	case 314:
		toCopyCutPasteSelectallUndo(EM_SETSEL);
		break;
	case 220:
	case 320:
		toShowDialog(EDIT_GOTO);
		break;
	case 221:
	case 321:
		toShowDialog(EDIT_SEARCH);
		break;
	case 222:
	case 322:
		toSearchText();
		break;
	case 223:
	case 323:
		toShowDialog(EDIT_REPLACE);
		break;
	case 230:
		toGetTextAndLineCount();
		break;
	case 231:
	case 331:
		toTrimString(TRIM_LEFT);
		break;
	case 232:
	case 332:
		toTrimString(TRIM_ALL);
		break;
	case 233:
	case 333:
		toTrimString(TRIM_RIGHT);
		break;
	case 234:
	case 334:
		toShiftSel(WORD_UPPER);
		break;
	case 235:
	case 335:
		toShiftSel(WORD_LOWER);
		break;
	case 240:
		Atwo(hWnd, 25, 2);
		ShellExecute(hFrame, "open", "https://github.com/my547552596/j8editor", NULL, NULL, SW_SHOWNORMAL);
		break;
	case 249:
	case 349:
		toHelp(toLoadString(IDS_ABOUT), toLoadString(IDS_ABOUT) + 5);
		break;
	case 299:
	case 399:
		PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	}
}

void toClickNotifyIconData(HWND hWnd, LPARAM lParam) {
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = GetSubMenu(GetMenu(hFrame), 0);
	switch(lParam) {
	case WM_LBUTTONDOWN:
		toSetNotifyIconData();
		break;
	case WM_RBUTTONDOWN:
		SetForegroundWindow(hWnd);
		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
		toSetNotifyIconData();
		break;
	}
}

void toConvertAnsiFromUnicodeBe(wchar_t *wcUnicode, char *cAnsi) {
	for(int i = 0; i < wcslen(wcUnicode) + 1; i++) {
		wcUnicode[i] = (unsigned char)cAnsi[(i + 1) * 2] * 256 + (unsigned char)cAnsi[(i + 1) * 2 + 1];
	}

	toConvertAnsiFromUnicodeLe(wcUnicode);
}

void toConvertAnsiFromUnicodeLe(wchar_t *wcUnicode) {
	int iLength = WideCharToMultiByte(CP_ACP, 0, wcUnicode, -1, NULL, 0, NULL, NULL);
	TCHAR cTextBuffer[iLength];
	WideCharToMultiByte(CP_ACP, 0, wcUnicode, -1, cTextBuffer, iLength, NULL, NULL);

	if(wcslen(wcUnicode) % 2) {
		cTextBuffer[iLength - 1] = '\0';
	}

	SetWindowText(hEditor, cTextBuffer);
}

void toConvertAnsiFromUtf8(char *cUtf8) {
	int iLength = MultiByteToWideChar(CP_UTF8, 0, cUtf8, -1, NULL, 0);
	wchar_t wcUnicode[iLength];
	MultiByteToWideChar(CP_UTF8, 0, cUtf8, -1, wcUnicode, iLength);
	toConvertAnsiFromUnicodeLe(wcUnicode);
}

int toConvertUtf8FromAnsi(char *cAnsi, char *cUtf8) {
	int iLength = MultiByteToWideChar(CP_ACP, 0, cAnsi, -1, NULL, 0);
	wchar_t wcBuffer[iLength];
	MultiByteToWideChar(CP_ACP, 0, cAnsi, -1, wcBuffer, iLength);
	iLength = WideCharToMultiByte(CP_UTF8, 0, wcBuffer, -1, NULL, 0, NULL, NULL);

	if(cUtf8) {
		WideCharToMultiByte(CP_UTF8, 0, wcBuffer, -1, cUtf8, iLength, NULL, NULL);
	}

	return iLength;
}

void toCopyCutPasteSelectallUndo(UINT uMsg) {
	SendMessage(hEditor, uMsg, 0, -1);
}

int toCountCharacter(char *cString) {
	int iLength = MultiByteToWideChar(CP_ACP, 0, cString, -1, NULL, 0);
	wchar_t wcUnicode[iLength];
	MultiByteToWideChar(CP_ACP, 0, cString, -1, wcUnicode, iLength);

	return wcslen(wcUnicode);
}

void toCreateEditor(HWND hParent) {
	RECT rect;
	GetClientRect(hParent, &rect);

	hEditor = CreateWindow("EDIT",
	                       "",
	                       WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_WANTRETURN,
	                       rect.left,
	                       rect.top,
	                       rect.right,
	                       rect.bottom,
	                       hParent,
	                       NULL,
	                       (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE),
	                       NULL
	                      );

	HFONT hFont = CreateFont(16,
	                         0,
	                         0,
	                         0,
	                         FALSE,
	                         FALSE,
	                         FALSE,
	                         0,
	                         DEFAULT_CHARSET,
	                         OUT_DEFAULT_PRECIS,
	                         CLIP_DEFAULT_PRECIS,
	                         DEFAULT_QUALITY,
	                         DEFAULT_PITCH | FF_MODERN,
	                         "Courier New"
	                        );

	SendMessage(hEditor, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void toCreateFile() {
	SetWindowText(hEditor, "");
	SendMessage(hEditor, EM_SETREADONLY, FALSE, 0);
	LoadString(GetModuleHandle(NULL), IDS_FILE_NEW, cFilePath, sizeof(cFilePath));
	CheckMenuItem(GetMenu(hFrame), 205, MF_BYCOMMAND | MF_UNCHECKED);
	toSetFrameTitle();
}

void toCreateFrame(HINSTANCE hInstance) {
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = hIcon;
	wc.hInstance = hInstance;
	wc.lpszClassName = cFrameName;
	wc.lpszMenuName = MAKEINTRESOURCE(200);
	RegisterClass(&wc);

	int screenHeight, screenWidth;
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);

	hFrame = CreateWindowEx(WS_EX_ACCEPTFILES,
	                        cFrameName,
	                        NULL,
	                        WS_OVERLAPPEDWINDOW,
	                        screenWidth / 3 / 2,
	                        screenHeight / 3 / 2,
	                        screenWidth / 3 * 2,
	                        screenHeight / 3 * 2,
	                        NULL,
	                        NULL,
	                        hInstance,
	                        NULL
	                       );

	ShowWindow(hFrame, SW_SHOW);
	UpdateWindow(hFrame);
}

void toCreateNotifyIconData(HWND hWnd) {
	nid.cbSize = sizeof(nid);
	nid.hIcon = hIcon;
	nid.hWnd = hWnd;
	nid.uCallbackMessage = WM_TASKBAR_TRAY;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uID = 20180930;
}

BOOL toDrawText(BOOL bCalc, HDC hDC, TCHAR* cFileBuffer) {
	UINT uPageHeight = GetDeviceCaps(hDC, VERTSIZE) / 25.4 * GetDeviceCaps(hDC, LOGPIXELSY) - 500, uFormat[2] = {
		DT_CALCRECT | DT_EXPANDTABS | DT_LEFT | DT_NOCLIP | DT_NOPREFIX | DT_WORDBREAK,
		DT_EXPANDTABS | DT_LEFT | DT_NOCLIP | DT_NOPREFIX | DT_WORDBREAK
	};

	RECT rect = {
		500,
		500,
		GetDeviceCaps(hDC, HORZSIZE) / 25.4 * GetDeviceCaps(hDC, LOGPIXELSX) - 500,
		uPageHeight
	};

	DrawText(hDC, cFileBuffer, -1, &rect, uFormat[bCalc]);
	return (rect.bottom > uPageHeight);
}

void toDropFiles(HWND hWnd, HDROP hDropInfo) {
	DragQueryFile(hDropInfo, 0, cLoadString, sizeof(cLoadString));
	DragFinish(hDropInfo);

	if(toBeFileExist(cLoadString)) {
		toReadFile();
	}
}

DWORD toGetCharacterCode(char *cAuu) {
	if(cAuu[0] == (char) 0xEF && cAuu[1] == (char) 0xBB && cAuu[2] == (char) 0xBF) {
		return CHAR_UTF8BOM;
	} else if(cAuu[0] == (char) 0xFF && cAuu[1] == (char) 0xFE) {
		return CHAR_USC2LE;
	} else if(cAuu[0] == (char) 0xFE && cAuu[1] == (char) 0xFF) {
		return CHAR_USC2BE;
	} else if(toBeUtf8WithoutBom(cAuu)) {
		return CHAR_UTF8;
	} else {
		return CHAR_ANSI;
	}
}

void toGetFileReadonly() {
	DWORD dAttr = GetFileAttributes(cFilePath);
	HMENU hMenu = GetMenu(hFrame);

	if(dAttr & FILE_ATTRIBUTE_READONLY) {
		SendMessage(hEditor, EM_SETREADONLY, TRUE, 0);
		CheckMenuItem(hMenu, 205, MF_BYCOMMAND | MF_CHECKED);
	} else {
		SendMessage(hEditor, EM_SETREADONLY, FALSE, 0);
		CheckMenuItem(hMenu, 205, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

void toGetTextAndLineCount() {
	TCHAR cEditorText[GetWindowTextLength(hEditor) + 1], cAllCount[99] = "字数：";
	GetWindowText(hEditor, cEditorText, sizeof(cEditorText) / sizeof(TCHAR));
	strcat(cAllCount, toGetStringFromInteger(toCountCharacter(cEditorText)));
	strcat(cAllCount, "\n行数：");
	strcat(cAllCount, toGetStringFromInteger(SendMessage(hEditor, EM_GETLINECOUNT, 0, 0)));
	toHelp("统计", cAllCount);
}

void toHelp(TCHAR *cTitle, TCHAR *cText) {
	if(strlen(cTitle)) {
		MessageBox(NULL, cText, cTitle, MB_OK);
	} else {
		TCHAR cDefaultTitle[5];
		LoadString(GetModuleHandle(NULL), IDS_DEFAULT_TITLE, cDefaultTitle, 5);
		MessageBox(NULL, cText, cDefaultTitle, MB_OK);
	}
}

TCHAR *toLoadString(int iDS) {
	LoadString(GetModuleHandle(NULL), iDS, cLoadString, sizeof(cLoadString));
	return cLoadString;
}

void toOpenOrSaveAsFile(BOOL bOS) {
	OPENFILENAME ofn = {0};
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.hwndOwner = hFrame;
	ofn.lpstrDefExt = "txt";
	ofn.lpstrInitialDir = ".\\";
	ofn.lpstrFilter = toLoadString(IDS_FILE_FILTER);
	ofn.lpstrFile = cFilePath;
	ofn.lStructSize = sizeof(ofn);
	ofn.nMaxFile = sizeof(cFilePath);

	if(bOS) {
		if(GetSaveFileName(&ofn)) {
			toWriteFile();
		}
	} else {
		if(GetOpenFileName(&ofn)) {
			toReadFile();
		}
	}
}

void toPrintFile() {
	PRINTDLG pd = {0};
	pd.Flags = PD_ALLPAGES | PD_RETURNDC;
	pd.hDC = NULL;
	pd.hDevMode = NULL;
	pd.hDevNames = NULL;
	pd.hwndOwner = hEditor;
	pd.lStructSize = sizeof(PRINTDLG);

	if(PrintDlg(&pd)) {
		DOCINFO di = {0};
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = cFilePath;
		di.lpszOutput = NULL;

		StartDoc(pd.hDC, &di);
		StartPage(pd.hDC);

		DWORD dTextLength = GetWindowTextLengthW(hEditor) + 1;
		TCHAR cEditText[dTextLength], cFileBuffer[dTextLength];
		GetWindowText(hEditor, cEditText, dTextLength / sizeof(TCHAR));

		for(int iCount = 1, iBegin = 0, iEnd; iCount < dTextLength; iCount++) {
			iEnd = iCount + iBegin;
			memset(cFileBuffer, 0, sizeof(TCHAR) * dTextLength);
			strncpy(cFileBuffer, cEditText + iBegin, iEnd - iBegin);

			if(toDrawText(FALSE, pd.hDC, cFileBuffer)) {
				toDrawText(TRUE, pd.hDC, cFileBuffer);
				EndPage(pd.hDC);
				StartPage(pd.hDC);

				iBegin = iEnd;
			}
		}

		toDrawText(TRUE, pd.hDC, cFileBuffer);
		EndPage(pd.hDC);
		EndDoc(pd.hDC);
		DeleteDC(pd.hDC);
	}
}

void toReadFile() {
	HANDLE hFile = CreateFile(cFilePath,
	                          GENERIC_READ,
	                          FILE_SHARE_READ,
	                          NULL,
	                          OPEN_EXISTING,
	                          FILE_ATTRIBUTE_NORMAL,
	                          NULL
	                         );

	DWORD dReadSize = GetFileSize(hFile, NULL);
	TCHAR cFileBuffer[dReadSize];
	wchar_t wcBuffer[dReadSize / 2];

	if(dReadSize > 0) {
		ReadFile(hFile, cFileBuffer, sizeof(cFileBuffer), &dReadSize, NULL);
		wcBuffer[dReadSize / 2] = cFileBuffer[dReadSize] = '\0';
		switch(toGetCharacterCode(cFileBuffer)) {
		case CHAR_ANSI:
			SetWindowText(hEditor, cFileBuffer);
			break;
		case CHAR_UTF8:
			toConvertAnsiFromUtf8(cFileBuffer);
			break;
		case CHAR_UTF8BOM:
			toConvertAnsiFromUtf8(cFileBuffer + 3);
			break;
		case CHAR_USC2BE:
			toConvertAnsiFromUnicodeBe(wcBuffer, cFileBuffer);
			break;
		case CHAR_USC2LE:
			toConvertAnsiFromUnicodeLe((wchar_t *) cFileBuffer + 1);
			break;
		}
	}

	toGetFileReadonly();
	toSetFrameTitle();
	CloseHandle(hFile);
}

void toRecoverNotifyIconData(UINT message) {
	if(message == WM_TASKBAR_CREATED) {
		toSetNotifyIconData();
	}
}

void toResizeEdit(WPARAM wParam) {
	if(wParam == SIZE_MINIMIZED) {
		toSetNotifyIconData();
	} else {
		RECT rect;
		GetClientRect(hFrame, &rect);
		MoveWindow(hEditor, rect.left, rect.top, rect.right, rect.bottom, TRUE);
	}
}

void toSaveFile() {
	if(strcmp(cFilePath, toLoadString(IDS_FILE_NEW))) {
		toWriteFile();
	} else {
		toOpenOrSaveAsFile(FILE_WRITE);
	}
}

void toSelectMenuItem() {
	HMENU hMenu = GetMenu(hFrame);

	if(SendMessage(hEditor, EM_CANUNDO, 0, 0)) {
		EnableMenuItem(hMenu, 210, MF_BYCOMMAND | MF_ENABLED);
	} else {
		EnableMenuItem(hMenu, 210, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}

	DWORD dBegin = 0, dEnd = 0;
	SendMessage(hEditor, EM_GETSEL, (WPARAM) &dBegin, (LPARAM) &dEnd);
	if(dEnd > dBegin) {
		EnableMenuItem(hMenu, 211, MF_BYCOMMAND | MF_ENABLED);
		EnableMenuItem(hMenu, 212, MF_BYCOMMAND | MF_ENABLED);
	} else {
		EnableMenuItem(hMenu, 211, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		EnableMenuItem(hMenu, 212, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}

	if(strlen(cSearchString)) {
		EnableMenuItem(hMenu, 222, MF_BYCOMMAND | MF_ENABLED);
	} else {
		EnableMenuItem(hMenu, 222, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}
}

void toSetFrameTitle() {
	TCHAR cFrameTitle[MAX_PATH];
	strcpy(cFrameTitle, cFilePath);
	strcat(cFrameTitle, " - ");
	strcat(cFrameTitle, toLoadString(IDS_PROGRAM_NAME));
	SetWindowText(hFrame, cFrameTitle);
}

void toSetFileReadonly() {
	if(toBeFileExist(cFilePath)) {
		DWORD dResult = GetFileAttributes(cFilePath);
		HMENU hMenu = GetMenu(hFrame);

		if(dResult & FILE_ATTRIBUTE_READONLY) {
			SetFileAttributes(cFilePath, FILE_ATTRIBUTE_NORMAL);
			SendMessage(hEditor, EM_SETREADONLY, FALSE, 0);
			CheckMenuItem(hMenu, 205, MF_BYCOMMAND | MF_UNCHECKED);
		} else {
			SetFileAttributes(cFilePath, FILE_ATTRIBUTE_READONLY);
			SendMessage(hEditor, EM_SETREADONLY, TRUE, 0);
			CheckMenuItem(hMenu, 205, MF_BYCOMMAND | MF_CHECKED);
		}
	}
}

void toSetNotifyIconData() {
	if(IsWindowVisible(hFrame)) {
		ShowWindow(hFrame, SW_HIDE);
		strcpy(nid.szTip, toGetFileName(cFilePath));
		Shell_NotifyIcon(NIM_ADD, &nid);
	} else {
		ShowWindow(hFrame, SW_SHOWNORMAL);
		Shell_NotifyIcon(NIM_DELETE, &nid);
	}
}

void toShiftSel(BOOL bUL) {
	DWORD dBegin, dEnd, dTextLength = GetWindowTextLength(hEditor) + 1;
	TCHAR cEditBuffer[dTextLength];
	GetWindowText(hEditor, cEditBuffer, sizeof(cEditBuffer) / sizeof(TCHAR));
	SendMessage(hEditor, EM_GETSEL, (WPARAM) &dBegin, (LPARAM) &dEnd);
	TCHAR cStringBuffer[dEnd - dBegin + 1];
	strncpy(cStringBuffer, cEditBuffer + dBegin, dEnd - dBegin);
	TCHAR *cTextBuffer = bUL ? strupr(cStringBuffer) : strlwr(cStringBuffer);
	SendMessage(hEditor, EM_REPLACESEL, TRUE, (LPARAM) cTextBuffer);
}

void toTrimString(int iFlag) {
	DWORD dBegin, dEnd = GetWindowTextLength(hEditor) + 1;
	TCHAR cEditBuffer[dEnd];
	GetWindowText(hEditor, cEditBuffer, sizeof(cEditBuffer) / sizeof(TCHAR));
	SendMessage(hEditor, EM_GETSEL, (WPARAM) &dBegin, (LPARAM) &dEnd);
	TCHAR cTextBuffer[dEnd - dBegin + 1];
	strncpy(cTextBuffer, cEditBuffer + dBegin, dEnd - dBegin);

	switch(iFlag) {
	case TRIM_ALL:
		SendMessage(hEditor, EM_REPLACESEL, TRUE, (LPARAM) toTrimAll(cTextBuffer));
		break;
	case TRIM_LEFT:
		SendMessage(hEditor, EM_REPLACESEL, TRUE, (LPARAM) toTrimLeft(cTextBuffer));
		break;
	case TRIM_RIGHT:
		SendMessage(hEditor, EM_REPLACESEL, TRUE, (LPARAM) toTrimRight(cTextBuffer));
		break;
	}
}

void toWriteFile() {
	HANDLE hFile = CreateFile(cFilePath,
	                          GENERIC_WRITE,
	                          FILE_SHARE_WRITE,
	                          NULL,
	                          CREATE_ALWAYS,
	                          FILE_ATTRIBUTE_NORMAL,
	                          NULL
	                         );

	DWORD dTextLength = GetWindowTextLength(hEditor) + 1;
	TCHAR cFileBuffer[dTextLength];
	GetWindowText(hEditor, cFileBuffer, sizeof(cFileBuffer) / sizeof(TCHAR));

	TCHAR cTextBuffer[toConvertUtf8FromAnsi(cFileBuffer, NULL)];
	toConvertUtf8FromAnsi(cFileBuffer, cTextBuffer);

	WriteFile(hFile, cTextBuffer, sizeof(cTextBuffer) - 1, &dTextLength, NULL);
	FlushFileBuffers(hFile);
	CloseHandle(hFile);
}

LRESULT CALLBACK indexProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message)	{
	case WM_COMMAND:
		return toDoIndex(hWnd, LOWORD(wParam));
	}

	return FALSE;
}

void toCreateDialog(HWND hParent) {
	hDialog = CreateDialog((HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE),
	                       MAKEINTRESOURCE(400),
	                       hParent,
	                       (DLGPROC)indexProc
	                      );
}

BOOL toDoIndex(HWND hWnd, WORD wId) {
	switch(wId) {
	case 402:
		GetDlgItemText(hWnd, 401, cSearchString, sizeof(cSearchString));
		GetDlgItemText(hWnd, 402, cBuffer16, sizeof(cBuffer16));
		if(strcmp(cBuffer16, toLoadString(IDS_INDEX))) {
			toSearchText();
		} else {
			toGetLine(atoi(cSearchString) - 1);
		}
		SendMessage(hDialog, WM_COMMAND, IDCANCEL, 0);
		break;
	case 404:
		GetDlgItemText(hWnd, 401, cSearchString, sizeof(cSearchString));
		GetDlgItemText(hWnd, 403, cReplaceString, sizeof(cReplaceString));
		toReplaceText();
		SendMessage(hDialog, WM_COMMAND, IDCANCEL, 0);
		break;
	case IDCANCEL:
		EndDialog(hWnd, wId);
		return TRUE;
	case IDOK:
		SendMessage(hDialog, WM_COMMAND, 402, 0);
		return TRUE;
	default:
		return FALSE;
	}
}

void toGetLine(int iNumber) {
	int iStart = SendMessage(hEditor, EM_LINEINDEX, iNumber, 0),
	    iLength = SendMessage(hEditor, EM_LINEINDEX, iNumber + 1, 0);
	SendMessage(hEditor, EM_SETSEL, iStart, iLength - 1);
	SendMessage(hEditor, EM_SCROLLCARET, 0, 0);
}

void toReplaceText() {
	if(strlen(cSearchString)) {
		SendMessage(hEditor, EM_SETSEL, 0, 0);

		while(toSearchText() >= 0) {
			SendMessage(hEditor, EM_REPLACESEL, TRUE, (LPARAM) &cReplaceString);
		}
	}
}

int toSearchText() {
	int iBegin, iEnd = GetWindowTextLength(hEditor) + 1;

	if(strlen(cSearchString)) {
		TCHAR cEditText[iEnd];
		GetWindowText(hEditor, cEditText, iEnd / sizeof(TCHAR));

		SendMessage(hEditor, EM_GETSEL, 0, (LPARAM) &iEnd);
		iBegin = toGetIndexOfString(cEditText + iEnd, (char *) &cSearchString);

		if(iBegin < 0) {
			SendMessage(hEditor, EM_SETSEL, 0, 0);
		} else {
			SendMessage(hEditor, EM_SETSEL, iBegin + iEnd, iBegin + iEnd + strlen(cSearchString));
		}
	}

	return iBegin;
}

void toShowDialog(int iGRS) {
	int iDialogHeight, iDialogWidth, iParentHeight, iParentWidth;
	long l401OldStyle = GetWindowLong(GetDlgItem(hDialog, 401), GWL_STYLE);
	RECT rect;

	GetClientRect(hFrame, &rect);
	iParentHeight = rect.bottom - rect.top;
	iParentWidth = rect.right - rect.left;

	POINT pScreen = {rect.left, rect.top};
	ClientToScreen(hFrame, (LPPOINT)&pScreen);

	GetWindowRect(hDialog, &rect);
	iDialogHeight = rect.bottom - rect.top;
	iDialogWidth = rect.right - rect.left;

	GetClientRect(hDialog, &rect);
	iDialogHeight -= rect.bottom - rect.top;
	iDialogHeight += iGRS ? 40 : 75;

	MoveWindow(hDialog,
	           pScreen.x + (iParentWidth - iDialogWidth) / 2,
	           pScreen.y + (iParentHeight - iDialogHeight) / 2,
	           iDialogWidth,
	           iDialogHeight,
	           TRUE
	          );


	if(iGRS < 0) {
		SetWindowLong(GetDlgItem(hDialog, 401), GWL_STYLE, l401OldStyle | ES_NUMBER);
		SetWindowText(GetDlgItem(hDialog, 401), "");
		SetWindowText(GetDlgItem(hDialog, 402), toLoadString(IDS_INDEX));
	} else {
		SetWindowLong(GetDlgItem(hDialog, 401), GWL_STYLE, l401OldStyle & ~ES_NUMBER);
		SetWindowText(GetDlgItem(hDialog, 402), toLoadString(IDS_INDEX) + 9);
	}
	
	EnableWindow(GetDlgItem(hDialog, 403), ~iGRS);
	EnableWindow(GetDlgItem(hDialog, 404), ~iGRS);
	ShowWindow(hDialog, SW_SHOW);
}