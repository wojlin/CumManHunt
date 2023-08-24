#include "../../include/GameData/GameData.h"
#include "../../include/Utils/Info.h"
#include "../../include/Utils/Exceptions.h"



/**
 * @brief Construct a new Game Data object. 
 * @note this objects stores all information about game resources
 */
GameData::GameData()
{

}

/**
 * @brief Destroy the Game Data object
 * 
 */
GameData::~GameData()
{
    for (const auto& entry : classInstances) 
    {
        delete entry.second;
    }
}

/**
 * @brief this function prints info about this class
 * 
 */
void GameData::printInfo()
{
    printInfoHeader("H1", "GAME DATA INFO");

    if(compiled)
    {
        WADStructure::WADStructure* wad = getResourceFromWAD<WADStructure::WADStructure>();
        ResourcesData::ResourcesData* resources = getResourceFromWAD<ResourcesData::ResourcesData>();
        PlayPalData::PlayPalData* playpal = getResourceFromWAD<PlayPalData::PlayPalData>();
        ColorMapData::ColorMapData* colormap = getResourceFromWAD<ColorMapData::ColorMapData>();

        cout << "IWAD Path: \""<< wad->filePath << "\"" << endl;
        cout << "PWAD's amount: " << wad->getPwadsAmount() << endl;

        if(wad->getPwadsAmount() > 0)
        {
            cout << "PWAD Paths: " << endl;
            int count = 1;
            for (string name: wad->getPwadsPaths()) 
            {
                cout <<  count << ".    " << name << endl;
                count++;
            }
        }
        

        cout << endl;
        cout << "levels amount: " << wad->getlevelsAmount() << endl;
        cout << "sprites amount: " << resources->getSpritesAmount() << endl;
        cout << "flats amount: " << resources->getFlatsAmount() << endl;
        cout << "patches amount: " << resources->getPatchesAmount() << endl;
        cout << "pallets amount: " << playpal->getPalletesAmount() << endl;
        cout << "colormaps amount: " << colormap->getColorMapsAmount() << endl;
        cout << "pnames amount: " << resources->getPNamesAmount() << endl;
    }
    else
    {
        cout << "game data is not compiled yet!" << endl;
    }

    printInfoHeader("H1");
}

/**
 * @brief use this method to load .WAD file of "iwad" type \n 
 * 
 * @note read more about "iwad" here: https://doomwiki.org/wiki/IWAD
 * 
 * @param path path to WAD file in string format
 */
void GameData::loadIWAD(string path)
{
    if(iWadLoaded == true)
    {
        throw WADReadoutException("iwad already loaded");
    }

    classInstances[&typeid(WADStructure::WADStructure)] = new WADStructure::WADStructure(path);
    iWadLoaded = true;
}

/**
 * @brief use this method to load .WAD file of "pwad" type \n 
 * 
 * @note read more about "pwad" here: https://doomwiki.org/wiki/PWAD
 * 
 * @param path path to WAD file in string format
 */
void GameData::loadPWAD(string path)
{
    WADStructure::WADStructure* wad = getResourceFromWAD<WADStructure::WADStructure>();
    wad->loadPWAD(path);
}

/**
 * @brief This method compiles loaded iwad and optional pwads into one archive. 
 * @brief It needs to be called after loading iwad and optional pwads!
 * @brief It needs to be called before retreiving any resource or level!
 */
void GameData::compile()
{
    WADStructure::WADStructure* wad = getResourceFromWAD<WADStructure::WADStructure>();
    wad->compile();
    classInstances[&typeid(EndoomData::EndoomData)] = new EndoomData::EndoomData(wad);
    classInstances[&typeid(PlayPalData::PlayPalData)] = new PlayPalData::PlayPalData(wad);
    classInstances[&typeid(ColorMapData::ColorMapData)] = new ColorMapData::ColorMapData(wad);
    PlayPalData::PlayPalData* playpal = getResourceFromWAD<PlayPalData::PlayPalData>();
    classInstances[&typeid(ResourcesData::ResourcesData)] = new ResourcesData::ResourcesData(wad, playpal);
    classInstances[&typeid(AudioInfoData::AudioInfoData)] = new AudioInfoData::AudioInfoData(wad);
    classInstances[&typeid(SoundData::SoundData)] = new SoundData::SoundData(wad);
    classInstances[&typeid(DemoData::DemoData)] = new DemoData::DemoData(wad);
    compiled = true;
}

/**
 * @brief this functions returns class that contains info about level based on that level name
 * 
 * @param number 
 * @return LevelData::LevelData
 */
LevelData::LevelData GameData::getLevelData(string name)
{
    WADStructure::WADStructure* wad = getResourceFromWAD<WADStructure::WADStructure>();
    for(int x =0; x<wad->levelsList.size(); x++)
    {
        if(wad->levelsList[x].name == name)
        {
            return LevelData::LevelData(wad, &wad->levelsList[0]);
        }
    }
    throw WADReadoutException("level not found");
}

/**
 * @brief this functions returns class that contains info about level based on that level number in vector
 * 
 * @param number 
 * @return LevelData::LevelData
 */
LevelData::LevelData GameData::getLevelData(int number)
{
    WADStructure::WADStructure* wad = getResourceFromWAD<WADStructure::WADStructure>();
    if(number >= wad->levelsList.size())
    {
        throw WADReadoutException("level not found");
    }
    return LevelData::LevelData(wad, &wad->levelsList[number]);
}


void GameData::errorExit(int status, string reason)
{   
    cout << "ERROR: " << reason << endl;
    exit(status);
}

void GameData::compileCheck()
{
    if(compiled == false)
    {
        errorExit(-1, "Game Data class needs to be first compiled before using it's methods!");
    }
}
