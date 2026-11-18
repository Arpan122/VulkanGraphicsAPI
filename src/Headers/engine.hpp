#pragma once

#include "config.hpp"

namespace GraphicsEngine {
	class Engine {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		static constexpr const char* APP_NAME = "Vulkan Engine";

		void run();
		
		Engine();
		~Engine();

	private:
		std::string title{ APP_NAME };
		bool debug = true;

		GLFWwindow* window { nullptr };
		vk::detail::DispatchLoaderDynamic dldi;
		vk::Instance instance{};
		vk::DebugUtilsMessengerEXT debugMessenger{};

		void buildGlfwWindow();
		void buildVulkanInstance();
		void setupDebugMessenger();

		bool checkSupport(const std::vector<const char*>& extensions, const std::vector<const char*>& layers);
	};
}