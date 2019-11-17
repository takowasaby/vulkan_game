#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/graphicDevice.h>

namespace wsb::graphic::vulkan {
	GraphicDevice::GraphicDevice(const Instance& instance, const Surface& surface)
		: _deviceExtensions({
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			})
		, _physicalDevice(instance, surface, _deviceExtensions)
	{
	}
}