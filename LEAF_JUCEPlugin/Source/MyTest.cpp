/*
 ==============================================================================
 FM.c
 Created: 23 Jan 2017 9:39:38am
 Author:  Michael R Mulshine
 ==============================================================================
 */

#include "LEAFTest.h"
#include "MyTest.h"


static void leaf_pool_report(void);
static void leaf_pool_dump(void);
static void run_pool_test(void);

tNoise noise;
tSVF bp1;
tSVF bp2;
tFormantShifter fs;

tSampler samp;
tBuffer buff;
tEnvelope env;

tAutotune at;

tTriangle tri;

tMinBLEP minblep;

tPhasor phasor;

float gain;
float freq;
float dtime;
bool buttonState;
int ratio = 2;
float x = 0.0f;
float y = 0.0f;
float a, b, c, d;

#define MSIZE 500000
char memory[MSIZE];

void    LEAFTest_init            (float sampleRate, int blockSize)
{
    LEAF_init(sampleRate, blockSize, memory, MSIZE, &getRandomFloat);
    
    tTriangle_init(&tri);
    
    tMinBLEP_init(&minblep);
    
    tPhasor_init(&phasor);
    //    tNoise_init(&noise, WhiteNoise);
    //
    //    tAutotune_init(&at, 1, 1024, 512);
    
    //    tSVF_init(&bp1, SVFTypeBandpass, 100, 4.0f);
    //    tSVF_init(&bp2, SVFTypeBandpass, 1000, 4.0f);
    //
    //    tFormantShifter_init(&fs, 20);
    //
    //    // Init and set record
    //    tBuffer_init (&buff, leaf.sampleRate); // init, 1 second buffer
    //    tBuffer_setRecordMode (&buff, RecordOneShot); // RecordOneShot records once through
    //
    //    // Init and set play
    //    tSampler_init (&samp, &buff); // init, give address of record buffer
    //    tSampler_setMode (&samp, PlayLoop); //set in Loop Mode
    //    tSampler_setRate(&samp, 1.763f); // Rate of 1.0
}

inline double getSawFall(double angle) {
    
    angle = fmod(angle + double_Pi, 2*double_Pi); // shift x
    double sample = angle/double_Pi - double(1); // computer as remainder
    
    return sample;
    
}

float   LEAFTest_tick            (float input)
{
    //    float sample = tNoise_tick(&noise);
    //    sample *= 0.5f;
    //    float b = tSVF_tick(&bp1, sample);
    //    b += tSVF_tick(&bp2, sample);
    //
    //    return (tFormantShifter_tick(&fs, input));
    //
    //    tBuffer_tick(&buff, input);
    
    //    return tSampler_tick(&samp);
    
    //    tAutotune_setFreq(&at, 440.0f, 0);
    
    //    return tAutotune_tick(&at, input)[0];
    
    tPhasor_setFreq(&phasor, y);
    
    
    float sample = tPhasor_tick(&phasor) * 2.0f - 1.0f;
    
    if (phasor->phaseDidReset)
    {
        float offset = 1.0f - ((phasor->inc - phasor->phase) / phasor->inc);
        tMinBLEP_addBLEP(&minblep, offset, 2, 0.0f);
    }
    
    return tMinBLEP_tick(&minblep, sample) - phasor->inc * 2.0f;
}

int firstFrame = 1;
bool lastState = false, lastPlayState = false;
void    LEAFTest_block           (void)
{
    //    if (firstFrame == 1)
    //    {
    //        tBuffer_record(&buff); // starts recording
    //        tSampler_play(&samp); // start spitting samples out
    //        firstFrame = 0;
    //    }
    
    float val = getSliderValue("mod freq");
    
    x = val ;
    
    //    a = val * tBuffer_getBufferLength(&buff);
    
    DBG("start: " + String(a));
    
    val = getSliderValue("mod depth");
    
    y = val * 20000.0f + 20.0f;
    
    DBG(String(y));
    //    b = val * tBuffer_getBufferLength(&buff);
    
    DBG("rate: " + String(b));
    //
    //    tSampler_setStart(&samp, a);
    //    tSampler_setEnd(&samp, b);
    //    tSampler_setRate(&samp, b);
    
    //    tFormantShifter_setShiftFactor(&fs, x);
    //    tFormantShifter_setIntensity(&fs, y);
    
}

void    LEAFTest_controllerInput (int cnum, float cval)
{
    
}

void    LEAFTest_pitchBendInput  (int pitchBend)
{
    
}

int lastNote;
void    LEAFTest_noteOn          (int note, float velocity)
{
}


void    LEAFTest_noteOff         (int note)
{
}



void    LEAFTest_end             (void)
{
    
}

// LEAF POOL UTILITIES

void leaf_pool_report(void)
{
    DBG(String(leaf_pool_get_used()) + " of  " + String(leaf_pool_get_size()));
}

void leaf_pool_dump(void)
{
    float* buff = (float*)leaf_pool_get_pool();
    unsigned long siz = leaf_pool_get_size();
    siz /= sizeof(float);
    for (int i = 0; i < siz; i++)
    {
        DBG(String(buff[i]));
    }
}

static void run_pool_test(void)
{
    leaf_pool_report();
    
    DBG("ALLOC BUFFER 1");
    int size = 50;
    float* buffer;
    buffer = (float*) leaf_alloc(sizeof(float) * size);
    
    for (int i = 0; i < size; i++)
    {
        buffer[i] = (float)i;
        
    }
    
    leaf_pool_report();
    
    DBG("ALLOC BUFFER 2");
    size = 25;
    
    buffer = (float*) leaf_alloc(sizeof(float) * size);
    
    leaf_pool_report();
    
    for (int i = 0; i < size; i++)
    {
        buffer[i] = (float)(i*2);
    }
    leaf_free(buffer);
    
    leaf_pool_report();
    
    DBG("ALLOC BUFFER 3");
    size = 15;
    
    buffer = (float*) leaf_alloc(sizeof(float) * size);
    
    for (int i = 0; i < size; i++)
    {
        buffer[i] = (float)(i*3);
    }
    
    leaf_pool_report();
    
    leaf_pool_dump();
}
