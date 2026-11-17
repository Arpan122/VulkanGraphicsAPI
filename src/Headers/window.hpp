#pragma once

#include "config.hpp"
#include <GLFW/glfw3.h>


namespace GraphicsEngine {
	class Window {
	public:
		//Constructor and Destructor
		Window(int width, int height, std::string title);
		~Window();

		//Prevent copying and assignment
		Window(const Window &) = delete;
		Window &operator=(const Window &) = delete;

		//Check to close
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