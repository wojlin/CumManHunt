#include "LevelData.h"
#include "ColorMapData.h"
#include "PlayPalData.h"
#include "ResourcesData.h"
#include "EndoomData.h"
#include "AudioInfoData.h"
#include "SoundData.h"
#include "DemoData.h"

#include "Interfaces.h"

#include "../Utils/info.h"

using namespace std;


class GameData
{
    public:

        /**
         * @brief Construct a new Game Data object. 
         * @note this objects stores all information about game resources
         */
        GameData()
        {

        }

        /**
         * @brief Destroy the Game Data object
         * 
         */
        ~GameData()
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
        void printInfo()
        {
            printInfoHeader("H2", "GAME DATA INFO");

            if(compiled)
            {
                WADStructure* wad = getResourceFromWAD<WADStructure>();
                ResourcesData* resources = getResourceFromWAD<ResourcesData>();
                PlayPalData* playpal = getResourceFromWAD<PlayPalData>();
                ColorMapData* colormap = getResourceFromWAD<ColorMapData>();

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

            printInfoHeader("H2");
        }

        /**
         * @brief use this method to load .WAD file of "iwad" type \n 
         * 
         * @note read more about "iwad" here: https://doomwiki.org/wiki/IWAD
         * 
         * @param path path to WAD file in string format
         */
        void loadIWAD(string path)
        {
            classInstances[&typeid(WADStructure)] = new WADStructure(path);
        }

        /**
         * @brief use this method to load .WAD file of "pwad" type \n 
         * 
         * @note read more about "iwad" here: https://doomwiki.org/wiki/PWAD
         * 
         * @param path path to WAD file in string format
         */
        void loadPWAD(string path)
        {
            WADStructure* wad = getResourceFromWAD<WADStructure>();
            wad->loadPWAD(path);
        }

        /**
         * @brief This method compiles loaded iwad and optional pwads into one archive. 
         * @brief It needs to be called after loading iwad and optional pwads!
         * @brief It needs to be called before retreiving any resource or level!
         */
        void compile()
        {
            WADStructure* wad = getResourceFromWAD<WADStructure>();
            wad->compile();
            classInstances[&typeid(EndoomData)] = new EndoomData(wad);
            classInstances[&typeid(PlayPalData)] = new PlayPalData(wad);
            classInstances[&typeid(ColorMapData)] = new ColorMapData(wad);
            PlayPalData* playpal = getResourceFromWAD<PlayPalData>();
            classInstances[&typeid(ResourcesData)] = new ResourcesData(wad, playpal);
            classInstances[&typeid(AudioInfoData)] = new AudioInfoData(wad);
            classInstances[&typeid(SoundData)] = new SoundData(wad);
            classInstances[&typeid(DemoData)] = new DemoData(wad);
            compiled = true;
        }

        /**
         * @brief this functions returns class that contains info about level based on that level name
         * 
         * @param number 
         * @return unique_ptr<LevelData> 
         */
        unique_ptr<LevelData> getLevelData(string name)
        {
            WADStructure* wad = getResourceFromWAD<WADStructure>();
            for(int x =0; x<wad->levelsList.size(); x++)
            {
                if(wad->levelsList[x].name == name)
                {
                    return make_unique<LevelData>(wad->filePath, &wad->levelsList[0]);
                }
            }
            return nullptr;
        }

        /**
         * @brief this functions returns class that contains info about level based on that level number in vector
         * 
         * @param number 
         * @return unique_ptr<LevelData> 
         */
        unique_ptr<LevelData> getLevelData(int number)
        {
            WADStructure* wad = getResourceFromWAD<WADStructure>();
            if(number >= wad->levelsList.size())
            {
                return nullptr;
            }
            return make_unique<LevelData>(wad->filePath, &wad->levelsList[number]);
        }

        /**
         * @brief Get the Resource From WAD object
         * 
         * @tparam BaseResorceTypeWAD wanted resource class
         * @return pointer to returned resource class that is derrived from BaseResorceTypeWAD class
         */
        template <typename BaseResorceTypeWAD>
        BaseResorceTypeWAD* getResourceFromWAD() 
        {
            const std::type_info* typeInfo = &typeid(BaseResorceTypeWAD);
            auto it = classInstances.find(typeInfo);
            if (it != classInstances.end()) {
                return dynamic_cast<BaseResorceTypeWAD*>(it->second);
            }
            return nullptr;
        }


    private:
        bool compiled = false;
        map<const type_info*, baseResourceWAD*> classInstances;

        void errorExit(int status, string reason)
        {   
            cout << "ERROR: " << reason << endl;
            exit(status);
        }

        void compileCheck()
        {
            if(compiled == false)
            {
                errorExit(-1, "Game Data class needs to be first compiled before using it's methods!");
            }
        }


};

int main()
{
    GameData gameData = GameData();
    
    gameData.loadIWAD("../../tests/iwad_doom2.WAD");
    gameData.loadPWAD("../../tests/pwad_append.WAD");
    gameData.loadPWAD("../../tests/pwad_replace.WAD");
    gameData.loadPWAD("../../tests/pwad_replace_map.WAD");
    gameData.loadPWAD("../../tests/pwad_append_map.WAD");
    //gameData.loadPWAD("../../tests/not_wad.WAD");
    gameData.compile();

    WADStructure* wad = gameData.getResourceFromWAD<WADStructure>();
    EndoomData* endoom = gameData.getResourceFromWAD<EndoomData>();
    PlayPalData* playpal = gameData.getResourceFromWAD<PlayPalData>();
    ColorMapData* colormap = gameData.getResourceFromWAD<ColorMapData>();
    ResourcesData* resources = gameData.getResourceFromWAD<ResourcesData>();
    AudioInfoData* audio = gameData.getResourceFromWAD<AudioInfoData>();
    SoundData* sound = gameData.getResourceFromWAD<SoundData>();
    DemoData* demo = gameData.getResourceFromWAD<DemoData>();

    gameData.printInfo();

    unique_ptr<LevelData> level = gameData.getLevelData(0);
    //level->printLevelInfo();


    //MUS FORMAT TEST
    //Sound sound1 = sound.readSound("D_OPENIN");
    //sound1.printInfo();
    //sound1.play();

    //DOOM FORMAT TEST
    //Sound sound2 = sound.readSound("DSSGTDTH");
    //sound2.printInfo();
    //sound2.play();

    //PC SPEAKER TEST
    //Sound sound3 = sound->readSound("DPPESIT");
    //sound3.printInfo(); 
    //sound3.play();
   
    
    //SPRITE TEST
    //string name1 = "CHGGA0";
    //Image image1 = resources.readSprite(name1);
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


    //endoom.printEndoom();
    //endoom.printInfo();
    //colormap.printInfo();
    //playpal.printInfo();
    //level.printLevelInfo();
    //audio.printInfo();
    //resources.printInfo();
    //demo.printInfo();

}