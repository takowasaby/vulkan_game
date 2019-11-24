#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <vector>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class ICommandPool {
			public:
				virtual ~ICommandPool() = default;

				virtual VkCommandBuffer allocCommandBuffer() = 0;
				virtual void freeCommandBuffer(VkCommandBuffer commandBuffer) = 0;
				virtual std::vector<VkCommandBuffer> allocCommandBuffers(uint32_t allocSize) = 0;
			};
		}
	}
}