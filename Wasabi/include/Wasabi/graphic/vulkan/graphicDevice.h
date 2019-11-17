#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/instance.h>
#include <Wasabi/graphic/vulkan/surface.h>
#include <Wasabi/graphic/vulkan/physicalDevice.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class GraphicDevice {
			public:
				GraphicDevice(const Instance& instance, const Surface& surface);
			private:
				std::vector<const char*> _deviceExtensions;

				PhysicalDevice _physicalDevice;
			};
		}
	}
}
