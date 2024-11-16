//	Description: The Window class handles the creation and managing of the window. this will only support Windows operating systems initially. 
//				 Currently very basic, creates a window that can process basic events.
//
//	Author: Svante Drakenberg

#pragma once
#define UNICODE
#include "Windows.h"
#include "../Logging/Logger.h"

namespace ExhibitEngine {

	class Window{
	public:
		void initilize();
		void shutDown();

		BOOL processEventSlow();


	private:
		HINSTANCE instanceHandle;
		HWND windowHandle;

		static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	};

}