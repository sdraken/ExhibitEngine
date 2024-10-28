#pragma once
#include <iostream>
#include "UI/Window.h"
#include "MemoryAllocation/BumpAllocator.h"

namespace ExhibitEngine {
	
	Window window;

	class Application{
	public:
		Application();
		~Application();

		void run();
	private:

	};

}
