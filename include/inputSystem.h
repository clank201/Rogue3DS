#pragma once
#include <3ds.h>
#include "entityx/entityx.h"
#include "core.h"

using namespace std;
namespace ex = entityx;

class inputSystem : public ex::System<inputSystem>
{
public:
	explicit inputSystem(u32* kDown, u32* kUp, u32* kHeld, point3D* playerPos) : kDown(kDown), kUp(kUp), kHeld(kHeld), playerPos(playerPos){}
	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;
	~inputSystem();
private:
	u32* kDown;
	u32* kUp;
	u32* kHeld;
	point3D* playerPos;
};

