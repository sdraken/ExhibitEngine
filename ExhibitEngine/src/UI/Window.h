#pragma once
#define UNICODE
#include "Windows.h"
#include <iostream>

namespace ExhibitEngine {

	class Window{
	public:
		Window():instanceHandle(NULL), windowHandle(NULL){}
		~Window(){}

		void initilize();
		void shutDown();

		BOOL processEventSlow();


	private:
		HINSTANCE instanceHandle;
		HWND windowHandle;

		static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	};

}