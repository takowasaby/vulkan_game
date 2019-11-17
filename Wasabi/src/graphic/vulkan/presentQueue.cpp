#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/presentQueue.h>

namespace wsb::graphic::vulkan {
	PresentQueue::PresentQueue(const LogicalDevice& device, uint32_t presentFamilyIndex)
		: QueueBase(device, presentFamilyIndex)
	{
	}

	bool PresentQueue::canUseAsPresentQueue(VkPhysicalDevice device, const Surface& surface, uint32_t index)
	{
		return surface.isPhysicalDeviceSupport(device, index);
	}
}