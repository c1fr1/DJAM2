#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#define KEY_PRESSED 1
#define KEY_DOWN 2
#define KEY_RELEASED -1

#define keyDown(k) (keys[k] > 0)
#define keyUp(k) (keys[k] <= 0)
#define keyPressed(k) (keys[k] == KEY_PRESSED)
#define keyReleased(k) (keys[k] == KEY_RELEASED)

using namespace glm;

class Callbacks {
public:
	static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int modes);
	static void mouseButtonCallback(GLFWwindow* window, int key, int action, int mods);
	static void cursorMovedCallback(GLFWwindow* window, double posX, double posY);
};

extern int keys[GLFW_KEY_LAST + 1];
extern int mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
extern vec2 mousePosition;
extern double mousePosX;
extern double mousePosY;