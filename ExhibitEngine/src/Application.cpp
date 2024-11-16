// no console #pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#include "Application.h"

namespace ExhibitEngine {

	Application::Application(){
		logger.initalize();
		window.initilize();
	}

	Application::~Application(){
		window.shutDown();
		logger.shutDown();
	}

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
