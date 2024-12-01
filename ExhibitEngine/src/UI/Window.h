//	Description: The Window class handles the creation and managing of the window. this will only support Windows operating systems initially. 
//				 Currently very basic, creates a window that can process basic events.
//
//	Author: Svante Drakenberg

#pragma once
#define UNICODE
#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "../Logging/Logger.h"

namespace ExhibitEngine {

	class Window{
	public:
		void initilize();
		void shutDown();

		BOOL processEventSlow();

		HINSTANCE getHINSTANCE() { return instanceHandle; }
		HWND getHWND(){ return windowHandle; }
	private:
		HINSTANCE instanceHandle;
		HWND windowHandle;

		static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	};

}