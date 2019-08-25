#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Util.h";

using namespace glm;

struct Asteroid {
	Asteroid();
	Asteroid(vec2 pos);
	void reset(vec2 pos);
	float rotation;
	float rotationRate;
	vec2 position;
	vec2 velocity;
	mat4 getMat(mat4 perspective, vec2 cam);
	void update(vec2 playerPos, float deltaTime);
	bool collidesWith(vec2 projectilePosition);
};

