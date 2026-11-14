#include "Headers/app.hpp"

namespace GraphicsEngine {
	void App::run() {
		while (!window.shouldClose()) {
			glfwPollEvents();
		}
	}
}