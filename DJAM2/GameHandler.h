#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include "Callbacks.h"

class GameHandler {
public:
	float getOffsetX();
	float getOffsetY();
	static void handleBackground();
	void main(float timeDelta);
private:
	float offsetX;
	float offsetY;
};

