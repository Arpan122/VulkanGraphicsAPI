#include "Headers/engine.hpp"

namespace GraphicsEngine {
	Engine::Engine() {
		dldi.init(vkGetInstanceProcAddr);
		buildGlfwWindow();
		buildVulkanInstance();
		setupDebugMessenger();
	}

	Engine::~Engine() {
		if (debug && debugMessenger) {
			instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
		}
		if (instance) {
			instance.destroy(nullptr, dldi);
			instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldi);
		}
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	//Main run loop
	void Engine::run() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	//Build GLFW window
	void Engine::buildGlfwWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(WIDTH, HEIGHT, APP_NAME, nullptr, nullptr);
	}

	//Check extension and layer support
	bool Engine::checkSupport(const std::vector<const char*>& extensions, const std::vector<const char*>& layers) {
		const auto supportedExtensions = vk::enumerateInstanceExtensionProperties(nullptr, dldi);

		if (debug) {
			std::cout << "Supported extensions:" << std::endl;
			for (const vk::ExtensionProperties& supportedExtension : supportedExtensions) {
				std::cout << "\t\"" << supportedExtension.extensionName << "\"\n";
			}
		}

		for (const char* extension : extensions) {
			bool found = false;
			for (const vk::ExtensionProperties& supportedExtension : supportedExtensions) {
				if (std::string(extension) == supportedExtension.extensionName) {
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
		const auto supportedLayers = vk::enumerateInstanceLayerProperties(dldi);

		if (debug) {
			std::cout << "Supported layers:" << std::endl;
			for (const vk::LayerProperties& supportedLayer : supportedLayers) {
				std::cout << "\t\"" << supportedLayer.layerName << "\"\n";
			}
		}

		// Check if required layers are supported
		for (const char* layer : layers) {
			bool found = false;
			for (const vk::LayerProperties& supportedLayer : supportedLayers) {
				if (std::string(layer) == supportedLayer.layerName) {
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

	//Build Vulkan instance object
	void Engine::buildVulkanInstance() {
		if (debug) {
			std::cout << "Creating Vulkan instance" << std::endl;
		}

		//Get Vulkan API version
		uint32_t version = vk::enumerateInstanceVersion(dldi);

		if (debug) {
			std::cout << "Vulkan version: "
				<< vk::apiVersionMajor(version) << "."
				<< vk::apiVersionMinor(version) << "."
				<< vk::apiVersionPatch(version) << std::endl;
		}

		version &= ~(0xFFFU);

		//Generate application info struct
		vk::ApplicationInfo appInfo(
			APP_NAME,
			version,
			APP_NAME,
			version,
			version
		);

		//Get required extensions from GLFW
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (debug) {
			extensions.push_back(vk::EXTDebugUtilsExtensionName);
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

		if (!checkSupport(extensions, layers)) {
			throw std::runtime_error("Required extensions or layers are not supported!");
		}

		//Generate instance creating info struct
		vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
			vk::InstanceCreateFlags(),
			&appInfo,
			static_cast<uint32_t>(layers.size()),
			layers.data(),
			static_cast<uint32_t>(extensions.size()),
			extensions.data()
		);

		//Create Vulkan instance
		instance = vk::createInstance(createInfo, nullptr, dldi);
		dldi.init(instance);
		if (debug) {
			std::cout << "Vulkan instance created successfully" << std::endl;
		}
	}

	//Debug callback function
	VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
		vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		vk::DebugUtilsMessageTypeFlagsEXT messageType,
		const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	) {
		std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
		return vk::Bool32{ false };
	}

	//Setup debug messenger
	void Engine::setupDebugMessenger() {
		vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
			vk::DebugUtilsMessengerCreateFlagsEXT(),
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
			debugCallback,
			nullptr
		);

		debugMessenger = instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
	}
}