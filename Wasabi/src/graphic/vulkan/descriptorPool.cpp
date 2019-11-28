#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/descriptorPool.h>

#include <array>

namespace wsb::graphic::vulkan {
	DescriptorPool::DescriptorPool(const LogicalDevice& device, uint32_t descriptorSize)
		: _device(device.getDeviceHandle())
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes = {};
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[1].descriptorCount = descriptorSize;
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = descriptorSize;

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = descriptorSize;

		if (vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
	}

	std::vector<VkDescriptorSet> DescriptorPool::allocDescriptorSets(uint32_t allocSize, VkDescriptorSetLayout layout)
	{
		std::vector<VkDescriptorSetLayout> layouts(allocSize, layout);
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = allocSize;
		allocInfo.pSetLayouts = layouts.data();

		std::vector<VkDescriptorSet> descriptorSets(allocSize);
		if (vkAllocateDescriptorSets(_device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor sets!");
		}

		return descriptorSets;
	}
}
