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

#include "../engine/include/Utils/Exceptions.h"

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


TEST(GameDataTest, NotExistLumpTest)
{

    GameData gameData = GameData();
    
    gameData.loadIWAD("../tests/iwad_doom2.WAD");
    gameData.compile();

    gameData.printInfo();

    ResourcesData::ResourcesData* resources = gameData.getResourceFromWAD<ResourcesData::ResourcesData>();

    string name = "NOT EXIST";
    WADStructure::WADStructure* wad = gameData.getResourceFromWAD<WADStructure::WADStructure>();

    EXPECT_THROW(wad->findLump(name), LumpReadoutException);

}


TEST(GameDataTest, NotExistLumpsTest)
{

    GameData gameData = GameData();
    
    gameData.loadIWAD("../tests/iwad_doom2.WAD");
    gameData.compile();

    gameData.printInfo();

    ResourcesData::ResourcesData* resources = gameData.getResourceFromWAD<ResourcesData::ResourcesData>();

    vector<string> names = {"HELP", "TITLEPIC", "CREDIT", "BOSSBACK", "NOT EXIST"};
    WADStructure::WADStructure* wad = gameData.getResourceFromWAD<WADStructure::WADStructure>();

    for(string name: names)
    {
        if(name == "NOT EXIST")
        {
            EXPECT_THROW(wad->findLump(name), LumpReadoutException);
        }
        else
        {
            WADStructure::lumpInfo_t lump = wad->findLump(name);
        }
    }

}


TEST(GameDataTest, WadInvalidFileTest)
{

    GameData gameData = GameData();
    
    EXPECT_THROW(gameData.loadIWAD("../tests/not_wad.WAD"), WADReadoutException);
}

TEST(GameDataTest, MultipleIwadsTest)
{

    GameData gameData = GameData();

    gameData.loadIWAD("../tests/iwad_doom2.WAD");
    
    EXPECT_THROW(gameData.loadIWAD("../tests/iwad_doom2.WAD"), WADReadoutException);
}

TEST(GameDataTest, ReadIwadAsPwadTest)
{

    GameData gameData = GameData();

    gameData.loadIWAD("../tests/iwad_doom2.WAD");
    
    EXPECT_THROW(gameData.loadPWAD("../tests/iwad_doom2.WAD"), WADReadoutException);
}


/*

int main()
{
    GameData gameData = GameData();
    
    gameData.loadIWAD("../tests/iwad_doom2.WAD");
    gameData.loadPWAD("../tests/pwad_append.WAD");
    gameData.loadPWAD("../tests/pwad_replace.WAD");
    gameData.loadPWAD("../tests/pwad_replace_map.WAD");
    gameData.loadPWAD("../tests/pwad_append_map.WAD");
    //gameData.loadPWAD("../tests/not_wad.WAD");
    gameData.compile();

    WADStructure::WADStructure* wad = gameData.getResourceFromWAD<WADStructure::WADStructure>();
    EndoomData::EndoomData* endoom = gameData.getResourceFromWAD<EndoomData::EndoomData>();
    PlayPalData::PlayPalData* playpal = gameData.getResourceFromWAD<PlayPalData::PlayPalData>();
    ColorMapData::ColorMapData* colormap = gameData.getResourceFromWAD<ColorMapData::ColorMapData>();
    ResourcesData::ResourcesData* resources = gameData.getResourceFromWAD<ResourcesData::ResourcesData>();
    AudioInfoData::AudioInfoData* audio = gameData.getResourceFromWAD<AudioInfoData::AudioInfoData>();
    SoundData::SoundData* sound = gameData.getResourceFromWAD<SoundData::SoundData>();
    DemoData::DemoData* demo = gameData.getResourceFromWAD<DemoData::DemoData>();

    gameData.printInfo();

    unique_ptr<LevelData> level = gameData.getLevelData(0);
    level->printInfo();
    //level->printDetailedInfo();


    //endoom->printEndoom();


    //MUS FORMAT TEST
    //baseSound* sound1 = sound->readSound("D_OPENIN");
    //sound1.printInfo();
    //sound1.play();

    //DOOM FORMAT TEST
    //baseSound* sound2 = sound->readSound("DSSGTDTH");
    //sound2->printInfo();
    //sound2->play();

    //PC SPEAKER TEST
    //baseSound* sound3 = sound->readSound("DPPESIT");
    //sound3->printInfo(); 
    //sound3->play();
   
    
    //SPRITE TEST
    //string name1 = "CHGGA0";
    //Image image1 = resources->readSprite(name1);
    //image1.printInfo();
    //image1.saveAsFile("/home/anon/PROJECTS/CumManHunt/" + name1 + ".bmp");


    //PATCH TEST
    //string name2 = "DOOR9_1";
    //Image image2 = resources.readPatch(name2);
    //image2.printInfo();
    //image2.saveAsFile("/home/anon/PROJECTS/CumManHunt/" + name2 + ".bmp");

    //FLAT TEST
    //string name3 = "CONS1_5";
    //Image image3 = resources.readFlat(name3);
    //image3.printInfo();
    //image3.saveAsFile("/home/anon/PROJECTS/CumManHunt/" + name3 + ".bmp");


    //Game SPRITE TEST
    //string name4 = "HELP";
    //ResourcesData::Image image4 = resources->readGameSprite(name4);
    //image4.printInfo();
    //image4.saveAsFile("/home/anon/PROJECTS/CumManHunt/" + name4 + ".bmp");


    //endoom.printEndoom();
    //endoom.printInfo();
    //colormap.printInfo();
    //playpal.printInfo();
    //level.printLevelInfo();
    //audio.printInfo();
    //resources.printInfo();
    //demo.printInfo();

    return 0;

}

*/