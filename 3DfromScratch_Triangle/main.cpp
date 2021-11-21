#include <windows.h>
#include <chrono>
#include <thread>
#include <vector>

#include "TriangleSimulation.h"
using namespace std;

// gloabal variables
unsigned width = 1200, height = 800;
bool bGameRunning = true;

// callback
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	MSG msg;
	static TCHAR szAppName[] = L"Triangle";

	// create window class
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	// register window class
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, L"Programm arbeitet mit Unicode und setzt Windows NT voraus!", szAppName, MB_ICONERROR);
		return 0;
	}

	// create window
	HWND window = CreateWindow(szAppName, L"Triangle",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL, NULL, hInstance, NULL);
	ShowWindow(window, iCmdShow);
	UpdateWindow(window);

	// get real client size (without border)
	RECT rect;
	if (GetWindowRect(window, &rect))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}

	// prepare buffer
	vector<unsigned int> buffer_memory(width*height, 0x000000);
	// further preparation
	HDC hdc = GetDC(window);
	BITMAPINFO bitmap_info;
	bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
	bitmap_info.bmiHeader.biWidth = width;
	bitmap_info.bmiHeader.biHeight = height;
	bitmap_info.bmiHeader.biPlanes = 1;
	bitmap_info.bmiHeader.biBitCount = 32;
	bitmap_info.bmiHeader.biCompression = BI_RGB;
	// prepare simulation
	TriangleSimulation triSim(width, height);

	// timer
	chrono::time_point<chrono::system_clock> t = chrono::system_clock::now();

	// run it
	double angle_z = 0;
	while (bGameRunning)
	{
		// 60FPS means new frame every 33.33ms
		t  = chrono::system_clock::now();
		t += chrono::milliseconds(16);

		// process inputs
		while (PeekMessage(&msg, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// simulate
		triSim.draw_triangle(angle_z, buffer_memory);
		angle_z += 0.03;

		// render
		StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, &buffer_memory.at(0), &bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		// delete buffer
		buffer_memory = vector<unsigned int>(width*height, 0x000000);

		// wait
        this_thread::sleep_until(t);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			bGameRunning = false;
			PostQuitMessage(0);
			break;
		}
	}
	
	return DefWindowProc(hwnd, message, wParam, lParam);
}