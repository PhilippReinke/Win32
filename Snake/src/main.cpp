#include "../include/resource.h"
#include "../include/snake.h"
#include "../include/score.h"

// Prozeduren, Threads und Funktionen
LRESULT CALLBACK ProcMain(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ProcGame(HWND, UINT, WPARAM, LPARAM);

typedef HRESULT(WINAPI* PGetDpiForMonitor)(HMONITOR hmonitor, int dpiType, UINT* dpiX, UINT* dpiY);
WORD GetWindowDPI(HWND hWnd);

void drawInfoBox(HDC);
void drawText(HDC, LPWSTR, int, int, int, UINT);
std::wstring s2ws(const std::string& s);

// globale Variablen
int cxClient, cyClient;
enum class InfoMode{ Standard, Leaderboard };
InfoMode InfoboxMode = InfoMode::Standard;
RECT rectUmrandung;
HWND hWindow;
Snake snake;
Score score;
int Schlangenbreite;

// Hauptprogramm
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	WNDCLASS WndClass;
	static TCHAR szAppName[] = TEXT("Snake");

	// Variablen initialisieren
	WndClass.style = 0;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.lpfnWndProc = ProcMain;
	WndClass.hInstance = hInstance;
	WndClass.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.lpszMenuName = 0;
	WndClass.lpszClassName = szAppName;

	RegisterClass(&WndClass);

	// DPI von primärem Monitor
	HDC hScreenDC = GetDC(0);
	int dpi = GetDeviceCaps(hScreenDC, LOGPIXELSX);
	ReleaseDC(0, hScreenDC);
	float scale = dpi / 96.0;

	// Schlangenbreite festlegen
	Schlangenbreite = int(scale*15); // globale Variable. Benutzt für Schlange und Nahrung
	snake.set_Schlangenbreite(Schlangenbreite);

	// compute size of the window
	RECT wr = { 0, 0, (Schlangenbreite + 1) * 20 + 4, (Schlangenbreite + 1) * 15 + 4 };
	AdjustWindowRect(&wr, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, TRUE);

	// create the window and use the result as the handle
	hWindow = CreateWindow(szAppName, szAppName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
		NULL,
		LoadMenu(hInstance, MAKEINTRESOURCE(101)),
		hInstance,
		NULL);

	ShowWindow(hWindow, iCmdShow);

	UpdateWindow(hWindow);

	MSG Message;
	while (GetMessage(&Message, NULL, 0, 0)){
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return(Message.wParam);
}

// Prozedur ProcGame
LRESULT CALLBACK ProcGame(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam){
	HDC hDC;
	PAINTSTRUCT ps;
	HMENU hMenu = GetMenu(hWnd);

	switch (uiMessage){
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		// Umrandung zeichnen
		FrameRect(hDC, &rectUmrandung, snake.get_hBrush());
		
		// überprüfe die Einhaltung der Regel
		if (snake.scrutinyRules(cyClient, cxClient)){
			// Menüleiste aktivieren
			for (int i = 40002; i <= 40011; i++)
				EnableMenuItem(hMenu, i, MF_ENABLED);

			// Prozedur wechseln
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)ProcMain);

			// Schlange löschen
			snake.spendFood(hDC, cyClient, cxClient);
			snake.drawSnake(hDC);
			snake.shrinkSnake(hDC, rectUmrandung);

			// Nahrung löschen
			snake.set_bFood(FALSE);

			// Punktestand speichern
			score.saveScore();

			// Fenster neuzeichnen
			InvalidateRect(hWnd, 0, false);
			return 0;
		}

		// verteile Beweungspunkte
		score += snake.iLength + 1;

		// Hat die Schlange die Nahrung gefressen?
		if (snake.get_rectFood().right + 3 == snake.get_rectSnakeHead().right &&
			snake.get_rectFood().bottom + 3 == snake.get_rectSnakeHead().bottom){
			snake.set_bFood(FALSE);
			score += 50;
		}

		// zeichne Nahrung
		snake.spendFood(hDC, cyClient, cxClient);

		// zeichne Schlange
		snake.drawSnake(hDC);
		
		// aufräumen
		DeleteDC(hDC);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_KEYDOWN:
		switch (wParam){
		case VK_UP:
			if (snake.get_iRichtung() != 2)
				snake.set_iRichtung(1);
			break;

		case VK_DOWN:
			if (snake.get_iRichtung() != 1)
				snake.set_iRichtung(2);
			break;

		case VK_LEFT:
			if (snake.get_iRichtung() != 4)
				snake.set_iRichtung(3);
			break;

		case VK_RIGHT:
			if (snake.get_iRichtung() != 3)
				snake.set_iRichtung(4);
			break;

		case VK_SPACE:
			snake.set_bFood(FALSE);
			break;
		}
		return 0;

	case WM_TIMER:
		int dx, dy;
		dx = dy = 0;

		switch (snake.get_iRichtung()){
		case 1:	// hoch
			dy = -1 * (Schlangenbreite + 1);
			break;

		case 2:	// runter
			dy = (Schlangenbreite + 1);
			break;

		case 3:	// links
			dx = -1 * (Schlangenbreite + 1);
			break;

		case 4:	// rechts
			dx = (Schlangenbreite + 1);
			break;
		}

		snake.savePosition();
		snake.set_rectSnakeHead(snake.get_rectSnakeHead().left + dx,
			snake.get_rectSnakeHead().top + dy,
			snake.get_rectSnakeHead().right + dx,
			snake.get_rectSnakeHead().bottom + dy);
		InvalidateRect(hWnd, 0, true);

		return 0;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}

// Prozedur ProcMain
LRESULT CALLBACK ProcMain(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam){
	HDC hDC;
	PAINTSTRUCT ps;
	HMENU hMenu = GetMenu(hWnd);
	static int iSelectionVelocity = ID_VELOCITY_MEDIUM , iSelectionSize = ID_SIZE_SMALL;
	RECT rectWindow, wr;

	switch (uiMessage){
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		snake.set_rectSnakeHead(2,
			2,
			Schlangenbreite + 2,
			Schlangenbreite + 2);

		SetRect(&rectUmrandung, 2,
			2,
			cxClient - 2,
			cyClient - 2);

		return 0;

	case WM_COMMAND:
		GetWindowRect(hWnd, &rectWindow);

		switch (LOWORD(wParam)){
		case ID_FILE_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		case ID_VELOCITY_SLOW:
			snake.set_iTime(250);
			break;
		case ID_VELOCITY_MEDIUM:
			snake.set_iTime(125);
			break;
		case ID_VELOCITY_FAST:
			snake.set_iTime(62);
			break;
		case ID_VELOCITY_FASTER:
			snake.set_iTime(31);
			break;
		case ID_SIZE_SMALL:
			// compute size of the window
			wr = { 0, 0, (Schlangenbreite + 1) * 20 + 4, (Schlangenbreite + 1) * 15 + 4 };
			AdjustWindowRect(&wr, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, TRUE);

			// set size
			MoveWindow(hWnd, rectWindow.left,
				rectWindow.top, 
				wr.right - wr.left,    // width of the window
				wr.bottom - wr.top,    // height of the window
				true);
			break;
		case ID_SIZE_MEDIUM:
			// compute size of the window
			wr = { 0, 0, (Schlangenbreite + 1) * 28 + 4, (Schlangenbreite + 1) * 21 + 4 };
			AdjustWindowRect(&wr, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, TRUE);

			// set size
			MoveWindow(hWnd, rectWindow.left,
				rectWindow.top,
				wr.right - wr.left,    // width of the window
				wr.bottom - wr.top,    // height of the window
				true);
			break;
		case ID_SIZE_BIG:
			// compute size of the window
			wr = { 0, 0, (Schlangenbreite + 1) * 40 + 4, (Schlangenbreite + 1) * 30 + 4 };
			AdjustWindowRect(&wr, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, TRUE);

			// set size
			MoveWindow(hWnd, rectWindow.left,
				rectWindow.top,
				wr.right - wr.left,    // width of the window
				wr.bottom - wr.top,    // height of the window
				true);
			break;
		case ID_SCORE_LEADERBOARD:
			InfoboxMode = InfoMode::Leaderboard;;
			InvalidateRect(hWnd, 0, true);
			return 0;
		case ID_SCORE_RESET:
			if (MessageBox(hWnd, L"Are you sure?", L"Deletion of leaderboard", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
				score.resetScore();
			InvalidateRect(hWnd, 0, true);
			return 0;
		case ID_HELP_ABOUT:
			MessageBox(hWnd, L"Snake Adaption by Philipp Reinke", L"About SNAKE...", MB_OK | MB_ICONINFORMATION);
			return 0;
		}

		// setze Haken
		CheckMenuItem(hMenu, iSelectionSize, MF_UNCHECKED);
		CheckMenuItem(hMenu, iSelectionVelocity, MF_UNCHECKED);
		(LOWORD(wParam) > 40006) ? iSelectionSize = LOWORD(wParam) : iSelectionVelocity = LOWORD(wParam);
		CheckMenuItem(hMenu, iSelectionSize, MF_CHECKED);
		CheckMenuItem(hMenu, iSelectionVelocity, MF_CHECKED);
		InvalidateRect(hWnd, 0, true);
		return 0;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		// Umrandung zeichnen
		FrameRect(hDC, &rectUmrandung, snake.hBrush);
		CheckMenuItem(hMenu, iSelectionSize, MF_CHECKED);
		CheckMenuItem(hMenu, iSelectionVelocity, MF_CHECKED);

		// Namen abfragen
		// if (!score.getName().compare("unknown"))
		// 	score.readName(hWnd, cxClient, cyClient);

		// Schlange zurücksetzen
		snake.resetSnake();

		// weißes Infofenster zeichnen
		drawInfoBox(hDC);

		// aufräumen
		DeleteDC(hDC);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_KEYDOWN:
		switch (wParam){
		case VK_SPACE:
			// Timer neustarten
			KillTimer(hWnd, 1);
			SetTimer(hWindow, 1, snake.get_iTime(), NULL);

			// Menüleiste deaktivieren
			for (int i = 40002; i <= 40011; i++)
				EnableMenuItem(hMenu, i, MF_GRAYED);

			// Punktestand zurücksetzen
			score.setScore(0);

			// zu Prozedur ProcGame wechseln
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)ProcGame);

			// Menü zurücksetzen
			InfoboxMode = InfoMode::Standard;
			break;
		}

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uiMessage, wParam, lParam);
}

// string to wstring
std::wstring s2ws(const std::string& s){
	int len;
	int slength = (int)s.length() + 1;

	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);

	delete[] buf;
	return r;
}

// zeichne Infofenster
void drawInfoBox(HDC hDC){
	RECT rectInfoBox, rect;
	std::wstring wPuffer;
	std::multimap <std::wstring, std::wstring> score_copy;
	
	// Bestimmung der Größe und Position des Infofensters
	SetRect(&rectInfoBox, cxClient / 2 - 100,
		cyClient / 2 - 75,
		cxClient / 2 + 100,
		cyClient / 2 + 75);
	
	// zeichne weißes Feld und Umrandung
	FillRect(hDC, &rectInfoBox, (HBRUSH)GetStockObject(WHITE_BRUSH));
	FrameRect(hDC, &rectInfoBox, CreateSolidBrush(RGB(200, 200, 200)));

	switch (InfoboxMode){
	case InfoMode::Standard:
		// zeichne Nahrung und Schlange
		SetRect(&rect, cxClient / 2 + 40,
			rectInfoBox.top + 15,
			cxClient / 2 + 40 + Schlangenbreite,
			rectInfoBox.top + 15 + Schlangenbreite);
		FillRect(hDC, &rect, snake.get_hBrush());

		OffsetRect(&rect, Schlangenbreite + 1, 0);
		FillRect(hDC, &rect, snake.get_hBrush());

		OffsetRect(&rect, 0, 27);
		rect.bottom -= 4;
		rect.right -= 4;
		rect.left += 4;
		rect.top += 4;

		FillRect(hDC, &rect, snake.hBrushFood);

		// zeichne den Text
		drawText(hDC, L"Snake", rectInfoBox.top + 15, rectInfoBox.right, rectInfoBox.left + 20, 0);
		drawText(hDC, L"Food", rectInfoBox.top + 42, rectInfoBox.right, rectInfoBox.left + 20, 0);
		drawText(hDC, L"Score", rectInfoBox.top + 69, rectInfoBox.right, rectInfoBox.left + 20, 0);
		drawText(hDC, (LPWSTR)score.convertScore().c_str(), rectInfoBox.top + 69, rectInfoBox.right + 140, rectInfoBox.left + 160, 0);
		drawText(hDC, L"Press SPACE to start", rectInfoBox.bottom - 30, rectInfoBox.right, rectInfoBox.left, DT_CENTER);

		break;
	case InfoMode::Leaderboard:
		// Punktestand aus Datei lesen
		score.readScore();
		int i = 1;
		for (auto &elem : score.getScore()){
			drawText(hDC, (LPWSTR)s2ws(std::to_string(elem.first)).c_str(), rectInfoBox.top + (15 * i), rectInfoBox.right + 140, rectInfoBox.left + 160, 0);
			drawText(hDC, (LPWSTR)s2ws(elem.second.c_str()).c_str(), rectInfoBox.top + (15 * i++), rectInfoBox.right, rectInfoBox.left + 20, 0);
			if (i == 7)
				break;
		}
		drawText(hDC, L"Press SPACE to start", rectInfoBox.bottom - 30, rectInfoBox.right, rectInfoBox.left, DT_CENTER);
		
		// aufräumen
		score_copy.clear();
		break;
	}
	
}

// zeichne Text
void drawText(HDC hDC, LPWSTR lpchText, int iTop, int iRight, int iLeft, UINT uiFormat = 0){
	HFONT hFont = CreateFont(16, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, FF_SWISS, 0);
	RECT rectText;
	TEXTMETRIC tm;

	// setze den Hintergrund transparent und ändere die Schriftart
	SelectObject(hDC, hFont);
	SetBkMode(hDC, TRANSPARENT);

	// hole Daten der Schriftart
	GetTextMetrics(hDC, &tm);

	// bestimme das Rechteck
	SetRect(&rectText, iLeft, iTop, iRight, iTop + tm.tmHeight);

	// zeichne den Text
	DrawText(hDC, lpchText, lstrlen(lpchText), &rectText, uiFormat);

	// aufräumen
	DeleteObject(hFont);
}

// DPI abfragen, für moderne high dpi Bildschirme
WORD GetWindowDPI(HWND hWnd)
{
	// Try to get the DPI setting for the monitor where the given window is located.
	// This API is Windows 8.1+.
	HMODULE hShcore = LoadLibraryW(L"shcore");
	if (hShcore)
	{
		PGetDpiForMonitor pGetDpiForMonitor =
			reinterpret_cast<PGetDpiForMonitor>(GetProcAddress(hShcore, "GetDpiForMonitor"));
		if (pGetDpiForMonitor)
		{
			HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);
			UINT uiDpiX;
			UINT uiDpiY;
			HRESULT hr = pGetDpiForMonitor(hMonitor, 0, &uiDpiX, &uiDpiY);
			if (SUCCEEDED(hr))
			{
				return static_cast<WORD>(uiDpiX);
			}
		}
	}

	// We couldn't get the window's DPI above, so get the DPI of the primary monitor
	// using an API that is available in all Windows versions.
	HDC hScreenDC = GetDC(0);
	int iDpiX = GetDeviceCaps(hScreenDC, LOGPIXELSX);
	ReleaseDC(0, hScreenDC);

	return static_cast<WORD>(iDpiX);
}