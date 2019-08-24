#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Util.h"

using namespace glm;

struct ROCreateInfo {
	uint32_t frameBufferCount;
	VkDevice* logicalDevice;
	VkPhysicalDevice* physicalDevice;
};
struct ROFinishCreateInfo {
	uint32_t frameBufferCount;
	VkDevice* logicalDevice;
	VkDescriptorPool* descriptorPool;
	VkDescriptorSetLayout* descriptorSetLayout;
	VkImageView* imageView;
	VkSampler* imageSampler;
};

class RenderObject {
public:
	RenderObject(ROCreateInfo* createInfo);
	void finishSetup(ROFinishCreateInfo* finishInfo);
	void cleanupVKObjects();
private: 
	VkDescriptorSet* descriptorSets;
	VkBuffer* buffers;
	VkDeviceMemory* memory;
};

