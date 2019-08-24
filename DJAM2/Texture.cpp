#include "Texture.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

Texture::Texture() {
}

Texture::Texture(VkImage img, VkDeviceMemory imgMemory, VkImageView imgView) {
	image = img;
	memory = imgMemory;
	view = imgView;
}

void Texture::cleanup(VkDevice device) {
	vkDestroyImageView(device, view, NULL);
	vkDestroyImage(device, image, NULL);
	vkFreeMemory(device, memory, NULL);
}

VkImageView* Texture::getView() {
	return &view;
}
