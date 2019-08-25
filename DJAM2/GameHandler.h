#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include "Callbacks.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RenderObject.h"

using namespace glm;

class GameHandler {
public:
	GameHandler();
	float getOffsetX();
	float getOffsetY();
	mat4 getBackgroundOffset(int index);
	mat4 getPlayerMat();
	mat4 getShotMat();
	static void handleBackground();
	void main(float timeDelta);
	void setPerspective(mat4* mat);
private:
	mat4* perspectiveMat;
	
	vec2 cameraPos;

	vec2 playerPos;
	vec2 playerVelocity;
	float playerAngle;

	vec2 shotPos;
	vec2 shotVel;
	float shotTimer;
	float shotAngle;
};

