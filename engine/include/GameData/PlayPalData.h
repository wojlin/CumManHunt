#ifndef PLAYPALDATA_H
#define PLAYPALDATA_H

#include <map>
#include <math.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

#include "Interfaces.h"
#include "WadStructure.h"

struct color_t
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};


class PlayPalData: public baseResourceWAD
{
    public:

        PlayPalData();

        PlayPalData(WADStructure *wad);

        color_t getColor(int, uint8_t);

        void printColor(int, uint8_t);

        int getPalletesAmount();

        void printInfo();

    private:
        string filePath;
        WADStructure::lumpInfo_t playPalLump;

        int PALLETE_COLORS = 255;
        int PALLETES_AMOUNT;

        vector<vector<color_t>> pallete;

        void readPlayPal();
};

#endif