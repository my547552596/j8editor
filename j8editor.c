#include "j8editor.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR sCmdLine, int iCmdShow) {
	hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINFRAME));
    toCreateFrame(hInstance);

    if(toConfirmFileExist(sCmdLine)) {
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
	UINT WM_TASKBARCREATED = RegisterWindowMessage("TaskbarCreated");
    switch(message)	{
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_COMMAND:
        toClickMenuItem(hWnd, LOWORD(wParam));
        break;
    case WM_CREATE:
        toCreateDialog(hWnd);
        toCreateEditor(hWnd);
		toCreateNotifyIconData(hWnd);
        break;
    case WM_DESTROY:
		Shell_NotifyIcon(NIM_DELETE, &nid);
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
		if(wParam == SIZE_MINIMIZED) {
			ShowWindow(hFrame, SW_HIDE);
		} else {
			toResizeEdit();
		}
        break;
	case WM_USER:
		if(lParam == WM_LBUTTONDOWN) {
			ShowWindow(hFrame, SW_SHOWNORMAL);
			SetWindowPos(hFrame, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE);
		}
        break;
	default:
		if(message == WM_TASKBARCREATED) {
			toCreateNotifyIconData(hWnd);
		}
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

void toClickMenuItem(HWND hWnd, WORD wId) {
    switch(wId) {
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
        toShowDialog(TEXT_SEARCH);
        break;
    case 221:
    case 321:
        toSearchText();
        break;
    case 222:
    case 322:
        toShowDialog(TEXT_REPLACE);
        break;
    case 230:
        toGetTextAndLineCount();
        break;
    case 240:
        Atwo(hWnd, 25, 2);
        ShellExecute(hFrame, "open", "https://github.com/my547552596/j8editor", NULL, NULL, SW_SHOWNORMAL);
        break;
    case 249:
    case 349:
        toHelp(IDS_ABOUT_TITLE, toLoadString(IDS_ABOUT_TEXT));
        break;
    case 299:
    case 399:
        PostMessage(hWnd, WM_CLOSE, 0, 0);
        break;
    }
}

void toCopyCutPasteSelectallUndo(UINT uMsg) {
    SendMessage(hEditor, uMsg, 0, -1);
}

BOOL toConfirmFileExist(char *cPath) {
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

    HFONT hFont = CreateFont(18,
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
	nid.uCallbackMessage = WM_USER;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uID = 0;
	strcpy(nid.szTip, VI_PROGRAM_NAME_CN);
	Shell_NotifyIcon(NIM_ADD, &nid);
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

    if(toConfirmFileExist(cLoadString)) {
        toReadFile();
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
    char cAllCount[50] = "������";
    strcat(cAllCount, toGetStrFromInt(GetWindowTextLength(hEditor)));
    strcat(cAllCount, "\n������");
    strcat(cAllCount, toGetStrFromInt(SendMessage(hEditor, EM_GETLINECOUNT, 0, 0)));
    toHelp("ͳ��", cAllCount);
}

void toHelp(TCHAR *cTitle, TCHAR *cText) {
    MessageBox(NULL, cText, cTitle, MB_OK);
}

TCHAR *toLoadString(int iDS) {
    LoadString(GetModuleHandle(NULL), iDS, cLoadString, sizeof(cLoadString));
    return cLoadString;
}

void toOpenOrSaveAsFile(BOOL iOS) {
    OPENFILENAME ofn = {0};
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.hwndOwner = hFrame;
    ofn.lpstrDefExt = "txt";
    ofn.lpstrInitialDir = ".\\";
    ofn.lpstrFilter = toLoadString(IDS_FILE_FILTER);
    ofn.lpstrFile = cFilePath;
    ofn.lStructSize = sizeof(ofn);
    ofn.nMaxFile = sizeof(cFilePath);

    if(iOS) {
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

        DWORD dTextLength = GetWindowTextLength(hEditor) + 1;
        TCHAR cFileBuffer[dTextLength], cEditText[dTextLength];
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
    if(dReadSize > 0) {
        ReadFile(hFile, cFileBuffer, sizeof(cFileBuffer), &dReadSize, NULL);
        cFileBuffer[dReadSize] = 0;
        SetWindowText(hEditor, cFileBuffer);
    }

    toGetFileReadonly();
    toSetFrameTitle();
    CloseHandle(hFile);
}

void toResizeEdit() {
    RECT rect;
    GetClientRect(hFrame, &rect);
    MoveWindow(hEditor, rect.left, rect.top, rect.right, rect.bottom, TRUE);
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
        EnableMenuItem(hMenu, 221, MF_BYCOMMAND | MF_ENABLED);
    } else {
        EnableMenuItem(hMenu, 221, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    }
}

void toSetFrameTitle() {
    TCHAR cFrameTitle[MAX_PATH];
    strcpy(cFrameTitle, cFilePath);
    strcat(cFrameTitle, " - ");
    strcat(cFrameTitle, VI_PROGRAM_NAME_CN);
    SetWindowText(hFrame, cFrameTitle);
}

void toSetFileReadonly() {
    if(toConfirmFileExist(cFilePath)) {
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

    WriteFile(hFile, cFileBuffer, sizeof(cFileBuffer) - 1, &dTextLength, NULL);
    FlushFileBuffers(hFile);
    CloseHandle(hFile);
}

LRESULT CALLBACK indexProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch(message)	{
    case WM_COMMAND:
        return toSearchOrCancel(hWnd, LOWORD(wParam));
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

void toReplaceText() {
    SendMessage(hEditor, EM_SETSEL, 0, 0);

    while(toSearchText() >= 0) {
        SendMessage(hEditor, EM_REPLACESEL, TRUE, (LPARAM) &cReplaceString);
    }
}

BOOL toSearchOrCancel(HWND hWnd, WORD wId) {
    switch(wId) {
    case IDCANCEL:
        EndDialog(hWnd, wId);
        ShowWindow(hWnd, SW_HIDE);
        return TRUE;
    case 402:
        GetDlgItemText(hWnd, 401, cSearchString, sizeof(cSearchString));
        toSearchText();
        SendMessage(hDialog, WM_COMMAND, IDCANCEL, 0);
        break;
    case 404:
        GetDlgItemText(hWnd, 401, cSearchString, sizeof(cSearchString));
        GetDlgItemText(hWnd, 403, cReplaceString, sizeof(cReplaceString));
        toReplaceText();
        SendMessage(hDialog, WM_COMMAND, IDCANCEL, 0);
        break;
    default:
        return FALSE;
    }
}

int toSearchText() {
    int iBegin, iEnd = GetWindowTextLength(hEditor) + 1;

    if(strlen(cSearchString)) {
        TCHAR cEditText[iEnd];
        GetWindowText(hEditor, cEditText, iEnd / sizeof(TCHAR));

        SendMessage(hEditor, EM_GETSEL, 0, (LPARAM) &iEnd);
        iBegin = toGetIndex(cEditText + iEnd, (char *) &cSearchString);

        if(iBegin < 0) {
            SendMessage(hEditor, EM_SETSEL, 0, 0);
        } else {
            SendMessage(hEditor, EM_SETSEL, iBegin + iEnd, iBegin + iEnd + strlen(cSearchString));
        }
    }

    return iBegin;
}

void toShowDialog(int iSOR) {
    RECT rect;
    int iDialogHeight, iDialogWidth, iParentHeight, iParentWidth;

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
    iDialogHeight += iSOR ? 75 : 40;

    MoveWindow(hDialog,
               pScreen.x + (iParentWidth - iDialogWidth) / 2,
               pScreen.y + (iParentHeight - iDialogHeight) / 2,
               iDialogWidth,
               iDialogHeight,
               TRUE
              );

    ShowWindow(GetDlgItem(hDialog, 403), iSOR);
    ShowWindow(GetDlgItem(hDialog, 404), iSOR);
    ShowWindow(hDialog, SW_SHOW);
}