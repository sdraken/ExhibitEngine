#include "Window.h"
#include <stdexcept>

namespace ExhibitEngine {

	void Window::initialize()
	{
		
		instanceHandle = GetModuleHandle(NULL);

		std::cout << "test\n";
		
		const wchar_t appName[5] = L"name";
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

		if (!RegisterClassEx(&windowClass)) throw std::runtime_error("failed to register class");

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

		if (!windowHandle)throw std::runtime_error("failed to create window");

		ShowWindow(windowHandle, SW_SHOW);
		SetForegroundWindow(windowHandle);
		SetFocus(windowHandle);
	}

	BOOL Window::processEventSlow()
	{
		MSG message;
		BOOL returnValue = GetMessage(&message, NULL, 0, 0);
		TranslateMessage(&message);
		DispatchMessage(&message);

		return returnValue;
	}

	LRESULT Window::windowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	{
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