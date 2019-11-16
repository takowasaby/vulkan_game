#pragma once

#include <vector>
#include <cstdint>

#include <Wasabi/libdef/vulkan.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class IImageDrawTarget {
			public:
				virtual ~IImageDrawTarget() = default;
				virtual size_t getNumberOfImage() = 0;
				virtual VkImageView getImageView(size_t index) = 0;
				virtual const VkPresentInfoKHR& getPresentInfo(uint32_t index, std::vector<VkSemaphore>& signalSemaphores) = 0;
			};
		}
	}
}