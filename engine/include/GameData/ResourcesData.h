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

#include "../../include/GameData/ColorMapData.h"
#include "../../include/GameData/PlayPalData.h"

struct imageColumn_t
{
    int column;
    int rowStart;
    int pixelCount;
    vector<color_t> pixels;
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
        
        Image(uint16_t w,  uint16_t h,  int16_t l,  int16_t t, vector<imageColumn_t> c);

        uint16_t getWidth();

        uint16_t getHeight();

        int16_t getLeftOffset();

        int16_t getRightOffset();

        int16_t getTopOffset();

        vector<imageColumn_t> getColumns();

        void printInfo();

        void saveAsFile(string filePath);

    private:
        uint16_t width;
        uint16_t height;
        int16_t leftOffset;
        int16_t topOffset;
        vector<imageColumn_t> columns;
};




class ResourcesData: public baseResourceWAD
{
    public:
        
        ResourcesData();

        ResourcesData(WADStructure *wad, PlayPalData*  playpalPointer);

        Image readFlat(string name);

        Image readPatch(string name);

        Image readSprite(string name);

        int getSpritesAmount();

        int getFlatsAmount();

        int getPatchesAmount();

        char* getPNameByIndex(int index);

        int getPNameIndexByName(char* name);

        int getPNamesAmount();

        vector<textureX_t> getTextureX();

        void printInfo();

        

    private:
        string filePath;

        map<std::string, WADStructure::lumpInfo_t*> spritesMap;
        map<std::string, WADStructure::lumpInfo_t*> flatsMap;
        map<std::string, WADStructure::lumpInfo_t*> patchesMap;

        vector<char*> pnames;
        vector<textureX_t> textureX;

        int32_t numMapPatches;

        int spritesAmount = 0;
        int flatsAmount = 0;
        int patchesAmount = 0;

        PlayPalData* playpal;


        void readTextureX(WADStructure::lumpInfo_t lump);
        
        void readPNames(WADStructure *wad);

        void readSprites(WADStructure *wad);

        void readFlats(WADStructure *wad);

        void readPatches(WADStructure *wad);

};

#endif