#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/uniformBufferObject.h>

namespace wsb::graphic::vulkan {
	constexpr size_t vulkan::getUniformBufferObjectSize()
	{
		return sizeof(UniformBufferObject);
	}
}