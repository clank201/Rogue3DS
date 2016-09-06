#include "physicsSystem.h"
#include "components.h"



physicsSystem::physicsSystem()
{
}


physicsSystem::~physicsSystem()
{
}

void physicsSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	entityx::ComponentHandle<Position> position;
	entityx::ComponentHandle<Velocity> velocity;
	entityx::ComponentHandle<Player> player;
	for (entityx::Entity entity : es.entities_with_components(position, velocity)) {
		// Do things with entity, position and direction.
		position->currentPosition.x += velocity->currentVelocity.x;
		position->currentPosition.y += velocity->currentVelocity.y;
		position->currentPosition.z += velocity->currentVelocity.z;
	}
	for(entityx::Entity entity : es.entities_with_components(player,position))
	{
		*player->pos = position->currentPosition;
	}
}
