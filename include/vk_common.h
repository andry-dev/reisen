#pragma once

#include "asl/types"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>
#include <array>
#include <cstring>


#ifdef NDEBUG
[[maybe_unused]]
constexpr bool g_validationLayersEnabled = false;
#else
[[maybe_unused]]
constexpr bool g_validationLayersEnabled = true;
#endif

constexpr std::array<const char*, 1> g_validationLayers = { {
	"VK_LAYER_LUNARG_standard_validation",
} };


inline bool checkValidationLayersSupport()
{
	asl::mut_u32 layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> avaibleLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, avaibleLayers.data());

	for (auto name : g_validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProps : avaibleLayers)
		{
			if (std::strcmp(name, layerProps.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;

}

inline auto getRequiresExtensions()
{
	asl::mut_u32 extCount = 0;
	const char** glfwExts = glfwGetRequiredInstanceExtensions(&extCount);

	std::vector<const char*> extVec;

	for (asl::mut_sizei i = 0; i < extCount; ++i)
	{
		extVec.push_back(glfwExts[i]);
	}

	if (g_validationLayersEnabled)
	{
		extVec.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extVec;
}


namespace details {
	[[maybe_unused]] static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		asl::u64 obj,
		asl::sizei location,
		asl::i32 code,
		const char* layerPrefix,
		const char* msg,
		void* data
	)
	{
		std::printf("[%s] %s",  layerPrefix, msg);


		return VK_FALSE;
	}

	inline VkResult createDebugReportCallbackEXT(VkInstance& instance,
			const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugReportCallbackEXT* pCallback)
	{
		auto func = (PFN_vkCreateDebugReportCallbackEXT)
			vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pCallback);
		} else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	inline void destroyDebugReportCallbackEXT(VkInstance& instance,
			VkDebugReportCallbackEXT& callback,
			const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}
}
