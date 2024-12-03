//	Description: Implementation of Window class. Comments aren't a high priority since the implementation is mostly
//				 derived from the official programming reference for the Win32 API.
//
//	Author: Svante Drakenberg

#include "Window.h"

namespace ExhibitEngine{

	void Window::initilize(){
		
		instanceHandle = GetModuleHandle(NULL);

		if (!instanceHandle)LOGFATAL("failed to get instance handle");
		
		const wchar_t appName[14] = L"ExhibitEngine";
		WNDCLASSEX windowClass = { 0 };

		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = windowProcedure;
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
			NULL);


		if (!windowHandle)LOGFATAL("failed to create window");

		ShowWindow(windowHandle, SW_SHOW);
		SetForegroundWindow(windowHandle);
		SetFocus(windowHandle);
	}

	void Window::shutDown()
	{
	}

	BOOL Window::processEventSlow(){
		MSG message;
		BOOL returnValue = GetMessage(&message, NULL, 0, 0);
		TranslateMessage(&message);
		DispatchMessage(&message);

		return returnValue;
	}

	LRESULT Window::windowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam){
		switch (message) {
		case WM_DESTROY:
			DestroyWindow(windowHandle);
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			ValidateRect(windowHandle, NULL);
			break;
		default:
			return DefWindowProc(windowHandle, message, wParam, lParam);
			break;
		}

		return 0;
	}

}