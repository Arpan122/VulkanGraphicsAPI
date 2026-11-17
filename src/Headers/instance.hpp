#pragma once

#include "config.hpp"

namespace GraphicsEngine {
	class Instance {
	public:
		Instance(bool debug, const char* appName);
		~Instance();
		//Prevent copying and assignment
		Instance(const Instance&) = delete;
		Instance& operator=(const Instance&) = delete;

	private:
		VkInstance instance;
		
		bool checkExtensionSupport(const std::vector<const char*>& extensions, const std::vector<const char*>& layers);
	};
}