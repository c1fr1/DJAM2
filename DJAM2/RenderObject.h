#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Util.h"
#include "Texture.h"

using namespace glm;

struct ROCreateInfo {
	uint32_t frameBufferCount;
	VkDevice* logicalDevice;
	VkPhysicalDevice* physicalDevice;
	VkDescriptorPool* descriptorPool;
	VkDescriptorSetLayout* descriptorSetLayout;
	Texture* texture;
	VkSampler* imageSampler;
};

class RenderObject {
public:
	RenderObject();
	RenderObject(ROCreateInfo* createInfo);
	void updateMatrix(VkDevice logDevice, mat4 newVal, int frameBufferIndex);
	void bindCmd(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, int framebufferIndex);
	void cleanupVKObjects(VkDevice& logDevice, int framebufferCount);
private: 
	VkDescriptorSet* descriptorSets;
	VkBuffer* buffers;
	VkDeviceMemory* memory;
};

