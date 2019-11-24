#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/physicalDevice.h>
#include <Wasabi/graphic/vulkan/queueFamilies.h>

#include <vector>
#include <set>
#include <stdexcept>

namespace wsb::graphic::vulkan {
	PhysicalDevice::PhysicalDevice(const Instance& instance, const Surface& surface, const std::vector<const char*>& deviceExtensions)
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance.getInstanceHandle(), &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance.getInstanceHandle(), &deviceCount, devices.data());

		_physicalDevice = VK_NULL_HANDLE;
		for (const auto& device : devices) {
			if (isDeviceSuitable(device, surface, deviceExtensions)) {
				_physicalDevice = device;
				break;
			}
		}

		if (_physicalDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("failed to find a suitable GPU!");
		}
	}

	VkPhysicalDevice PhysicalDevice::getPhysicalDeviceHandle() const
	{
		return _physicalDevice;
	}

	uint32_t PhysicalDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device, const Surface& surface, const std::vector<const char*>& deviceExtensions)
	{
		QueueFamilies::QueueFamilyIndices indices = QueueFamilies::findQueueFamilies(device, surface);

		bool extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			Surface::SwapChainSupportDetails swapChainSupport = surface.querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return indices.isComplete() && extensionsSupported&& swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}
}