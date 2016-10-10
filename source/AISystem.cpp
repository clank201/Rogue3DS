#include "../include/components.h"
#include "../include/entityx/entityx.h"
#include "../include/HardwareInterface.h"
#include "../include/AISystem.h"


void AISystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) {
	entityx::ComponentHandle<Velocity> velocity;
	entityx::ComponentHandle<AIFollower> follower;
	entityx::ComponentHandle<Position> position;
	for (entityx::Entity entity : es.entities_with_components(follower, velocity, position)) {
		if (follower->target->inRange(position->currentPosition, follower->dist)) {
			velocity->currentVelocity = point3D{ 0,0,0 };
		}
		else {
			if (follower->target->x - position->currentPosition.x > follower->dist) { velocity->currentVelocity.x = 1; }
			else if (follower->target->x - position->currentPosition.x < -follower->dist) { velocity->currentVelocity.x = -1; }
			else velocity->currentVelocity.x = 0;
			if (follower->target->y - position->currentPosition.y > follower->dist) { velocity->currentVelocity.y = 1; }
			else if (follower->target->y - position->currentPosition.y < -follower->dist) { velocity->currentVelocity.y = -1; }
			else velocity->currentVelocity.y = 0;
		}
	}
}

AISystem::~AISystem() {
} 