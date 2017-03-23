#include "Queue.h"

#include "Device.h"

Queue::Queue(const Device& dev, const QueueFamilyIndices& indices)
{
	vkGetDeviceQueue(dev.getLogicalDevice(), indices.present, 0, &m_queue);
}
