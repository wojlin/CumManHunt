#ifndef AUDIOINFODATA_H
#define AUDIOINFODATA_H

#include <map>
#include <math.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

#include "Interfaces.h"
#include "WadStructure.h"


struct voiceData_t
{
    uint8_t modulatorMulti;
    uint8_t modulatorAttackRate;
    uint8_t modulatorSustainLevel;
    uint8_t modulatorWaveformSelect;
    uint8_t modulatorKeyScaleLevel;
    uint8_t modulatorOutputLevel;
    uint8_t feedback;
    uint8_t carrierMulti;
    uint8_t carrierAttackRate;
    uint8_t carrierSustainLevel;
    uint8_t carrierWaveformSelect;
    uint8_t carrierKeyScaleLevel;
    uint8_t carrierOutputLevel;
    uint8_t unused;
    uint16_t noteOffset;
};

struct midiData_t
{
    uint16_t flags;
    uint8_t tuning;
    uint8_t note;
    voiceData_t voice1;
    voiceData_t voice2;
};


struct DMXData_t
{
    uint16_t patch;
    uint8_t column256K;
    uint8_t column512K;
    uint8_t column768K;
    uint8_t column1024K;
    string patchName;
};

class AudioInfoData: public baseResourceWAD
{
    public:

        AudioInfoData();

        AudioInfoData(WADStructure::WADStructure *wad);

        void printInfo();

        vector<midiData_t> getMIDIData();

        midiData_t getMIDIDataByIndex(int index);

        vector<DMXData_t> getDMXData();

    private:
        string filePath;
        WADStructure::lumpInfo_t GENMIDILump;
        WADStructure::lumpInfo_t DMXGUSLump;

        int INSTRUMENTS_AMOUNT = 175;
        

        vector<midiData_t> midiData;
        vector<char*> instrumentsData;


        vector<DMXData_t> dmxData;

        void readGENMIDIData();

        int safe_stoi(const std::string& str);

        void trim(std::string& str);

        void readDMXGUSData();
};

#endif