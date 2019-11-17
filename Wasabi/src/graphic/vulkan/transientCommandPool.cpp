#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/transientCommandPool.h>

namespace wsb::graphic::vulkan {
	TransientCommandPool::TransientCommandPool(const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices)
		: _device(device.getDeviceHandle())
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

		if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_transientCommandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool!");
		}
	}

	TransientCommandPool::~TransientCommandPool()
	{
		vkDestroyCommandPool(_device, _transientCommandPool, nullptr);
	}

	VkCommandBuffer TransientCommandPool::allocCommandBuffer()
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = _transientCommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);
		return commandBuffer;
	}

	void TransientCommandPool::freeCommandBuffer(VkCommandBuffer commandBuffer)
	{
		vkFreeCommandBuffers(_device, _transientCommandPool, 1, &commandBuffer);
	}
}