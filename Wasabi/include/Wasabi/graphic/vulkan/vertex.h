#pragma once

#include <Wasabi/libdef/vulkan.h>

#include <array>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			struct Vertex;

			VkVertexInputBindingDescription getVertexBindingDescription();
			std::array<VkVertexInputAttributeDescription, 2> getVertexAttributeDescriptions();
		}
	}
}