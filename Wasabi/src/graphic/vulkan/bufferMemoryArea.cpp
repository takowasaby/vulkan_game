#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/bufferMemoryArea.h>

#include <Wasabi/graphic/vulkan/uniformBufferObject.h>

namespace wsb::graphic::vulkan {
	VkDescriptorSetLayout BufferMemoryArea::createDescriptorSetLayout(const LogicalDevice& device)
	{
		VkDescriptorSetLayout descriptorSetLayout;

		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = 1;
		layoutInfo.pBindings = &uboLayoutBinding;

		if (vkCreateDescriptorSetLayout(device.getDeviceHandle(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}

		return descriptorSetLayout;
	}

	BufferMemoryArea::BufferMemoryArea(const PhysicalDevice& physicalDevice, const GraphicRender& render, const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices, const SwapChain& swapChain)
		: _device(device.getDeviceHandle())
		, _commandPool(device, indices)
		, _transientCommandPool(device, indices)
		, _descriptorPool(device, swapChain.getImageSize())
		, _swapChainFramebuffers(render.createFrameBufferFromSwapChain(swapChain))
		, _deviceMemoryProperity(std::make_unique<VkPhysicalDeviceMemoryProperties>(physicalDevice.getPhysicalDeviceMemoryProperties()))
	{
	}

	BufferMemoryArea::~BufferMemoryArea()
	{
		for (size_t i = 0; i < _swapChainFramebuffers.size(); i++) {
			vkDestroyFramebuffer(_device, _swapChainFramebuffers[i], nullptr);
		}

		vkDestroyBuffer(_device, _indexBuffer, nullptr);
		vkFreeMemory(_device, _indexBufferMemory, nullptr);

		vkDestroyBuffer(_device, _vertexBuffer, nullptr);
		vkFreeMemory(_device, _vertexBufferMemory, nullptr);

		for (size_t i = 0; i < _uniformBuffers.size(); i++) {
			vkDestroyBuffer(_device, _uniformBuffers[i], nullptr);
			vkFreeMemory(_device, _uniformBufferMemories[i], nullptr);
		}
	}

	void BufferMemoryArea::CreateBuffersForRendering(const QueueFamilies& queueFamilies, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const SwapChain& swapChain, const GraphicRender& render)
	{
		createVertexBuffer(queueFamilies, vertices);
		createIndexBuffer(queueFamilies, indices);
		createUniformBuffers(swapChain);
	}

	void BufferMemoryArea::createVertexBuffer(const QueueFamilies& queueFamilies, const std::vector<Vertex>& vertices) {
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBufferUsageFlags stagingBufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		VkMemoryPropertyFlags stagingBufferProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		VkBufferUsageFlags vertexBufferUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		VkMemoryPropertyFlags vertexBufferProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;


		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, stagingBufferUsage, stagingBufferProperties, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
		std::memcpy(data, vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(_device, stagingBufferMemory);

		createBuffer(bufferSize, vertexBufferUsage, vertexBufferProperties, _vertexBuffer, _vertexBufferMemory);

		copyBuffer(queueFamilies, stagingBuffer, _vertexBuffer, bufferSize);

		vkDestroyBuffer(_device, stagingBuffer, nullptr);
		vkFreeMemory(_device, stagingBufferMemory, nullptr);
	}

	void BufferMemoryArea::createIndexBuffer(const QueueFamilies& queueFamilies, std::vector<uint16_t> indices) {
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBufferUsageFlags stagingBufferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		VkMemoryPropertyFlags stagingBufferProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		VkBufferUsageFlags indexBufferUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		VkMemoryPropertyFlags indexBufferProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, stagingBufferUsage, stagingBufferProperties, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
		std::memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(_device, stagingBufferMemory);

		createBuffer(bufferSize, indexBufferUsage, indexBufferProperties, _indexBuffer, _indexBufferMemory);

		copyBuffer(queueFamilies, stagingBuffer, _indexBuffer, bufferSize);

		vkDestroyBuffer(_device, stagingBuffer, nullptr);
		vkFreeMemory(_device, stagingBufferMemory, nullptr);
	}

	void BufferMemoryArea::createUniformBuffers(const SwapChain& swapChain) {
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		_uniformBuffers.resize(swapChain.getImageSize());
		_uniformBufferMemories.resize(swapChain.getImageSize());

		for (size_t i = 0; i < swapChain.getImageSize(); i++) {
			createBuffer(
				bufferSize,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				_uniformBuffers[i],
				_uniformBufferMemories[i]);
		}
	}

	void BufferMemoryArea::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(_device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vertex buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(_device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(_device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(_device, buffer, bufferMemory, 0);
	}

	uint32_t BufferMemoryArea::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		for (uint32_t i = 0; i < _deviceMemoryProperity->memoryTypeCount; i++) {
			if (typeFilter & (1 << i) && (_deviceMemoryProperity->memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	void BufferMemoryArea::copyBuffer(const QueueFamilies& queueFamilies, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBuffer commandBuffer = _transientCommandPool.allocCommandBuffer();

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		queueFamilies.submitGraphicQueue(1, &submitInfo, VK_NULL_HANDLE);
		queueFamilies.waitGraphicQueueIdle();

		_transientCommandPool.freeCommandBuffer(commandBuffer);
	}

	void BufferMemoryArea::createDescriptorSets(const SwapChain& swapChain, const GraphicRender& render) {
		_descriptorSets = _descriptorPool.allocDescriptorSets(static_cast<uint32_t>(swapChain.getImageSize()), render.getDescriptorSetLayout());

		for (size_t i = 0; i < _descriptorSets.size(); i++) {
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = _uniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = _descriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr;
			descriptorWrite.pTexelBufferView = nullptr;
			vkUpdateDescriptorSets(_device, 1, &descriptorWrite, 0, nullptr);
		}
	}

	void BufferMemoryArea::createCommandBuffers(const std::vector<uint16_t>& indices, const SwapChain& swapChain, const GraphicRender& render) {
		_commandBuffers = _commandPool.allocCommandBuffers(static_cast<uint32_t>(_swapChainFramebuffers.size()));

		for (size_t i = 0; i < _commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0;
			beginInfo.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassinfo = {};
			renderPassinfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassinfo.renderPass = render.getRenderPassHandle();
			renderPassinfo.framebuffer = _swapChainFramebuffers[i];

			renderPassinfo.renderArea.offset = { 0, 0 };
			renderPassinfo.renderArea.extent = swapChain.getExtent();

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassinfo.clearValueCount = 1;
			renderPassinfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(_commandBuffers[i], &renderPassinfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, render.getPipelineHandle());

			std::array<VkBuffer, 1> vertexBuffers = { _vertexBuffer };
			std::array<VkDeviceSize, 1> offsets = { 0 };
			vkCmdBindVertexBuffers(_commandBuffers[i], 0, static_cast<uint32_t>(vertexBuffers.size()), vertexBuffers.data(), offsets.data());
			vkCmdBindIndexBuffer(_commandBuffers[i], _indexBuffer, 0, VK_INDEX_TYPE_UINT16);

			vkCmdBindDescriptorSets(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, render.getPipelineLayoutHandle(), 0, 1, &_descriptorSets[i], 0, nullptr);
			vkCmdDrawIndexed(_commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
			vkCmdEndRenderPass(_commandBuffers[i]);
			if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	}
}
