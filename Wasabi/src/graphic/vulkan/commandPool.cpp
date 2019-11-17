#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/commandPool.h>

namespace wsb::graphic::vulkan {
	CommandPool::CommandPool(const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices)
		: _device(device.getDeviceHandle())
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = indices.graphicsFamily.value();
		poolInfo.flags = 0;

		if (vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool!");
		}
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(_device, _commandPool, nullptr);
	}

	std::vector<VkCommandBuffer> CommandPool::allocCommandBuffers(uint32_t allocSize)
	{
		std::vector<VkCommandBuffer> commandBuffers(allocSize);

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = _commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(_device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		return commandBuffers;
	}
}