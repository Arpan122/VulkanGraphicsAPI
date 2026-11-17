#pragma once

#include "window.hpp"
#include "Pipeline.hpp"
#include "instance.hpp"

namespace GraphicsEngine {
	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		static constexpr const char* APP_NAME = "Vulkan Engine";

		void run();

	private:
		std::string title{ APP_NAME };
		Window window{ WIDTH, HEIGHT, title };
		Pipeline pipeline{ "src/Shaders/simple_shader.vert.spv", "src/Shaders/simple_shader.frag.spv" };
		Instance instance{ true, APP_NAME };
		VkDebugUtilsMessengerEXT debugMessenger{ nullptr };
	};
}