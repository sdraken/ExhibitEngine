//	Description: Implementation of Application class
//
//	Author: Svante Drakenberg

// no console #pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

#include "Application.h"

namespace ExhibitEngine {

	//Components are initalized in the correct order
	Application::Application(){
		logger.initalize();
		window.initilize();
		renderer.initilize();
	}

	//Components are shut down in the correct order
	Application::~Application(){
		renderer.shutDown();
		window.shutDown();
		logger.shutDown();
	}

	//start loop
	void Application::run(){

		while (window.processEventSlow()) {

		}
	}

}


int main(){
	ExhibitEngine::Application app;
	app.run();
}
