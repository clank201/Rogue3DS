#include "inputSystem.h"
#include "components.h"
#include "entityx/entityx.h"
#include <3ds.h>


void inputSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	entityx::ComponentHandle<Velocity> velocity;
	for (entityx::Entity entity : es.entities_with_components(velocity)){
		// Do things with entity, position and direction.
		if ((*kHeld | *kDown) & KEY_DDOWN)
		{
			velocity->currentVelocity.y++;
		}
			if ((*kHeld | *kDown) & KEY_DUP)
		{
			velocity->currentVelocity.y--;
		}
			if ((*kHeld | *kDown) & KEY_DLEFT)
		{
			velocity->currentVelocity.x--;
		}
			if ((*kHeld | *kDown) & KEY_DRIGHT)
		{
			velocity->currentVelocity.x++;
		}
	}
}

inputSystem::~inputSystem()
{
}
