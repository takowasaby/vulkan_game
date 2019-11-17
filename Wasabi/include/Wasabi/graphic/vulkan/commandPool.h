#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/logicalDevice.h>
#include <Wasabi/graphic/vulkan/queueFamilies.h>

#include <vector>
#include <cstdint>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class CommandPool {
			public:
				CommandPool(const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices);
				~CommandPool();

				std::vector<VkCommandBuffer> allocCommandBuffers(uint32_t allocSize);

			private:
				VkDevice _device;
				VkCommandPool _commandPool;

			};
		}
	}
}
