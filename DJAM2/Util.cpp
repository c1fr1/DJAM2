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

void Util::createBuffer(VkDevice device, VkPhysicalDevice* physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	vkCreateBuffer(device, &bufferInfo, NULL, &buffer);

	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(device, buffer, &memReqs);

	VkMemoryAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocateInfo.allocationSize = memReqs.size;
	allocateInfo.memoryTypeIndex = findMemoryType(physicalDevice, memReqs.memoryTypeBits, properties);

	vkAllocateMemory(device, &allocateInfo, NULL, &bufferMemory);

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

VkCommandBuffer Util::beginSingleUseCommandBuffer(VkDevice* device, VkCommandPool commandPool) {
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandBufferCount = 1;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	VkCommandBuffer ret;
	vkAllocateCommandBuffers(*device, &allocInfo, &ret);

	VkCommandBufferBeginInfo beginfo = {};
	beginfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(ret, &beginfo);
	return ret;
}

void Util::endSingleUseCommandBuffer(VkDevice* device, VkCommandBuffer commandBuffer, VkQueue queue, VkCommandPool commandPool) {

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitfo = {};
	submitfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitfo.commandBufferCount = 1;
	submitfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(queue, 1, &submitfo, NULL);

	vkQueueWaitIdle(queue);

	vkFreeCommandBuffers(*device, commandPool, 1, &commandBuffer);
}

float Util::randf() {
	return (float)(rand()) / (float)(RAND_MAX);;
}

float Util::rmod(float a, float b) {
	float ret = fmod(a, b);
	if (a < 0) {
		ret += 1;
	}
	return ret;
}
