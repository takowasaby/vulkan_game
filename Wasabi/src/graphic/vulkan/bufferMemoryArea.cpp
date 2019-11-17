#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/bufferMemoryArea.h>

#include <Wasabi/graphic/vulkan/queueFamilies.h>

namespace wsb::graphic::vulkan {
	BufferMemoryArea::BufferMemoryArea(const Surface& surface, const PhysicalDevice& physicalDevice, const LogicalDevice& device, const SwapChain& swapChain)
		: _commandPool()
		, _transientCommandPool()
		, _descriptorPool(device, swapChain.getImageSize())
	{
	}
}
