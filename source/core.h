#pragma once

#include <string.h>
#include <sstream>
#include <dirent.h>

#include <fstream>

#include "sf2d.h"


using namespace std;

#define CHUNK_SIZE 64
#define CHUNK_NUM  27 //( 19 = rubik's sense corners (n=2) - corners, 11 = baldufa(n = 2, per� z pondera m�s) , 7 = rubik's core (n = 1))  la n3ds aguanta almenys 80, la old 26
#define ENTITY_LIST_SIZE  100
#define TERRAIN_LIST_MAX_SIZE  100
#define TEX_TABLE_SIZE  30
#define RENDER_HEIGHT 3
#define FLOOR_HEIGHT 100
#define SEA_LEVEL 150

#define TICKS_PER_SEC (268123480)
#define TICKS_PER_FRAME (4468724)

enum mode {
	PRRT,
	TRRN,
	NTT,
	RAY,
	BLCK,
};

enum nttype {
	NPC,
	PLR,
};

enum direction {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	FRONT,
	BACK,
};

enum gameState {
	STATE_MENU,
	STATE_WORLD,
};
struct point3D {
	int x = -1;
	int y = -1;
	int z = -1;
};
struct entity {
	string spriteName = "ENULL";
	bool visible = true;
	bool solid = true;
	point3D pos;
	nttype type = NPC;
	bool fly = false;
};

string get_string(int number);

struct textureName {

	sf2d_texture* texture;

	string name = "";
};

struct terrain {
	string textureFile = "TNULL";
	bool visible = true;
	bool solid = true;
};

bool fsIsDirectory(const std::string path);
bool fsExists(const std::string path);


bool fsCreateDir(const std::string path);