#include "Stdafx.h"
#include "Color4f.h"
#include "BitmapImage.h"

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Page : public string
{
public:
	Page(HWND hWnd, string text = "")
	{
		m_HWND = hWnd;
		ResizeCanvas(1280, 720);
		append(text);
		m_CaretPos = length();
		Draw();
	}

	void ResizeCanvas(uint32 width, uint32 height, uint32 bpp = 32)
	{
		if (width < 0 && height < 0)
			return;

		if (m_Canvas)
			VirtualFree(m_Canvas, NULL, MEM_RELEASE);

		m_Width = width;
		m_Height = height;
		m_BPP = bpp;

		m_BMP.bmiHeader.biSize = sizeof(BITMAPINFO);
		m_BMP.bmiHeader.biWidth = width;
		m_BMP.bmiHeader.biHeight = height;
		m_BMP.bmiHeader.biPlanes = 1;
		m_BMP.bmiHeader.biBitCount = bpp;
		m_BMP.bmiHeader.biCompression = BI_RGB;

		m_Canvas = (uint32 *)VirtualAlloc(0, width * height * bpp, MEM_COMMIT, PAGE_READWRITE);
	}
	void Draw()
	{
		if (!m_Canvas)
			return;

		ClearCanvas();
		DrawString();

		RECT rect;
		GetClientRect(m_HWND, &rect);
		uint32 width = rect.right - rect.left;
		uint32 height = rect.bottom - rect.top;

		StretchDIBits(GetDC(m_HWND),
			0, 0, width, height,
			0, 0, m_Width, m_Height,
			m_Canvas, &m_BMP, DIB_RGB_COLORS, SRCCOPY);
	}
	void Update(const char c)
	{
		switch (c) {
			case '\r':
			case '\n':
				m_LineCount++;
			default:
				insert(m_CaretPos++, &c);
				break;
			case 8:
				--m_CaretPos;
				m_CaretPos = max(0, m_CaretPos);
				erase(m_CaretPos);
				break;
			case 27:
				break;
		}
		
		DrawString();
	}
	void SetFont(BitmapImage *font)
	{
		m_Font = font;
	}

private:
	void ClearCanvas()
	{
		memset(m_Canvas, 0, m_Width * m_Height * (m_BPP / 8));
	}
	void DrawString()
	{
		uint32 line_height = 16;
		uint32 char_width = 16;
		uint32 y = m_Height - 16 - 2;
		uint32 x = 2;

		for (uint32 i = 0; i < length(); ++i) {
			char c = at(i);
			if (c == '\n' || c == '\r') {
				//if (c == '\r')
				//	i++;
				x = 2;
				y -= line_height;
				continue;
			}

			Color4f color = Color4f::WHITE;
			DrawChar(x, y, c, color);
			x += char_width;
		}
	}
	void DrawChar(uint32 x_pos, uint32 y_pos, uint32 character, Color4f color)
	{
		if (x_pos > m_Width || y_pos > m_Height)
			return;

		uint32 char_width = m_Font->width / 16;
		uint32 char_height = m_Font->height / 16;

		uint32 char_x = character % 16;
		uint32 char_x_start = char_x * char_width;
		uint32 char_y = character / 16;
		uint32 char_y_start = m_Font->height - (char_y + 1) * char_height;
		uint32 char_size = char_width * char_height;
		uint32 *char_bits = new uint32[char_size];

		uint32 * font = (uint32 *)m_Font->data;

		for (uint32 y = 0; y < char_height; ++y) {
			for (uint32 x = 0; x < char_width; ++x) {
				char_bits[x + y * char_width] = font[(x + char_x_start) + ((y + char_y_start) * m_Font->width)];
			}
		}

		for (uint32 y = 0; y < char_height; ++y) {
			for (uint32 x = 0; x < char_width; ++x) {
				Color4f color(char_bits[x + y * char_width]);
				m_Canvas[(x + x_pos) + (y + y_pos) * m_Width] = Blend(color, Color4f::RED, BLEND_MULTIPLY).toColor32();
			}
		}

		delete[] char_bits;
	}

	HWND m_HWND;
	BITMAPINFO m_BMP;
	uint32 m_Width, m_Height, m_BPP;
	uint32 *m_Canvas;
	
	uint32 m_LineCount;
	int32 m_CaretPos;
	BitmapImage *m_Font;
};

Page *pCurrentPage;
int32 WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR pCmdLine, int32 nCmdShow)
{
	WNDCLASSEXW wcex = {};
	LPWSTR class_name = L"TextEditorWindowClass";
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WinProc;
	wcex.hInstance = hInst;
	wcex.lpszClassName = class_name;

	if (RegisterClassExW(&wcex)) {
		HWND hWnd = CreateWindowExW(NULL,
			class_name, L"Text Editor",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL, hInst, NULL);

		if (hWnd) {
			UpdateWindow(hWnd);
			ShowWindow(hWnd, nCmdShow);

			BitmapImage consolas("Consolas.bmp");
			Page page(hWnd);
			page.SetFont(&consolas);
			pCurrentPage = &page;

			MSG msg = {};
			while (GetMessageW(&msg, NULL, 0, 0)) {
				if (msg.message == WM_QUIT)
					break;

				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		} else {
			Log("***ERROR***\n");
			Log("Failed to create window!\n");
		}
	} else {
		Log("***ERROR***\n");
		Log("Failed to register window class.\n");
	}

	return 0;
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (msg) {
		case WM_SIZE: {
			pCurrentPage->ResizeCanvas(LOWORD(lParam), HIWORD(lParam));
		} break;

		case WM_PAINT: {
			pCurrentPage->Draw();
		} break;

		case WM_CHAR: {
			Log("%c %i\n", wParam, wParam);
			pCurrentPage->Update(wParam);
		} break;

		case WM_CLOSE: {
			PostQuitMessage(0);
		} break;

		default: {
			result = DefWindowProcW(hWnd, msg, wParam, lParam);
		} break;
	}

	return result;
}