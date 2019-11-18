#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Wasabi/graphic/vulkan/uniformBufferObject.h>

namespace wsb::graphic::vulkan {
	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	constexpr size_t vulkan::getUniformBufferObjectSize()
	{
		return sizeof(UniformBufferObject);
	}
}