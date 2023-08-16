#ifndef SOUNDDATA_H
#define SOUNDDATA_H

#include <map>
#include <math.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

#include "Interfaces.h"
#include "WadStructure.h"

namespace Sound
{

    class Sound
    {
        public:
        
            Sound(string formatValue, string nameString, int sampleRateValue, int numberOfSamplesValue, vector<uint8_t> samplesVector);

            int getSampleRate();

            int getNumberOfSamples();

            vector<uint8_t> getSamples();

            void printInfo();

            void play();

        private:
            string format;
            string name;
            vector<uint8_t> samples;
            uint16_t sampleRate;
            uint32_t numberOfSamples;

    };



    class SoundData: public baseResourceWAD
    {
        public:

            SoundData();

            SoundData(WADStructure::WADStructure *wad);

            Sound readSound(string name);

        private:
            string filePath;
            WADStructure::WADStructure *wadStructure;
    };
}

#endif