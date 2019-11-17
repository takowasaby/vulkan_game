#pragma once

#include <array>

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/instance.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			constexpr std::array<const char*, 1> validationLayers = {
				"VK_LAYER_KHRONOS_validation"
			};

			class DebugTool {
			public:
				DebugTool(const Instance& instance);
				~DebugTool();
			public:
				static bool checkValidationLayerSupport();
				static VkDebugUtilsMessengerCreateInfoEXT populateDebugMessengerCreateInfo();
			private:
				VkInstance _instance;
				VkDebugUtilsMessengerEXT _debugMessenger;
			};
		}
	}
}
