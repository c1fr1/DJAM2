#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include "Callbacks.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RenderObject.h"
#include "AsteroidFragment.h"
#include "Asteroid.h"

#define GAMESTATE_MENU 0
#define GAMESTATE_INGAME 1
#define GAMESTATE_GAMEOVER -1

using namespace glm;

class GameHandler {
public:
	GameHandler();
	float getOffsetX();
	float getOffsetY();
	static void handleBackground(VkExtent2D windowSize);
	void main(float timeDelta, float aspectRatio);
	void game(float timeDelta, float aspectRatio);
	void gameOver();

	void reset();
	void setPerspective(mat4* mat);

	mat4 getMatforAstFrag(int index);
	mat4 getMatforAst(int index);
	mat4 getBackgroundOffset(int index);
	mat4 getPlayerMat();
	mat4 getShotMat();
	mat4 getEmptyFuelbarMat(VkExtent2D extent);
	mat4 getFuelbarMat(VkExtent2D extent);
	mat4 getEmptyCurrencybarMat(VkExtent2D extent);
	mat4 getCurrencybarMat(VkExtent2D extent);
	mat4 getEmptyShotbarMat(VkExtent2D extent);
	mat4 getShotbarMat(VkExtent2D extent);
	mat4* getWarningIndicatorMats();
	mat4 getPlayerShieldMat();

	bool hasFuel();
	int gameState = GAMESTATE_GAMEOVER;
private:
	void addAsteroidFragments(int count, Asteroid pos);
	bool attemptPlayerKill();

	mat4* perspectiveMat;
	
	vec2 cameraPos;

	vec2 playerPos;
	vec2 playerVelocity;
	float playerAngle;
	float playerRes = 0;

	Asteroid* asteroids;

	AsteroidFragment* asteroidFrags;
	bool* asteroidFragsEnabled;

	vec2 shotPos;
	vec2 shotVel;
	float shotTimer;
	float shotAngle;

	float playerFuel = 1.0f;
	int playerShots = 16;

	float playerAcceleration = 0.4f;
	float playerTurnRate = 4.0f;
	float shieldState = 1.0f;
	bool shieldUnlocked = true;
};

