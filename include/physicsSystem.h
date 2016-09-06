#pragma once

#include "entityx/entityx.h"

namespace ex = entityx;

class physicsSystem : public ex::System<physicsSystem>
{
public:
	physicsSystem();
	~physicsSystem();
	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)override;
};

