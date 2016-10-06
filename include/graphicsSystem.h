 #pragma once
#include "gameMap.h"
#include "core.h"
#include "UI.h"
#include "entityx/entityx.h"
#include "HardwareInterface.h"

namespace ex = entityx;

class graphicsSystem : public ex::System<graphicsSystem>
{
private:
	point3D* playerPos;
	gameMap* mapObj;
	textureName texTable[TEX_TABLE_SIZE];
	bool isTextureLoaded(string textureFile) const;
	int freeTexturePos() const;
	int getTexturePos(string fileName) const;
	void loadTexture(string fileName);
	void freeTexture(string fileName);
	void cameraUpdate();
	HI::HITexture entitiesTexture;
	HI::HITexture terrainTexture;
	HI::HITexture getTexture(point3D p, mode mode_t = PRRT) const;
	HI::HITexture arrowTexture;
	//sftd_font* font;
	point3D cameraPos;
public:
	explicit graphicsSystem(gameMap* map, point3D* pos);
	~graphicsSystem();

	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;
	void freeAllTextures();
	void drawFrame(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt);
	void reloadTextures();

};
