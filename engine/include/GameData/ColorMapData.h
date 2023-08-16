#ifndef COLORMAP_H
#define COLORMAP_H

#include <map>
#include <math.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

#include "Interfaces.h"
#include "WadStructure.h"

namespace ColorMapData
{

    class ColorMapData: public baseResourceWAD
    {
        public:

            ColorMapData();

            ColorMapData(WADStructure::WADStructure *wad);

            uint8_t getColor(int, uint8_t);

            int getColorMapsAmount();

            void printInfo();


        private:
            string filePath;
            WADStructure::lumpInfo_t colormapLump;
            vector<vector<uint8_t>> colormap;

            int COLORMAPS_AMOUNT;
            int COLORMAP_SIZE = 256;

            void readColormap();
    };
}
#endif