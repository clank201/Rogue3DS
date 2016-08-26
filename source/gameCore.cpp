#include <iostream>
#include <string>
#include <fstream>
#include "gameCore.h"
#include "core.h"
#include "3ds.h"
#include "sf2d.h"
#include "sfil.h"
#include "sftd.h"
#include "FreeSans_ttf.h"

using namespace std;

/**
 * \brief Moves currentEntity in the dir direction, if possible
 * \param currentEntity Entity that will be moved
 * \param dir Direction to move the entity
 */
void gameCore::moveEntity(entity &currentEntity, direction dir) const
{
	switch (dir) {
	case DOWN:
		if (map->isChunkLoaded(map->getChunk(currentEntity.pos.x, currentEntity.pos.y, currentEntity.pos.z - 1)) && map->simpleCollision(currentEntity.pos.x, currentEntity.pos.y, currentEntity.pos.z - 1) == 0) {
			currentEntity.pos.z--;
		}
		break;
	case UP:
		if (map->isChunkLoaded(map->getChunk(currentEntity.pos.x, currentEntity.pos.y, currentEntity.pos.z + 1)) && map->simpleCollision(currentEntity.pos.x, currentEntity.pos.y, currentEntity.pos.z + 1) == 0 && map->simpleCollision(currentEntity.pos.x, currentEntity.pos.y, currentEntity.pos.z - 1) == 1) {
			currentEntity.pos.z++;
		}
		break;
	case FRONT:
		if (map->isChunkLoaded(map->getChunk(currentEntity.pos.x, currentEntity.pos.y - 1, currentEntity.pos.z)) && map->simpleCollision(currentEntity.pos.x, currentEntity.pos.y - 1, currentEntity.pos.z) == 0) {
			currentEntity.pos.y--;
		}
		else if (map->isChunkLoaded(map->getChunk(currentEntity.pos.x, currentEntity.pos.y - 1, currentEntity.pos.z + 1)) && (map->simpleCollision(currentEntity.pos.x, currentEntity.pos.y, currentEntity.pos.z + 1) | map->simpleCollision(currentEntity.pos.x, currentEntity.pos.y - 1, currentEntity.pos.z + 1)) == 0) {
			currentEntity.pos.y--;
			currentEntity.pos.z++;
		}
		break;
	case BACK:
		if (map->isChunkLoaded(map->getChunk(currentEntity.pos.x, currentEntity.pos.y + 1, currentEntity.pos.z)) && map->simpleCollision(currentEntity.pos.x, currentEntity.pos.y + 1, currentEntity.pos.z) == 0) {
			currentEntity.pos.y++;
		}
		else if (map->isChunkLoaded(map->getChunk(currentEntity.pos.x, currentEntity.pos.y + 1, currentEntity.pos.z + 1)) && (map->simpleCollision(currentEntity.pos.x, currentEntity.pos.y, currentEntity.pos.z + 1) | map->simpleCollision(currentEntity.pos.x, currentEntity.pos.y + 1, currentEntity.pos.z + 1)) == 0) {
			currentEntity.pos.y++;
			currentEntity.pos.z++;
		}
		break;
	case LEFT:
		if (map->isChunkLoaded(map->getChunk(currentEntity.pos.x - 1, currentEntity.pos.y, currentEntity.pos.z)) && map->simpleCollision(currentEntity.pos.x - 1, currentEntity.pos.y, currentEntity.pos.z) == 0) {
			currentEntity.pos.x--;
		}
		else if (map->isChunkLoaded(map->getChunk(currentEntity.pos.x - 1, currentEntity.pos.y, currentEntity.pos.z + 1)) && (map->simpleCollision(currentEntity.pos.x, currentEntity.pos.y, currentEntity.pos.z + 1) | map->simpleCollision(currentEntity.pos.x - 1, currentEntity.pos.y, currentEntity.pos.z + 1)) == 0) {
			currentEntity.pos.x--;
			currentEntity.pos.z++;
		}
		break;
	case RIGHT:
		if (map->isChunkLoaded(map->getChunk(currentEntity.pos.x + 1, currentEntity.pos.y, currentEntity.pos.z)) && map->simpleCollision(currentEntity.pos.x + 1, currentEntity.pos.y, currentEntity.pos.z) == 0) {
			currentEntity.pos.x++;
		}
		else if (map->isChunkLoaded(map->getChunk(currentEntity.pos.x + 1, currentEntity.pos.y, currentEntity.pos.z + 1)) && (map->simpleCollision(currentEntity.pos.x, currentEntity.pos.y, currentEntity.pos.z + 1) | map->simpleCollision(currentEntity.pos.x + 1, currentEntity.pos.y, currentEntity.pos.z + 1)) == 0) {
			currentEntity.pos.x++;
			currentEntity.pos.z++;
		}
		break;
	default:
		break;
	}
}

void gameCore::updateEntity(entity &currentEntity)
{
	point3D c = currentEntity.pos;
	c.z--;
	if (map->simpleCollision(c) == 0 && currentEntity.fly == 0) {
		//cout<< "que es faci la gravetat " << endl;
		currentEntity.pos.z--;
		if (currentEntity.pos.z < 0) {
			cout << "has caigut del mon, capoll" << endl;
		}
	}
}

void gameCore::updateEntities()
{
	for (int i = 0; i < ENTITY_LIST_SIZE && map->entityList[i].pos.x >= 0; i++)
	{
		updateEntity(map->entityList[i]);
	}
}

void gameCore::handleInput()
{
	if (kHeld & KEY_START) {
		cout << "EXITING..." << endl;
		exitBool = true;
	}
	if (!(kHeld & KEY_RIGHT))
	{
	}
	else
	{
		moveEntity(*player, RIGHT);
	}
	if (kHeld & KEY_LEFT) {
		moveEntity(*player, LEFT);
	}
	if (kHeld & KEY_UP) {
		moveEntity(*player, FRONT);
	}
	if (kHeld & KEY_DOWN) {
		moveEntity(*player, BACK);
	}
	if (kHeld & KEY_A) {
		moveEntity(*player, UP);
	}
}

void gameCore::gameLoop()
{
	hidScanInput();
	kDown = kDown | hidKeysDown();
	kHeld = kHeld | hidKeysHeld();
	kUp = kUp | hidKeysUp();
	if (tick % 12 == 0) {
		updateEntities();
		handleInput();
		graphicsObj.drawFrame();
		sf2d_swapbuffers();
		cout << player->pos.z << endl;
		kDown = hidKeysDown();
		kHeld = hidKeysHeld();
		kUp = hidKeysUp();
	}
	else  gspWaitForVBlank();
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

	sf2d_set_clear_color(RGBA8(0, 0, 0, 0));
	player = map->entityList;
	ifstream general;
	general.open("saves/" + saveName + "/general.txt");
	if (!general.is_open()) {
		cout << "couldn't open file: " << ("saves/" + saveName + "/general.txt") << endl;
	}
	string playerSprite = "player.png", playerName;
	general >> playerName >> player->pos.x >> player->pos.y >> player->pos.z;
	general.close();
	player->spriteName = playerSprite;
	for (int i = 0; i < CHUNK_NUM; i++) {
		map->loadNewChunk(player->pos);
	}
	map->loadTerrainTable();
	graphicsObj.edit(*map, *player);
	graphicsObj.reloadTextures();
	soundObj.playFromFile("data/sounds/bgm/wilderness.ogg");
	map->startChunkLoader(&player->pos);
	tick = 0;
	while (aptMainLoop() && !exitBool) {
		gameLoop();
		tick++;
	}

	string generalFile = "saves/" + saveName + "/general.txt";
	std::remove(generalFile.c_str());
	ofstream generalO(generalFile);
	generalO << playerName << endl << player->pos.x << endl << player->pos.y << endl << player->pos.z << endl;
	generalO.close();

	map->exit();
	soundObj.exit();
	graphicsObj.freeAllTextures();


}

void gameCore::createSavefile(string saveName)
{

	fsCreateDir("saves/" + saveName + "/");
	fsCreateDir("saves/" + saveName + "/chunks/");
	std::ifstream  iFile1("data/gameData/defaultSavefile/general.txt", std::ios::binary);
	std::ofstream  oFile1("saves/" + saveName + "/general.txt", std::ios::binary);
	oFile1 << iFile1.rdbuf();
	std::ifstream  iFile2("data/gameData/defaultSavefile/terrainTable.txt", std::ios::binary);
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
	kDown = 0;
	kUp = 0;
	kHeld = 0;
	player = nullptr;
	map = nullptr;
	exitBool = false;
	tick = 0;
}

gameCore::~gameCore()
{
	delete map;
}

void gameCore::gameMenu()
{
	struct button
	{
		sf2d_texture* pressed;
		sf2d_texture* free;
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
		void update(touchPosition touch, u32 kDown, u32 kHeld)
		{
			if ((kDown | kHeld) & KEY_TOUCH)
			{
				if (touch.px >= posX && touch.px <= posX + sizeX && touch.py >= posY && touch.py <= posY + sizeY)
				{
					state = true;
				}
				else state = false;
			}
			else state = false;
		}
		sf2d_texture* getTexture()
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
	sf2d_set_clear_color(RGBA8(53, 159, 35, 0xFF));
	sf2d_texture* topImage = sfil_load_PNG_file("data/sprites/menu_top.png", SF2D_PLACE_RAM);
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	sf2d_draw_texture(topImage, 0, 0);
	sf2d_end_frame();
	sf2d_swapbuffers();

	sf2d_texture*  unpressedButton = sfil_load_PNG_file("data/sprites/unpressed_button.png", SF2D_PLACE_RAM);
	sf2d_texture*  pressedButton = sfil_load_PNG_file("data/sprites/pressed_button.png", SF2D_PLACE_RAM);

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

	touchPosition touch;
	sftd_font *font = sftd_load_font_mem(FreeSans_ttf, FreeSans_ttf_size);

	while (aptMainLoop()) {
		hidScanInput();

		hidTouchRead(&touch);
		kDown = hidKeysDown();
		kHeld = hidKeysHeld();
		kUp = hidKeysUp();

		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		
		sf2d_draw_texture(newGame.getTexture(), newGame.posX, newGame.posY);
		sf2d_draw_texture(loadGame.getTexture(), loadGame.posX, loadGame.posY);

		sftd_draw_textf(font, 145, 70, RGBA8(0, 0, 0, 255), 10, "NEW");
		sftd_draw_textf(font, 145, 150, RGBA8(0, 0,0, 255), 10, "LOAD");

		sf2d_end_frame();
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(topImage, 0, 0);
		sf2d_end_frame();

		sf2d_swapbuffers();
		if (kDown&KEY_START)return;
		
		if (newGame.state && (kUp & KEY_TOUCH))
		{
			createSavefile("default");
			loadSavefile("default");
			gameLaunch();
			sf2d_free_texture(topImage);
			sf2d_free_texture(newGame.getTexture());
			sf2d_free_texture(loadGame.getTexture());
			return;
		}
		if (loadGame.state && (kUp & KEY_TOUCH))
		{
			loadSavefile("default");
			gameLaunch();
			sf2d_free_texture(topImage);
			sf2d_free_texture(newGame.getTexture());
			sf2d_free_texture(loadGame.getTexture());
			return;
		}

		loadGame.update(touch, kDown, kHeld);
		newGame.update(touch, kDown, kHeld);

	}
	sf2d_free_texture(topImage);
	sf2d_free_texture(newGame.getTexture());
	sf2d_free_texture(loadGame.getTexture());
}