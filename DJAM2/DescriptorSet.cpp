#include "DescriptorSet.h"

RenderObject::RenderObject(ROCreateInfo* createInfo) {
	VkDeviceSize bufferSize = sizeof(mat4);
	buffers = new VkBuffer[createInfo->frameBufferCount];
	memory = new VkDeviceMemory[createInfo->frameBufferCount];
	descriptorSets = new VkDescriptorSet[createInfo->frameBufferCount];
	for (int i = 0; i < createInfo->frameBufferCount; ++i) {
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		vkCreateBuffer(*createInfo->logicalDevice, &bufferInfo, NULL, &buffers[i]);

		VkMemoryRequirements memReqs;
		vkGetBufferMemoryRequirements(*createInfo->logicalDevice, buffers[i], &memReqs);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memReqs.size;
		allocateInfo.memoryTypeIndex = Util::findMemoryType(createInfo->physicalDevice, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		vkAllocateMemory(*createInfo->logicalDevice, &allocateInfo, NULL, &memory[i]);

		vkBindBufferMemory(*createInfo->logicalDevice, buffers[i], memory[i], 0);
	}
}

void RenderObject::finishSetup(ROFinishCreateInfo* finishInfo) {
	VkDescriptorSetLayout* setLayouts = new VkDescriptorSetLayout[finishInfo->frameBufferCount];
	for (int i = 0; i < finishInfo->frameBufferCount; ++i) {
		setLayouts[i] = *finishInfo->descriptorSetLayout;
	}
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = *finishInfo->descriptorPool;
	allocInfo.descriptorSetCount = finishInfo->frameBufferCount;
	allocInfo.pSetLayouts = setLayouts;

	descriptorSets = new VkDescriptorSet[finishInfo->frameBufferCount];
	vkAllocateDescriptorSets(*finishInfo->logicalDevice, &allocInfo, descriptorSets);

	for (int i = 0; i < finishInfo->frameBufferCount; ++i) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = buffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(mat4);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = *finishInfo->imageView;
		imageInfo.sampler = *finishInfo->imageSampler;

		VkWriteDescriptorSet* descriptorWrites = new VkWriteDescriptorSet[2];
		descriptorWrites[0] = {};
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;//pos in shader?
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1] = {};
		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(*finishInfo->logicalDevice, 2, descriptorWrites, 0, NULL);
	}
}
