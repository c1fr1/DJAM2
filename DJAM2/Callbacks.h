#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

#define KEY_PRESSED 1
#define KEY_DOWN 2
#define KEY_RELEASED -1

#define keyDown(k) (keys[k] > 0)
#define keyUp(k) (keys[k] <= 0)
#define keyPressed(k) (keys[k] == KEY_PRESSED)
#define keyReleased(k) (keys[k] == KEY_RELEASED)

class Callbacks {
public:
	static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int modes);
};

extern int keys[GLFW_KEY_LAST + 1];