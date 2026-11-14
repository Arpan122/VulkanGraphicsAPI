#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>


namespace GraphicsEngine {
	class Window {
	public:
		Window(int width, int height, std::string title);
		~Window();

		Window(const Window &) = delete;
		Window &operator=(const Window &) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); };

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		GLFWwindow* window;
		const int width;
		const int height;
		std::string title;
			
		void initWindow();
	};
}