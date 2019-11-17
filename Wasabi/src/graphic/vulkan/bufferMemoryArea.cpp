#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/bufferMemoryArea.h>

namespace wsb::graphic::vulkan {
	BufferMemoryArea::BufferMemoryArea(const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices, const SwapChain& swapChain)
		: _commandPool(device, indices)
		, _transientCommandPool(device, indices)
		, _descriptorPool(device, swapChain.getImageSize())
	{
	}
}
