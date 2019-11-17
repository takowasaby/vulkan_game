#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/queueFamilies.h>
#include <Wasabi/graphic/vulkan/logicalDevice.h>
#include <Wasabi/graphic/vulkan/swapChain.h>

#include <Wasabi/graphic/vulkan/commandPool.h>
#include <Wasabi/graphic/vulkan/transientCommandPool.h>
#include <Wasabi/graphic/vulkan/descriptorPool.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class BufferMemoryArea {
			public:
				BufferMemoryArea(const LogicalDevice& device, QueueFamilies::QueueFamilyIndices indices, const SwapChain& swapChain);

			private:
				CommandPool _commandPool;
				TransientCommandPool _transientCommandPool;
				DescriptorPool _descriptorPool;

			};
		}
	}
}
