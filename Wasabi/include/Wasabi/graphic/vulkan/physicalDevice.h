#pragma once

#include <Wasabi/libdef/vulkan.h>
#include <Wasabi/graphic/vulkan/surface.h>

#include <optional>
#include <array>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			constexpr std::array<const char*, 1> deviceExtensions = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			};

			class PhysicalDevice {
			public:
				struct QueueFamilyIndices {
					std::optional<uint32_t> graphicsFamily;
					std::optional<uint32_t> presentFamily;

					bool isComplete(); 
				};

			public:
				PhysicalDevice(const VkInstance& instance, const Surface& surface);

				VkPhysicalDevice getPhysicalDeviceHandle() const;
				uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
			
			private:
				static bool isDeviceSuitable(VkPhysicalDevice device, const Surface& surface);
				static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, const Surface& surface);
				static bool checkDeviceExtensionSupport(VkPhysicalDevice device);

			private:
				const VkInstance& _instance;

				VkPhysicalDevice _physicalDevice;
			};
		}
	}
}
