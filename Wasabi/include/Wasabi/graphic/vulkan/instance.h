#pragma once

#include <vector>

#include <Wasabi/libdef/vulkan.h>

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