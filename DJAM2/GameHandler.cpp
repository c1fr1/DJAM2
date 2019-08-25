#include "GameHandler.h"
#include <cmath>

#define PLAYER_ACCELERATION 0.4f
#define PLAYER_ROTATION_RATE 4.0f
#define PLAYER_BRAKE_RATE 0.3f
#define SHOT_SPEED 4.0f;

GameHandler::GameHandler() {
	playerPos.x = 0;
	playerPos.y = 0;
	shotTimer = -1;
}

float GameHandler::getOffsetX() {
	return playerPos.x;
}

float GameHandler::getOffsetY() {
	return playerPos.y;
}

mat4 GameHandler::getBackgroundOffset(int index) {
	mat4 ret = mat4(*perspectiveMat);
	ret = scale(ret, vec3(3.0f));
	ret = translate(ret, vec3((float)(index % 3 - 1) - fmod(cameraPos.x / 3.0f, 1.0f), (float)(index / 3 - 1) - fmod(cameraPos.y / 3.0f, 1.0f), 0.0f));
	return ret;
}

mat4 GameHandler::getPlayerMat() {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3(playerPos.x - cameraPos.x, playerPos.y - cameraPos.y, 0.0f));
	ret = scale(ret, vec3(0.2f));
	ret = rotate(ret, playerAngle + 3.14159265384f / 2.0f, vec3(0.0f, 0.0f, 1.0f));
	return ret;
}

mat4 GameHandler::getShotMat() {
	if (shotTimer < 0) {
		return mat4();
	} else {
		mat4 ret = mat4(*perspectiveMat);
		ret = translate(ret, vec3(shotPos.x - cameraPos.x, shotPos.y - cameraPos.y, 0.0f));
		ret = scale(ret, vec3(0.1f));
		ret = rotate(ret, shotAngle + 3.14159265384f / 2.0f, vec3(0.0f, 0.0f, 1.0f));
		return ret;
	}
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
		playerVelocity += vec2(cos(playerAngle), sin(playerAngle)) * timeDelta * PLAYER_ACCELERATION;
	}
	if (keyDown(GLFW_KEY_S)) {
		playerVelocity *= pow(PLAYER_BRAKE_RATE, timeDelta);
	}
	if (keyDown(GLFW_KEY_A)) {
		playerAngle -= timeDelta * PLAYER_ROTATION_RATE;
	}
	if (keyDown(GLFW_KEY_D)) {
		playerAngle += timeDelta * PLAYER_ROTATION_RATE;
	}
	if (playerPos.x - cameraPos.x > 0.5f) {
		cameraPos.x += playerPos.x - cameraPos.x - 0.5f;
	}
	if (playerPos.y - cameraPos.y > 0.5f) {
		cameraPos.y += playerPos.y - cameraPos.y - 0.5f;
	}
	if (playerPos.x - cameraPos.x < -0.5f) {
		cameraPos.x += playerPos.x - cameraPos.x + 0.5f;
	}
	if (playerPos.y - cameraPos.y < -0.5f) {
		cameraPos.y += playerPos.y - cameraPos.y + 0.5f;
	}
	if (shotTimer > 0) {
		shotTimer -= timeDelta;
		shotPos += shotVel * timeDelta;
	} else {
		if (keyDown(GLFW_KEY_SPACE)) {
			shotPos = playerPos;
			shotAngle = playerAngle;
			shotVel = playerVelocity + vec2(cos(shotAngle), sin(shotAngle)) * SHOT_SPEED;
			shotTimer = 1;
		}
	}

	playerPos += playerVelocity * timeDelta;
}

void GameHandler::setPerspective(mat4* mat) {
	perspectiveMat = mat;
}

