#include <map>
#include <math.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

#include <gtest/gtest.h>


#include "../engine/include/GameData/GameData.h"
#include "../engine/include/GameData/LevelData.h"
#include "../engine/include/GameData/ColorMapData.h"
#include "../engine/include/GameData/PlayPalData.h"
#include "../engine/include/GameData/ResourcesData.h"
#include "../engine/include/GameData/EndoomData.h"
#include "../engine/include/GameData/AudioInfoData.h"
#include "../engine/include/GameData/SoundData.h"
#include "../engine/include/GameData/DemoData.h"

using namespace std;

TEST(GameDataTest, IWadReadoutTest)
{

    GameData gameData = GameData();
    
    gameData.loadIWAD("../tests/iwad_doom2.WAD");
    gameData.compile();

    gameData.printInfo();

    WADStructure::WADStructure* wad = gameData.getResourceFromWAD<WADStructure::WADStructure>();

    WADStructure::header_t header = wad->getHeader();
    
    EXPECT_EQ(header.identification, "IWAD");
    EXPECT_EQ(header.numlumps, 2919);
    EXPECT_EQ(header.infotableofs, 14557880);

}


TEST(GameDataTest, PWadAppendReadoutTest)
{

    GameData gameData = GameData();
    
    gameData.loadIWAD("../tests/iwad_doom2.WAD");
    gameData.loadPWAD("../tests/pwad_append.WAD");
    gameData.compile();

    gameData.printInfo();

    ResourcesData::ResourcesData* resources = gameData.getResourceFromWAD<ResourcesData::ResourcesData>();

    string name = "CZASZKA";
    ResourcesData::Image image = resources->readGameSprite(name);
    image.printInfo();
    
    EXPECT_EQ(image.getHeight(), 500);
    EXPECT_EQ(image.getWidth(), 500);
    EXPECT_EQ(image.getSize(), 121585);

}


TEST(GameDataTest, PWadReplaceReadoutTest)
{

    GameData gameData = GameData();
    
    gameData.loadIWAD("../tests/iwad_doom2.WAD");
    gameData.loadPWAD("../tests/pwad_replace.WAD");
    gameData.compile();

    gameData.printInfo();

    ResourcesData::ResourcesData* resources = gameData.getResourceFromWAD<ResourcesData::ResourcesData>();

    string name = "HELP";
    ResourcesData::Image image = resources->readGameSprite(name);
    image.printInfo();
    EXPECT_EQ(image.getHeight(), 500);
    EXPECT_EQ(image.getWidth(), 500);
    EXPECT_EQ(image.getSize(), 121585);

}