#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/logicalDevice.h>

#include <Wasabi/graphic/vulkan/debugTool.h>
#include <Wasabi/graphic/vulkan/queueFamilies.h>

#include <stdexcept>

namespace wsb::graphic::vulkan {
	LogicalDevice::LogicalDevice(const PhysicalDevice& physicalDevice, const Surface& surface, const std::vector<const char*>& deviceExtensions)
		: _device(nullptr)
	{
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos = QueueFamilies::getQueueCreateInfos(physicalDevice.getPhysicalDeviceHandle(), surface);

		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

#ifdef _DEBUG
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
#else
		createInfo.enabledLayerCount = 0;
#endif

		if (vkCreateDevice(physicalDevice.getPhysicalDeviceHandle(), &createInfo, nullptr, &_device) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical deice!");
		}
	}

	LogicalDevice::~LogicalDevice()
	{
		vkDestroyDevice(_device, nullptr);
	}

	VkDevice LogicalDevice::getDeviceHandle() const
	{
		return _device;
	}
}