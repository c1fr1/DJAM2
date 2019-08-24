#include "Callbacks.h"

void Callbacks::keyCallback(GLFWwindow* window, int key, int scanCode, int action, int modes) {
	if (key > 0 && key < GLFW_KEY_LAST + 1) {
		if (action == GLFW_PRESS) {
			keys[key] = KEY_DOWN;
		} else if (action == GLFW_RELEASE) {//key up
			keys[key] = KEY_RELEASED;
		}
	}
}


int keys[GLFW_KEY_LAST + 1];