#pragma once

#include "vulkan/vulkan.h"

#include <string>

#include "backends/imgui_impl_vulkan.h"

namespace vkb 
{
	enum class ShadingLanguage
	{
		GLSL,
		HLSL,
		SLANG,
	};

	ShadingLanguage get_shading_language();

	const std::string to_string(VkResult result);
}

namespace Cubed
{
	ImGui_ImplVulkan_InitInfo* GetVulkanInfo();
}

#define VK_CHECK(x)                                                                    \
	do                                                                                 \
	{                                                                                  \
		VkResult err = x;                                                              \
		if (err)                                                                       \
		{                                                                              \
			throw std::runtime_error("Detected Vulkan error: " + vkb::to_string(err)); \
		}                                                                              \
	} while (0)

