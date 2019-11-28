#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/swapChain.h>
#include <Wasabi/graphic/vulkan/graphicRender.h>

#include <Wasabi/graphic/vulkan/commandPool.h>
#include <Wasabi/graphic/vulkan/transientCommandPool.h>
#include <Wasabi/graphic/vulkan/descriptorPool.h>

#include <Wasabi/graphic/vulkan/vertex.h>
#include <Wasabi/graphic/vulkan/uniformBufferObject.h>

#include <Wasabi/graphic/vulkan/textureSampler.h>
#include <Wasabi/graphic/vulkan/textureImageLoader.h>

#include <vector>
#include <memory>
#include <cstdint>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class TextureImageLoader;

			class BufferMemoryArea {
			public:
				static VkDescriptorSetLayout createDescriptorSetLayout(const LogicalDevice& device);
				static void createBuffer(const PhysicalDevice& physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

			public:
				BufferMemoryArea(const GraphicRender& render, const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices, const SwapChain& swapChain);
				~BufferMemoryArea();

				void createBuffersForRendering(const PhysicalDevice& physicalDevice, const QueueFamilies& queueFamilies, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const SwapChain& swapChain, const GraphicRender& render, const TextureImageLoader& imageLoader, const TextureSampler& sampler);
				void updateVertexBuffer(const PhysicalDevice& physicalDevice, const QueueFamilies& queueFamilies, const std::vector<Vertex>& vertices);
				void updateUniformBuffer(const PhysicalDevice& physicalDevice, uint32_t currentImage, const UniformBufferObject& ubo);

				VkCommandBuffer getCommandBufferHandle(uint32_t imageIndex) const;

				VkCommandBuffer beginSingleTimeCommands() const;
				void endSingleTimeCommands(const QueueFamilies& queueFamilies, VkCommandBuffer commandBuffer) const;

				void updateSwapChainInfo(const PhysicalDevice& physicalDevice, const GraphicRender& render, const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices, const SwapChain& swapChain, const TextureImageLoader& imageLoader, const TextureSampler& sampler);

			private:
				void createVertexBuffer(const PhysicalDevice& physicalDevice, const QueueFamilies& queueFamilies, const std::vector<Vertex>& vertices);
				void createIndexBuffer(const PhysicalDevice& physicalDevice, const QueueFamilies& queueFamilies, std::vector<uint16_t> indices);
				void createUniformBuffers(const PhysicalDevice& physicalDevice, const SwapChain& swapChain);

				VkCommandBuffer beginSingleTimeCommands(const std::unique_ptr<ICommandPool>& commandPool) const;
				void endSingleTimeCommands(const std::unique_ptr<ICommandPool>& commandPool, const QueueFamilies& queueFamilies, VkCommandBuffer commandBuffer) const;
				void copyBuffer(const QueueFamilies& queueFamilies, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

				void createDescriptorSets(const SwapChain& swapChain, const GraphicRender& render, const TextureImageLoader& imageLoader, const TextureSampler& sampler);
				void createCommandBuffers(size_t indicesSize, const SwapChain& swapChain, const GraphicRender& render);

			private:
				VkDevice _device;

				std::unique_ptr<ICommandPool> _commandPool;
				std::unique_ptr<ICommandPool> _transientCommandPool;
				std::unique_ptr<DescriptorPool> _descriptorPool;

				std::vector<VkFramebuffer> _swapChainFramebuffers;

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
