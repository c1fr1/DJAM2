#include "Callbacks.h"

void Callbacks::keyCallback(GLFWwindow* window, int key, int scanCode, int action, int modes) {
	if (key >= 0 && key < GLFW_KEY_LAST + 1) {
		if (action == GLFW_PRESS) {
			keys[key] = KEY_PRESSED;
		} else if (action == GLFW_RELEASE) {//key up
			keys[key] = KEY_RELEASED;
		}
	}
}

void Callbacks::mouseButtonCallback(GLFWwindow* window, int key, int action, int mods) {
	if (key >= 0 && key < GLFW_MOUSE_BUTTON_LAST + 1) {
		if (action == GLFW_PRESS) {
			mouseButtons[key] = KEY_PRESSED;
		} else if (action == GLFW_RELEASE) {
			mouseButtons[key] = KEY_RELEASED;
		}
	}
}

void Callbacks::cursorMovedCallback(GLFWwindow* window, double posX, double posY) {
	mousePosX = posX;
	mousePosY = posY;
}


int keys[GLFW_KEY_LAST + 1];

int mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1]; 

vec2 mousePosition = vec2();
double mousePosX;
double mousePosY;