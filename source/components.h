#pragma once
#include "string.h"
#include "core.h"

using namespace std;

struct  Animation{
	explicit Animation(int frames, string filename) : frames(frames), filename(filename) {}

	int frames;
	string filename;
};

struct  FixedSprite {
	explicit FixedSprite(string filename) : filename(filename) {}

	string filename;
};

struct  AIFollower {
	explicit AIFollower(point3D target) : target(target){}

	point3D target;
};

struct Health
{
	explicit Health(int maxHealth, int currentHealth) : maxHealth(maxHealth), currentHealth(currentHealth){}

	int currentHealth;
	int maxHealth;
};

struct Stamina
{
	explicit Stamina(int maxStamina, int currentStamina) :maxStamina(maxStamina), currentStamina(currentStamina) {}

	int currentStamina;
	int maxStamina;
};

struct Velocity
{
	explicit Velocity(point3D currentVelocity) : currentVelocity(currentVelocity){}

		point3D currentVelocity;
};

struct Collision{};
struct CanSwim{};
struct CanFly{};
struct Input{};

struct Inventory{};