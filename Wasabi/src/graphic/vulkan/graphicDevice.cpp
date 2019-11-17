#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/graphicDevice.h>

#include <Wasabi/graphic/vulkan/queueFamilies.h>

namespace wsb::graphic::vulkan {
	GraphicDevice::GraphicDevice(GLFWwindow* window, const Instance& instance, const Surface& surface)
		: _deviceExtensions({
				VK_KHR_SWAPCHAIN_EXTENSION_NAME
			})
		, _physicalDevice(instance, surface, _deviceExtensions)
		, _logicalDevice(_physicalDevice, surface, _deviceExtensions)
		, _queueFamilies(_logicalDevice, QueueFamilies::findQueueFamilies(_physicalDevice.getPhysicalDeviceHandle(), surface))
		, _swapChain(window, surface, _physicalDevice, _logicalDevice)
	{
	}
}