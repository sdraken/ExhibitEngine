#pragma once

#include "Logging/Logger.h"
#include "MemoryAllocation/BumpAllocator.h"
#include "UI/Window.h"
#include "MemoryAllocation/BumpAllocator.h"

namespace ExhibitEngine {
	
	Window window;
	Logger logger;

	class Application{
	public:
		Application();
		~Application();

		void run();
	private:

	};

}
