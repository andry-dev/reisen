#include "Instance.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "asl/types"
#include "asl/debug_only"

#define GSL_ASSERT_WITH_MESSAGE
#include "gsl/assert"

#include <vector>

#include "vk_common.h"

#include "Log.h"

Instance::Instance(const Window& window)
	: m_window(window)
{
	log_info("Instance::Instance");

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vk test";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "tewi/vkfw";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instInfo = {};
	instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pApplicationInfo = &appInfo;

	auto exts = getRequiresExtensions();

	instInfo.enabledExtensionCount = exts.size();
	instInfo.ppEnabledExtensionNames = exts.data();

	if (g_validationLayersEnabled)
	{
		instInfo.enabledLayerCount = g_validationLayers.size();
		instInfo.ppEnabledLayerNames = g_validationLayers.data();
	}
	else
	{
		instInfo.enabledLayerCount = 0;
	}

	{
		VkResult res = vkCreateInstance(&instInfo, nullptr, &m_instance);

		Expects(res == VK_SUCCESS, "Can't create instance");
	}

#ifndef NDEBUG
	Expects(g_validationLayersEnabled && checkValidationLayersSupport(),
			"Validation layers not enabled");
#endif

	if (g_validationLayersEnabled)
	{
		VkDebugReportCallbackCreateInfoEXT cbInfo = {};
		cbInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		cbInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT
						| VK_DEBUG_REPORT_WARNING_BIT_EXT
						| VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		cbInfo.pfnCallback = details::debugCallback;

		asl::debug_only<VkResult> res = details::createDebugReportCallbackEXT(m_instance, &cbInfo, nullptr, &m_callback);
		Expects(res == VK_SUCCESS, "Can't create the debug callback");
	}

	glfwCreateWindowSurface(m_instance, m_window.getWindow(), nullptr, &m_surface);
}

Instance::~Instance()
{
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
	details::destroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);
	vkDestroyInstance(m_instance, nullptr);
	log_info("Instance::~Instance");
}
