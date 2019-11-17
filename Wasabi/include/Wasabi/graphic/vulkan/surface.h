#pragma once

#include <Wasabi/libdef/vulkan.h>
#include <Wasabi/libdef/glfw.h>

#include <Wasabi/graphic/vulkan/instance.h>

#include <vector>
#include <memory>
#include <cstdint>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class Surface {
			public:
				struct SwapChainSupportDetails {
					std::shared_ptr<VkSurfaceCapabilitiesKHR> capabilities;
					std::vector<VkSurfaceFormatKHR> formats;
					std::vector<VkPresentModeKHR> presentModes;
					SwapChainSupportDetails();
				};

			public:
				Surface(const Instance& instance, GLFWwindow* window);
				~Surface();

				VkSurfaceKHR getSurfaceHandle() const;
				bool isPhysicalDeviceSupport(VkPhysicalDevice device, uint32_t index) const;
				SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;

			private:
				VkInstance _instance;
				VkSurfaceKHR _surface;

			};
		}
	}
}
