#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>



#include "Util.h"
class Texture {
public:
	Texture();
	Texture(VkImage img, VkDeviceMemory imgMemory, VkImageView imgView);
	void cleanup(VkDevice device);
	VkImageView* getView();
private:
	VkImage image;
	VkDeviceMemory memory;
	VkImageView view;
};

