#pragma once
#include <glm/glm.hpp>

#include <Wasabi/libdef/vulkan.h>

#include <array>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			struct Vertex {
				alignas(8) glm::vec2 pos;
				alignas(16) glm::vec4 color;
			};

			VkVertexInputBindingDescription getVertexBindingDescription();
			std::array<VkVertexInputAttributeDescription, 2> getVertexAttributeDescriptions();
		}
	}
}