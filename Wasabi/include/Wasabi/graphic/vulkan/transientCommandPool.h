#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/logicalDevice.h>
#include <Wasabi/graphic/vulkan/queueFamilies.h>

#include <vector>
#include <cstdint>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class TransientCommandPool {
			public:
				TransientCommandPool(const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices);
				~TransientCommandPool();

				VkCommandBuffer allocCommandBuffer();
				void freeCommandBuffer(VkCommandBuffer commandBuffer);

			private:
				VkDevice _device;
				VkCommandPool _transientCommandPool;

			};
		}
	}
}
