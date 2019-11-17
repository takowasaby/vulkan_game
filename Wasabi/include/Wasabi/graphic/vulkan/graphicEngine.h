#pragma once

#include <Wasabi/graphic/vulkan/instance.h>
#include <Wasabi/graphic/vulkan/debugTool.h>
#include <Wasabi/graphic/vulkan/surface.h>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class GraphicEngine {
			public:
				GraphicEngine(GLFWwindow* targetWindow);
			private:
				Instance _instance;
#ifdef _DEBUG
				DebugTool _debugTool;
#endif
				Surface _surface;
			};
		}
	}
}