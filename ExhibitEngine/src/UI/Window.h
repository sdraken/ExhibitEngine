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