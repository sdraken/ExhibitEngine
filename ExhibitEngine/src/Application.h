#pragma once
#include <iostream>
#include "UI/Window.h"

namespace ExhibitEngine {
	
	
	class Application{
	public:
		Application();

		void run();
	private:
		Window* window;

	};

}
