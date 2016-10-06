#include <iostream>
#include <string>
#include <fstream>
#include "../include/core.h"
#include "../include/entityx/entityx.h"
#include "../include/inputSystem.h"
#include "../include/movementSystem.h"
#include "../include/components.h"
#include "../include/gameCore.h"

using namespace std;
namespace ex = entityx;

void gameCore::gameLoop()
{
	HI::updateHID();
	kDown = kDown | HI::getKeysDown();
	kHeld = kHeld | HI::getKeysHeld();
	kUp = kUp | HI::getKeysUp();
	
	if (tick % 12 == 0) {
		EntityWorld->systems.update_all(0);
		kDown = HI::getKeysDown();
		kHeld = HI::getKeysHeld();
		kUp = HI::getKeysUp();
	}
	else  HI::waitForVBlank();
	if (kDown & HI::HI_KEY_START) {
		exitBool = true;
	}
	//cout<< player->posX << ' ' << player->posY << ' ' << player->posZ << endl;




	//processar input
	//refrescar coses carregades
	//dibuixar


}

void gameCore::gameLaunch()
{

	// Inits

	//demanar arxius a carregar i ficar un menu o halgo

	//

	/*
	ifstream index;
	index.open("saves/index.txt");
	int saveNum;
	index >> saveNum;
	string saveList[saveNum];
	for (int i=0;i<saveNum;i++){
	index >> saveList[i];
	}
	index.close();
	//select which one
	saveName = selected;
	*/
	
	HI::setBackgroundColor(RGBA8(0, 0, 0, 0));
	ifstream general;
	general.open("saves/" + saveName + "/general.txt");
	if (!general.is_open()) {
		cout << "couldn't open file: " << ("saves/" + saveName + "/general.txt") << endl;
	}
	string playerSprite = "player.png", playerName;
	string cacota;
	general >> cacota >> playerPos->x >> playerPos->y >> playerPos->z;
	general.close();
	map->addPlayer(playerPos);
	for (int i = 0; i < CHUNK_NUM; i++) {
		map->loadNewChunk();
	}
	map->loadTerrainTable();
	EntityWorld->systems.add<inputSystem>(&kDown,&kUp,&kHeld);
	EntityWorld->systems.add<movementSystem>(map);
	EntityWorld->systems.add<graphicsSystem>(map, playerPos);
	EntityWorld->systems.configure();
	entityx::Entity test = EntityWorld->entities.create();
	test.assign<Position>(*playerPos);
	point3D caca;
	caca.x++;
	caca.y++;
	caca.z++;
	test.assign<Velocity>(caca);
	test.assign<Player>(playerPos);

	soundObj.playFromFile("data/sounds/bgm/wilderness.ogg");
	map->startChunkLoader(playerPos);
	tick = 0;
	while (HI::aptMainLoop() && !exitBool) {
		gameLoop();
		tick++;
	}

	string generalFile = "saves/" + saveName + "/general.txt";
	std::remove(generalFile.c_str());
	
	ofstream generalO(generalFile);
	generalO << playerName << endl << playerPos->x << endl << playerPos->y << endl << playerPos->z << endl;
	generalO.close();
	map->exit();
}

void gameCore::createSavefile(string saveName)
{
	HI::createDir("saves/" + saveName + "/");
	HI::createDir("saves/" + saveName + "/chunks/");
	std::ifstream  iFile1("data/defaultSavefile/general.txt", std::ios::binary);
	std::ofstream  oFile1("saves/" + saveName + "/general.txt", std::ios::binary);
	oFile1 << iFile1.rdbuf();
	std::ifstream  iFile2("data/defaultSavefile/terrainTable.txt", std::ios::binary);
	std::ofstream  oFile2("saves/" + saveName + "/terrainTable.txt", std::ios::binary);
	oFile2 << iFile2.rdbuf();
	iFile1.close();
	iFile2.close();
	oFile1.close();
	oFile2.close();

}

void gameCore::loadSavefile(string saveID)
{
	map = new gameMap(saveID);
	saveName = saveID;
}

gameCore::gameCore()
{
	EntityWorld = new ex::EntityX();
	kDown = 0;
	kUp = 0;
	kHeld = 0;
	playerPos = new point3D;
	map = nullptr;
	exitBool = false;
	tick = 0;
}

gameCore::~gameCore()
{
}

void gameCore::exit()
{
	delete map;
	delete EntityWorld;
	delete playerPos;
}

void gameCore::gameMenu()
{
	struct button
	{
		HI::HITexture pressed;
		HI::HITexture free;
		int sizeX;
		int sizeY;
		int posX;
		int posY;
		bool state;
		button()
		{
			pressed = nullptr;
			free = nullptr;
			sizeX = 0;
			sizeY = 0;
			posX = 0;
			posY = 0;
			state = false;
		}
		void update(point2D touch, unsigned int kDown, unsigned int kHeld)
		{
			if ((kDown | kHeld) & HI::HI_KEY_TOUCH)
			{
				if (touch.x >= posX && touch.x <= posX + sizeX && touch.y >= posY && touch.y <= posY + sizeY)
				{
					state = true;
				}
				else state = false;
			}
			else state = false;
		}
		HI::HITexture getTexture() const
		{
			if (state) return pressed;
			else return free;
		}
	};
	/*
	<Create save>
	<Load save>
	<Options>
	<Quit>
	*/
	HI::setBackgroundColor(RGBA8(53, 159, 35, 0xFF));
	HI::HITexture topImage = HI::loadPngFile("data/sprites/menu_top.png");
	HI::startFrame(HardwareInterface::SCREEN_TOP);
	HI::drawTexture(topImage, 0, 0);
	HI::endFrame();
	HI::swapBuffers();

	HI::HITexture unpressedButton = HI::loadPngFile("data/sprites/unpressed_button.png");
	HI::HITexture  pressedButton = HI::loadPngFile("data/sprites/pressed_button.png");

	button newGame;
	newGame.posX = 85;
	newGame.posY = 60;
	newGame.sizeX = 150;
	newGame.sizeY = 40;
	newGame.pressed = pressedButton;
	newGame.free = unpressedButton;
	newGame.state = false;

	button loadGame;
	loadGame.posX = 85;
	loadGame.posY = 140;
	loadGame.sizeX = 150;
	loadGame.sizeY = 40;
	loadGame.pressed = pressedButton;
	loadGame.free = unpressedButton;
	loadGame.state = false;

	point2D touch;

	while (HI::aptMainLoop()) {
		HI::updateHID();

		HI::updateTouch(touch);
		kDown = HI::getKeysDown();
		kHeld = HI::getKeysHeld();
		kUp = HI::getKeysUp();
		cout << "LMAOOOO" << endl;
		HI::startFrame(HardwareInterface::SCREEN_BOT);

		HI::drawTexture(newGame.getTexture(), newGame.posX, newGame.posY);
		HI::drawTexture(loadGame.getTexture(), loadGame.posX, loadGame.posY);

		HI::endFrame();
		HI::startFrame(HardwareInterface::SCREEN_TOP);
		HI::drawTexture(topImage, 0, 0);
		HI::endFrame();

		HI::swapBuffers();
		if (kDown& HI::HI_KEY_START)return;

		if (newGame.state && (kUp & HI::HI_KEY_TOUCH))
		{
			createSavefile("default");
			loadSavefile("default");
			gameLaunch();
			HI::freeTexture(topImage);
			HI::freeTexture(newGame.getTexture());
			HI::freeTexture(loadGame.getTexture());
			return;
		}
		if (loadGame.state && (kUp & HI::HI_KEY_TOUCH))
		{
			loadSavefile("default");
			gameLaunch();
			HI::freeTexture(topImage);
			HI::freeTexture(newGame.getTexture());
			HI::freeTexture(loadGame.getTexture());
			return;
		}

		loadGame.update(touch, kDown, kHeld);
		newGame.update(touch, kDown, kHeld);

	}
	HI::freeTexture(topImage);
	HI::freeTexture(newGame.getTexture());
	HI::freeTexture(loadGame.getTexture());
}
