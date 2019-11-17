#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/graphicQueue.h>

namespace wsb::graphic::vulkan {
	GraphicQueue::GraphicQueue(const LogicalDevice& device, uint32_t graphicsFamilyIndex)
	{
		vkGetDeviceQueue(device.getDeviceHandle(), graphicsFamilyIndex, 0, &_graphicQueue);
	}

	bool GraphicQueue::canUseAsGraphicQueue(VkQueueFlags flags)
	{
		return flags & VK_QUEUE_GRAPHICS_BIT;
	}
}