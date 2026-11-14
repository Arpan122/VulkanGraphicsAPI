#pragma once

#include <GLFW/glfw3.h>
#include <string>

#define GLFW_INCLUDE_VULKAN

namespace GraphicsEngine {
	class Window {
	public:
		Window(int width, int height, std::string title);
		~Window();
		bool shouldClose() { return glfwWindowShouldClose(window); };

	private:
		GLFWwindow* window;
		const int width;
		const int height;
		std::string title;
			
		void initWindow();
	};
}