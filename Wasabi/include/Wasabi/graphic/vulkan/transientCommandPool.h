#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/iCommandPool.h>

#include <Wasabi/graphic/vulkan/logicalDevice.h>
#include <Wasabi/graphic/vulkan/queueFamilies.h>

#include <vector>
#include <cstdint>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class TransientCommandPool : public ICommandPool {
			public:
				TransientCommandPool(const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices);
				~TransientCommandPool();

				VkCommandBuffer allocCommandBuffer() override;
				void freeCommandBuffer(VkCommandBuffer commandBuffer) override;
				std::vector<VkCommandBuffer> allocCommandBuffers(uint32_t allocSize) override;

			private:
				VkDevice _device;
				VkCommandPool _transientCommandPool;

			};
		}
	}
}
