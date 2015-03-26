#include "ATLAS_Stdafx.h"
#include "ATLAS_RenderContext.h"
#include "ATLAS_Texture.h"
#include <Windows.h>
#undef CreateWindow

using namespace ATLAS;

LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Win32
{
	struct Window
	{
		HINSTANCE		hinst;
		HWND			hwnd;
		HDC				hdc;
		BITMAPINFO		frame_buffer_info;
		void			*frame_buffer;
		uint32			fb_width;
		uint32			fb_height;
		uint32			fb_bpp;
		uint32			width;
		uint32			height;
		uint32			style;
	};

	bool32 HandleMessages()
	{
		MSG msg = {};
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				return FALSE;

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return TRUE;
	}

	void ResizeScreenBuffers(Window *win32_window, int32 width, int32 height, int32 bpp = 32)
	{
		BITMAPINFO *fb_info = &win32_window->frame_buffer_info;

		if (win32_window->frame_buffer)
			VirtualFree(win32_window->frame_buffer, NULL, MEM_RELEASE);

		win32_window->fb_width = width;
		win32_window->fb_height = height;
		win32_window->fb_bpp = bpp;

		fb_info->bmiHeader.biSize = sizeof(fb_info->bmiHeader);
		fb_info->bmiHeader.biWidth = width;
		fb_info->bmiHeader.biHeight = height;
		fb_info->bmiHeader.biPlanes = 1;
		fb_info->bmiHeader.biBitCount = bpp;
		fb_info->bmiHeader.biCompression = BI_RGB;

		win32_window->frame_buffer = VirtualAlloc(0, width * height * bpp, MEM_COMMIT, PAGE_READWRITE);
	}
	void SwapBuffers(Window *win32_window)
	{
		BITMAPINFO *fb_info = &win32_window->frame_buffer_info;

		RECT rc = {};
		GetClientRect(win32_window->hwnd, &rc);
		win32_window->width = rc.right - rc.left;
		win32_window->height = rc.bottom - rc.top;

		StretchDIBits(win32_window->hdc,
			0, 0, win32_window->width, win32_window->height,
			0, 0, win32_window->fb_width, win32_window->fb_height,
			win32_window->frame_buffer, fb_info, DIB_RGB_COLORS, SRCCOPY);
	};
	bool32 CreateWindow(Window *win32_window, LPCWSTR title, int32 window_width = CW_USEDEFAULT, int32 window_height = CW_USEDEFAULT, int32 style = 1, int32 buffer_width = 1280, int32 buffer_height = 720)
	{
		win32_window->hinst = GetModuleHandleW(NULL);

		LPCWSTR lpCLassName = title;

		WNDCLASSEXW wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WinProc;
		wcex.hInstance = win32_window->hinst;
		wcex.lpszClassName = lpCLassName;

		if (RegisterClassExW(&wcex)) {
			DWORD dwExStyle = 0;
			LPCWSTR lpWindowName = title;
			DWORD dwStyle = WS_OVERLAPPEDWINDOW | (style & 1 ? WS_VISIBLE : NULL);
			INT nWindow_X = CW_USEDEFAULT;
			INT nWindow_Y = CW_USEDEFAULT;
			INT nWindow_W = window_width;
			INT nWindow_H = window_height;

			win32_window->hwnd = CreateWindowExW(
				dwExStyle, lpCLassName,
				lpWindowName, dwStyle,
				nWindow_X, nWindow_Y,
				nWindow_W, nWindow_H,
				NULL, NULL,
				win32_window->hinst, NULL);

			if (!win32_window->hwnd)
			{
				win32_window = {};
				return FALSE;
			}

			win32_window->hdc = GetDC(win32_window->hwnd);

			RECT rc;
			GetWindowRect(win32_window->hwnd, &rc);

			win32_window->width = rc.right - rc.left;
			win32_window->height = rc.bottom - rc.top;
			win32_window->style = style;

			ResizeScreenBuffers(win32_window, buffer_width, buffer_height);

			return TRUE;
		}

		return FALSE;
	}
	void CloseWindow(Window *win32_window)
	{
		DestroyWindow(win32_window->hwnd);

		if (&win32_window->frame_buffer)
			VirtualFree(&win32_window->frame_buffer, NULL, MEM_RELEASE);

		win32_window = {};
	}
}

GLOBAL Win32::Window win32_window = {};
GLOBAL RenderContext *pRC;

LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (msg) {
		case WM_SIZE: {
			pRC->Resize(LOWORD(lParam), HIWORD(lParam));
			Win32::ResizeScreenBuffers(&win32_window, LOWORD(lParam), HIWORD(lParam), 32);
			Win32::SwapBuffers(&win32_window);
		} break;

		case WM_KEYDOWN: {
			real32 speed = 1.0f;

			switch (wParam) {
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				default:
					break;
			}

			
		} break;

		case WM_CLOSE: {
			PostQuitMessage(0);
		} break;

		default: {
			result = DefWindowProcW(hwnd, msg, wParam, lParam);
		} break;
	}

	return result;
}

void DrawChar(Point p, uint32 height, char ch)
{
	uint32 width = height / 2.0f;
		switch (ch) {
		case ' ': {
		}break;

		case '_': {
			pRC->DrawLine(p, Point(p.x + width, p.y));
		}break;

		case 'A': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x + width, p.y);
			Point p4(p.x, p.y + height / 2);
			Point p5(p.x + width, p.y + height / 2);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p4, p5);
		}break;

		case 'B': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width * 1.25f, p.y + height);
			Point p3(p.x, p.y + height / 2);
			Point p4(p.x + width * 1.25f, p.y + height / 2);
			Point p5(p.x + width * 1.25f, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawCurve(p1, p2, p4, p3);
			pRC->DrawCurve(p3, p4, p5, p);
		}break;

		case 'C': {
			Point p1(p.x + width, p.y + height / 4);
			Point p2(p.x, p.y - height / 2);
			Point p3(p.x, p.y + height * 1.5f);
			Point p4(p.x + width, p.y + (height / 4) * 3);
			pRC->DrawCurve(p4, p3, p2, p1);
		}break;

		case 'D': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x + width, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawCurve(p1, p2, p3, p);
		}break;

		case 'E': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x, p.y + height / 2);
			Point p4(p.x + width * 0.75f, p.y + height / 2);
			Point p5(p.x + width, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p3, p4);
			pRC->DrawLine(p, p5);
		}break;

		case 'F': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x, p.y + height / 2);
			Point p4(p.x + width * 0.75f, p.y + height / 2);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p3, p4);
		}break;

		case 'G': {
			Point p1(p.x + width, p.y + height);
			Point p2(p.x - width * 0.75f, p.y + height);
			Point p3(p.x - width * 0.75f, p.y);
			Point p4(p.x + width, p.y);
			Point p5(p.x + width, p.y + height / 2);
			Point p6(p.x + width / 2, p.y + height / 2);
			pRC->DrawCurve(p1, p2, p3, p4);
			pRC->DrawLine(p4, p5);
			pRC->DrawLine(p5, p6);
		}break;

		case 'H': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y);
			Point p3(p.x + width, p.y + height);
			Point p4(p.x, p.y + height / 2);
			Point p5(p.x + width, p.y + height / 2);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p4, p5);
		}break;

		case 'I': {
			Point p1(p.x + width, p.y);
			Point p2(p.x, p.y + height);
			Point p3(p.x + width, p.y + height);
			Point p4(p.x + width / 2, p.y + height);
			Point p5(p.x + width / 2, p.y);

			pRC->DrawLine(p, p1);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p4, p5);
		}break;

		case 'J': {
			Point p1(p.x + width, p.y + height);
			Point p2(p.x + width, p.y - height * 0.5f);
			Point p3(p.x, p.y + height * 0.25f);
			pRC->DrawCurve(p1, p2, p, p3);
		}break;

		case 'K': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x, p.y + height / 2);
			Point p4(p.x + width, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p3, p4);
		}break;

		case 'L': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p, p2);
		}break;

		case 'M': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x + width, p.y);
			Point p4(p.x + width / 2, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p1, p4);
			pRC->DrawLine(p4, p2);
		}break;

		case 'N': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x + width, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p1, p3);
		}break;

		case 'O': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x + width, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p, p3);
		}break;

		case 'P': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width * 1.25f, p.y + height);
			Point p3(p.x + width * 1.25f, p.y + height / 2);
			Point p4(p.x, p.y + height / 2);
			pRC->DrawLine(p, p1);
			pRC->DrawCurve(p1, p2, p3, p4);
		}break;

		case 'Q': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x + width, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p, p3);
		}break;

		case 'R': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width * 1.25f, p.y + height);
			Point p3(p.x + width * 1.25f, p.y + height / 2);
			Point p4(p.x, p.y + height / 2);
			Point p5(p.x + width, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawCurve(p1, p2, p3, p4);
			pRC->DrawLine(p4, p5);
		}break;

		case 'S': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x + width, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p, p3);
		}break;

		case 'T': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x + width / 2, p.y + height);
			Point p4(p.x + width / 2, p.y);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p3, p4);
		}break;

		case 'U': {
			Point p1(p.x, p.y + height);
			Point p2(p.x, p.y - height * 0.25f);
			Point p3(p.x + width, p.y - height * 0.25f);
			Point p4(p.x + width, p.y + height);
			pRC->DrawCurve(p1, p2, p3, p4);
		}break;

		case 'V': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width / 2, p.y);
			Point p3(p.x + width, p.y + height);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p2, p3);
		}break;

		case 'W': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x + width, p.y);
			Point p4(p.x + width / 2, p.y + height);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p, p4);
			pRC->DrawLine(p4, p3);
		}break;

		case 'X': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y);
			Point p3(p.x + width, p.y + height);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p, p3);
		}break;

		case 'Y': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width / 2, p.y + height / 2);
			Point p3(p.x + width, p.y + height);
			Point p4(p.x + width / 2, p.y);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p2, p4);
		}break;

		case 'Z': {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x + width, p.y);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p2, p);
			pRC->DrawLine(p, p3);
		}break;

		default: {
			Point p1(p.x, p.y + height);
			Point p2(p.x + width, p.y + height);
			Point p3(p.x + width, p.y);
			pRC->DrawLine(p, p1);
			pRC->DrawLine(p1, p2);
			pRC->DrawLine(p2, p3);
			pRC->DrawLine(p, p3);
		}break;
	}
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR pCmdLine, int nCmdShow)
{
	if (!Win32::CreateWindow(&win32_window, L"SoftwareRenderer", 1280, 720, 1))
		return -1;

	RenderContext rc( win32_window.frame_buffer,
		win32_window.fb_width, win32_window.fb_height,
		win32_window.fb_bpp);

	pRC = &rc;
	pRC->SetClearColor(Color(0.1f, 0.2f, 0.3f));
	pRC->SetPointSize(2);
	
	char str[512] = "";
	LARGE_INTEGER current_time, last_time, elapsed_time, frequency;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&last_time);

	while (Win32::HandleMessages()) {
		QueryPerformanceCounter(&current_time);
		elapsed_time.QuadPart = current_time.QuadPart - last_time.QuadPart;
		elapsed_time.QuadPart *= 1000000;
		elapsed_time.QuadPart /= frequency.QuadPart;
		real64 delta_time = elapsed_time.QuadPart / 1000.0f;
		last_time = current_time;
		
		Point p(100, 600);
		uint32 size = 10;

		pRC->Clear();
			// Render code goes here
		DrawChar(p, size, 'A');
		DrawChar(Point(p.x += 100, p.y), size, 'B');
		DrawChar(Point(p.x += 100, p.y), size, 'C');
		DrawChar(Point(p.x += 100, p.y), size, 'D');
		DrawChar(Point(p.x += 100, p.y), size, 'E');
		DrawChar(Point(p.x += 100, p.y), size, 'F');
		DrawChar(Point(p.x += 100, p.y), size, 'G');
		DrawChar(Point(p.x += 100, p.y), size, 'H');
		DrawChar(Point(p.x += 100, p.y), size, 'I');
		DrawChar(Point(p.x += 100, p.y), size, 'J');
		DrawChar(Point(p.x -= 900, p.y -= 150), size, 'K');
		DrawChar(Point(p.x += 100, p.y), size, 'L');
		DrawChar(Point(p.x += 100, p.y), size, 'M');
		DrawChar(Point(p.x += 100, p.y), size, 'N');
		DrawChar(Point(p.x += 100, p.y), size, 'O');
		DrawChar(Point(p.x += 100, p.y), size, 'P');
		DrawChar(Point(p.x += 100, p.y), size, 'Q');
		DrawChar(Point(p.x += 100, p.y), size, 'R');
		DrawChar(Point(p.x += 100, p.y), size, 'S');
		DrawChar(Point(p.x += 100, p.y), size, 'T');
		DrawChar(Point(p.x -= 900, p.y -= 150), size, 'U');
		DrawChar(Point(p.x += 100, p.y), size, 'V');
		DrawChar(Point(p.x += 100, p.y), size, 'W');
		DrawChar(Point(p.x += 100, p.y), size, 'X');
		DrawChar(Point(p.x += 100, p.y), size, 'Y');
		DrawChar(Point(p.x += 100, p.y), size, 'Z');
		DrawChar(Point(p.x += 100, p.y), size, ' ');
		DrawChar(Point(p.x += 100, p.y), size, '_');
		Win32::SwapBuffers(&win32_window);
	}

	Win32::CloseWindow(&win32_window);

	return(0);
}