#include "gameMap.h"
#include "common.h"

using namespace std;



point3D gameMap::getChunk(point3D pos)
{
	point3D b;
	b.x = floor(pos.x / CHUNK_SIZE);
	b.y = floor(pos.y / CHUNK_SIZE);
	b.z = floor(pos.z / CHUNK_SIZE);
	return b;
}

unsigned char* gameMap::getBlock(point3D posBlock) {
	point3D chunkPos = getChunk(posBlock);
	int chunkID = getChunkID(chunkPos);
	return &terrainMap[chunkID][posBlock.x - (chunkPos.x * CHUNK_SIZE)][posBlock.y - (chunkPos.y * CHUNK_SIZE)][posBlock.z - (chunkPos.z * CHUNK_SIZE)];
}


void gameMap::putBlock(int block, point3D posBlock)
{
	*getBlock(posBlock) = block;
}

void gameMap::createMapAndLoad(unsigned char*** map, point3D c) {
	//cout<< "intento crear un chunk a " << chunkX << chunkY << chunkZ << endl;
	if (c.z < FLOOR_HEIGHT / CHUNK_SIZE) {
		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				for (int k = 0; k < CHUNK_SIZE; k++) {
					map[i][j][k] = 1;
				}
			}
		}
	}
	else if (c.z == FLOOR_HEIGHT / CHUNK_SIZE) {
		for (int n = 0; n < CHUNK_SIZE; n++) {
			for (int m = 0; m < CHUNK_SIZE; m++) {
				for (int l = 0; l < CHUNK_SIZE; l++) {
					if (n < CHUNK_SIZE / 2) {
						map[l][m][n] = 1;
					}
					else if (n == CHUNK_SIZE / 2) {
						int bloc = rand() % 7;
						if (bloc == 1) { bloc++; }
						else bloc = 0;
						map[l][m][n] = bloc;
					}
					else {
						map[l][m][n] = 0;
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				for (int k = 0; k < CHUNK_SIZE; k++) {
					map[i][j][k] = 0;
				}
			}
		}
	}
}
int gameMap::chunkValue(point3D chunkN, point3D chunkO) {
	int res = (pow(chunkN.x - chunkO.x, 2) + pow(chunkN.y - chunkO.y, 2) + pow(chunkN.z - chunkO.z, 8));
	return res;
}
int gameMap::freeChunkID() { //returns the position inside texTable[] of the first free texture space
	for (int i = 0; i < CHUNK_NUM; i++) {
		if (mapIndex[i].x == -1) {
			return i;
		}
	}
	//cout<< "NO FREE SPACE IN MAPINDEX" << endl;
	return -1;
}
int gameMap::getChunkID(point3D p) { //returns the position inside mapIndex of the aforementioned chunk;
	for (int i = 0; i < CHUNK_NUM; i++) {
		if (p.x == mapIndex[i].x) {
			if (p.y == mapIndex[i].y) {
				if (p.z == mapIndex[i].z) {
					return i;
				}
			}
		}
	}
	return -1;
}
int gameMap::getBlocksChunkID(point3D b) { //returns the position inside mapIndex of the aforementioned chunk;
	point3D p = getChunk(b);
	for (int i = 0; i < CHUNK_NUM; i++) {
		if (p.x == mapIndex[i].x) {
			if (p.y == mapIndex[i].y) {
				if (p.z == mapIndex[i].z) {
					return i;
				}
			}
		}
	}
	return -1;
}
bool gameMap::isChunkLoaded(point3D p) { // tells if said chunk is loaded in mapIndex[]
	for (int i = 0; i < CHUNK_NUM; i++) {
		if (mapIndex[i].z == p.z) {
			if (mapIndex[i].x == p.x) {
				if (mapIndex[i].y == p.y) {
					return 1;
				}
			}
		}
	}
	return 0;
}
void gameMap::saveChunk(point3D c) { //unloads a chunk from memory and saves it in its file
	cout << "SAVING" << endl;
	//cout<< "saving chunk " << chunkX << chunkY << chunkZ << endl;
	int chunkPos = getChunkID(c);
	if (chunkPos == -1) {
		cout << "chunk is already unloaded" << endl;
		return;
	}
	ofstream chunkFile;
	string terrainName = ("saves/" + saveName + "/chunks/terrain." + get_string(c.x) + '.' + get_string(c.y) + '.' + get_string(c.z));
	chunkFile.open(terrainName, ios_base::binary);
	if (!chunkFile.is_open()) {
		//cout<< "couldn't open file: " << terrainName << endl;
	}
	char current = 255;
	int num = 0;
	stringstream sstream;
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				if ((int)terrainMap[chunkPos][k][j][i] != current) {
					sstream << (int)current << ' ' << num << ' ';
					current = (int)terrainMap[chunkPos][k][j][i];
					num = 1;
				}
				else {
					num++;
					terrainMap[chunkPos][k][j][i] = 1;
				}
			}
		}
	}
	sstream << (int)current << ' ' << num << ' ';
	chunkFile << sstream.rdbuf();
	chunkFile.flush();
	chunkFile.close();

	mapIndex[chunkPos].x = -1;
	mapIndex[chunkPos].y = -1;
	mapIndex[chunkPos].z = -1;
	cout << " END" << endl;
	//cout<< "                 SAVE " << (float)(clock() - t) / CLOCKS_PER_SEC * 1000 << endl;
	//cout<< "done saving" << endl;
}
void gameMap::freeAChunk(point3D playerPos) {
	point3D playerChunk = getChunk(playerPos);
	for (int i = 0; i < CHUNK_NUM; i++) {
		point3D chunkN = mapIndex[i];
		if (chunkValue(chunkN, playerChunk) > 5) {
			saveChunk(chunkN);
			//cout<< "                 FREE " << (float)(clock() - t) / CLOCKS_PER_SEC * 1000 << endl;
			return;
		}
	}
}

void gameMap::freeAllChunks()
{
	for (int i = 0; i < CHUNK_NUM; i++) {
		point3D chunkN = mapIndex[i];
		if (chunkN.x != -1) {
			saveChunk(chunkN);
		}
	}
}

void gameMap::loadChunk(point3D c, point3D playerPos) { //Loads a certain chunk inside mapIndex[] and terrainMap[][][][];
	int chunkPos = freeChunkID();
	if (chunkPos == -1) {
		freeAChunk(playerPos);
		chunkPos = freeChunkID();
	}
	mapIndex[chunkPos].x = c.x;
	mapIndex[chunkPos].y = c.y;
	mapIndex[chunkPos].z = c.z;
	ifstream chunkFile;
	string terrainName = ("saves/" + saveName + "/chunks/terrain." + get_string(c.x) + '.' + get_string(c.y) + '.' + get_string(c.z));
	chunkFile.open(terrainName, ios_base::binary);
	if (!chunkFile.is_open()) {
		cout << "CREATING";
		//cout<< "couldn't open file: " << terrainName << endl;
		createMapAndLoad(terrainMap[chunkPos], c);
		cout << " END" << endl;
		return;
		//chunkFile.open(terrainName, ios_base::binary);
	}
	cout << "LOADING";
	int currentTerrain;
	int amountTerrain;
	stringstream sstream;
	sstream << chunkFile.rdbuf();
	sstream >> currentTerrain >> amountTerrain;
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				if (amountTerrain == 0) {
					sstream >> currentTerrain >> amountTerrain;
				}
				terrainMap[chunkPos][k][j][i] = currentTerrain;
				amountTerrain--;
			}
		}
	}
	int emptyChunkPos = freeChunkID();
	chunkFile.close();
	ifstream entitiesFile;
	string entitiesName = ("saves/" + saveName + "/entities." + get_string(c.x) + '.' + get_string(c.y) + '.' + get_string(c.z));
	entitiesFile.open(entitiesName);
	if (!entitiesFile.is_open()) {
		//cout<< "couldn't open file: " << entitiesName << endl;
	}
	if (entitiesFile.is_open()) {
		while (!entitiesFile.eof()) {
			entitiesFile >> entityList[emptyChunkPos].spriteName >> entityList[emptyChunkPos].visible >> entityList[emptyChunkPos].solid >> entityList[emptyChunkPos].pos.x >> entityList[emptyChunkPos].pos.y >> entityList[emptyChunkPos].pos.z;

			emptyChunkPos++;
			if (emptyChunkPos >= ENTITY_LIST_SIZE) {
				//cout<< "No entity space available";
			}
		}
	}
	cout << " END" << endl;
	//cout<< "                 LOAD " << (float)(clock() - t) / CLOCKS_PER_SEC * 1000 << endl;
}
void gameMap::loadTerrainTable() {
	ifstream terrainTable;
	string terrainName = ("saves/" + saveName + "/terrainTable.txt");
	terrainTable.open(terrainName);
	if (!terrainTable.is_open()) {
		//cout<< "error opening terrainTable" << endl;

		return;
	}
	int i = 0;
	while (!terrainTable.eof()) {
		terrainTable >> terrainList[i].textureFile >> terrainList[i].visible >> terrainList[i].solid;
		terrainListSize++;
		i++;
	}
	terrainTable.close();
}
void gameMap::loadNewChunk(point3D playerPos) {
	point3D playerChunk = getChunk(playerPos);

	for (int i = -1; i < 5; i++)
	{
		for (int j = -1; j < 5; j++)
		{
			for (int k = -1; k < 5; k++)
			{
				point3D chunkPos;
				chunkPos.x = playerChunk.x + i;
				chunkPos.y = playerChunk.y + j;
				chunkPos.z = playerChunk.z + k;
				if (chunkPos.x >= 0 && chunkPos.y >= 0 && chunkPos.z >= 0) {
					if (chunkValue(chunkPos, playerChunk) <= 5) {
						if (isChunkLoaded(chunkPos) == 0) {
							//cout<< "trying to load chunk " << chunkPos.x << chunkPos.y << chunkPos.z << endl;
							if (freeChunkID() == -1) {
								freeAChunk(playerPos);
								if (freeChunkID() == -1) {
									return;
								}
							}
							loadChunk(chunkPos, playerPos);
							return;
						}
					}
				}
			}
		}
	}
}
bool gameMap::simpleCollision(int posX, int posY, int posZ, mode collisionMode) {	//Tells if terrain at position is occupied
	point3D b;
	b.x = posX;
	b.y = posY;
	b.z = posZ;
	//switch case pels tipus de modes
	if (terrainList[*getBlock(b)].solid == 1) {
		return 1;
	}
	return 0;
}
bool gameMap::simpleCollision(point3D p, mode collisionMode) {	//Tells if terrain at position is occupied

	//switch case pels tipus de modes
	if (terrainList[*getBlock(p)].solid == 1) {
		return 1;
	}
	return 0;
}
int gameMap::visibleEntity(point3D p) { // returns the position inside entityList if the entered 3d position contains a visible entity
	for (int i = 0; i < ENTITY_LIST_SIZE && entityList[i].pos.x != -1; i++) {
		if (entityList[i].visible) {
			if (entityList[i].pos.x == p.x) {
				if (entityList[i].pos.y == p.y) {
					if (entityList[i].pos.z == p.z) {
						return i;
					}
				}
			}
		}
	}
	return -1;
}
bool gameMap::isVisible(point3D p, mode mode_t) { //
	point3D b;
	b.x = floor(p.x / CHUNK_SIZE);
	b.y = floor(p.y / CHUNK_SIZE);
	b.z = floor(p.z / CHUNK_SIZE);
	int chunkPosition = getChunkID(b);
	if (chunkPosition == -1) {
		return (visibleEntity(p) != -1);
	}

	switch (mode_t) {
	case TRRN:
		return terrainList[terrainMap[chunkPosition][p.x - b.x * CHUNK_SIZE][p.y - b.y * CHUNK_SIZE][p.z - b.z * CHUNK_SIZE]].visible;	  // AIXO ES ULTRA LLEIG
		break;
	case NTT:
		return (visibleEntity(p) != -1);
		break;
	case PRRT:
		return (isVisible(p, TRRN)) ? 1 : isVisible(p, NTT);
		break;
	}
}
int gameMap::getTerrainListSize() {
	return terrainListSize;
}
bool gameMap::isVisible(int n) {
	return terrainList[n].visible;
}
string gameMap::getTextureName(int n) {
	return terrainList[n].textureFile;
}

int gameMap::getTerrainListPos(point3D p) {
	point3D b;
	b.x = floor(p.x / CHUNK_SIZE);												   //AKESTA FUNCIO ES EL PUTO SIDA, I DEMOSTRA QUE HI HA MOLTA COSA A CANVIAR, MOLTISSIMA
	b.y = floor(p.y / CHUNK_SIZE);
	b.z = floor(p.z / CHUNK_SIZE);
	int chunkPosition = getChunkID(b);
	if (chunkPosition == -1) {
		cout << "NO S'HA TROBAT EL CHUNK" << endl;
	}
	return terrainMap[chunkPosition][p.x - b.x * CHUNK_SIZE][p.y - b.y * CHUNK_SIZE][p.z - b.z * CHUNK_SIZE];
}

string gameMap::getTerrainName(point3D p)
{
	return terrainList[getTerrainListPos(p)].textureFile;
}

bool gameMap::getTerrainVisible(point3D p)
{
	return terrainList[getTerrainListPos(p)].visible;
}

bool gameMap::getTerrainSolid(point3D p)
{
	return terrainList[getTerrainListPos(p)].solid;
}

std::string gameMap::getEntityName(point3D p)
{
	return (entityList[visibleEntity(p)].spriteName);
}

bool gameMap::getEntityVisible(point3D p)
{
	return (entityList[visibleEntity(p)].visible);
}

bool gameMap::getEntitySolid(point3D p)
{
	return (entityList[visibleEntity(p)].solid);
}

gameMap::gameMap() {
	terrainListSize = 0;
	saveName = "default";
	terrainMap = new unsigned char***[CHUNK_NUM];
	for (int i = 0; i < CHUNK_NUM; i++) {
		terrainMap[i] = new unsigned char**[CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			terrainMap[i][j] = new unsigned char*[CHUNK_SIZE];
			for (int k = 0; k < CHUNK_SIZE; k++) {
				terrainMap[i][j][k] = new unsigned char[CHUNK_SIZE];
				for (int l = 0; l < CHUNK_SIZE; l++) {
					terrainMap[i][j][k][l] = 1;
				}
			}
		}
	}
}