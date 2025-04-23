// NahimicMemoryTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>

HWND hWND;
HDC hDC;
HGLRC hGLRC;

HWND _CreateWindow();

void initializeOGLWindow() {
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
	0
	};
	pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
	pixelFormatDescriptor.nVersion = 1;

	pixelFormatDescriptor.dwFlags =
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.dwLayerMask = PFD_MAIN_PLANE;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.cAlphaBits = 8;
	pixelFormatDescriptor.cColorBits = 32;
	pixelFormatDescriptor.cDepthBits = 16;
	pixelFormatDescriptor.cStencilBits = 8;

	int pixelFormat;
	pixelFormat = ChoosePixelFormat(hDC, &pixelFormatDescriptor);
	SetPixelFormat(hDC, pixelFormat, &pixelFormatDescriptor);


	// Create context
	hGLRC = wglCreateContext(hDC);

	// Make the context current
	wglMakeCurrent(hDC, hGLRC);
}

void destroyOGLWindow() {
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hGLRC);
}

LRESULT WINAPI _windowProc(HWND hWndArg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_SPACE:
			// Destroy the old window
			destroyOGLWindow();
			ReleaseDC(hWND, hDC);
			DestroyWindow(hWND);

			// Create and show the new
			hWND = _CreateWindow();
			hDC = GetDC(hWND);
			initializeOGLWindow();
			ShowWindow(hWND, SW_SHOW);

			// Watch memory go boom
			SwapBuffers(hDC);
		}
		break;
	}
	return DefWindowProcW(hWndArg, uMsg, wParam, lParam);
}

HWND _CreateWindow()
{
	WNDCLASS wc = { 0 };
	RECT wRect = {0, 0, 1024, 1024};
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	wc.lpfnWndProc = _windowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"OGLWindow";

	RegisterClass(&wc);

	AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	// center the window by czk
	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int windowWidth = wRect.right - wRect.left;
	const int windowHeight = wRect.bottom - wRect.top;
	const int windowLeft = (screenWidth - windowWidth) / 2;
	const int windowRight = (screenHeight - windowHeight) / 2;
	return CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"OGLWindow",
		L"MemoryTest",
		WS_OVERLAPPEDWINDOW,
		windowLeft,
		windowRight,
		windowWidth, windowHeight,
		nullptr,
		nullptr,
		hInstance,
		nullptr);
}

int main()
{
	// Create a basic window with an OpenGL context
	hWND = _CreateWindow();
	hDC = GetDC(hWND);
	initializeOGLWindow();
	ShowWindow(hWND, SW_SHOW);
	SwapBuffers(hDC);
	while (true)
	{
		MSG Message{ 0 };
		while (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE)) {
			if (Message.message == WM_QUIT) {
				return 0;
			}
			else {
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
		}
	}

	// Cleanup
	destroyOGLWindow();
	ReleaseDC(hWND, hDC);
	return 0;
}