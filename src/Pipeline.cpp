#include "Headers/Pipeline.hpp"
#include <fstream>
#include <iostream>

namespace GraphicsEngine {
	Pipeline::Pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
		createGraphicsPipeline(vertexShaderPath, fragmentShaderPath);
	}

	std::vector<char> Pipeline::readFile(const std::string& filePath) {
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file: " + filePath);
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}

	void Pipeline::createGraphicsPipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
		auto vertexShaderCode = readFile(vertexShaderPath);
		auto fragmentShaderCode = readFile(fragmentShaderPath);

		std::cout << "Vertex Shader Code Size: " << vertexShaderCode.size() << " bytes" << std::endl;
		std::cout << "Fragment Shader Code Size: " << fragmentShaderCode.size() << " bytes" << std::endl;
	}
} 