#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/surface.h>
#include <Wasabi/graphic/vulkan/logicalDevice.h>
#include <Wasabi/graphic/vulkan/queueBase.h>

#include <cstdint>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class PresentQueue : public QueueBase {
			public:
				PresentQueue(const LogicalDevice& device, uint32_t presentFamilyIndex);

			public:
				static bool canUseAsPresentQueue(VkPhysicalDevice device, const Surface& surface, uint32_t index);

			};
		}
	}
}
