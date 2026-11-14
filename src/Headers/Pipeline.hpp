#pragma once
#include <string>
#include <vector>

namespace GraphicsEngine {
	class Pipeline {
	public:
		Pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

	private:
		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	};
}