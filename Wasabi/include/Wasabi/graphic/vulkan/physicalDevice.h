#pragma once

#include <Wasabi/libdef/vulkan.h>
#include <Wasabi/graphic/vulkan/instance.h>
#include <Wasabi/graphic/vulkan/surface.h>

#include <optional>
#include <array>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class PhysicalDevice {
			public:
				PhysicalDevice(const Instance& instance, const Surface& surface, const std::vector<const char*>& deviceExtensions);

				VkPhysicalDevice getPhysicalDeviceHandle() const;
				VkPhysicalDeviceMemoryProperties getPhysicalDeviceMemoryProperties() const;
			
			private:
				static bool isDeviceSuitable(VkPhysicalDevice device, const Surface& surface, const std::vector<const char*>& deviceExtensions);
				static bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions);

			private:
				VkPhysicalDevice _physicalDevice;
			};
		}
	}
}
