#include "Device.h"

#include "Instance.h"
#include "asl/types"
#include "vk_common.h"

#define GSL_ASSERT_WITH_MESSAGE
#include "gsl/assert"
#include "asl/debug_only"

#include <vector>
#include <set>
#include <iostream>

#include "Log.h"

Device::Device(Instance& instance)
	: m_instance(instance)
{
	log_info("Device::Device");
	asl::mut_u32 deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instance.getInstance(), &deviceCount, nullptr);

	Expects(deviceCount > 0, "Failed to find GPUs with Vulkan support");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instance.getInstance(), &deviceCount, devices.data());

	for (const auto& dev : devices)
	{
		if (isDeviceSuitable(dev, m_instance.getSurface()))
		{
			m_physicalDevice = dev;
			break;
		}
	}

	Expects(m_physicalDevice != VK_NULL_HANDLE, "Can't find a suitable device");

	VkPhysicalDeviceProperties deviceProps;
	vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProps);

	VkPhysicalDeviceFeatures deviceFeats;
	vkGetPhysicalDeviceFeatures(m_physicalDevice, &deviceFeats);

	m_indices = findQueueFamilies(m_physicalDevice, m_instance.getSurface());

	std::vector<VkDeviceQueueCreateInfo> queueInfos;
	std::set<int> uniqueQueueFamilies = { m_indices.graphics, m_indices.present };

	float queuePriority = 1.0f;

	for (auto queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueInfo = {};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queueFamily;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &queuePriority;
	
		queueInfos.push_back(queueInfo);
	}

	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pQueueCreateInfos = queueInfos.data();
	deviceInfo.queueCreateInfoCount = queueInfos.size();
	deviceInfo.pEnabledFeatures = &deviceFeats;
	deviceInfo.enabledExtensionCount = 0;

	if (g_validationLayersEnabled)
	{
		deviceInfo.enabledLayerCount = g_validationLayers.size();
		deviceInfo.ppEnabledLayerNames = g_validationLayers.data();
	}
	else
	{
		deviceInfo.enabledLayerCount = 0;
	}

	{
		asl::debug_only<VkResult> res = vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_device);
		Expects(res == VK_SUCCESS, "Can't create logical device");
	}

	vkGetDeviceQueue(m_device, m_indices.present, 0, &m_queue);
}

Device::~Device()
{
	vkDestroyDevice(m_device, nullptr);
	log_info("Device::~Device");
}


inline QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& dev,
                                            const VkSurfaceKHR& surface)
{
	QueueFamilyIndices indices = {};

	asl::mut_u32 queueCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueCount);
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueCount, queueFamilies.data());

	{
		asl::mut_sizei count = 0;
		for (const auto& family : queueFamilies)
		{
			VkBool32 supportsPresentQueue = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(dev, count, surface, &supportsPresentQueue);

			if (family.queueCount > 0 &&
				(family.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
				supportsPresentQueue)
			{
				indices.graphics = count;
				indices.present = count;
			}

			if (indices.isComplete())
			{
				break;
			}

			++count;
		}
	}

	return indices;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice& dev) noexcept
{
	asl::mut_u32 extCount = 0;
	vkEnumerateDeviceExtensionProperties(dev, nullptr, &extCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extCount);
	vkEnumerateDeviceExtensionProperties(dev, nullptr, &extCount, availableExtensions.data());

	std::set<std::string
}

bool isDeviceSuitable(const VkPhysicalDevice& dev,
							 const VkSurfaceKHR& surface) noexcept
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(dev, &deviceProperties);
    vkGetPhysicalDeviceFeatures(dev, &deviceFeatures);

	auto families = findQueueFamilies(dev, surface);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
		&& families.isComplete();
}
