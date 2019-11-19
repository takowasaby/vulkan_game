#pragma once

#include <Wasabi/graphic/vulkan/instance.h>
#include <Wasabi/graphic/vulkan/debugTool.h>
#include <Wasabi/graphic/vulkan/surface.h>
#include <Wasabi/graphic/vulkan/graphicDevice.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class GraphicEngine {
			public:
				static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

			public:
				GraphicEngine(GLFWwindow* targetWindow);

				void createBuffers(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
				void drawFrame(const UniformBufferObject& ubo);
				void terminate();

				void frameBufferResize(GLFWwindow* window);

			private:
				Instance _instance;
#ifdef _DEBUG
				DebugTool _debugTool;
#endif
				Surface _surface;
				GraphicDevice _device;
			};
		}
	}
}