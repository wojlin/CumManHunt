#ifndef DEMODATA_H
#define DEMODATA_H

#include <map>
#include <math.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <iostream>

#include "Interfaces.h"
#include "WadStructure.h"


struct demoHeader_t
{
    string version;
    int skillLevel;
    int episode;
    int map;
    bool multiplayerRule;
    bool respawn;
    bool fast;
    bool nomonsters;
    bool recordingPlayer;
    bool greenPlayer;
    bool indigoPlayer;
    bool brownPlayer;
    bool redPlayer;
};

struct tick_t
{
    int movement;
    int strafe;
    int turn;
    bool fireWeapon;
    bool use;
    bool changeWeapon;
    int weaponInUse;
    bool pause;
    bool save;
    int saveSlot;
};


struct demo_t
{
    string name;
    demoHeader_t header;
    vector<tick_t> ticks;
};

class DemoData: public baseResourceWAD
{
    public:

        DemoData();

        DemoData(WADStructure *wad);

        vector<demo_t> getDemos();

        demo_t getDemoByName(string);

        demoHeader_t getDemoHeaderByName(string);

        vector<tick_t> getDemoTicksByName(string);

        void printInfo();


    private:
        string filePath;
        vector<demo_t> demos;
        int TICKS_PER_SECOND = 35;

        void printBool(bool);

        void readDEMO(WADStructure::lumpInfo_t lump);
};

#endif