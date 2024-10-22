﻿// ExhibitEngine.cpp : Defines the entry point for the application.
//

// no console #pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

#include "Application.h"

namespace ExhibitEngine {

	Application::Application()
	{
		window = &(Window::getInstance());
		window->initialize();
	}

	void Application::run()
	{
		while (window->processEventSlow()) {

		}
	}

}

int main() {
	ExhibitEngine::Application app;
	app.run();
}
