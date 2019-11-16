#pragma once

#include <array>

class VkDebugUtilsMessengerEXT_T;
using VkDebugUtilsMessengerEXT = VkDebugUtilsMessengerEXT_T *;

namespace wsb {
	namespace graphic {
		namespace vulkan {
			constexpr std::array<const char*, 1> validationLayers = {
				"VK_LAYER_KHRONOS_validation"
			};

			class DebugTool {
			public:
				DebugTool(const VkInstance& instance);
				~DebugTool();
			public:
				static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
					VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
					VkDebugUtilsMessageTypeFlagsEXT messageType,
					const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
					void* pUserData);
				static bool checkValidationLayerSupport();
				static const VkDebugUtilsMessengerCreateInfoEXT& populateDebugMessengerCreateInfo();
			private:
				const VkInstance& _instance;

				VkDebugUtilsMessengerEXT _debugMessenger;
			};
		}
	}
}
