#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Wasabi/graphic/vulkan/graphicEngine.h>

namespace wsb::graphic::vulkan {
	void GraphicEngine::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto engine = reinterpret_cast<GraphicEngine*>(glfwGetWindowUserPointer(window));
		engine->frameBufferResize(window);
	}

	GraphicEngine::GraphicEngine(GLFWwindow* targetWindow)
		: _instance()
#ifdef _DEBUG
		, _debugTool(_instance)
#endif
		, _surface(_instance, targetWindow)
		, _device(targetWindow, _instance, _surface)
	{
		glfwSetWindowUserPointer(targetWindow, this);
		glfwSetFramebufferSizeCallback(targetWindow, framebufferResizeCallback);
	}

	void GraphicEngine::createBuffers(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
	{
		_device.createBuffers(vertices, indices);
	}
	void GraphicEngine::drawFrame(const UniformBufferObject& ubo)
	{
		_device.drawFrame(ubo);
	}
	void GraphicEngine::terminate()
	{
		_device.terminate();
	}

	void GraphicEngine::frameBufferResize(GLFWwindow* window)
	{
		_device.raiseFramebufferResizedFlag(window, &_surface);
	}
}