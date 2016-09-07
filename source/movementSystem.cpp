#include "../include/movementSystem.h"
#include "../include/components.h"

movementSystem::~movementSystem()
{
}

void movementSystem::update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt)
{
	entityx::ComponentHandle<Position> position;
	entityx::ComponentHandle<Velocity> velocity;
	entityx::ComponentHandle<Player> player;
	for (entityx::Entity entity : es.entities_with_components(position, velocity)) {
		// Do things with entity, position and direction.
		point3D temp = position->currentPosition;
		temp.x += velocity->currentVelocity.x;
		temp.y += velocity->currentVelocity.y;
		temp.z += velocity->currentVelocity.z;
		if(!basicCollision(temp))
		{
			position->currentPosition = temp;
		}
		else
		{
			temp.z++;
			if(!basicCollision(temp))
			{
				position->currentPosition = temp;
			}
		}
		temp = position->currentPosition;
		temp.z--;
		if(!basicCollision(temp))
		{
			position->currentPosition = temp;
		}
		
	}
	for(entityx::Entity entity : es.entities_with_components(player,position))
	{
		*player->pos = position->currentPosition;
		cout << player->pos->x << " " << player->pos->y << " " << player->pos->z << endl;
	}
}

bool movementSystem::basicCollision(point3D pos)
{
	return mapObj->getTerrain(pos).solid;
}
