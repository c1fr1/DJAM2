#include "GameHandler.h"
#include <cmath>

#define PLAYER_FUEL_DRAIN_RATE_A 0.05f
#define PLAYER_FUEL_DRAIN_RATE_B 0.025f
#define PLAYER_BRAKE_RATE 0.3f
#define SHOT_SPEED 4.0f

GameHandler::GameHandler() {
	asteroidFrags = new AsteroidFragment[14];
	asteroidFragsEnabled = new bool[14];
	for (int i = 0; i < 14; ++i) {
		asteroidFrags[i] = AsteroidFragment(vec2(), vec2());
		asteroidFragsEnabled[i] = false;
	}
	asteroids = new Asteroid[10];
	for (int i = 0; i < 10; ++i) {
		asteroids[i] = Asteroid(playerPos);
	}
	cameraPos = vec2();
	perspectiveMat = NULL;

	playerPos.x = 0;
	playerPos.y = 0;
	shotTimer = -1;
}

void GameHandler::main(float timeDelta, float aspectRatio) {
	if (gameState == GAMESTATE_MENU) {
		menu(aspectRatio);
	} else if (gameState == GAMESTATE_INGAME) {
		game(timeDelta, aspectRatio);
	} else if (gameState == GAMESTATE_GAMEOVER || gameState == GAMESTATE_VICTORY) {
		gameOver();
	}
	if (keyReleased(GLFW_KEY_ESCAPE)) {
		if (gameState == GAMESTATE_MENU) {
			shouldQuit = true;
		}
		gameState = GAMESTATE_MENU;
	}
}

void GameHandler::menu(float aspectRatio) {
	if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT] == KEY_RELEASED) {
		if (mousePosition.x * aspectRatio > -0.25 && mousePosition.x < 0.25) {
			if (mousePosition.y > -0.3 - 0.125 && mousePosition.y < -0.3 + 0.125) {
				gameState = GAMESTATE_INGAME;
				difficulty = DIFFICULTY_EASY;
				reset();
			}
			if (mousePosition.y > 0.1 - 0.125 && mousePosition.y < 0.1 + 0.125) {
				gameState = GAMESTATE_INGAME;
				difficulty = DIFFICULTY_MEDIUM;
				reset();
			}
			if (mousePosition.y > 0.5 - 0.125 && mousePosition.y < 0.5 + 0.125) {
				gameState = GAMESTATE_INGAME;
				difficulty = DIFFICULTY_HARD;
				reset();
			}
			if (mousePosition.y > 0.8 - 0.125 && mousePosition.y < 0.8 + 0.125) {
				gameState = GAMESTATE_TUTORIAL;
			}
		}
	}
}

void GameHandler::game(float timeDelta, float aspectRatio) {
	if (keyDown(GLFW_KEY_LEFT_SHIFT)) {
		playerAcceleration /= 2;
		playerTurnRate /= 2;
	}
	if (playerFuel > 0) {
		if (keyDown(GLFW_KEY_W)) {
			playerVelocity += vec2(cos(playerAngle), sin(playerAngle)) * timeDelta * playerAcceleration;
			playerFuel -= timeDelta * PLAYER_FUEL_DRAIN_RATE_A;
		}
		bool extraMovement = false;
		if (keyDown(GLFW_KEY_S)) {
			playerVelocity *= pow(PLAYER_BRAKE_RATE, timeDelta);
			extraMovement = true;
		}
		if (keyDown(GLFW_KEY_A)) {
			playerAngle -= timeDelta * playerTurnRate;
			extraMovement = true;
		}
		if (keyDown(GLFW_KEY_D)) {
			playerAngle += timeDelta * playerTurnRate;
			extraMovement = true;
		}
		if (extraMovement) {
			playerFuel -= timeDelta * PLAYER_FUEL_DRAIN_RATE_B;
		}
	}
	if (keyDown(GLFW_KEY_LEFT_SHIFT)) {
		playerAcceleration *= 2;
		playerTurnRate *= 2;
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
		if (keyPressed(GLFW_KEY_SPACE)) {
			if (playerShots > 0) {
				shotPos = playerPos;
				shotAngle = playerAngle;
				shotVel = playerVelocity + vec2(cos(shotAngle), sin(shotAngle)) * SHOT_SPEED;
				shotTimer = 1;
				--playerShots;
			}
		}	
	}
	if (length(playerVelocity) > 2) {
		playerVelocity *= 2.0f / length(playerVelocity);
	}

	playerPos += playerVelocity * timeDelta;

	for (int i = 0; i < 14; ++i) {
		if (asteroidFragsEnabled[i]) {
			asteroidFragsEnabled[i] = asteroidFrags[i].update(playerPos, timeDelta);
			if (shotTimer > 0) {
				if (asteroidFrags[i].collidesWith(shotPos)) {
					shotTimer = -1;
					asteroidFragsEnabled[i] = false;
				}
			}
			if (asteroidFragsEnabled[i]) {
				if (length(playerPos - asteroidFrags[i].position) < 0.1f) {
					if (length(playerVelocity - asteroidFrags[i].velocity) < impactdeltaFragments) {
						playerRes += 1;
						if (playerRes > 8) {
							playerRes = 8;
						}
						asteroidFragsEnabled[i] = false;
					} else {
						if (!attemptPlayerKill()) {
							asteroidFragsEnabled[i] = false;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 10; ++i) {
		asteroids[i].update(playerPos, timeDelta);
		if (shotTimer > 0) {
			if (asteroids[i].collidesWith(shotPos)) {
				addAsteroidFragments(2 + Util::randf() * 3.0f, asteroids[i]);
				asteroids[i].reset(playerPos);
				shotTimer = -1;
			}
		}
		if (length(playerPos - asteroids[i].position) < 0.1f) {
			if (length(playerVelocity - asteroids[i].velocity) < impactdeltaAsteroids) {
				playerVelocity = asteroids[i].velocity;
			} else {
				if (!attemptPlayerKill()) {
					asteroids[i].reset(playerPos);
				}
			}
		}
	}

	if (playerRes > 0) {
		if (mousePosition.x * aspectRatio > aspectRatio - 0.5) {
			if (mousePosition.y > 0.75f) {
				if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT] == KEY_RELEASED) {
					playerFuel += 0.25f;
					if (playerFuel > 1.0f) {
						playerFuel = 1.0f;
					}
					--playerRes;
				}
			}
		}
		if (mousePosition.x * aspectRatio > aspectRatio - 0.5) {
			if (mousePosition.y > 0.5f && mousePosition.y < 0.75f) {
				if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT] == KEY_RELEASED) {
					playerShots += 2;
					if (playerShots > 16) {
						playerFuel = 16;
					}
					--playerRes;
				}
			}
		}
	}
	if (shieldState < 0 && shieldUnlocked) {
		shieldState += timeDelta;
	}
	if (powerupEngineDropped) {
		float dist = length(playerPos - powerupEnginePos);
		if (dist > 3) {
			powerupEngineDropped = false;
		} else if (dist < 0.1f) {
			powerupEngineDropped = false;
			enginePowerupUnlocked = true;
			playerAcceleration *= 2.0f;
		}
		powerupEnginePos += powerupEngineVelocity * timeDelta;
	}
	if (powerupTurnrateDropped) {
		float dist = length(playerPos - powerupTurnratePos);
		if (dist > 3) {
			powerupTurnrateDropped = false;
		}
		else if (dist < 0.1f) {
			powerupTurnrateDropped = false;
			turnratePowerupUnlocked = true;
			playerTurnRate *= 1.5f;
		}
		powerupTurnratePos += powerupTurnrateVelocity * timeDelta;
	}
	if (powerupShieldDropped) {
		float dist = length(playerPos - powerupShieldPos);
		if (dist > 3) {
			powerupShieldDropped = false;
		}
		else if (dist < 0.1f) {
			powerupShieldDropped = false;
			shieldUnlocked = true;
			shieldState = 1.0f;
		}
		powerupShieldPos += powerupShieldVelocity * timeDelta;
	}

	if (enginePowerupUnlocked && turnratePowerupUnlocked && shieldUnlocked) {
		gameState = GAMESTATE_VICTORY;
	}
}

void GameHandler::gameOver() {
	if (keyPressed(GLFW_KEY_SPACE) || keyPressed(GLFW_KEY_ENTER) || mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
		gameState = GAMESTATE_INGAME;
		reset();
	}
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
	ret = translate(ret, vec3((float)(index % 3 - 1) - Util::rmod(cameraPos.x / 3.0f + 0.5f, 1.0f) + 0.5f, (float)(index / 3 - 1) - Util::rmod(cameraPos.y / 3.0f, 1.0f), 0.0f));
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

mat4 GameHandler::getEmptyFuelbarMat(VkExtent2D extent) {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3((float) extent.width / (float) extent.height - 0.25f, 0.875f, 0.0f));
	ret = scale(ret, vec3(-0.5f, 0.25f, 1.0f));
	return ret;
}

mat4 GameHandler::getFuelbarMat(VkExtent2D extent) {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3((float) extent.width / (float) extent.height - 0.25f + (1.0f - playerFuel) / 2.0f, 0.875f, 0.0f));
	ret = scale(ret, vec3(-0.5f, 0.25f, 1.0f));
	return ret;
}

mat4 GameHandler::getEmptyCurrencybarMat(VkExtent2D extent) {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3(-(float)extent.width / (float)extent.height + 0.25f, 0.875f, 0.0f));
	ret = scale(ret, vec3(-0.5f, 0.25f, 1.0f));
	return ret;
}

mat4 GameHandler::getCurrencybarMat(VkExtent2D extent) {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3(-(float)extent.width / (float)extent.height - 0.25f + (float) playerRes / 16.0f, 0.875f, 0.0f));
	ret = scale(ret, vec3(-0.5f, 0.25f, 1.0f));
	return ret;
}

mat4 GameHandler::getEmptyShotbarMat(VkExtent2D extent) {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3((float)extent.width / (float)extent.height - 0.25f, 0.625f, 0.0f));
	ret = scale(ret, vec3(-0.5f, 0.25f, 1.0f));
	return ret;
}

mat4 GameHandler::getShotbarMat(VkExtent2D extent) {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3((float)extent.width / (float)extent.height - 0.25f + (1.0f - (float)playerShots / 16.0f) / 2.0f, 0.625f, 0.0f));
	ret = scale(ret, vec3(-0.5f, 0.25f, 1.0f));
	return ret;
}

mat4* GameHandler::getWarningIndicatorMats() {
	mat4* ret = new mat4[7];
	for (int i = 0; i < 7; ++i) {
		ret[i] = mat4();
	}
	int ind = 0;
	for (int i = 0; i < 10; ++i) {
		if (length(playerPos - asteroids[i].position) < 0.5f) {
			if (length(playerVelocity - asteroids[i].velocity) > impactdeltaAsteroids) {
				ret[ind] = mat4(*perspectiveMat);
				ret[ind] = translate(ret[ind], vec3(asteroids[i].position.x - cameraPos.x, asteroids[i].position.y - cameraPos.y, 0.0f));
				ret[ind] = scale(ret[ind], vec3(0.2f));
				++ind;
				if (ind == 7) {
					return ret;
				}
			}
		}
	}
	for (int i = 0; i < 14; ++i) {
		if (asteroidFragsEnabled[i]) {
			if (length(playerPos - asteroidFrags[i].position) < 0.5f) {
				if (length(playerVelocity - asteroidFrags[i].velocity) > impactdeltaFragments) {
					ret[ind] = mat4(*perspectiveMat);
					ret[ind] = translate(ret[ind], vec3(asteroidFrags[i].position.x - cameraPos.x, asteroidFrags[i].position.y - cameraPos.y, 0.0f));
					ret[ind] = scale(ret[ind], vec3(0.2f));
					++ind;
					if (ind == 7) {
						return ret;
					}
				}
			}
		}
	}
	return ret;
}

mat4 GameHandler::getPlayerShieldMat() {
	if (shieldState > 0 && shieldUnlocked) {
		mat4 ret = mat4(*perspectiveMat);
		ret = translate(ret, vec3(playerPos.x - cameraPos.x, playerPos.y - cameraPos.y, 0.0f));
		ret = scale(ret, vec3(0.2f));
		return ret;
	} else {
		return mat4();
	}
}

mat4 GameHandler::getPowerupEngineMat(VkExtent2D extent) {
	if (powerupEngineDropped) {
		mat4 ret = mat4(*perspectiveMat);
		ret = translate(ret, vec3(powerupEnginePos.x - cameraPos.x, powerupEnginePos.y - cameraPos.y, 0.0f));
		ret = scale(ret, vec3(0.2f));
		return ret;
	} else {
		if (enginePowerupUnlocked) {
			mat4 ret = mat4(*perspectiveMat);
			ret = translate(ret, vec3(-(float)extent.width / (float)extent.height + 0.05f, 0.625f, 0.0f));
			ret = scale(ret, vec3(0.3, 0.3f, 1.0f));
			return ret;
		}
		return mat4();
	}
}

mat4 GameHandler::getPowerupTurnrateMat(VkExtent2D extent) {
	if (powerupTurnrateDropped) {
		mat4 ret = mat4(*perspectiveMat);
		ret = translate(ret, vec3(powerupTurnratePos.x - cameraPos.x, powerupTurnratePos.y - cameraPos.y, 0.0f));
		ret = scale(ret, vec3(0.2f));
		return ret;
	} else {
		if (turnratePowerupUnlocked) {
			mat4 ret = mat4(*perspectiveMat);
			ret = translate(ret, vec3(-(float)extent.width / (float)extent.height + 0.25f, 0.625f, 0.0f));
			ret = scale(ret, vec3(0.3, 0.3f, 1.0f));
			return ret;
		}
		return mat4();
	}
}

mat4 GameHandler::getPowerupShieldMat(VkExtent2D extent) {
	if (powerupShieldDropped) {
		mat4 ret = mat4(*perspectiveMat);
		ret = translate(ret, vec3(powerupShieldPos.x - cameraPos.x, powerupShieldPos.y - cameraPos.y, 0.0f));
		ret = scale(ret, vec3(0.2f));
		return ret;
	} else {
		if (shieldUnlocked) {
			mat4 ret = mat4(*perspectiveMat);
			ret = translate(ret, vec3(-(float)extent.width / (float)extent.height + 0.45f, 0.625f, 0.0f));
			ret = scale(ret, vec3(0.3, 0.3f, 1.0f));
			return ret;
		}
		return mat4();
	}
}

mat4 GameHandler::getTitleMat() {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3(0.0f, -0.7f, 0.0f));
	ret = scale(ret, vec3(-1.0f, 0.5f, 0.0f));
	return ret;
}

mat4 GameHandler::getEasyMat() {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3(0.0f, -0.3f, 0.0f));
	ret = scale(ret, vec3(-0.5f, 0.25f, 0.0f));
	return ret;
}

mat4 GameHandler::getMediumMat() {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3(0.0f, 0.1f, 0.0f));
	ret = scale(ret, vec3(-0.5f, 0.25f, 0.0f));
	return ret;
}

mat4 GameHandler::getHardMat() {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3(0.0f, 0.5f, 0.0f));
	ret = scale(ret, vec3(-0.5f, 0.25f, 0.0f));
	return ret;
}

mat4 GameHandler::getLearnMat() {
	mat4 ret = mat4(*perspectiveMat);
	ret = translate(ret, vec3(0.0f, 0.8f, 0.0f));
	ret = scale(ret, vec3(-0.5f, 0.25f, 0.0f));
	return ret;
}

void GameHandler::handleBackground(VkExtent2D windowSize) {
	for (int i = 0; i < GLFW_KEY_LAST + 1; ++i) {
		if (keys[i] == KEY_PRESSED || keys[i] == KEY_RELEASED) {
			++keys[i];
		}
	}
	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; ++i) {
		if (mouseButtons[i] == KEY_PRESSED || mouseButtons[i] == KEY_RELEASED) {
			++mouseButtons[i];
		}
	}
	mousePosition.x = 2.0f * (float) mousePosX / (float) windowSize.width - 1.0f;
	mousePosition.y = 2.0f * (float) mousePosY / (float) windowSize.height - 1.0f;
}

void GameHandler::reset() {
	if (difficulty == DIFFICULTY_EASY) {
		playerAcceleration = 0.6f;
		playerTurnRate = 5.0f;
		impactdeltaAsteroids = 0.3f;
		impactdeltaFragments = 0.5f;
	} else if (difficulty == DIFFICULTY_MEDIUM) {
		playerAcceleration = 0.4f;
		playerTurnRate = 4.0f;
		impactdeltaAsteroids = 0.1f;
		impactdeltaFragments = 0.3f;
	} else if (difficulty == DIFFICULTY_HARD) {
		playerAcceleration = 0.3f;
		playerTurnRate = 3.0f;
		impactdeltaAsteroids = 0.0f;
		impactdeltaFragments = 0.1f;
	}
	playerPos = vec2();
	playerFuel = 1.0f;
	playerVelocity = vec2();
	playerShots = 16;
	playerAngle = 0.0f;
	playerRes = 0;
	cameraPos = vec2();
	for (int i = 0; i < 14; ++i) {
		asteroidFragsEnabled[i] = false;
	}
	shotTimer = -1.0f;
	if (difficulty > DIFFICULTY_EASY) {
		shieldUnlocked = false;
	} else {
		shieldUnlocked = true;
	}

	powerupEngineDropped = false;
	powerupTurnrateDropped = false;
	powerupShieldDropped = false;
	enginePowerupUnlocked = false;
	turnratePowerupUnlocked = false;
}

void GameHandler::setPerspective(mat4* mat) {
	perspectiveMat = mat;
}

mat4 GameHandler::getMatforAstFrag(int index) {
	if (asteroidFragsEnabled[index]) {
		return asteroidFrags[index].getMat(*perspectiveMat, cameraPos);
	} else {
		return mat4();
	}
}

mat4 GameHandler::getMatforAst(int index) {
	return asteroids[index].getMat(*perspectiveMat, cameraPos);
}

bool GameHandler::hasFuel() {
	return playerFuel > 0;
}

void GameHandler::addAsteroidFragments(int count, Asteroid pos) {
	for (int i = 0; i < count; ++i) {
		for (int i = 0; i < 14; ++i) {
			if (!asteroidFragsEnabled[i]) {
				asteroidFragsEnabled[i] = true;
				asteroidFrags[i] = AsteroidFragment(pos.position, pos.velocity);
				break;
			}
		}
	}
	float probabilityDrop;
	if (difficulty == DIFFICULTY_EASY) {
		probabilityDrop = 0.15f;
	} else if (difficulty == DIFFICULTY_MEDIUM) {
		probabilityDrop = 0.1f;
	} else if (difficulty == DIFFICULTY_HARD) {
		probabilityDrop = 0.05f;
	}
	if (Util::randf() < probabilityDrop) {
		if (!powerupEngineDropped && !enginePowerupUnlocked) {
			powerupEngineDropped = true;
			powerupEnginePos = pos.position;
			powerupEngineVelocity = pos.velocity;
		}
	} else if (Util::randf() < probabilityDrop) {
		if (!powerupTurnrateDropped && !turnratePowerupUnlocked) {
			powerupTurnrateDropped = true;
			powerupTurnratePos = pos.position;
			powerupTurnrateVelocity = pos.velocity;
		}
	} else if (Util::randf() < probabilityDrop) {
		if (difficulty > DIFFICULTY_EASY) {
			if (!powerupShieldDropped && !shieldUnlocked) {
				powerupShieldDropped = true;
				powerupShieldPos = pos.position;
				powerupShieldVelocity = pos.velocity;
			}
		}
	}
}

bool GameHandler::attemptPlayerKill() {
	if (shieldState > 0 && shieldUnlocked) {
		if (difficulty == DIFFICULTY_EASY) {
			shieldState = -4.0f;
		} else if (difficulty == DIFFICULTY_MEDIUM) {
			shieldState = -5.0f;
		} else if (difficulty == DIFFICULTY_HARD) {
			shieldState = -10.0f;
		}
		return false;
	} else {
		gameState = GAMESTATE_GAMEOVER;
		return true;
	}
}

