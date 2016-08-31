#include <string.h>
#include <3ds.h>
#include <iostream>
#include <stdio.h>
#include "sound.h"
#include "stb_vorbis.h"


using namespace std;



sound::sound()
{
	for (int i = 0; i < 8; i++)
	{
		channelStatus[i] = false;
	}
	threadStatus = false;
	exitRequest = false;
	waveBuf = new ndspWaveBuf[2];
	memset(waveBuf, 0, 2 * sizeof(ndspWaveBuf));

	ndspSetOutputMode(NDSP_OUTPUT_STEREO);

	ndspChnSetInterp(0, NDSP_INTERP_LINEAR);

	ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);
}

sound::~sound()
{
	exit();
}

void sound::playFromFile(string file)
{

	stb_vorbis_info info;
	int error;
	vorbisFile = stb_vorbis_open_filename(file.c_str(), &error, nullptr);
	if (vorbisFile == nullptr) {
		cout << "Failed to open sound file" << endl;
		return;
	}
	info = stb_vorbis_get_info(vorbisFile);
	Samples = info.sample_rate;



	audioBuffer = (u32*)linearAlloc(Samples * sizeof(s16) * 2);

	fillBlock = 0;



	ndspChnSetRate(0, Samples);
	float mix[12];
	memset(mix, 0, sizeof(mix));
	mix[0] = 1.0;
	mix[1] = 1.0;
	ndspChnSetMix(0, mix);

	memset(waveBuf, 0, sizeof(ndspWaveBuf) * 2);
	waveBuf[0].data_vaddr = &audioBuffer[0];
	waveBuf[0].nsamples = Samples;
	waveBuf[1].data_vaddr = &audioBuffer[Samples];
	waveBuf[1].nsamples = Samples;

	ndspChnWaveBufAdd(0, &waveBuf[0]);
	ndspChnWaveBufAdd(0, &waveBuf[1]);
	threadCreate((ThreadFunc)audioMainThread, this, 5900, 0x30, 0, true);
}

void sound::exit()
{
	exitRequest = true;
	while (threadStatus == true)
	{
		svcSleepThread(10000000);
	}
	delete[] waveBuf;
}

short sound::assignChannel()
{
	for(int i = 0;i<8;i++)
	{
		if(channelStatus[i]==false)
		{
			return i;
		}
	}
	return -1;
}

void sound::audioMainThread(u32 arg)
{
	sound* soundObj = (sound*)arg;
	soundObj->threadStatus = true;
	int samplesLeft = 1;
	while (samplesLeft && !soundObj->exitRequest) {
		if (soundObj->waveBuf[soundObj->fillBlock].status == NDSP_WBUF_DONE) {
			samplesLeft = stb_vorbis_get_samples_short_interleaved(soundObj->vorbisFile, 2, (short*)soundObj->waveBuf[soundObj->fillBlock].data_pcm16, soundObj->Samples * 2);
			DSP_FlushDataCache(&soundObj->waveBuf[soundObj->fillBlock].data_pcm16, soundObj->waveBuf[soundObj->fillBlock].nsamples);
			ndspChnWaveBufAdd(0, &soundObj->waveBuf[soundObj->fillBlock]);
			soundObj->fillBlock++;
			if (soundObj->fillBlock > 1)soundObj->fillBlock = 0;
		}
		svcSleepThread(50000000);
	}
	soundObj->threadStatus = false;
	stb_vorbis_close(soundObj->vorbisFile);

}
