#include "GameHandler.h"

float GameHandler::getOffsetX() {
	return offsetX;
}

float GameHandler::getOffsetY() {
	return offsetY;
}

void GameHandler::handleBackground() {
	for (int i = 0; i < GLFW_KEY_LAST + 1; ++i) {
		if (keys[i] == KEY_PRESSED || keys[i] == KEY_RELEASED) {
			++keys[i];
		}
	}
}

void GameHandler::main(float timeDelta) {
	if (keyDown(GLFW_KEY_W)) {
		offsetY += timeDelta;
	}
	if (keyDown(GLFW_KEY_S)) {
		offsetY -= timeDelta;
	}
	if (keyDown(GLFW_KEY_A)) {
		offsetX -= timeDelta;
	}
	if (keyDown(GLFW_KEY_D)) {
		offsetX += timeDelta;
	}
}
