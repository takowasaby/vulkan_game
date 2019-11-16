#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/surface.h>

#include <stdexcept>

namespace wsb::graphic::vulkan {
	Surface::Surface(const VkInstance& instance, GLFWwindow* window)
		: _instance(instance)
	{
		if (glfwCreateWindowSurface(_instance, window, nullptr, &_surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(_instance, _surface, nullptr);
	}

	VkSurfaceKHR Surface::getSurfaceHandle() const
	{
		return _surface;
	}

	bool Surface::isPhysicalDeviceSupport(VkPhysicalDevice device, uint32_t index) const
	{
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, index, _surface, &presentSupport);
		return static_cast<bool>(presentSupport);
	}

	Surface::SwapChainSupportDetails Surface::querySwapChainSupport(VkPhysicalDevice device) const
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}
}