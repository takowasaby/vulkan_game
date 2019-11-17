#pragma once

#include <Wasabi/libdef/vulkan.h>
#include <Wasabi/graphic/vulkan/instance.h>
#include <Wasabi/graphic/vulkan/surface.h>
#include <Wasabi/graphic/vulkan/physicalDevice.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class LogicalDevice {
			public:
				LogicalDevice(const PhysicalDevice& physicalDevice, const Surface& surface, const std::vector<const char*>& deviceExtensions);
				~LogicalDevice();

				VkDevice getDeviceHandle() const;

			private:
				VkDevice _device;

			};
		}
	}
}
