#include "Headers/instance.hpp"
#include <GLFW/glfw3.h>

namespace GraphicsEngine {
	Instance::Instance(bool debug, const char* appName) {
		if (debug) {
			std::cout << "Creating Vulkan instance" << std::endl;
		}

		//Get Vulkan API version
		uint32_t version = 0;
		vkEnumerateInstanceVersion(&version);

		if (debug) {
			std::cout << "Vulkan version: "
				<< VK_VERSION_MAJOR(version) << "."
				<< VK_VERSION_MINOR(version) << "."
				<< VK_VERSION_PATCH(version) << std::endl;
		}

		version &= ~(0xFFFU);

		//Generate application info struct
		VkApplicationInfo appInfo{
			.pApplicationName = appName,
			.applicationVersion = version,
			.pEngineName = appName,
			.engineVersion = version,
			.apiVersion = version,
		};

		//Get required extensions from GLFW
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (debug) {
			std::cout << "Required extensions:" << std::endl;
			for (const char* extension : extensions) {
				std::cout << "\t\"" << extension << "\"\n";
			}
		}

		//Generate Validation layers
		std::vector<const char*> layers;

		if (!checkExtensionSupport(extensions, layers)) {
			throw std::runtime_error("Required extensions are not supported!");
		}

		//Generate instance creating info struct
		VkInstanceCreateInfo createInfo {
			.flags = VkInstanceCreateFlags(),
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = nullptr,
			.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
			.ppEnabledExtensionNames = extensions.data(),
		};
		
		//Create Vulkan instance
		try {
			vkCreateInstance(&createInfo, nullptr, &instance);
			if (debug) {
				std::cout << "Vulkan instance created successfully" << std::endl;
			}
		}
		catch (const std::exception& e) {
			if (debug)	throw std::runtime_error("Failed to create Vulkan instance: " + std::string(e.what()));
		}
	}

	Instance::~Instance() {
		vkDestroyInstance(instance, nullptr);
	}

	bool Instance::checkExtensionSupport(const std::vector<const char*>& extensions, const std::vector<const char*>& layers) {
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> supportedExtensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensions.data());

		std::cout << "Supported extensions:" << std::endl;
		for (const VkExtensionProperties& supportedExtension : supportedExtensions) {
			std::cout << "\t\"" << supportedExtension.extensionName << "\"\n";
		}
		/*
		for (const char* extension : extensions) {
			bool found = false;
			for (const VkExtensionProperties& supportedExtension : supportedExtensions) {
				
				if (strcmp(extension, supportedExtension.extensionName) == 0) {
					found = true;
					break;
				}
			}
			if (!found) {
				return false;
			}
		}*/
		return true;
	}
}