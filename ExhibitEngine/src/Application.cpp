//	Description: Implementation of Application class
//
//	Author: Svante Drakenberg

// no console #pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

#include "Application.h"

int main(){
    using namespace ExhibitEngine;
    bool running = true;
    while (running) {
        running = window.processEventSlow();

        renderer.drawFrame();
	}
}
