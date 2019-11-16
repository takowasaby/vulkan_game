#pragma once

#include <vector>

class VkInstance_T;
using VkInstance = VkInstance_T *;

namespace wsb {
	namespace graphic {
		namespace vulkan {
			class Instance {
			public:
				static std::vector<const char*> getRequiredExtensions();
			public:
				Instance();
				~Instance();

				const VkInstance& getInstanceRef();
			private:
				VkInstance _instance;
			};
		}
	}
}