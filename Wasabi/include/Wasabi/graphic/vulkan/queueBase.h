#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/surface.h>
#include <Wasabi/graphic/vulkan/logicalDevice.h>

#include <cstdint>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class QueueBase {
			public:
				QueueBase(const LogicalDevice& device, uint32_t presentFamilyIndex);
				virtual ~QueueBase() = default;

				VkQueue getQueueHandle() const;
				void submit(uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence) const;
				void waitIdle() const;

			private:
				VkQueue _queue;

			};
		}
	}
}
