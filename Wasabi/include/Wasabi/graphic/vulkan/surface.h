#pragma once

#include <Wasabi/libdef/vulkan.h>
#include <Wasabi/libdef/glfw.h>

#include <vector>
#include <cstdint>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class Surface {
			public:
				struct SwapChainSupportDetails {
					VkSurfaceCapabilitiesKHR capabilities;
					std::vector<VkSurfaceFormatKHR> formats;
					std::vector<VkPresentModeKHR> presentModes;
				};

			public:
				Surface(VkInstance instance, GLFWwindow* window);
				~Surface();

				VkSurfaceKHR getSurfaceHandle();
				bool getPhysicalDeviceSupport(VkPhysicalDevice device, uint32_t index);
				SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

			private:
				const VkInstance& _instance;

				VkSurfaceKHR _surface;

			};
		}
	}
}
