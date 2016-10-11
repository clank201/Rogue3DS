#ifdef _VITA
#include "../include/core.h"
#include "../include/HardwareInterface.h"
#include <vita2d.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/display.h>
//#include <pthread.h>
void HI::systemInit() {
	vita2d_init();
}

void HI::systemFini() {
	vita2d_fini();
	sceKernelExitProcess(0);
}

void HI::consoleInit() {
}

void HI::consoleFini() {
};

void HI::startFrame(HI_SCREEN screen) {
	vita2d_start_drawing();
	vita2d_clear_screen();
}

short HI::getRComponent(HIColor color) {
	return((short)color) & 0b0000000011111111;
}

short HI::getGComponent(HIColor color) {
	return((short)color >> 8) & 0b0000000011111111;
}
short HI::getBComponent(HIColor color) {
	return ((short)color >> 16) & 0b0000000011111111;
}
short HI::getAComponent(HIColor color) {
	return((short)color >> 24) & 0b0000000011111111;
}

void HI::setBackgroundColor(HIColor color) {
	vita2d_set_clear_color(color);
}

HI::HITexture HI::loadPngFile(std::string path) {
	return (HI::HITexture)vita2d_load_PNG_file(path.c_str());
}
HI::HITexture HI::loadBmpFile(std::string path) {
	return (HI::HITexture)vita2d_load_BMP_file(path.c_str());
}

void HI::drawTexture(HI::HITexture texture, int posX, int posY) {
	vita2d_draw_texture((vita2d_texture*)texture, posX, posY);
}

void HI::drawTextureRotate(HI::HITexture texture, int posX, int posY, float angle) {
	vita2d_draw_texture_rotate((vita2d_texture*)texture, posX, posY, angle);
}

void HI::drawTexturePart(HI::HITexture texture, int startX, int startY, int posX, int posY, int sizeX, int sizeY) {
	vita2d_draw_texture_part((vita2d_texture*)texture, posX, posY, startX, startX, sizeX, sizeY);
}

void HI::mergeTextures(HITexture origin, HITexture destination, short posX, short posY) {			  //BORKEN
}

void HI::drawRectangle(int posX, int posY, int width, int height, HI::HIColor color) {
	vita2d_draw_rectangle(posX, posY, width, height, color);
}

void HI::freeTexture(HITexture texture) {
	vita2d_free_texture((vita2d_texture*)texture);
}
HI::HITexture HI::createTexture(int sizeX, int sizeY) {
	vita2d_create_empty_texture(sizeX, sizeY);
}

void HI::endFrame() {
	vita2d_end_drawing();
}

void HI::swapBuffers() {
	vita2d_swap_buffers();
}

std::string HI::getDataPath() {
	return "ux0:/data/RogueVITA/data/";
}

std::string HI::getSavesPath() {
	return "ux0:/data/RogueVITA/saves/";
}

bool HardwareInterface::createDir(std::string path) {  		  //BORKEN
	return false;
}
bool HardwareInterface::copyFile(std::string input, std::string output) { 		  //DIRTY BUT NOT BORKEN
	std::ifstream  iFile(input, std::ios::binary);
	std::ofstream  oFile(output, std::ios::binary);
	oFile << iFile.rdbuf();
	oFile.close();
	iFile.close();
}

int HI::getScreenHeight() {
	return 544;
}
int HI::getScreenWidth() {
	return 960;
}

HardwareInterface::HI_CONSOLE HI::getConsole() {
	return CONSOLE_PSVITA;
}

void HI::createThread(void* entrypoint, void* arg, size_t stack_size, int prio, int affinity, bool detached) {			  //SUPA BORKEN
	//sceKernelCreateThread("test", (SceKernelThreadEntry)entrypoint, 0x00000100, stack_size,(SceUInt)arg, 0, NULL);
}

void HI::updateTouch(point2D &touch) {			  //BORKEN
	SceCtrlData pad;
	memset(&pad, 0, sizeof(pad));
	sceCtrlPeekBufferPositive(0, &pad, 1);
	//
}
void HI::updateHID() {
}

int HI::getKeysUp() {  		  //BORKEN
	return 0;
}
int HI::getKeysHeld() {
	SceCtrlData pad;
	memset(&pad, 0, sizeof(pad));
	sceCtrlPeekBufferPositive(0, &pad, 1);
	HI::HI_KEYS keys = HI_KEY_B;
	if (pad.buttons & SCE_CTRL_DOWN) keys =(HI_KEYS) ((int)keys | HI::HI_KEY_DOWN);
	if (pad.buttons & SCE_CTRL_LEFT)keys = (HI_KEYS)((int)keys | HI::HI_KEY_LEFT);
	if (pad.buttons & SCE_CTRL_RIGHT)keys = (HI_KEYS)((int)keys | HI::HI_KEY_RIGHT);
	if (pad.buttons & SCE_CTRL_UP)keys = (HI_KEYS)((int)keys | HI::HI_KEY_UP);
	if (pad.buttons & SCE_CTRL_START)keys = (HI_KEYS)((int)keys | HI::HI_KEY_START);

	return keys;
}
int HI::getKeysDown() {				//SEMIBORKEN
	return HI::getKeysHeld();
}

void HI::getCirclePadPos(point2D &circle, HI_CIRCLEPAD circlePadID) { 		  //BORKEN
}

void HI::sleepThread(unsigned long ns) {		  //BORKEN
}


void* HI::linearAllocator(size_t size) {		  //BORKEN
	return (void*)&size;
}

//Sound
void HI::dspSetOutputMode(dspOutputMode mode) {				  //BORKEN
}
void HI::dspChnSetInterp(int id, HI::dspInterpType type) {	  		  //BORKEN
}
void HI::dspChnSetFormat(int id, unsigned short format) {  		  //BORKEN
}
void HI::dspChnSetRate(int id, float rate) {	   		  //BORKEN
}
void HI::dspChnSetMix(int id, float mix[12]) {	   		  //BORKEN
}
void HI::dspChnWaveBufAdd(int id, HI::dspWaveBuf* buf) {	   		  //BORKEN
}

//??
void HI::DSP_FlushDataCache(const void* address, unsigned int size) {			  //BORKEN
}

void HI::gspWaitForEvent(HardwareInterface::GSPGPU_Event id, bool nextEvent) {	  		  //BORKEN
}

void HI::waitForVBlank() {
	sceDisplayWaitVblankStart();
}

bool HI::aptMainLoop() {
}
#endif