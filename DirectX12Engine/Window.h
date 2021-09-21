#pragma once
#include <d3d12.h>
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wp, LPARAM lp);
class Window
{
public:
	Window() {};
	HWND GetHWND()const { return m_HWND; }
	
	bool InitWnd();
	UINT GetWindowWidth() { return m_WindowWidth; };
	UINT GetWindowHeight() { return m_WindowHeight; };
private:
	 UINT m_WindowWidth = 1280;
 UINT m_WindowHeight = 720;
	HWND m_HWND;
	HINSTANCE m_hInst;

};

