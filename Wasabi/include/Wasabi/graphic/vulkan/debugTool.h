#pragma once

#include <array>

#include <Wasabi/graphic/vulkan/IImageDrawTarget.h>

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
				static VkDebugUtilsMessengerCreateInfoEXT populateDebugMessengerCreateInfo();
			private:
				const VkInstance& _instance;

				VkDebugUtilsMessengerEXT _debugMessenger;
			};
		}
	}
}
