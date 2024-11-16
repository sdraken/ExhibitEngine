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
	}

	//Components are shut down in the correct order
	Application::~Application(){
		window.shutDown();
		logger.shutDown();
	}

	//start loop
	void Application::run(){

		//testing Logger and BumpAllcator
		std::vector<int, BumpAllocator<int>> vec1 = { 1,2,3 };
		std::vector<char, BumpAllocator<char>> vec2 = { '1','2','3' };
		std::vector<int, BumpAllocator<int>> vec3 = { 1,2,3 };

		while (window.processEventSlow()) {

		}
	}

}


int main(){
	ExhibitEngine::Application app;
	app.run();
}
