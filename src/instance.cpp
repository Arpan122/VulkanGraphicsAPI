#include "Headers/instance.hpp"
#include <GLFW/glfw3.h>
#include <cstring>

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
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		if (debug) {
			std::cout << "Required extensions:" << std::endl;
			for (const char* extension : extensions) {
				std::cout << "\t\"" << extension << "\"\n";
			}
		}

		//Generate Validation layers
		std::vector<const char*> layers;
		if (debug) {
			layers.push_back("VK_LAYER_KHRONOS_validation");
		}

		if (!checkSupport(extensions, layers, debug)) {
			throw std::runtime_error("Required extensions or layers are not supported!");
		}

		//Generate instance creating info struct
		VkInstanceCreateInfo createInfo {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.flags = VkInstanceCreateFlags(),
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = static_cast<uint32_t>(layers.size()),
			.ppEnabledLayerNames = layers.data(),
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

	bool Instance::checkSupport(const std::vector<const char*>& extensions, const std::vector<const char*>& layers, bool debug) {
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> supportedExtensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, supportedExtensions.data());

		if (debug) {
			std::cout << "Supported extensions:" << std::endl;
			for (const VkExtensionProperties& supportedExtension : supportedExtensions) {
				std::cout << "\t\"" << supportedExtension.extensionName << "\"\n";
			}
		}
		
		for (const char* extension : extensions) {
			bool found = false;
			for (const VkExtensionProperties& supportedExtension : supportedExtensions) {
				if (strcmp(extension, supportedExtension.extensionName) == 0) {
					found = true;
					if (debug) {
						std::cout << "Extension \"" << extension << "\" is supported." << std::endl;
					}
					break;
				}
			}
			if (!found) {
				if (debug) {
					std::cout << "Extension \"" << extension << "\" is not supported." << std::endl;
				}
				return false;
			}
		}

		// Retrieve supported layers
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> supportedLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, supportedLayers.data());
		
		if (debug) {
			std::cout << "Supported layers:" << std::endl;
			for (const VkLayerProperties& supportedLayer : supportedLayers) {
				std::cout << "\t\"" << supportedLayer.layerName << "\"\n";
			}
		}

		// Check if required layers are supported
		for (const char* layer : layers) {
			bool found = false;
			for (const VkLayerProperties& supportedLayer : supportedLayers) {
				if (strcmp(layer, supportedLayer.layerName) == 0) {
					found = true;
					if (debug) {
						std::cout << "Layer \"" << layer << "\" is supported." << std::endl;
					}
					break;
				}
			}
			if (!found) {
				if (debug) {
					std::cout << "Layer \"" << layer << "\" is not supported." << std::endl;
				}
				return false;
			}
		}

		return true;
	}
}