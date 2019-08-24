#include "Util.h"

uint32_t Util::findMemoryType(VkPhysicalDevice* physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(*physicalDevice, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
		if (typeFilter & (1 << i) && ((memProperties.memoryTypes[i].propertyFlags & properties) == properties)) {
			return i;
		}
	}
	throw std::runtime_error("no compatible memory type");
}
