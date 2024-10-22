#pragma once
#define UNICODE
#include "Windows.h"
#include <iostream>

namespace ExhibitEngine {

	class Window
	{
	public:
        static Window& getInstance()
        {
            static Window instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }

		void initialize();

		BOOL processEventSlow();

		Window(Window const&) = delete;
		void operator=(Window const&) = delete;

	private:
		Window(){}
		HINSTANCE instanceHandle = NULL;
		HWND windowHandle = NULL;

		static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	};

}