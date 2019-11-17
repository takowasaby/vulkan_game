#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/graphicDevice.h>

namespace wsb::graphic::vulkan {
	GraphicDevice::GraphicDevice(const Instance& instance, const Surface& surface)
		: _physicalDevice(instance, surface)
	{
	}
}