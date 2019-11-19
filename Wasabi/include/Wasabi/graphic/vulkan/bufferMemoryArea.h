#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/swapChain.h>
#include <Wasabi/graphic/vulkan/graphicRender.h>

#include <Wasabi/graphic/vulkan/commandPool.h>
#include <Wasabi/graphic/vulkan/transientCommandPool.h>
#include <Wasabi/graphic/vulkan/descriptorPool.h>

#include <Wasabi/graphic/vulkan/vertex.h>
#include <Wasabi/graphic/vulkan/uniformBufferObject.h>

#include <vector>
#include <memory>
#include <cstdint>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class BufferMemoryArea {
			public:
				static VkDescriptorSetLayout createDescriptorSetLayout(const LogicalDevice& device);

			public:
				BufferMemoryArea(const PhysicalDevice& physicalDevice, const GraphicRender& render, const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices, const SwapChain& swapChain);
				~BufferMemoryArea();

				void createBuffersForRendering(const QueueFamilies& queueFamilies, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const SwapChain& swapChain, const GraphicRender& render);
				void updateUniformBuffer(uint32_t currentImage, const UniformBufferObject& ubo);

				VkCommandBuffer getCommandBufferHandle(uint32_t imageIndex) const;

				void updateSwapChainInfo(const PhysicalDevice& physicalDevice, const GraphicRender& render, const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices, const SwapChain& swapChain);

			private:
				void createVertexBuffer(const QueueFamilies& queueFamilies, const std::vector<Vertex>& vertices);
				void createIndexBuffer(const QueueFamilies& queueFamilies, std::vector<uint16_t> indices);
				void createUniformBuffers(const SwapChain& swapChain);

				void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
				uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
				void copyBuffer(const QueueFamilies& queueFamilies, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

				void createDescriptorSets(const SwapChain& swapChain, const GraphicRender& render);
				void createCommandBuffers(size_t indicesSize, const SwapChain& swapChain, const GraphicRender& render);

			private:
				VkDevice _device;

				CommandPool _commandPool;
				TransientCommandPool _transientCommandPool;
				std::unique_ptr<DescriptorPool> _descriptorPool;

				std::vector<VkFramebuffer> _swapChainFramebuffers;

				std::unique_ptr<VkPhysicalDeviceMemoryProperties> _deviceMemoryProperity;

				VkBuffer _vertexBuffer;
				VkDeviceMemory _vertexBufferMemory;

				VkBuffer _indexBuffer;
				VkDeviceMemory _indexBufferMemory;

				std::vector<VkBuffer> _uniformBuffers;
				std::vector<VkDeviceMemory> _uniformBufferMemories;

				std::vector<VkDescriptorSet> _descriptorSets;

				std::vector<VkCommandBuffer> _commandBuffers;

				size_t indicisSizeCache;

			};
		}
	}
}
