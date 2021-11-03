#include <Windows.h>
#include <stdio.h>

// Funktionen
LRESULT CALLBACK MainProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SubProc(HWND, UINT, WPARAM, LPARAM);
void CALLBACK Timer_Shutdown(HWND, UINT, UINT, DWORD);

// gloable Variablen
HINSTANCE hInstGlobal;
HWND hButtonRadioButton, hButtonRadioButton2,
hButtonRadioButton4, hButtonCheckBox1,
hButtonCheckBox2, hEditTime_Minuten,
hEditTime_Sekunden, hWindow, hSubWindow;
HFONT hFont;
int imin, isec;
float scale;	// for high DPI screens

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
	hInstGlobal = hInstance;
	WNDCLASS WndClass;
	WndClass.style = 0;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.lpfnWndProc = MainProc;
	WndClass.hInstance = hInstance;
	WndClass.hbrBackground = (HBRUSH)(COLOR_MENU + 12);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.lpszMenuName = 0;
	WndClass.lpszClassName = L"MainProc";

	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = SubProc;
	WndClass.lpszClassName = L"SubProc";

	RegisterClass(&WndClass);

	// DPI von primärem Monitor
	HDC hScreenDC = GetDC(0);
	int dpi = GetDeviceCaps(hScreenDC, LOGPIXELSX);
	ReleaseDC(0, hScreenDC);
	scale = dpi / 96.0;

	// screen width and height
	int x, y;
	x = GetSystemMetrics(SM_CXSCREEN);
	y = GetSystemMetrics(SM_CYSCREEN);

	// Hauptfenster
	hWindow = CreateWindow(L"MainProc", L"Herunterfahren", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		((x / 2) - 115), (y / 2) - 142, int(scale*230), int(scale*285), NULL,
		NULL, hInstance, NULL);
	ShowWindow(hWindow, iCmdShow);
	UpdateWindow(hWindow);

	// Nebenfenster
	hSubWindow = CreateWindow(L"SubProc", L"Information", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		((x / 2) - 115), (y / 2) - 75, int(scale*230), int(scale*150), NULL,
		NULL, hInstance, NULL);

	MSG Message;
	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return(Message.wParam);
}

// Prozedur MainProc
LRESULT CALLBACK MainProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam) {
	switch (uiMessage) {
	case WM_CREATE:
		HWND hButtonOK, hButtonExit, hButtonGroupBox, hButtonGroupBox2;
		// Button - OK
		hButtonOK = CreateWindow(L"BUTTON", L"OK",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			int(scale*15), int(scale*215), int(scale*85), int(scale*23), hWnd, (HMENU)1,
			hInstGlobal, NULL);
		SendMessage(hButtonOK, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
		// Button - Abbrechen
		hButtonExit = CreateWindow(L"BUTTON", L"Abbrechen",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			int(scale*120), int(scale*215), int(scale*85), int(scale*23), hWnd, (HMENU)2,
			hInstGlobal, NULL);
		SendMessage(hButtonExit, WM_SETFONT, (WPARAM)GetStockObject(int(DEFAULT_GUI_FONT)), 0);
		// GroupBox - Aktion
		hButtonGroupBox = CreateWindow(L"BUTTON", L"Aktion",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			int(scale*15), int(scale*10), int(scale*190), int(scale*82), hWnd, (HMENU)3,
			hInstGlobal, NULL);
		SendMessage(hButtonGroupBox, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
		// RadioButton - Herunterfahren
		hButtonRadioButton = CreateWindow(L"BUTTON", L"Herunterfahren",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			int(scale*25), int(scale*26), int(scale*120), int(scale*20), hWnd, (HMENU)4,
			hInstGlobal, NULL);
		SendMessage(hButtonRadioButton, BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(hButtonRadioButton, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
		// RadioButton - Neustart
		hButtonRadioButton2 = CreateWindow(L"BUTTON", L"Neustart",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			int(scale*25), int(scale*46), int(scale*100), int(scale*20), hWnd, (HMENU)5,
			hInstGlobal, NULL);
		SendMessage(hButtonRadioButton2, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
		// RadioButton - Abmelden
		hButtonRadioButton4 = CreateWindow(L"BUTTON", L"Abmelden",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			int(scale*25), int(scale*66), int(scale*100), int(scale*20), hWnd, (HMENU)7,
			hInstGlobal, NULL);
		SendMessage(hButtonRadioButton4, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
		// GroupBox - Optionen
		hButtonGroupBox2 = CreateWindow(L"BUTTON", L"Optionen",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			int(scale*15), int(scale*100), int(scale*190), int(scale*85), hWnd, (HMENU)8,
			hInstGlobal, NULL);
		SendMessage(hButtonGroupBox2, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
		// CheckBox - keine Benachrichtigung
		hButtonCheckBox1 = CreateWindow(L"BUTTON", L"keine Benachrichtigung",
			WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
			int(scale*25), int(scale*116), int(scale*160), int(scale*20), hWnd, (HMENU)9,
			hInstGlobal, NULL);
		SendMessage(hButtonCheckBox1, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
		// CheckBox - Verzögerung
		hButtonCheckBox2 = CreateWindow(L"BUTTON", L"Verzögerung",
			WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
			int(scale*25), int(scale*136), int(scale*150), int(scale*20), hWnd, (HMENU)10,
			hInstGlobal, NULL);
		SendMessage(hButtonCheckBox2, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
		// Edit - Minuten
		hEditTime_Minuten = CreateWindow(L"EDIT", L"Min",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,
			int(scale*45), int(scale*156), int(scale*40), int(scale*20), hWnd, (HMENU)11,
			hInstGlobal, NULL);
		EnableWindow(hEditTime_Minuten, false);		// Edit deaktiviert
		SendMessage(hEditTime_Minuten, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
		// Edit - Sekunden
		hEditTime_Sekunden = CreateWindow(L"EDIT", L"Sek",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,
			int(scale*90), int(scale*156), int(scale*40), int(scale*20), hWnd, (HMENU)12,
			hInstGlobal, NULL);
		EnableWindow(hEditTime_Sekunden, false);	// Edit deaktiviert
		SendMessage(hEditTime_Sekunden, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
		return 0;
	case WM_COMMAND:
		// wenn Focus auf Editbox und Verzögerung ausgewählt
		if (HIWORD(wParam) == EN_SETFOCUS && SendMessage(hButtonCheckBox2, BM_GETCHECK, 0, 0) == BST_CHECKED) {
			switch (LOWORD(wParam)) {
				// Minuten
			case 11:
				SetWindowText((HWND)lParam, L"");
				return 0;
				// Sekunden
			case 12:
				SetWindowText((HWND)lParam, L"");
				return 0;
			}
		}

		// wenn Button, CheckBox oder RadioBox angeklickt
		if (HIWORD(wParam) == BN_CLICKED) {
			switch (LOWORD(wParam)) {
				// OK
			case 1:
				int iParameter;
				wchar_t* szsekunden, * szminuten;
				// Variablen initialisieren
				szsekunden = new wchar_t[20];
				szminuten = new wchar_t[20];
				iParameter = -1;

				// OK/wenn verzögertes Herunterfahren aktiviert ist
				if (SendMessage(hButtonCheckBox2, BM_GETCHECK, 0, 0) == BST_CHECKED) {
					// auslesen der Edit-Boxen
					GetWindowText(hEditTime_Sekunden, szsekunden, 10);
					GetWindowText(hEditTime_Minuten, szminuten, 10);
					// Umwandlung Text in int
					isec = _wtoi(szsekunden);
					imin = _wtoi(szminuten);
					ShowWindow(hWindow, SW_HIDE); // verstecke MainWindow
					ShowWindow(hSubWindow, SW_SHOWNORMAL); // zeige SubWindow
					UpdateWindow(hSubWindow);
					SetTimer(0, 0, isec * 1000 + imin * 60000, Timer_Shutdown);
					SetTimer(hSubWindow, 1, 1000, 0);
					return 0;
				}
				// OK/Herunterfahren
				if (SendMessage(hButtonRadioButton, BM_GETCHECK, 0, 0) == BST_CHECKED)
					iParameter = EWX_SHUTDOWN;
				// OK/Neustart
				if (SendMessage(hButtonRadioButton2, BM_GETCHECK, 0, 0) == BST_CHECKED)
					iParameter = EWX_REBOOT;
				// OK/Abmelden
				if (SendMessage(hButtonRadioButton4, BM_GETCHECK, 0, 0) == BST_CHECKED)
					iParameter = EWX_LOGOFF;
				// OK/keine Nachricht an laufende Anwendenungen
				if (SendMessage(hButtonCheckBox1, BM_GETCHECK, 0, 0) == BST_CHECKED)
					iParameter = iParameter | EWX_FORCE;
				// Privilegien holen
				HANDLE hToken;
				TOKEN_PRIVILEGES tkp;
				OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
				LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
				tkp.PrivilegeCount = 1;  // one privilege to set    
				tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
				AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
				// gewählte Funktion ausführen
				ExitWindowsEx(iParameter, 0);
				return 0;
				// Abbrechen
			case 2:
				SendMessage(GetParent((HWND)lParam), WM_DESTROY, 0, 0);
				return 0;
				// Herunterfahren
			case 4:
				if (!(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED)) {
					SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED, 0);
					SendMessage(hButtonRadioButton2, BM_SETCHECK, BST_UNCHECKED, 0);
					SendMessage(hButtonRadioButton4, BM_SETCHECK, BST_UNCHECKED, 0);
				}
				return 0;
				// Neustart
			case 5:
				if (!(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED)) {
					SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED, 0);
					SendMessage(hButtonRadioButton, BM_SETCHECK, BST_UNCHECKED, 0);
					SendMessage(hButtonRadioButton4, BM_SETCHECK, BST_UNCHECKED, 0);
				}
				return 0;
				// Abmelden
			case 7:
				if (!(SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED)) {
					SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED, 0);
					SendMessage(hButtonRadioButton, BM_SETCHECK, BST_UNCHECKED, 0);
					SendMessage(hButtonRadioButton2, BM_SETCHECK, BST_UNCHECKED, 0);
				}
				return 0;
				// keine Nachricht an laufende Anwendungen
			case 9:
				if (SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED)
					SendMessage((HWND)lParam, BM_SETCHECK, BST_UNCHECKED, 0);
				else
					SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED, 0);
				return 0;
				// Verzögerung
			case 10:
				if (SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED) {
					SendMessage((HWND)lParam, BM_SETCHECK, BST_UNCHECKED, 0);
					SendMessage(hEditTime_Sekunden, WM_SETTEXT, 0, (LPARAM)L"Sek");
					SendMessage(hEditTime_Minuten, WM_SETTEXT, 0, (LPARAM)L"Min");
					EnableWindow(hEditTime_Sekunden, false);
					EnableWindow(hEditTime_Minuten, false);
					return 0;
				}
				else {
					SendMessage((HWND)lParam, BM_SETCHECK, BST_CHECKED, 0);
					EnableWindow(hEditTime_Sekunden, true);
					EnableWindow(hEditTime_Minuten, true);
					return 0;
				}
			}
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, uiMessage, wParam, lParam);
	}
}

// Prozedur SubProc
LRESULT CALLBACK SubProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam) {
	wchar_t* szText1, * szText2, * szText3;
	switch (uiMessage) {
	case WM_CREATE:
		HWND hButtonAbbrechen;

		hButtonAbbrechen = CreateWindow(L"BUTTON", L"Abbrechen",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			int(scale*127), int(scale*84), int(scale*85), int(scale*23), hWnd, (HMENU)2,
			hInstGlobal, NULL);
		SendMessage(hButtonAbbrechen, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), 0);
		return 0;
	case WM_PAINT:
		// Variablen
		wchar_t* szsec, * szmin;
		HDC hDC;
		PAINTSTRUCT ps;
		LOGFONT lf;
		RECT rect;

		// Initialisierung der Variablen
		SetRect(&rect, 0, int(scale*77), int(scale*230), int(scale*150));
		szText1 = new wchar_t[50];
		szText2 = new wchar_t[50];
		szText3 = new wchar_t[50];
		szsec = new wchar_t[5];
		szmin = new wchar_t[5];

		// Umwandlung der Zeit in Zeichenketten
		swprintf(szsec, L"%d", isec);
		swprintf(szmin, L"%d", imin);

		lstrcpy(szText1, L"Das Sytstem wird in der");
		lstrcpy(szText2, L"angegebenen Zeit herunter-");
		lstrcpy(szText3, L"gefahren:");
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);

		// Schrift anpassen:
		hFont = CreateFontIndirect(&lf);

		// Zeichnen beginnen
		hDC = BeginPaint(hWnd, &ps);

		FillRect(hDC, &rect, CreateSolidBrush(RGB(230, 230, 230)));
		SelectObject(hDC, hFont);
		SetBkMode(ps.hdc, TRANSPARENT);

		// Ausgabe Text
		TextOut(hDC, int(scale*10), int(scale*5), szText1, lstrlen(szText1));
		TextOut(hDC, int(scale*10), int(scale*22), szText2, lstrlen(szText2));
		TextOut(hDC, int(scale*10), int(scale*39), szText3, lstrlen(szText3));

		// Ausgabe Zeit
		SetTextAlign(hDC, TA_RIGHT);
		TextOut(hDC, int(scale*120), int(scale*39), szsec, lstrlen(szsec));
		TextOut(hDC, int(scale*96), int(scale*39), szmin, lstrlen(szmin));
		TextOut(hDC, int(scale*110), int(scale*39), L":", 2);

		delete[] szText1; delete[] szText2; delete[] szText3;
		EndPaint(hWnd, &ps);
		return 0;
	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED) {
			switch (LOWORD(wParam)) {
			case 1:
				return 0;
			case 2:
				SendMessage(GetParent((HWND)lParam), WM_DESTROY, 0, 0);
				return 0;
			default:
				return 0;
			}
		}
		return 0;
	case WM_TIMER:
		if (isec == 0 && imin == 0)
			return 0;
		else if (isec != 0)
			isec = isec - 1;
		else if (isec == 0) {
			imin--;
			isec = 59;
		}
		// Fenster neu zeichnen
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, uiMessage, wParam, lParam);
	}
}


void CALLBACK Timer_Shutdown(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime) {
	int iParameter;
	iParameter = -1;
	// OK/Herunterfahren
	if (SendMessage(hButtonRadioButton, BM_GETCHECK, 0, 0) == BST_CHECKED)
		iParameter = EWX_SHUTDOWN;
	// OK/Neustart
	if (SendMessage(hButtonRadioButton2, BM_GETCHECK, 0, 0) == BST_CHECKED)
		iParameter = EWX_REBOOT;
	// OK/Abmelden
	if (SendMessage(hButtonRadioButton4, BM_GETCHECK, 0, 0) == BST_CHECKED)
		iParameter = EWX_LOGOFF;
	// OK/keine Nachricht an laufende Anwendenungen
	if (SendMessage(hButtonCheckBox1, BM_GETCHECK, 0, 0) == BST_CHECKED)
		iParameter = iParameter | EWX_FORCE;
	// Privilegien
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	ExitWindowsEx(iParameter, 0);
}