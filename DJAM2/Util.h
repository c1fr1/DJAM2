#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class Util {
public:
	static uint32_t findMemoryType(VkPhysicalDevice* physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	static void createBuffer(VkDevice device, VkPhysicalDevice* physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	static VkCommandBuffer beginSingleUseCommandBuffer(VkDevice* device, VkCommandPool commandPool);
	static void endSingleUseCommandBuffer(VkDevice* device, VkCommandBuffer commandBuffer, VkQueue queue, VkCommandPool commandPool);
};

