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
		Window();
		~Window();

		bool processEventSlow();

		const HINSTANCE getHINSTANCE() const{ return instanceHandle; }
		const HWND getHWND()const{ return windowHandle; }
		bool& getWindowResized()const {return windowResized;}
	private:
		HINSTANCE instanceHandle;
		HWND windowHandle;
		mutable bool windowResized = false;

		static LRESULT CALLBACK staticWindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
	};

}