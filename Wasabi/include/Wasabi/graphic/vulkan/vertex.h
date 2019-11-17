#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <array>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			struct Vertex {
				alignas(8) float pos[3];
				alignas(16) float color[4];

				static VkVertexInputBindingDescription getBindingDescription();
				static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
			};
		}
	}
}