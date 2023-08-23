#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <map>
#include <math.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

#include "LevelData.h"
#include "ColorMapData.h"
#include "PlayPalData.h"
#include "ResourcesData.h"
#include "EndoomData.h"
#include "AudioInfoData.h"
#include "SoundData.h"
#include "DemoData.h"

#include "Interfaces.h"
#include "../Utils/Info.h"



class GameData
{
    public:

        /**
         * @brief Construct a new Game Data object. 
         * @note this objects stores all information about game resources
         */
        GameData();

        /**
         * @brief Destroy the Game Data object
         * 
         */
        ~GameData();

        /**
         * @brief this function prints info about this class
         * 
         */
        void printInfo();

        /**
         * @brief use this method to load .WAD file of "iwad" type \n 
         * 
         * @note read more about "iwad" here: https://doomwiki.org/wiki/IWAD
         * 
         * @param path path to WAD file in string format
         */
        void loadIWAD(string path);

        /**
         * @brief use this method to load .WAD file of "pwad" type \n 
         * 
         * @note read more about "iwad" here: https://doomwiki.org/wiki/PWAD
         * 
         * @param path path to WAD file in string format
         */
        void loadPWAD(string path);

        /**
         * @brief This method compiles loaded iwad and optional pwads into one archive. 
         * @brief It needs to be called after loading iwad and optional pwads!
         * @brief It needs to be called before retreiving any resource or level!
         */
        void compile();

        /**
         * @brief this functions returns class that contains info about level based on that level name
         * 
         * @param number 
         * @return unique_ptr<LevelData> 
         */
        unique_ptr<LevelData::LevelData> getLevelData(string name);

        /**
         * @brief this functions returns class that contains info about level based on that level number in vector
         * 
         * @param number 
         * @return unique_ptr<LevelData> 
         */
        unique_ptr<LevelData::LevelData> getLevelData(int number);

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
        bool iWadLoaded = false;
        map<const type_info*, baseResourceWAD*> classInstances;

        void errorExit(int status, string reason);

        void compileCheck();


};

#endif