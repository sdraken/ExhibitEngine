//	Description: Implementation of Window class. Comments aren't a high priority since the implementation is mostly
//				 derived from the official programming reference for the Win32 API.
//
//	Author: Svante Drakenberg

#include "Window.h"

namespace ExhibitEngine{

	Window::Window(){
		
		instanceHandle = GetModuleHandle(NULL);

		if (!instanceHandle)LOGFATAL("failed to get instance handle");
		
		const wchar_t appName[14] = L"ExhibitEngine";
		WNDCLASSEX windowClass = { 0 };

		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = staticWindowProcedure;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = instanceHandle;
		windowClass.hIcon = LoadIcon(instanceHandle, IDI_APPLICATION);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = appName;
		windowClass.hIconSm = LoadIcon(windowClass.hInstance, IDI_APPLICATION);

		if (!RegisterClassEx(&windowClass)) LOGFATAL("failed to register class");

		windowHandle = CreateWindow(
			appName,
			appName,
			WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			instanceHandle,
			this);


		if (!windowHandle)LOGFATAL("failed to create window");

		ShowWindow(windowHandle, SW_SHOW);
		SetForegroundWindow(windowHandle);
		SetFocus(windowHandle);
	}

	Window::~Window()
	{
	}

    bool Window::processEventSlow()
    {
		MSG msg = {};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
				return false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		return true;
    }

    LRESULT Window::staticWindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
    {

        if (message == WM_NCCREATE) {
            // Extract the 'this' pointer from CREATESTRUCT and associate it with the window
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            Window* pThis = (Window*)pCreate->lpCreateParams;
            SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)pThis);
        } else {
            // Retrieve the stored 'this' pointer
            Window* pThis = (Window*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);
            if (pThis) {
                return pThis->WindowProcedure(windowHandle, message, wParam, lParam);
            }
        }

        return DefWindowProc(windowHandle, message, wParam, lParam);
    }

    LRESULT Window::WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message) {
		case WM_CLOSE:
			DestroyWindow(windowHandle);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			windowResized = true;
			break;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
    }
}