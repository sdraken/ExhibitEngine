//	Description: The application class handles the initilization and shut down of the ExhibitEngine's components, as well as containing the game loop. 
//				 This ensures that the components are created and destroyed in the correct order. The entry point (i.e the main function) starts the game
//				 by creating an instance of the Application class and calling run(). 
//
//	Author: Svante Drakenberg


#pragma once

#include "Logging/Logger.h"
#include "UI/Window.h"
#include "Renderer/Renderer.h"


namespace ExhibitEngine {
	
	//member variables consist of ExhibitEngine components

	//Component responsible for logging useful/crucial information during runtime
	Logger logger;
	//Component responsible for creating/managing window
	Window window;
	//Component responsible for rendering
	Renderer renderer(window);

}
