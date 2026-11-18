#include "Headers/engine.hpp"
#include <iostream>


int main() {
	GraphicsEngine::Engine Engine{};
	try {
		Engine.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	 
	return 0;
}