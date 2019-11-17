#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/surface.h>
#include <Wasabi/graphic/vulkan/logicalDevice.h>

#include <cstdint>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class GraphicQueue {
			public:
				GraphicQueue(const LogicalDevice& device, uint32_t graphicsFamilyIndex);

			public:
				static bool canUseAsGraphicQueue(VkQueueFlags flags);

			private:
				VkQueue _graphicQueue;

			};
		}
	}
}
