#pragma once

#include "window.hpp"
#include "Pipeline.hpp"

namespace GraphicsEngine {
	class App {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		Window window{WIDTH, HEIGHT, "Vulkan G-Engine"};
		Pipeline pipeline{ "src/Shaders/simple_shader.vert.spv", "src/Shaders/simple_shader.frag.spv" };
	};
}