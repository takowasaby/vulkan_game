#pragma once
#include <glm/glm.hpp>

#include <Wasabi/libdef/vulkan.h>

#include <array>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			struct UniformBufferObject {
				alignas(16) glm::mat4 model;
				alignas(16) glm::mat4 view;
				alignas(16) glm::mat4 proj;
			};

			constexpr size_t getUniformBufferObjectSize();
		}
	}
}