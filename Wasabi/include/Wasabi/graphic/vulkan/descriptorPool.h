#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/logicalDevice.h>

#include <vector>
#include <cstdint>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class DescriptorPool {
			public:
				DescriptorPool(const LogicalDevice& device, uint32_t descriptorSize);
				~DescriptorPool();

				std::vector<VkDescriptorSet> allocDescriptorSets(uint32_t allocSize, VkDescriptorSetLayout layout);

			private:
				VkDevice _device;
				VkDescriptorPool _descriptorPool;

			};
		}
	}
}
