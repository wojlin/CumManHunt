#ifndef RESOURCESDATA_H
#define RESOURCESDATA_H

#include <map>
#include <math.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

#include "Interfaces.h"
#include "WadStructure.h"
#include "../Utils/BMPImage.h"
#include "../Utils/Info.h"
#include "../Utils/Exceptions.h"

#include "../../include/GameData/ColorMapData.h"
#include "../../include/GameData/PlayPalData.h"


namespace ResourcesData
{

    struct imageColumn_t
    {
        int column;
        int rowStart;
        int pixelCount;
        vector<uint8_t> pixels;
    };



    struct mappatch_t 
    {
        short originX;
        short originY;
        short patch;
        short stepDir;
        short colorMap;
    };

    struct maptexture_t  
    {
        string name; 
        bool masked;
        short width;
        short height;
        uint32_t columnDirectory;
        uint16_t patchCount;
        vector<mappatch_t> mapPatches;
    };

    struct textureX_t
    {
        uint32_t numTextures;
        vector<maptexture_t> mapTextures;
    };



    class Image
    {
        public:

            Image();
            
            Image(PlayPalData::PlayPalData*  playpalPointer, uint16_t w,  uint16_t h,  int16_t l,  int16_t t, vector<imageColumn_t> c, int lSize, string name);

            uint16_t getWidth();

            int getSize();

            uint16_t getHeight();

            int16_t getLeftOffset();

            int16_t getRightOffset();

            int16_t getTopOffset();

            vector<imageColumn_t> getColumns();

            void printInfo();

            void saveAsFile(string filePath);

            string getName();

            void setPallete(int value);

            int getPallete();

            PlayPalData::colorRGB_t getPixel(int x, int y);

        private:
            uint16_t width;
            uint16_t height;
            int16_t leftOffset;
            int16_t topOffset;
            int size;
            string name;
            int pallete;
            vector<imageColumn_t> columns;
            PlayPalData::PlayPalData*  playpal;
            vector<vector<PlayPalData::colorRGB_t>> pixels;
    };




    class ResourcesData: public baseResourceWAD
    {
        public:
            
            ResourcesData();

            ResourcesData(WADStructure::WADStructure *wad, PlayPalData::PlayPalData*  playpalPointer);

            Image readFlat(string name);

            Image readPatch(string name);

            Image readSprite(string name);

            Image readGameSprite(string name);

            Image readGameFlat(string name);

            Image readGamePatch(string name);

            int getSpritesAmount();

            int getFlatsAmount();

            int getPatchesAmount();

            string getPNameByIndex(int index);

            int getPNameIndexByName(string name);

            int getPNamesAmount();

            vector<textureX_t> getTextureX();

            void printInfo();

            

        private:
            WADStructure::WADStructure *wadStructure;
            string filePath;

            map<std::string, WADStructure::lumpInfo_t*> spritesMap;
            map<std::string, WADStructure::lumpInfo_t*> flatsMap;
            map<std::string, WADStructure::lumpInfo_t*> patchesMap;

            vector<string> pnames;
            vector<textureX_t> textureX;

            int32_t numMapPatches;

            int spritesAmount = 0;
            int flatsAmount = 0;
            int patchesAmount = 0;

            PlayPalData::PlayPalData* playpal;

            string charsToString(char* chars, int size);

            string toUpper(string name);

            void readTextureX(WADStructure::lumpInfo_t lump);
            
            void readPNames(WADStructure::WADStructure *wad);

            void readSprites(WADStructure::WADStructure *wad);

            void readFlats(WADStructure::WADStructure *wad);

            void readPatches(WADStructure::WADStructure *wad);

    };
}
#endif