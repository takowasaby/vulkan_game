#pragma once

#include <Wasabi/graphic/vulkan/instance.h>
#include <Wasabi/graphic/vulkan/debugTool.h>

#include <vector>
#include <array>

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class GraphicEngine {
			public:
				GraphicEngine();
			private:
				Instance _instance;
#ifdef _DEBUG
				DebugTool _debugTool;
#endif
			};
		}
	}
}