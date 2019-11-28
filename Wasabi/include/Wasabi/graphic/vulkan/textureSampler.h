#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <Wasabi/graphic/vulkan/logicalDevice.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class TextureSampler {
			public:
				TextureSampler(const LogicalDevice& device);
				~TextureSampler();

				VkSampler getSamplerHandle() const;

			private:
				VkDevice _device;
				VkSampler _sampler;

			};
		}
	}
}